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
#include "VarioFrame6015Impl.h"

VarioFrame6015Impl::VarioFrame6015Impl(QWidget* parent, const char* name, Qt::WFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

VarioFrame6015Impl::~VarioFrame6015Impl()
{
}

void VarioFrame6015Impl::update()
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

void VarioFrame6015Impl::store()
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Response Delay
	uiValue = slider_RespDelay->value();
	pDev->memoryWrite(MemFa, FILT_TYPE, UInt8, uiValue);

	// Digital Filter Time
	uiValue = spinBox_FiltTime->value();
	pDev->memoryWrite(MemFa, VARIO_DIG_FK, UInt8, uiValue);

	// Min/Max Filter Time
	uiValue = spinBox_FiltTimeMinMax->value();
	pDev->memoryWrite(MemFa, VARIO_MIN_MAX_FK, UInt8, uiValue);
	
	// Min/Max Rise Reject
	uiValue = spinBox_TimeRiseRej->value();
	pDev->memoryWrite(MemFa, MAX_RISE_REJ, UInt16, uiValue);
}

#include "moc_VarioFrame6015Impl.cxx"

