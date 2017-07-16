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
#include <stdlib.h>
#include <stdbool.h>
#include <util/atomic.h>
#include "tq.h"

#define TQ_TM_ENABLED 2
typedef struct {
  uint8_t flags;
  taskq_p tfun;
  uint16_t ticks;
  uint16_t reload;
} timer;

static timer timers[MAX_TIMERS];

#define TQ_BUFMASK (TQ_SIZE - 1)
static volatile taskq_p tq[TQ_SIZE];
static volatile uint8_t tq_head, tq_tail;

/* gets next task from the queue and executes it
 * call this function from the main loop
 */
inline void tq_run(void)
{
  if (tq_head != tq_tail)
    (tq[tq_tail = (tq_tail + 1) & TQ_BUFMASK])();
}

/* puts a task to the queue */
bool tq_put(taskq_p tfun)
{
  uint8_t tmp_head = (tq_head + 1) & TQ_BUFMASK;
  if (tmp_head == tq_tail)
    /* no place for new task */
    return false;
  tq[tq_head = tmp_head] = tfun;
  return true;
}

void tq_init(void)
{
  int i;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    tq_tail = 0;
    tq_head = 0;

    for (i = 0; i < MAX_TIMERS; i++)
        timers[i].flags = 0;
  }
}

void tq_tm_putI(uint8_t n, taskq_p tfun, uint16_t ticks, uint8_t flags)
{
	timers[n].tfun = tfun;
	timers[n].ticks = ticks;
	timers[n].reload = ticks;
	timers[n].flags = flags | TQ_TM_ENABLED;
}

void tq_tm_put(uint8_t n, taskq_p tfun, uint16_t ticks, uint8_t flags)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    tq_tm_putI(n, tfun, ticks, flags);
	}
}

/* cancels the task */
void tq_tm_cancelI(uint8_t n)
{
	timers[n].flags = 0;
}

void tq_tm_cancel(uint8_t n)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		timers[n].flags = 0;
  }
}

/* call this function from a timer interrupt */
inline void tq_tick(void)
{
  int i;
  /* for every timerbled decrement ticks
   * if a timer is ready put its task into the task queue
  */
  for(i=0; i < MAX_TIMERS; i++) {
    if (!(timers[i].flags & TQ_TM_ENABLED))
      continue;

    if (--timers[i].ticks == 0) {
      if (timers[i].tfun)
        tq_put(timers[i].tfun);

      if (timers[i].flags & TQ_TM_ONESHOT)
        timers[i].flags = 0;
      else
        timers[i].ticks = timers[i].reload;
    }
  }
}
