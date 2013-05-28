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
#include <qcheckbox.h>
#include "Flytec.h"
#include "GPSFrame6015Impl.h"
#include "IFlyHighRC.h"

GPSFrame6015Impl::GPSFrame6015Impl(QWidget* parent, const char* name, Qt::WindowFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

GPSFrame6015Impl::~GPSFrame6015Impl()
{
}

void GPSFrame6015Impl::update()
{
	Flytec *pDev;
	uint uiValue;
	int iValue;

	pDev = static_cast<Flytec*>(IGPSDevice::pInstance());

	// grid system
	uiValue = pDev->parRead(MemFa, UNIT_FLAGS, FtUInt16).toUInt();
  comboBox_GridSys->setCurrentIndex((uiValue & MASK_UNIT_GRID) >> POS_UNIT_GRID);

	// utc offset
	iValue = pDev->parRead(MemFa, UTC_OFFSET, FtInt8).toInt();
	spinBox_UTCoffset->setValue(iValue);
}

void GPSFrame6015Impl::store()
{
	Flytec *pDev;
	uint uiValue;
	int iValue;

	pDev = static_cast<Flytec*>(IGPSDevice::pInstance());

	// grid system
	uiValue = pDev->parRead(MemFa, UNIT_FLAGS, FtUInt16).toUInt();
	uiValue &= ~MASK_UNIT_GRID;
  uiValue |= (comboBox_GridSys->currentIndex() << POS_UNIT_GRID);
	pDev->parWrite(MemFa, UNIT_FLAGS, FtUInt16, uiValue);

	// utc offset
	iValue = spinBox_UTCoffset->value();
	pDev->parWrite(MemFa, UTC_OFFSET, FtInt8, iValue);
}
