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
#include "Flytec5020.h"
#include "GPSFrameImpl.h"
#include "IFlyHighRC.h"

GPSFrameImpl::GPSFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
  :QWidget(parent)
{
  setupUi(this);
}

GPSFrameImpl::~GPSFrameImpl()
{
}

void GPSFrameImpl::update(QByteArray &arr)
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Grid System
  comboBox_GridSys->setCurrentIndex(pFlytec->parRead(GRID_SYS_POS, FtUInt8).toUInt());

	// UTC Offset
	spinBox_UTCoffset->setValue(pFlytec->parRead(UTC_OFFSET_POS, FtInt8).toInt());

	// Half UTC offset
	checkBox_UTChalfOffset->setChecked(pFlytec->parRead(UTC_HALF_OFFSET_POS, FtUInt8).toUInt());

//	spinBox_GeoID->setValue(arr[GEO_ID_POS]);
}

void GPSFrameImpl::store(QByteArray &arr)
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Grid System
	pFlytec->parWrite(GRID_SYS_POS, FtUInt8, comboBox_GridSys->currentIndex());

	// UTC offset
	pFlytec->parWrite(UTC_OFFSET_POS, FtInt8, spinBox_UTCoffset->value());

	// sync UTC offset with ressources
	IFlyHighRC::pInstance()->setUtcOffset(spinBox_UTCoffset->value());

	// Half UTC offset
  pFlytec->parWrite(UTC_HALF_OFFSET_POS, FtUInt8, checkBox_UTChalfOffset->isChecked());

	// Geodic ID
//	arr[GEO_ID_POS] = spinBox_GeoID->value();
}
