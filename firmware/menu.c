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
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "sdebug.h"
#include "ibus.h"
#include "menu.h"
#include "csr.h"
#include "tq.h"


#define VOL_TMOUT 5000
#define SERVICE_TMOUT 5000

#define MENU_CHOICE   0
#define MENU_PAIR     1
#define MENU_RESETPDL 2
#define MENU_MENMU    3
#define MENU_MDIMU    4
#define MENU_VOLDN    5
#define MENU_VOLUP    6
#define MENU_NOCSR    7
#define MENU_BLANK    8

static const char menutext[][13] PROGMEM = {
  [MENU_CHOICE]   = {"Choice      "},
  [MENU_PAIR]     = {"Pairing?    "},
  [MENU_RESETPDL] = {"Reset PDL?  "},
  [MENU_MENMU]    = {"Enable MP?  "},
  [MENU_MDIMU]    = {"Disable MP? "},
  [MENU_VOLDN]    = {"Vol Down    "},
  [MENU_VOLUP]    = {"Vol Up      "},
  [MENU_NOCSR]    = {"CSR Lost    "},
  [MENU_BLANK]    = {"            "},
};

static const char csrtext[][13] PROGMEM = {
  {"STATE 00    "},
  {"Connecting  "},
  {"Discovery   "},
  {"Connected   "},
  {"Outg. Call  "},
  {"Incom. Call "},
  {"Active Call "},
  {"STATE 07    "},
  {"Call Waiting"},
  {"Call Hold   "},
  {"Multi Call  "},
  {"In.Call Hold"},
  {"Actve No SCO"},
  {"Streaming   "},
};

#define DELAY_RADIO_TEXT 250

#ifdef DELAY_RADIO_TEXT
static const char *prompt_p;
#define set_text_delayed(p) { \
  prompt_p = p; \
  tq_tm_put(TM_MAIN_FSM_PMT, set_prompt_cb, DELAY_RADIO_TEXT, TQ_TM_ONESHOT); \
} 

#define set_text_csr() set_text_delayed(csrtext[csr_get_status()])
#define set_text(i) set_text_delayed(menutext[i])
#else
#define set_text_csr() ibus_text_to_radio_p(csrtext[csr_get_status()])
#define set_text(i) ibus_text_to_radio_p(menutext[i])
#endif
#define clear_text() set_text(MENU_BLANK)

#define timeout_set(tv) tq_tm_put(TM_MAIN_FSM_TMO,\
    timed_out_cb, tv, TQ_TM_ONESHOT)
#define timeout_cancel() tq_tm_cancel(TM_MAIN_FSM_TMO)

#define SM_STATES \
  X(INACTIVE) \
  X(MAIN) \
  X(FSTFRWD) \
  X(FSTBKWRD) \
  X(SETVOL) \
  X(SERVICE) \
  X(CFRMPAIR) \
  X(CFRMRPDL) \
  X(CFRMENMU) \
  X(CFRMDIMU) \
  X(MAX_STATES)

typedef enum {
#define X(s) s,
  SM_STATES
#undef X
} states;

static volatile states curr_state;
void menu_fsm(events ev);

#ifdef DELAY_RADIO_TEXT
static void set_prompt_cb(void)
{
  ibus_text_to_radio_p(prompt_p);
}
#endif

static void timed_out_cb(void)
{
  menu_fsm(TMOUT);
}

static void act_all_stop(void)
{
  curr_state = INACTIVE;
  timeout_cancel();
  clear_text();
  debugsln_P("Exiting cdc mode");
}

static void act_cancel_main(void)
{
  curr_state = MAIN;
  timeout_cancel();
}

static void act_inactive_play(void)
{
  curr_state = MAIN;
  debugsln_P("Entering cdc mode");
}

static void act_phone_answer(void)
{
  csr_cmd_answer();
  debugsln_P("Answering call");
}

static void act_phone_cancel(void)
{
  csr_cmd_reject();
  debugsln_P("Rejecting call");
}

static void act_phone_transfer_audio(void)
{
  csr_cmd_transfaud();
  debugsln_P("Transfering audio");
}

static void act_mute(void)
{
  csr_cmd_mute();
  debugsln_P("Muting");
}

static void act_phone_last_redial(void)
{
  csr_cmd_lastredial();
  debugsln_P("Last redial");
}

static void act_play_pause(void)
{
  csr_cmd_playpaus();
  debugsln_P("Play/Pause");
}

/* this one doesn't work */
static void act_phone_voicedial(void)
{
  csr_cmd_voicedial();
  debugsln_P("Voice dial");
}

static void act_service_menu(void)
{
  curr_state = SERVICE;
  timeout_set(SERVICE_TMOUT);
  set_text(MENU_CHOICE);
}

static void act_play_next(void)
{
  csr_cmd_skipfw();
  set_text_csr();
  debugsln_P("Next");
}

static void act_play_prev(void)
{
  csr_cmd_skipbk();
  set_text_csr();
  debugsln_P("Prev");
}

static void act_begin_ffwd(void)
{
  curr_state = FSTFRWD;
  csr_cmd_ffpress();
  debugsln_P("fast forw begin");
}

static void act_begin_fbwd(void)
{
  curr_state = FSTBKWRD;
  csr_cmd_rewpress();
  debugsln_P("fast backw begin");
}

static void act_end_ffwd(void)
{
  curr_state = MAIN;
  csr_cmd_ffrelea();
  debugsln_P("fast forw end");
}

static void act_end_fbkw(void)
{
  curr_state = MAIN;
  csr_cmd_rewrelea();
  debugsln_P("fast backw end");
}

static void act_voldn(void)
{
  set_text(MENU_VOLDN);
  csr_cmd_voldn();
  timeout_set(VOL_TMOUT);
  debugsln_P("vol dn");
}

static void act_volup(void)
{
  set_text(MENU_VOLUP);
  csr_cmd_volup();
  timeout_set(VOL_TMOUT);
  debugsln_P("vol up");
}

/* waiting for confirmation for pairing */
static void act_cfrmpair(void)
{
  curr_state = CFRMPAIR;
  set_text(MENU_PAIR);
  timeout_set(SERVICE_TMOUT);
  debugsln_P("Waiting for conf. pair");
}

/* waiting for confirmation for resetting PDL */
static void act_cfrmrpdl(void)
{
  curr_state = CFRMRPDL;
  set_text(MENU_RESETPDL);
  timeout_set(SERVICE_TMOUT);
  debugsln_P("Waiting for conf. reset PDL");
}

/* waiting for confirmation for enabling multipoint */
static void act_cfrmenmu(void)
{
  curr_state = CFRMENMU;
  set_text(MENU_MENMU);
  timeout_set(SERVICE_TMOUT);
  debugsln_P("Waiting for conf. enable multipoint");
}

/* waiting for confirmation for disabling multipoint */
static void act_cfrmdimu(void)
{
  curr_state = CFRMDIMU;
  set_text(MENU_MDIMU);
  timeout_set(SERVICE_TMOUT);
  debugsln_P("Waiting for conf. disable multipoint");
}

static void act_voldn_setvol(void)
{
  curr_state = SETVOL;
  set_text(MENU_VOLDN);
  csr_cmd_voldn();
  timeout_set(VOL_TMOUT);
  debugsln_P("vol dn");
}

static void act_volup_setvol(void)
{
  curr_state = SETVOL;
  set_text(MENU_VOLUP);
  csr_cmd_volup();
  timeout_set(VOL_TMOUT);
  debugsln_P("vol up");
}

/* confirm pairing */
static void act_setpair(void)
{
  curr_state = MAIN;
  csr_cmd_pairing();
  timeout_cancel();
  debugsln_P("set pairing");
}

/* confirm resetting PDL */
static void act_setrpdl(void)
{
  curr_state = MAIN;
  csr_cmd_pdlreset();
  timeout_cancel();
  debugsln_P("reset pdl");
}

/* confirm enabling multipoint */
static void act_setenmu(void)
{
  curr_state = MAIN;
  csr_cmd_mulpena();
  timeout_cancel();
  debugsln_P("multipoint enable");
}

/* confirm disabling multipoint */
static void act_setdimu(void)
{
  curr_state = MAIN;
  csr_cmd_mulpdis();
  timeout_cancel();
  debugsln_P("multipoint disable");
}

typedef void (*act_handler)(void);
static const act_handler state_table[MAX_STATES][MAX_EVENTS] PROGMEM = {
  /* INACTIVE */
  {NULL, NULL, NULL,
   NULL, NULL, NULL,
   NULL, NULL,
   NULL, NULL,
   NULL, NULL,
   act_inactive_play, NULL, NULL,
   NULL
  },
  /* MAIN */
  {act_phone_answer, act_phone_cancel, act_phone_transfer_audio,
   act_mute, act_phone_last_redial, act_play_pause,
   act_phone_voicedial, act_service_menu,
   act_play_next, act_play_prev,
   act_begin_ffwd, act_begin_fbwd,
   NULL, act_all_stop, NULL,
   NULL
  },
  /* FSTFRWD */
  {NULL, NULL, NULL,
   NULL, NULL, NULL,
   NULL, NULL,
   NULL, NULL,
   NULL, NULL,
   act_end_ffwd, act_all_stop, NULL,
   NULL
  },
  /* FSTBKWRD */
  {NULL, NULL, NULL,                   /* [1] [2] [3]  */
   NULL, NULL, NULL,                   /* [4] [5] [6]  */
   NULL, NULL,                         /* [RND] [SCAN] */
   NULL, NULL,                         /* [>] [<] */
   NULL, NULL,                         /* [>>] [<<] */
   act_end_fbkw, act_all_stop, NULL,   /* play stop pause */
   NULL                                /* timeout */
  },
  /* SETVOL */
  {NULL, NULL, NULL,
   NULL, act_voldn, act_volup,
   act_cancel_main, act_cancel_main,
   NULL, NULL,
   NULL, NULL,
   NULL, act_all_stop, NULL,
   act_cancel_main
  },
  /* SERVICE */
  {act_cfrmpair, act_cfrmrpdl, act_cfrmenmu,
   act_cfrmdimu, act_voldn_setvol, act_volup_setvol,
   act_cancel_main, act_cancel_main,
   NULL, NULL,
   NULL, NULL,
   NULL, act_all_stop, NULL,
   act_cancel_main
  },
  /* CFRMPAIR */
  {NULL, act_cfrmrpdl, act_cfrmenmu, /* [1] [2] [3]  */
   act_cfrmdimu, NULL, NULL,         /* [4] [5] [6]  */
   act_setpair, act_cancel_main,     /* [RND] [SCAN] */
   NULL, NULL,                       /* [>] [<] */
   NULL, NULL,                       /* [>>] [<<] */
   NULL, act_all_stop, NULL,         /* play stop pause */
   act_cancel_main                   /* timeout */
  },
  /* CFRMRPDL */
  {act_cfrmpair, NULL, act_cfrmenmu, /* [1] [2] [3]  */
   act_cfrmdimu, NULL, NULL,         /* [4] [5] [6]  */
   act_setrpdl, act_cancel_main,     /* [RND] [SCAN] */
   NULL, NULL,                       /* [>] [<] */
   NULL, NULL,                       /* [>>] [<<] */
   NULL, act_all_stop, NULL,         /* play stop pause */
   act_cancel_main                   /* timeout */
  },
  /* CFRMENMU */
  {act_cfrmpair, act_cfrmrpdl, NULL, /* [1] [2] [3]  */
   act_cfrmdimu, NULL, NULL,         /* [4] [5] [6]  */
   act_setenmu, act_cancel_main,     /* [RND] [SCAN] */
   NULL, NULL,                       /* [>] [<] */
   NULL, NULL,                       /* [>>] [<<] */
   NULL, act_all_stop, NULL,         /* play stop pause */
   act_cancel_main                   /* timeout */
  },
  /* CFRMDIMU */
  {act_cfrmpair, act_cfrmrpdl, act_cfrmenmu,
   NULL, NULL, NULL,
   act_setdimu, act_cancel_main,
   NULL, NULL,
   NULL, NULL,
   NULL, act_all_stop, NULL,
   act_cancel_main
  },
};

#ifdef DEBUG_SERIAL
static void dump_event_name(PGM_P n, events e)
{
  switch (e)
  {
#define X(e) case e: debugs_p(n); debugsln_P(#e); return;
  SM_EVENTS
#undef X
  }
}

static void dump_state_name(PGM_P n, states s)
{
  switch (s)
  {
#define X(x) case x: debugs_p(n); debugsln_P(#x); return;
  SM_STATES
#undef X
  }
}
#endif

void menu_fsm(events ev)
{
  act_handler cb;

  if (ev < 0 || ev >= MAX_EVENTS || curr_state < 0 || curr_state >= MAX_STATES)
    return;

#ifdef DEBUG_SERIAL
  dump_event_name(PSTR("event = "), ev);
  dump_state_name(PSTR("state before = "), curr_state);
#endif
  cb = (act_handler) pgm_read_word(&state_table[curr_state][ev]);
  
  if (cb)
    (cb)();

  if (curr_state == MAIN)
    set_text_csr();

#ifdef DEBUG_SERIAL
  dump_state_name(PSTR("state after = "), curr_state);
#endif
}

static void csr_status(uint8_t status)
{
  (void) status;
  if (curr_state == MAIN)
    set_text_csr();

  debugs_P("mode = ");
  debugsln_p(csrtext[csr_get_status()]);
}

static void csr_watchdog(void)
{
  set_text(MENU_NOCSR);
  debugsln_P("CSR Lost");
}

void menu_init(void)
{
  curr_state = INACTIVE;
  csr_set_watchdog_cb(csr_watchdog);
  csr_set_status_cb(csr_status);
}
