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
#include <stdlib.h>
#include <stdbool.h>
#include "tq.h"
#include "csr.h"


static volatile uint16_t ticks;
static volatile uint8_t  csr_status;
static csr_watchdog_cb watchdog_cb = NULL;
static csr_status_cb status_cb = NULL;

/* CSR watchdog timer callback */
static void csr_watchdog(void){
  (watchdog_cb)();
  csr_status = 0;
}

/*
 * CSR configres led pattern using:
 * On time:  0-2550 ms with 10 ms step
 * Off time: 0-2550 ms with 10 ms step
 * Repeat time: 0... with 50ms step
 * Number of flashes in the burst: 0..15
 * Dim time: 0
 * Timeout: 0s
 
 * This code assumes that CSR is configured:
 * On time = idx * 10 ms 
 * Off time: 10 ms
 * Repeat time: 50 ms
 * Number of flashes in the burst: 1
 * where idx is csr_state_s_table index
*/

#define TICK10MS_SHIFT 4
#define TICK10MS (1<<TICK10MS_SHIFT)
#define LED0() (PINB & _BV(PB0))
static volatile uint16_t ticks_1 = 0, ticks_0 = 0;

/* CSR burst end timer callback */
static void burst_end(void)
{
  static uint8_t window[3];
  uint8_t val;

  if (ticks_1 < 75 || ticks_1 > 85){
    ticks_1 = ticks_0 = 0;
    return;
  }

  val = (ticks_0 + TICK10MS / 2 ) >> TICK10MS_SHIFT;

  ticks_1 = ticks_0 = 0;

  /* on-time must be between 10 and 130ms */
  if (val >= 1 && val <= 13)
  {
    if (val != csr_status)
    {
      if (val == window[0] && val == window[1] && val == window[2])
      {
        csr_status = val;
        if (status_cb)
          (status_cb)(csr_status);
      }
    }
    window[0] = window[1];
    window[1] = window[2];
    window[2] = val;
  }

  /* feed watchdog */
  tq_tm_put(TM_CSR_WATCHDOG, csr_watchdog, CSR_WATCHDOG_TM, TQ_TM_ONESHOT);
}


ISR(PCINT0_vect)
{
  if(LED0()){
    /* called on rising */
    ticks_0 += ticks;
    tq_put(burst_end);
  }
  else {
    ticks_1 += ticks;
  }
  ticks = 0;
}

/* called every 625us
 * or 16 ticks per 10 milliseconds
 * see TICK10MS
 */
ISR(TIMER1_COMPA_vect) 
{
  ticks++;
}

/* returns current CSR status */
uint8_t csr_get_status(void)
{
  return csr_status;
}

static volatile uint8_t cmd_pulse_active;
static volatile uint8_t cmd_pulse_cnt;
static volatile uint8_t cmd_pulse_inputs;

static void cmd_pulser(void)
{
  if(--cmd_pulse_cnt == 0){
    tq_tm_cancel(TM_CSR_CMDPULSE);
    cmd_pulse_active = false;
  }
  else
    PORTC ^= cmd_pulse_inputs;
}


void csr_sendcmd(uint8_t width, uint8_t clicks, uint8_t inputs)
{
  if (cmd_pulse_active)
    return;

  cmd_pulse_inputs = inputs;
  cmd_pulse_active = true;
  cmd_pulse_cnt = clicks << 1;

  PORTC = inputs & 0x3f;
  tq_tm_put(TM_CSR_CMDPULSE, cmd_pulser, width, TQ_TM_REPEAT);
}

void csr_set_watchdog_cb(csr_watchdog_cb cb)
{
  watchdog_cb = cb;
}

void csr_set_status_cb(csr_status_cb cb)
{
  status_cb = cb;
}

/* inits CSR routines and callbacks */
void csr_init(void)
{
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    ticks = 0;

    /* Timer 1 */
    // 16e6/(1*(1+9999)) = 1600.0 Hz
    TCCR1A = 0;
    TCCR1B = _BV(WGM12) | _BV(CS10);
    // Output Compare Match A Interrupt Enable
    TCNT1 = 0;
    OCR1A = 9999;
    TIMSK1 = (1 << OCIE1A);

    /* LED 0 PCINT */
    PCMSK0 |= _BV(PCINT0);
    DDRB &= ~_BV(PB0);
    PORTB &= ~_BV(PB0);
    PCICR |= _BV(PCIE0);

    /* LED 1 not used currently */
    PORTD &= ~_BV(PD6);
#ifdef CSR_USE_LED1
    PCMSK2 |= _BV(PCINT22);
    DDRD &= ~_BV(PD6);
    PCICR |= _BV(PCIE2);
#endif

    /* PC0..PC4 - outputs to control CSR module */
    DDRC = _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3) | _BV(PC4);
    PORTC = 0;

    /* first watchdog feeding */
    tq_tm_put(TM_CSR_WATCHDOG, csr_watchdog, CSR_WATCHDOG_TM, TQ_TM_ONESHOT);
  }
}
