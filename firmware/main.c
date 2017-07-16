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
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdio.h>
#include <stdlib.h>

#include "ibus.h"
#include "tq.h"
#include "sdebug.h"
#include "ibus_dispatch.h"
#include "csr.h"
#include "menu.h"


static inline void timer2_init(void)
{
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    /* 16e6/(128*(1+124)) = 1000.0 Hz */
    TCCR2A = _BV(WGM21);
    TCCR2B = _BV(CS22) | _BV(CS20);
    /* Output Compare Match A Interrupt Enable */
    TCNT2  = 0;
    OCR2A = 124;
    TIMSK2 = _BV(OCIE2A);
  }
}

/* called every 1ms */
ISR(TIMER2_COMPA_vect) 
{
  ibus_tick();
  tq_tick();
}


int main(void)
{
  clock_prescale_set(clock_div_1);

  /* enable audio amplifier */
  DDRD |= _BV(PD2);
  PORTD |= _BV(PD2);

  sdebug_init();
  timer2_init();
  tq_init();
  ibus_init();
  csr_init();
  menu_init();

  sei();
  debugsln_P("Hello! Here I am!");

  while (1) {
    tq_run();
    ibus_run();
  }

  return 0;
}
