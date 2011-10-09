/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <qspinbox.h>
#include <qcombobox.h>
#include "Flytec5020.h"
#include "MemoryFrame6015Impl.h"

MemoryFrame6015Impl::MemoryFrame6015Impl(QWidget* parent, const char* name, Qt::WFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

MemoryFrame6015Impl::~MemoryFrame6015Impl()
{
}

void MemoryFrame6015Impl::update()
{
	Flytec5020 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// recording interval
	uiValue = pDev->parRead(MemFa, REC_INTERVALL, FtUInt8).toUInt();
	spinBox_Intervall->setValue(uiValue);

	// flight end detection
	uiValue = pDev->parRead(MemFa, DIV_FLAGS, FtUInt16).toUInt();
  comboBox_FlightEnd->setCurrentIndex((uiValue & MASK_FLIGHT_END) >> POS_FLIGHT_END);
}

void MemoryFrame6015Impl::store()
{
	Flytec5020 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// recording interval
	uiValue = spinBox_Intervall->value();
	pDev->parWrite(MemFa, REC_INTERVALL, FtUInt8, uiValue);

	// flight end detection
	uiValue = pDev->parRead(MemFa, DIV_FLAGS, FtUInt16).toUInt();
	uiValue &= ~MASK_FLIGHT_END;
  uiValue |= (comboBox_FlightEnd->currentIndex() << POS_FLIGHT_END);
	pDev->parWrite(MemFa, DIV_FLAGS, FtUInt16, uiValue);
}
