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

#include "GPSFrame6015Impl.h"

GPSFrame6015Impl::GPSFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: GPSFrame6015(parent,name,fl)
{
}

GPSFrame6015Impl::~GPSFrame6015Impl()
{
}

void GPSFrame6015Impl::update(QByteArray &arr)
{
/*
	char i8value;

	// Grid System
	comboBox_GridSys->setCurrentItem(arr[GRID_SYS_POS]);
	
	// UTC Offset
	i8value = arr[UTC_OFFSET_POS];
	spinBox_UTCoffset->setValue(i8value);

	// Half UTC offset
	i8value = arr[UTC_HALF_OFFSET_POS];
	checkBox_UTChalfOffset->setChecked(i8value);
*/
//	spinBox_GeoID->setValue(arr[GEO_ID_POS]);
}

void GPSFrame6015Impl::store(QByteArray &arr)
{
/*
	// Grid System
	arr[GRID_SYS_POS] = comboBox_GridSys->currentItem();
	
	// UTC offset
	arr[UTC_OFFSET_POS] = spinBox_UTCoffset->value();
	
	// sync UTC offset with ressources
	IFlyHighRC::pInstance()->setUtcOffset(spinBox_UTCoffset->value());

	// Half UTC offset
	arr[UTC_HALF_OFFSET_POS] = checkBox_UTChalfOffset->isChecked();
*/
	// Geodic ID
//	arr[GEO_ID_POS] = spinBox_GeoID->value();
}

#include "GPSFrame6015Impl.moc"

