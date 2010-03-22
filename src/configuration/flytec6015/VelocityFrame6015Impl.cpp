/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <qcombobox.h>
#include <qspinbox.h>

#include "VelocityFrame6015Impl.h"

VelocityFrame6015Impl::VelocityFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: VelocityFrame6015(parent,name,fl)
{
}

VelocityFrame6015Impl::~VelocityFrame6015Impl()
{
}

void VelocityFrame6015Impl::update(QByteArray &arr)
{
/*
	u_int16_t c16value;

	// best L/D
	spinBox_BestLD->setValue(arr[BEST_LD_POS]);

	// speed best L/D
	spinBox_SpeedBestLD->setValue(arr[SPEED_BEST_LD_POS]);

	// Windweel Gain
	spinBox_Windweel->setValue(arr[SPEED_GAIN_WHEEL_POS]);

	// Stall Speed
	spinBox_Stallspeed->setValue(arr[STALL_SPEED_POS]);

	// Stall Altitude
	c16value = arr[STALL_ALT_POS] << 8;
	c16value += arr[STALL_ALT_POS+1];
	spinBox_Stallaltitude->setValue(c16value);
*/
}

void VelocityFrame6015Impl::store(QByteArray &arr)
{
/*
	u_int16_t c16value;

	// best L/D
	arr[BEST_LD_POS] = spinBox_BestLD->value();

	// speed best L/D
	arr[SPEED_BEST_LD_POS] = spinBox_SpeedBestLD->value();

	// Windweel Gain
	arr[SPEED_GAIN_WHEEL_POS] = spinBox_Windweel->value();

	// Stall Speed
	arr[STALL_SPEED_POS] = spinBox_Stallspeed->value();

	// Stall Altitude
	c16value = spinBox_Stallaltitude->value();
	arr[STALL_ALT_POS] = (u_char)(c16value >> 8);
	arr[STALL_ALT_POS+1] = (u_char)(c16value & 0xFF);
*/
}

#include "VelocityFrame6015Impl.moc"

