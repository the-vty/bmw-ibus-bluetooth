/* * BMW Ibus Bluetooth CDC emulator - Emulates CD changer on IBus
 * Copyright (C) 2017-2099 victor naumov <vicnaumov@gmail.com>
 *
 * This file is part of BMW Ibus Bluetooth CDC emulator.
 *
 * BMW Ibus Bluetooth CDC emulator is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * BMW Ibus Bluetooth CDC emulator is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BMW Ibus Bluetooth CDC emulator.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <string.h>
#include "ibus.h"

#define BAUD 9600
#include <util/setbaud.h>

#define IBUS_RX_BUFMASK (IBUS_RX_BUFSIZE - 1)
#define IBUS_TX_BUFMASK (IBUS_TX_BUFSIZE - 1)
#define add_idx_rx(p, n) (((p) + (n)) & IBUS_RX_BUFMASK)
#define add_idx_tx(p, n) (((p) + (n)) & IBUS_TX_BUFMASK)
#define inc_idx_rx(p) add_idx_rx(p, 1)
#define inc_idx_tx(p) add_idx_tx(p, 1)

#if (IBUS_RX_BUFSIZE & IBUS_RX_BUFMASK)
  #error IBus RX buffer size is not a power of 2
#endif
#if (IBUS_TX_BUFSIZE & IBUS_TX_BUFMASK)
  #error IBus TX buffer size is not a power of 2
#endif

/* delayed tasks */
#define TSK_INTERCHAR   0
#define TSK_TXCLEAR     1
#define TSK_TXREADY     2

#if IBUS_SHUTDN_TMO > 0
#define TSK_SHUTDOWN    3
#define MAX_TIMERS      4
#else
#define MAX_TIMERS      3
#endif

#define random(m) ((rxbuf[3] ^ txbuf[5] ^ tx_tail ^ ticks) & (m))

/* Global variables */

static volatile uint8_t txbuf[IBUS_TX_BUFSIZE];
static volatile uint8_t rxbuf[IBUS_RX_BUFSIZE];
static volatile uint8_t tx_head, tx_tail, rx_head, rx_tail;
static volatile uint8_t last_rx_err;

/* flag is set after silence is detected on the bus + IBUS_TXCLEAR_TMO */
static volatile bool clear_to_send;

/* flag is set after transmission + IBUS_TXREADY_TMO delay */
static volatile bool ready_to_send;

typedef enum { WAIT_RX, WAIT_LEN, WAIT_BODY } rx_states;
typedef enum { WAIT_TX, DO_TX } tx_states;
static rx_states rx_fsm_state;
static tx_states tx_fsm_state;

typedef void (*ibus_task)(void);
typedef struct 
{
  bool enabled;
  uint16_t cnt;
  ibus_task tfun;
} timer;

static timer timers[MAX_TIMERS];

/* sets the task for delayed execution */
static inline void task_set(uint8_t n, uint16_t ticks, ibus_task tfun)
{
  timers[n].cnt = ticks;
  timers[n].tfun = tfun;
  timers[n].enabled = true;
}

/* cancels the task */
static inline void task_cancel(uint8_t n)
{
  timers[n].enabled = false;
}

/* task processor
 * must be run from tick handler every 1ms
*/
static volatile uint8_t ticks;
void ibus_tick(void)
{
  int i;
  ticks++;

  /* execute timers */
  for (i = 0; i < MAX_TIMERS; i++) {
    if (!timers[i].enabled)
      continue;

    if (--timers[i].cnt == 0) {
      timers[i].enabled = false;
      if (timers[i].tfun)
        (timers[i].tfun)();
    }
  }

  /* start pending transmissions if allowed */
  if (clear_to_send && ready_to_send && tx_head != tx_tail)
    /* enable UDRE interrupt */
    UCSR0B |= _BV(UDRIE0);
}


/* delayed task. resets receiver if no bytes are coming. gap detection */
static void receiver_reset(void)
{
  rx_fsm_state = WAIT_RX;
}

/* delayed task. reenables transmitter after previous transmission */
static void transmitter_ready(void)
{
  ready_to_send = true;
}

/* delayed task. called when SEN/STA is low + IBUS_TXCLEAR_TMO */
static void bus_clear(void)
{
  clear_to_send = true;
  PORTB &= ~_BV(PB5);
}

#if IBUS_SHUTDN_TMO > 0
/* delayed task. shuts down the TH3122 LDO */
static void shutdown(void)
{
  PORTD &= ~_BV(PD4);
}
#endif

/* tries to put byte at the next position from *idx
 * increments *idx
*/
static void put_rx_byte(uint8_t *idx, uint8_t c)
{
  uint8_t _idx = inc_idx_rx(*idx);
  if (_idx == rx_tail) {
    /* if a packet is not consumed in time and a new is about to
     * overwrite it we need discard the packet and move the
     * tail to the next unread packet
     */
    uint8_t len = rxbuf[add_idx_rx(rx_tail, 2)];
    rx_tail = add_idx_rx(rx_tail, 1 + len);
  }

  rxbuf[_idx] = c;
  *idx = _idx;
}

ISR(USART_RX_vect)
{
  static uint8_t cksum, len;
  static uint8_t curr_head;
  uint8_t status, data;

  /* the sequence is important. 
   * 1. read status 
   * 2. read data
   * */
  status = UCSR0A;
  data = UDR0;
  last_rx_err = status & (_BV(FE0)|_BV(DOR0));

  if (last_rx_err){
    /* frame or data overrun error
     * start over
     * */
    rx_fsm_state = WAIT_RX;
    task_cancel(TSK_INTERCHAR);
    return;
  }

  switch(rx_fsm_state) {
    case WAIT_RX:
      // got first byte
      rx_fsm_state = WAIT_LEN;
      curr_head = rx_head;
      task_set(TSK_INTERCHAR, IBUS_INTERCHAR_TMO, receiver_reset);
      put_rx_byte(&curr_head, data);
      cksum = data;
      break;

    case WAIT_LEN:
      rx_fsm_state = WAIT_BODY;
      task_set(TSK_INTERCHAR, IBUS_INTERCHAR_TMO, receiver_reset);
      cksum ^= data;

      len = data;
      if (len < 3 || len > IBUS_MAX_MSGSIZE)
        /* lenth is out of bounds. reset the rx fsm */
        rx_fsm_state = WAIT_RX;
      else
        put_rx_byte(&curr_head, data);
      break;

    case WAIT_BODY:
      cksum ^= data;
      if (--len) {
        task_set(TSK_INTERCHAR, IBUS_INTERCHAR_TMO, receiver_reset);
        put_rx_byte(&curr_head, data);
      }
      else {
        /* got the whole packet */
        rx_fsm_state = WAIT_RX;
        task_cancel(TSK_INTERCHAR);
        if (cksum == 0){
          /* message is ready */
          rx_head = curr_head;
#if IBUS_SHUTDN_TMO > 0
          /* schedule shutdown if no activity
           * on the bus within IBUS_SHUTDN_TMO
           */
          task_set(TSK_SHUTDOWN, IBUS_SHUTDN_TMO, shutdown);
#endif
        }
      }
      break;
  }
}


/* UART Data Register Empty interrupt
 * called when the UART is ready to transmit the next byte
*/
ISR(USART_UDRE_vect)
{
  static uint8_t curr_tail;
  static uint8_t len;
  static uint8_t cksum;
  uint8_t data;

  if (tx_head == tx_tail) {
      /* everything we had is sent. disable tx interrupts*/
      UCSR0B &= ~_BV(UDRIE0);
      tx_fsm_state = WAIT_TX;
      return;
  }

  switch(tx_fsm_state){
    case WAIT_TX:
      /* (tail + 1) points to the try counter */
      if(txbuf[inc_idx_tx(tx_tail)]-- == 0) {
        /* expire the packet */
        len = txbuf[add_idx_tx(tx_tail, 3)];
        tx_tail = add_idx_tx(tx_tail, 2 + len);
      }

      /* total lenth including the src and len fields */
      len = txbuf[add_idx_tx(tx_tail, 3)] + 2;
      cksum = 0;
      tx_fsm_state = DO_TX;
      curr_tail = inc_idx_tx(tx_tail);

    case DO_TX:
      if (!clear_to_send) {
        /* collision */
        tx_fsm_state = WAIT_TX;
        break;
      }

      if (--len > 0){
        curr_tail = inc_idx_tx(curr_tail);
        data = txbuf[curr_tail];
        UDR0 = data;
        cksum ^= data;
      }
      else {
        UDR0 = cksum;
        UCSR0B &= ~_BV(UDRIE0);
        tx_tail = curr_tail;
        tx_fsm_state = WAIT_TX;
        ready_to_send = false;
        /* packet is sent 
         * mute transiever for IBUS_TXREADY_TMO + 2 ticks
         * (2 ticks because UART has 2 chars yet to transmit)
				*/
        task_set(TSK_TXREADY,
                 IBUS_TXREADY_TMO + 2 + random(0x07),
                 transmitter_ready);
      }
      break;
  }
}

/* TH3122 SEN/STA interrupt */
ISR(INT1_vect)
{
  if (PIND & _BV(PD3)){
    /* rising - sobebody is transmitting */
    clear_to_send = false;
    task_cancel(TSK_TXCLEAR);
    PORTB |= _BV(PB5);
    rx_fsm_state = WAIT_RX;
  }
  else {
    /* falling - after few milliseconds we can try transmitting */
    task_set(TSK_TXCLEAR, IBUS_TXCLEAR_TMO + random(0x0f), bus_clear);
  }
}

/*
 * tries to get byte at the next position from *idx
 * increments *idx and returns true if success
 * false otherwise
*/
static bool get_rx_byte(uint8_t *idx, uint8_t *c)
{
  if (*idx == rx_head)
    return false;
  *idx = inc_idx_rx(*idx);
  *c = rxbuf[*idx];
  return true;
}


/*
 * tries to put byte at the next position from *idx
 * increments *idx and returns true if success
 * false otherwise
*/
static bool put_tx_byte(uint8_t *idx, uint8_t c)
{
  uint8_t _idx = inc_idx_tx(*idx);
  if (_idx == tx_tail)
    return false;

  txbuf[_idx] = c;
  *idx = _idx;
  return true;
}

/*
 * puts new message *msg with len to the send buffer
 * sender will try to send this packet ntries times
 * src and dst are the IBus addresses
 * msg is a pure payload not including checksum and dst
 * len is the size of msg not including checksum and dst
 * checksum is calculated on the fly
 * returns:
 *    false if there is no space in transmit buffer
 *    true if message has been placed
 */
bool ibus_write_msg(uint8_t src, uint8_t dst, uint8_t* msg, uint8_t len, uint8_t ntries)
{
  /* put message into send buffer */
  uint8_t i;
  uint8_t curr_head = tx_head;
  if (!put_tx_byte(&curr_head, ntries)
   || !put_tx_byte(&curr_head, src)
   || !put_tx_byte(&curr_head, len+2)
   || !put_tx_byte(&curr_head, dst))
    return false;

  for (i=0; i<len; i++)
    if(!put_tx_byte(&curr_head, msg[i]))
      return false;

  tx_head = curr_head;
  return true;
}

/*
 * gets new message *msg with len from the receive buffer
 * src and dst are the IBus addresses
 * msg is a pure payload not including checksum and dst
 * len is the size of msg not including checksum and dst
 * returns:
 *    false if there is nothing to read
 *    true if message has ben read
 */

bool ibus_read_msg(uint8_t *src, uint8_t *dst, uint8_t *msg, uint8_t *len)
{
  uint8_t i;
  uint8_t curr_tail = rx_tail;
  if (rx_head == rx_tail)
    return false;

  if (!get_rx_byte(&curr_tail, src)
   || !get_rx_byte(&curr_tail, len)
   || !get_rx_byte(&curr_tail, dst))
    return false;

  /* sanity check */
  if (*len < 3 || *len > IBUS_MAX_MSGSIZE)
    return false;

  /* subtract 2 bytes for dst and crc */
  *len -= 2;

  for (i=0; i<*len; i++)
    if(!get_rx_byte(&curr_tail, msg + i)) {
      return false;
    }

  rx_tail = curr_tail;
  return true;
}

void ibus_init(void)
{
  uint8_t i;

  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    tx_head = 0;
    tx_tail = 0;
    rx_head = 0;
    rx_tail = 0;
    rx_fsm_state = WAIT_RX;
    tx_fsm_state = WAIT_TX;
    clear_to_send = false;
    ready_to_send = true;
    for (i = 0; i < MAX_TIMERS; i++)
      timers[i].enabled = false;
  }

  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  /* Enable USART receiver and transmitter and receive complete interrupt */
  UCSR0B = _BV(RXCIE0)|(1<<RXEN0)|(1<<TXEN0);

  /* Set frame format: asynchronous, 8E1 */
#ifdef URSEL0
  UCSR0C = (1<<URSEL0)|(3<<UCSZ00)|(2<<UPM00);
#else
  UCSR0C = (3<<UCSZ00)|(2<<UPM00);
#endif

  // PD3 - SEN/STA level change interrupt
  EIMSK = _BV(INT1);
  EICRA |= _BV(ISC10);
  EICRA &= ~_BV(ISC11);

  // PD3 - TH3122 SEN/STA input, no pullup
  DDRD &= ~_BV(PD3);
  PORTD &= ~_BV(PD3);

  // ENABLE LDO
  // PD4 - TH3122 EN output
  DDRD |= _BV(PD4);
  PORTD |= _BV(PD4);

  // IBus activity LED  
  DDRB |= _BV(PB5);
  PORTB &= ~_BV(PB5);
}

uint8_t ibus_get_rx_error(void)
{
  return last_rx_err;
}

void ibus_rx_flush(void)
{
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    rx_head = rx_tail;
  }
}

void ibus_tx_flush(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		rx_head = rx_tail;
	}
}

bool ibus_rx_available(void)
{
  uint8_t ret;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    ret = rx_head != rx_tail;
  }
  return ret;
}

void ibus_text_to_radio(const char* p)
{
  //uint8_t i;
  uint8_t msg[15] = {0x23, 0x42, 0x32 };
  /* max 12 chars can be displayed on the radio */
  strncpy((char*)msg+3, p, 12);

  /* if first char is space replace it with 0x86
   * otherwise it will align left ignoring space
   */
  if(msg[3] == 0x20)
    msg[3] = 0x86;

  ibus_write_msg(0xc8, 0x80, msg, sizeof(msg), 3);
}

void ibus_text_to_radio_p(PGM_P p)
{
  uint8_t msg[15] = {0x23, 0x42, 0x32 };

  /* max 12 chars can be displayed on the radio */
  strncpy_P((char*)msg+3, p, 12);

  /* if first char is space replace it with 0x86
   * otherwise it will align left ignoring space
   */
  if(msg[3] == 0x20)
    msg[3] = 0x86;

  ibus_write_msg(0xc8, 0x80, msg, sizeof(msg), 3);
}

void ibus_cdc_report_status(uint8_t status, uint8_t ack)
{
  uint8_t msg[] = {0x39, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x07, 0x01};
  msg[1] = status;
  msg[2] = ack;
  ibus_write_msg(IBUS_DEV_CDC, IBUS_DEV_RAD, msg, sizeof(msg), 5);
}
