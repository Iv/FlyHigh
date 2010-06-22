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

#include "Flytec6015.h"
#include "UnitFrame6015Impl.h"

UnitFrame6015Impl::UnitFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: UnitFrame6015(parent,name,fl)
{
}

UnitFrame6015Impl::~UnitFrame6015Impl()
{
}

void UnitFrame6015Impl::update(QByteArray &arr)
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());
	uiValue = pDev->memoryRead(MemFa, UNIT_FLAGS, UInt16).toUInt();

	// Distance 1
	comboBox_Distance1->setCurrentItem((uiValue & MASK_UNIT_DIST1) >> POS_UNIT_DIST1);

	// Distance 2
	comboBox_Distance2->setCurrentItem((uiValue & MASK_UNIT_DIST2) >> POS_UNIT_DIST2);

	// Speed 1
	comboBox_Velocity1->setCurrentItem((uiValue & MASK_UNIT_SPEED1) >> POS_UNIT_SPEED1);

	// Speed 2
	comboBox_Velocity2->setCurrentItem((uiValue & MASK_UNIT_SPEED2) >> POS_UNIT_SPEED2);

	// Temperature
	comboBox_Temp->setCurrentItem((uiValue & MASK_UNIT_DEG) >> POS_UNIT_DEG);

	// Pressure
	comboBox_Press->setCurrentItem((uiValue & MASK_UNIT_PRESS) >> POS_UNIT_PRESS);

	// Time Format
	comboBox_TimeFormat->setCurrentItem((uiValue & MASK_UNIT_TIME) >> POS_UNIT_TIME);
}

void UnitFrame6015Impl::store(QByteArray &arr)
{
/*
	u_char ch = 0;
	int index;
	
	// Distance
	index = comboBox_Distance->currentItem();
	ch |= index;

	// Velocity 1
	index = comboBox_Velocity1->currentItem();
	index <<= 1;
	ch |= index;
	
	// Velocity 2
	index = comboBox_Velocity2->currentItem();
	index <<= 2;
	ch |= index;
	
	// Temperature
	index = comboBox_Temp->currentItem();
	index <<= 4;
	ch |= index;
	
	// store
	arr[UNITS_POS] = ch;
*/
}

#include "UnitFrame6015Impl.moc"

