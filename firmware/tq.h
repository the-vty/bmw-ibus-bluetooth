/* * BMW Ibus Bluetooth CDC emulator - Emulates CD changer on IBus
 * Copyright (C) 2017-2099 victor naumov <vicnaumov@gmail.com
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
#ifndef TQ_H
#define TQ_H

#include <stdbool.h>

#define TM_CSR_WATCHDOG 1
#define TM_CSR_BURSTEND 2
#define TM_CSR_CMDPULSE 3
#define TM_MAIN_FSM_TMO 4
#define TM_MAIN_FSM_PMT 5
#define TM_TEST_1       6
#define TM_TEST_2       7
#define MAX_TIMERS      8

#define TQ_TM_REPEAT  0
#define TQ_TM_ONESHOT 1

/* size of the task queue (power of 2) */
#define TQ_SIZE 16

typedef void (*taskq_p)(void);

/* to be put before the main run loop */
void tq_init(void);

/* to be put into the main run loop */
void tq_run(void);

/* to be put into the timer ISR (1ms) */
void tq_tick(void);

/* create a task */
bool tq_put(taskq_p tfun);

/* create a timer task */
void tq_tm_put(uint8_t n, taskq_p tfun, uint16_t ticks, uint8_t flags);
void tq_tm_putI(uint8_t n, taskq_p tfun, uint16_t ticks, uint8_t flags);

/* cances a timer task */
void tq_tm_cancel(uint8_t n);
void tq_tm_cancelI(uint8_t n);

#endif
