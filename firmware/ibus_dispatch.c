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
#include <ctype.h>
#include "sdebug.h"
#include "ibus.h"
#include "menu.h"

static inline void
dump_msg(uint8_t src, uint8_t dst, uint8_t *msg, uint8_t len)
{
#ifdef DEBUG_SERIAL
  uint8_t i;
  debugh(src);
  debugc(' ');
  debugh(len+2);
  debugc(' ');
  debugh(dst);
  debugc(' ');

  for (i=0; i<len;i++){
    debugh(msg[i]);
    debugc(' ');
  }

  for (i=0; i<len;i++){
    uint8_t c = msg[i];
    if(isascii(c))
      debugc(c);
  }
  debugs("\r\n");
#endif
}

static void dispatch_cdc_poll(uint8_t *msg, uint8_t len)
{
  static bool cdc_reset = true;
  /* cdc poll request */
  debugsln_P("CDC_POLL");
  msg[0] = IBUS_MSG_DEV_READY;
  msg[1] = cdc_reset ? 1:0;
  cdc_reset = false;
  ibus_write_msg(IBUS_DEV_CDC, IBUS_DEV_LOC, msg, 2, 5);
}

static void dispatch_cdc_commands(uint8_t *msg, uint8_t len)
{
  uint8_t cmd = msg[1];
  uint8_t arg = msg[2];
  debugs_P("CDC_STATUS_REQ: ");

  switch(cmd) {
    case IBUS_CDCMD_REFRESH:
      debugsln_P("refresh");
      break;

    case IBUS_CDCMD_STOP:
      debugsln_P("stop");
      ibus_cdc_report_stop();
      menu_fsm(STOP);
      break;

    case IBUS_CDCMD_PAUSE:
      debugsln_P("pause");
      ibus_cdc_report_stop();
      menu_fsm(PAUSE);
      break;

    case IBUS_CDCMD_PLAY:
      debugsln_P("play");
      ibus_cdc_report_play();
      menu_fsm(PLAY);
      break;

    case IBUS_CDCMD_FFREW:
      ibus_cdc_report_play();
      debugs_P("fast ");
      switch(arg){
        case IBUS_CDCMDARG_REW:
          debugsln_P("backward");
          menu_fsm(FBWD);
          break;
        case IBUS_CDCMDARG_FF:
          debugsln_P("forward");
          menu_fsm(FFWD);
          break;
        default:
          debugsln_P("unknown");
          break;
      }
      break;

    case IBUS_CDCMD_TRACK5:
    case IBUS_CDCMD_TRACKA:
      debugs_P("track ");
      switch(arg){
        case IBUS_CDCMDARG_NEXT:
          debugsln_P("next");
          menu_fsm(NEXT);
          break;
        case IBUS_CDCMDARG_PREV:
          debugsln_P("previous");
          menu_fsm(PREV);
          break;
        default:
          debugsln_P("unknown");
          break;
      }
      break;

    case IBUS_CDCMD_DISK:
      debugs_P("disk ");
      debugh(arg);
      debugnl();
      {
        uint8_t ev = arg - 1;
        if (ev >=0 && ev <6)
          menu_fsm(ev);
      }
      break;     

    case IBUS_CDCMD_SCAN:
      ibus_cdc_report_play();
      debugs_P("scan ");
      switch(arg){
        case IBUS_CDCMDARG_OFF:
          debugsln_P("off");
          break;
        case IBUS_CDCMDARG_ON:
          debugsln_P("on");
          menu_fsm(SCAN);
          break;
        default:
          debugsln_P("unknown");
          break;
      }
      break;  

    case IBUS_CDCMD_RANDOM:
      ibus_cdc_report_play();
      debugs_P("random ");
      switch(arg){
        case IBUS_CDCMDARG_OFF:
          debugsln_P("off");
          break;
        case IBUS_CDCMDARG_ON:
          debugsln_P("on");
          menu_fsm(RND);
          break;
        default:
          debugsln_P("unknown");
          break;
      }
      break;  

    default:
      debugsln_P("Unknown CDC CMD");
      break;  
  }
}

static void dispatch_rad_to_cdc(uint8_t *msg, uint8_t len)
{
  switch(msg[0]) {
    case IBUS_MSG_DEV_POLL:
      dispatch_cdc_poll(msg, len);
      break;

    case IBUS_MSG_RADIO_CDC:
      dispatch_cdc_commands(msg, len);
      break;

    default:
      break;
  }
}

void ibus_run(void)
{

  uint8_t msg[36];
  uint8_t src, dst, len;

  while(ibus_read_msg(&src, &dst, msg, &len)){
    dump_msg(src, dst, msg, len);

    if (src == IBUS_DEV_RAD && dst == IBUS_DEV_CDC)
      dispatch_rad_to_cdc(msg, len);
  }
}
