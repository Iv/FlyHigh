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

#include <qspinbox.h>
#include <qcombobox.h>
#include "Flytec6015.h"
#include "MemoryFrame6015Impl.h"

MemoryFrame6015Impl::MemoryFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: MemoryFrame6015(parent,name,fl)
{
}

MemoryFrame6015Impl::~MemoryFrame6015Impl()
{
}

void MemoryFrame6015Impl::update(QByteArray &arr)
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// recording interval
	uiValue = pDev->memoryRead(MemFa, REC_INTERVALL, UInt8).toUInt();
	spinBox_Intervall->setValue(uiValue);

	// flight end detection
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();
	comboBox_FlightEnd->setCurrentItem((uiValue & MASK_FLIGHT_END) >> POS_FLIGHT_END);
}

void MemoryFrame6015Impl::store(QByteArray &arr)
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// recording interval
	uiValue = spinBox_Intervall->value();
	pDev->memoryWrite(MemFa, REC_INTERVALL, UInt8, uiValue);

	// flight end detection
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();
	uiValue &= ~MASK_FLIGHT_END;
	uiValue |= (comboBox_FlightEnd->currentItem() << POS_FLIGHT_END);
	pDev->memoryWrite(MemFa, DIV_FLAGS, UInt16, uiValue);
}

#include "MemoryFrame6015Impl.moc"

