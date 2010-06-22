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

#include <qslider.h>
#include <qspinbox.h>
#include "Flytec6015.h"
#include "VarioFrame6015Impl.h"

VarioFrame6015Impl::VarioFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: VarioFrame6015(parent,name,fl)
{
}

VarioFrame6015Impl::~VarioFrame6015Impl()
{
}

void VarioFrame6015Impl::update(QByteArray &arr)
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Response Delay
	uiValue = pDev->memoryRead(MemFa, FILT_TYPE, UInt8).toUInt();
	slider_RespDelay->setValue(uiValue);

	// Digital Filter Time
	uiValue = pDev->memoryRead(MemFa, VARIO_DIG_FK, UInt8).toUInt();
	spinBox_FiltTime->setValue(uiValue);

	// Min/Max Filter Time
	uiValue = pDev->memoryRead(MemFa, VARIO_MIN_MAX_FK, UInt8).toUInt();
	spinBox_FiltTimeMinMax->setValue(uiValue);

	// Min/Max Rise Reject
	uiValue = pDev->memoryRead(MemFa, MAX_RISE_REJ, UInt16).toUInt();
	spinBox_TimeRiseRej->setValue(uiValue);
}

void VarioFrame6015Impl::store(QByteArray &arr)
{
/*
	// Response Delay
//	arr[RESP_DELAY_POS] = spinBox_RespDelay->value() / 200;
	
	// Vario Mode
//	arr[VARIOMODE_POS] = comboBox_Variomode->currentItem();

	// Integration Time
	arr[I_TIME_POS] = spinBox_ITime->value();
	
	// Total Enery Compensation
//	arr[TEC_POS] = spinBox_TEC->value();
*/
}

#include "VarioFrame6015Impl.moc"

