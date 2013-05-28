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
#include "GPSFrameImpl.h"
#include "IFlyHighRC.h"

GPSFrameImpl::GPSFrameImpl(QWidget* parent, const char* name, Qt::WindowFlags fl)
  :QWidget(parent)
{
  setupUi(this);
}

GPSFrameImpl::~GPSFrameImpl()
{
}

void GPSFrameImpl::update()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// Grid System
  comboBox_GridSys->setCurrentIndex(pFlytec->parRead(MemUndef, GRID_SYS_POS, FtUInt8).toUInt());

	// UTC Offset
	spinBox_UTCoffset->setValue(pFlytec->parRead(MemUndef, UTC_OFFSET_POS, FtInt8).toInt());

	// Half UTC offset
	checkBox_UTChalfOffset->setChecked(pFlytec->parRead(MemUndef, UTC_HALF_OFFSET_POS, FtUInt8).toUInt());

//	spinBox_GeoID->setValue(arr[GEO_ID_POS]);
}

void GPSFrameImpl::store()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// Grid System
	pFlytec->parWrite(MemUndef, GRID_SYS_POS, FtUInt8, comboBox_GridSys->currentIndex());

	// UTC offset
	pFlytec->parWrite(MemUndef, UTC_OFFSET_POS, FtInt8, spinBox_UTCoffset->value());

	// sync UTC offset with ressources
	IFlyHighRC::pInstance()->setUtcOffset(spinBox_UTCoffset->value());

	// Half UTC offset
  pFlytec->parWrite(MemUndef, UTC_HALF_OFFSET_POS, FtUInt8, checkBox_UTChalfOffset->isChecked());

	// Geodic ID
//	arr[GEO_ID_POS] = spinBox_GeoID->value();
}
