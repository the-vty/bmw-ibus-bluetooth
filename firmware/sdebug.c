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
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "sdebug.h"

#define SDEBUG_PORT  PORTD
#define SDEBUG_DDR   DDRD
#define SDEBUG_TXBIT PD7

#if (F_CPU != 16000000)
  #error DEBUG TX uart needs adapting for clock speeds other than 16Mhz
#endif

#define BAUD9600   { OCR0A = 25 ; TCCR0B = (_BV(CS01) | _BV(CS00)); }
#define BAUD19200  { OCR0A = 103 ; TCCR0B = _BV(CS01); }
#define BAUD38400  { OCR0A = 51 ; TCCR0B = _BV(CS01); }
#define BAUD115200 { OCR0A = 138 ; TCCR0B = _BV(CS00); }
#define init_timer() {   \
  TCCR0A = _BV(WGM01);   \
  TCCR0B = 0; TCNT0 = 0; \
  BAUD19200    \
}

#define tx_high() (SDEBUG_PORT |= _BV(SDEBUG_TXBIT))
#define tx_low() (SDEBUG_PORT &= ~_BV(SDEBUG_TXBIT))
#define enable_timer_int() { TIMSK0 |= _BV(OCIE0A); }
#define disable_timer_int() { TIMSK0 &= ~_BV(OCIE0A); }
#define SDEBUG_COMPA_vect TIMER0_COMPA_vect

/* ring buffer size */
#define SDEBUG_BUFSIZE 256
#define SDEBUG_BUFMASK (SDEBUG_BUFSIZE - 1)
#define inc_idx_tx(p) (((p) + 1) & SDEBUG_BUFMASK)

#if (SDEBUG_BUFSIZE & SDEBUG_BUFMASK)
	#error TX buffer size is not a power of 2
#endif

static volatile uint8_t txbuf[SDEBUG_BUFSIZE];
static volatile uint8_t tx_head, tx_tail;

typedef enum {WAITCHAR, BITBANG} tx_fsm_state;
static tx_fsm_state tx_state;


ISR(SDEBUG_COMPA_vect)
{
  static uint16_t bitbang_word;
  static uint8_t bit_cnt;

  switch(tx_state){
    case WAITCHAR:
      if (tx_head == tx_tail){
        /* everything is sent */
        disable_timer_int();
        return;
      }

      /* set up transmission */
      tx_tail = (tx_tail + 1) & SDEBUG_BUFMASK;
      bit_cnt = 10;
      bitbang_word = (txbuf[tx_tail] << 1) | 0x200;
      tx_state = BITBANG;
      break;

    case BITBANG:
      if (bitbang_word & 0x01)
        tx_high();
      else 
        tx_low();

      bitbang_word >>= 1;
      if (--bit_cnt == 0) {
        /* char is sent */
        tx_state = WAITCHAR;
        tx_high();
      }
      break;
  }
}

void sdebug_init(void)
{
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    SDEBUG_DDR |= _BV(SDEBUG_TXBIT);
    tx_state = WAITCHAR;
    tx_tail = 0;
    tx_head = 0;

    init_timer();
    tx_high();
  }
}

void sdebug_putc(uint8_t c)
{
  uint8_t tmp_head;

  tmp_head = (tx_head + 1) & SDEBUG_BUFMASK;
  /* wait for free space in the buffer */
  while(tmp_head == tx_tail);
  txbuf[tmp_head] = c;
  tx_head = tmp_head;
  enable_timer_int();
}

void sdebug_puts(const char *p)
{
  while (*p) {
    sdebug_putc(*p++);
  }
}

void sdebug_puts_p(PGM_P p)
{
  char c;
  while ((c = pgm_read_byte(p++))) {
    sdebug_putc(c);
  }
}

const char pgnl[] PROGMEM = "\r\n";
