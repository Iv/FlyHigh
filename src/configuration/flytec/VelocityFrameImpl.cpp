/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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

#include "VelocityFrameImpl.h"
extern "C"
{
	#include "flytec_al.h"
}

VelocityFrameImpl::VelocityFrameImpl(QWidget* parent, const char* name, WFlags fl)
: VelocityFrame(parent,name,fl)
{
	// not available for 5020
	VelocityFrame::comboBox_Scaling->setEnabled(false);
	comboBox_DisplayMode->setEnabled(false);
	comboBox_PressSpeed->setEnabled(false);
	spinBox_SpeedGain->setEnabled(false);
}

VelocityFrameImpl::~VelocityFrameImpl()
{
}

void VelocityFrameImpl::update(QByteArray &arr)
{
	u_int16_t c16value;

	// Windweel Gain
	spinBox_Windweel->setValue(arr[SPEED_GAIN_WHEEL_POS]);

	// Stall Speed
	spinBox_Stallspeed->setValue(arr[STALL_SPEED_POS]);

	// Stall Altitude
	c16value = arr[STALL_ALT_POS] << 8;
	c16value += arr[STALL_ALT_POS+1];
	spinBox_Stallaltitude->setValue(c16value);
}

void VelocityFrameImpl::store(QByteArray &arr)
{
	u_int16_t c16value;

	// Windweel Gain
	arr[SPEED_GAIN_WHEEL_POS] = spinBox_Windweel->value();

	// Stall Speed
	arr[STALL_SPEED_POS] = spinBox_Stallspeed->value();

	// Stall Altitude
	c16value = spinBox_Stallaltitude->value();
	arr[STALL_ALT_POS] = (u_char)(c16value >> 8);
	arr[STALL_ALT_POS+1] = (u_char)(c16value & 0xFF);
}

#include "VelocityFrameImpl.moc"

