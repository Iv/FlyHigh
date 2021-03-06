/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                *
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

#ifndef _Defs5020_h
#define _Defs5020_h

#include <QString>

#define FT_STRING_SIZE 17

#define PILOT_NAME_POS 0
#define LCD_CONTRAST_POS 32
#define QNH_CORR_A1_POS 54
#define VARIO_DELAY 73 // Vario/Spd Delay F2 (not used)
#define BASE_VOL_BEEP_POS 74
#define ACOUSTIC_LIFT_THR_POS 76
#define ACOUSTIC_SINK_POS 77
#define VARIO_FILT 79 // Vario/Spd Delay F1 (not used)
#define REC_STOP_MODE_POS 89
#define UTC_OFFSET_POS 92
#define STALL_SPEED_POS 94
#define STALL_ALT_POS 95
#define REC_INTERVAL_POS 97
#define ACOUSTIC_I_POS 100
#define UP_BASE_FRQ_POS 102
#define DOWN_BASE_FRQ_POS 104
#define FRQ_MODULATION_POS 110
#define SINC_ACOUSTIC_EN_POS 119
#define I_TIME_POS 121 // Digit. Variomode Aver.time
#define SPEED_GAIN_WHEEL_POS 128
#define UNITS_POS 141
#define GRID_SYS_POS 142
#define QNH_CORR_A2_POS 147
#define ACOUSTIC_PITCH_POS 158
#define SW_VERSION_POS 159 // read with deviceInfo()
#define LAST_THERM_THERS_POS 185
#define COMPANY_POS 186
#define LANGUAGE_POS 187
#define GLIDER_TYPE_POS 192
#define GLIDER_ID_POS 224
#define USERFIELD_0_POS 320
#define USERFIELD_1_POS 330
#define USERFIELD_2_POS 340
#define BEST_LD_POS 350
#define SPEED_BEST_LD_POS 351
#define BATT_TYPE_POS 352
#define UTC_HALF_OFFSET_POS 384

static const uint Flytec5020PageSize = 8;

static const uint Flytec5020MemSize = 385;

typedef enum FtDataType{FtInt8, FtUInt8, FtInt16, FtUInt16, FtUInt32, FtInt32, FtString, FtArray}FtDataType;

typedef struct FlytecDate
{
	uchar day;
	uchar month;
	uint year;
}FlytecDateType;

typedef struct FlytecTime
{
	uint hour;
	uint min;
	uint sec;
}FlytecTimeType;

typedef struct DeviceInfo
{
	QString deviceIdent;
	QString pilotName;
	uint serialNr;
	QString swVersion;
}DeviceInfoType;

#endif
