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
#include <qcheckbox.h>
#include "IFlyHighRC.h"

#include "GPSFrameImpl.h"
extern "C"
{
	#include "flytec_al.h"
}

GPSFrameImpl::GPSFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

GPSFrameImpl::~GPSFrameImpl()
{
}

void GPSFrameImpl::update(QByteArray &arr)
{
	char i8value;

	// Grid System
        comboBox_GridSys->setCurrentIndex(arr[GRID_SYS_POS]);
	
	// UTC Offset
	i8value = arr[UTC_OFFSET_POS];
	spinBox_UTCoffset->setValue(i8value);

	// Half UTC offset
	i8value = arr[UTC_HALF_OFFSET_POS];
	checkBox_UTChalfOffset->setChecked(i8value);

//	spinBox_GeoID->setValue(arr[GEO_ID_POS]);
}

void GPSFrameImpl::store(QByteArray &arr)
{
	// Grid System
        arr[GRID_SYS_POS] = comboBox_GridSys->currentIndex();
	
	// UTC offset
	arr[UTC_OFFSET_POS] = spinBox_UTCoffset->value();
	
	// sync UTC offset with ressources
	IFlyHighRC::pInstance()->setUtcOffset(spinBox_UTCoffset->value());

	// Half UTC offset
	arr[UTC_HALF_OFFSET_POS] = checkBox_UTChalfOffset->isChecked();

	// Geodic ID
//	arr[GEO_ID_POS] = spinBox_GeoID->value();
}

#include "moc_GPSFrameImpl.cxx"

