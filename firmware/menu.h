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
#ifndef MENU_H
#define MENU_H

#define SM_EVENTS \
  X(B1) \
  X(B2) \
  X(B3) \
  X(B4) \
  X(B5) \
  X(B6) \
  X(RND) \
  X(SCAN) \
  X(NEXT) \
  X(PREV) \
  X(FFWD) \
  X(FBWD) \
  X(PLAY) \
  X(STOP) \
  X(PAUSE) \
  X(TMOUT) \
  X(MAX_EVENTS)

typedef enum {
#define X(s) s,
  SM_EVENTS
#undef X
} events;

void menu_fsm(events ev);
void menu_init(void);
#endif

