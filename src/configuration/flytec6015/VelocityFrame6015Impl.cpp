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
#include "Flytec6015.h"
#include "VelocityFrame6015Impl.h"

VelocityFrame6015Impl::VelocityFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: VelocityFrame6015(parent,name,fl)
{
}

VelocityFrame6015Impl::~VelocityFrame6015Impl()
{
}

void VelocityFrame6015Impl::update()
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Speedgain Windweel
	uiValue = pDev->memoryRead(MemFa, WIND_WHEEL_GAIN, UInt8).toUInt();
	spinBox_Windweel->setValue(uiValue);

	// Stallspeed
	uiValue = pDev->memoryRead(MemFa, STALL_SPEED, UInt16).toUInt();
	spinBox_Stallspeed->setValue(uiValue);
}

void VelocityFrame6015Impl::store()
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Speedgain Windweel
	uiValue = spinBox_Windweel->value();
	pDev->memoryWrite(MemFa, WIND_WHEEL_GAIN, UInt8, uiValue);

	// Stallspeed
	uiValue = spinBox_Stallspeed->value();
	pDev->memoryWrite(MemFa, STALL_SPEED, UInt16, uiValue);
}

#include "VelocityFrame6015Impl.moc"

