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
#ifndef SDEBUG_H
#define SDEBUG_H
#include <avr/pgmspace.h>

void sdebug_init(void);
void sdebug_putc(uint8_t c);
void sdebug_puts(const char *p);
void sdebug_puts_p(PGM_P p);
#define sdebug_puts_P(x) sdebug_puts_p(PSTR(x))

#define nib2hex(n) ((n) < 0xa ? (0x30 + (n)) : (0x57 + (n)))

#ifdef DEBUG_SERIAL
#define put_hex(b) { \
  sdebug_putc(nib2hex((b) >> 4));  \
  sdebug_putc(nib2hex((b) & 0xf)); \
}

extern const char pgnl[] PROGMEM;

#define debugs(x) sdebug_puts(x)
#define debugs_p(x) sdebug_puts_p(x)
#define debugs_P(x) sdebug_puts_P(x)
#define debugc(x) sdebug_putc(x)
#define debugh(x) put_hex(x)
#define debugnl() debugs_p(pgnl)
#define debugsln_P(x) {debugs_P(x); debugnl();}
#define debugsln_p(x) {debugs_p(x); debugnl();}
#define debugsln(x) {debugs(x); debugnl();}

#else

#define put_hex(x)
#define debugs(x)
#define debugs_p(x)
#define debugs_P(x)
#define debugc(x)
#define debugh(x)
#define debugnl()
#define debugc_hex(x)
#define debugsln_P(x)
#define debugsln_p(x)
#define debugsln(x)

#endif

#endif
