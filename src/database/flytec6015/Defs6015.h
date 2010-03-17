/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
 *   grafal@sourceforge.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _Defs6015_h
#define _Defs6015_h

typedef enum MemType{MemFa, MemPa}MemType;

typedef enum DataType{UInt8, Int16, UInt16, Int32, String}DataType;

// FA Settings
#define OWNER            0x00
#define AC_TYPE          0x01
#define AC_ID            0x02
#define UNITS            0x03
#define DIV_FLAGS        0x04
#define FILT_TYPE        0x05
#define ALT1_DIFF        0x06
#define VARIO_DIG_FK     0x07
#define B_FREQ_RISE      0x08
#define B_FREQ_SINK      0x09
#define AUDIO_RISE       0x0a
#define AUDIO_SINK       0x0b
#define SINK_ALARM       0x0c
#define FREQ_GAIN        0x0d
#define PITCH_GAIN       0x0e
#define MAX_RISE_REJ     0x0f
#define VARIO_MIN_MAX_FK 0x10
#define REC_INTERVALL    0x11
#define AUDIO_VOLUME     0x12
#define UTC_OFFSET       0x13
#define PRESS_OFFSET     0x14
#define THERM_THRESHOLD  0x15
#define POWER_OFF_TIME   0x16
#define STALL_SPEED      0x1a
#define WIND_WHEEL_GAIN  0x1c

// PA Settings
#define DEVICE_NR        0x00
#define DEVICE_TYPE      0x01
#define SW_VERS          0x02
#define KALIB_TYPE       0x03
#define FILT1_K          0x04
#define FILT2_K          0x05
#define FILT4_K          0x06
#define AUDIO_HYST       0x07
#define AUDIO_RS_THR_FAK 0x08
#define BATT_LEVEL_1     0x09
#define BATT_LEVEL_2     0x0a
#define BATT_LEVEL_3     0x0b
#define ALT_DIFF_FLA     0x0c
#define VARIO_FLA        0x0d
#define SPEED_FLA        0x0e

#endif