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
#ifndef CSR_H
#define CSR_H
#include <avr/pgmspace.h>

/* Read CS-209659-ANP1 document. It is found inside
 * CSR86XX ConfigurationTool-3.0.70
*/

/* watchdog timeout if there is no LED activity */
#define CSR_WATCHDOG_TM 5000

typedef void (*csr_watchdog_cb)(void);
typedef void (*csr_status_cb)(uint8_t);
void csr_init(void);
void csr_set_watchdog_cb(csr_watchdog_cb cb);
void csr_set_status_cb(csr_status_cb cb);
uint8_t csr_get_status(void);
void csr_sendcmd(uint8_t width, uint8_t clicks, uint8_t inputs);

/* Note that all button timeouts changed to be 10 times lower! */
#define csr_btn_shsingle(b) csr_sendcmd(10, 1, (b))
#define csr_btn_long(b)     csr_sendcmd(110, 1, (b))
#define csr_btn_vlong(b)    csr_sendcmd(110, 1, (b))
#define csr_btn_vvlong(b)   csr_sendcmd(510, 1, (b))
#define csr_btn_double(b)   csr_sendcmd(10, 2, (b))

#define csr_cmd_voicedial()  csr_btn_double(_BV(2))
#define csr_cmd_pairing()    csr_btn_double(_BV(4) | _BV(0))
#define csr_cmd_lastredial() csr_btn_double(_BV(4))
#define csr_cmd_answer()     csr_btn_shsingle(_BV(4))
#define csr_cmd_cancel()     csr_btn_shsingle(_BV(4))
#define csr_cmd_reject()     csr_btn_double(_BV(4) | _BV(3))

#define csr_cmd_transfaud()  csr_btn_double(_BV(3) | _BV(2))
#define csr_cmd_volup()      csr_btn_shsingle(_BV(0))
#define csr_cmd_voldn()      csr_btn_shsingle(_BV(1))
#define csr_cmd_mute()       csr_btn_shsingle(_BV(1) | _BV(0))
#define csr_cmd_mulpena()    csr_btn_shsingle(_BV(2) | _BV(1))
#define csr_cmd_mulpdis()    csr_btn_double(_BV(2) | _BV(1))
#define csr_cmd_pdlreset()   csr_btn_double(_BV(3) | _BV(0))

#define csr_cmd_playpaus()   csr_btn_shsingle(_BV(4))
#define csr_cmd_skipfw()     csr_btn_shsingle(_BV(2))
#define csr_cmd_skipbk()     csr_btn_shsingle(_BV(3))
#define csr_cmd_ffpress()    csr_btn_shsingle(_BV(2) | _BV(0))
#define csr_cmd_ffrelea()    csr_btn_double(_BV(2) | _BV(0))
#define csr_cmd_rewpress()   csr_btn_shsingle(_BV(3) | _BV(1))
#define csr_cmd_rewrelea()   csr_btn_double(_BV(3) | _BV(1))

#define CSR_STATE_00             0
#define CSR_STATE_CONNECTABLE    1 // Headset is on and connectable
#define CSR_STATE_CONNDISC       2 // Headset is both connectable and discoverable
#define CSR_STATE_CONNECTED      3 // Headset is connected to an AG
#define CSR_STATE_OUTG_CALL      4 // Headset is connected and an Outgoing call is in progress
#define CSR_STATE_INCM_CALL      5 // Headset is connected and an Incoming call is in progress
#define CSR_STATE_ACTV_CALL      6 // Headset is connected and an active call is in progress
#define CSR_STATE_07             7
#define CSR_STATE_TWC_CALWAI     8 // An active call is in progress and a second incoming call is waiting
#define CSR_STATE_TWC_CALHLD     9 // A call is on hold (There may also be an active call in progress)
#define CSR_STATE_TWC_MLTCAL    10 // Multiple calls are active
#define CSR_STATE_INC_CALHLD    11 // An Incoming call is on hold
#define CSR_STATE_ACT_CALNSCO   12 // Active call with no SCO present
#define CSR_STATE_A2DP_STREAM   13 // Headset is Streaming A2dp audio

#endif
