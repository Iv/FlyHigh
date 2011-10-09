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

#include <qcombobox.h>

#include "Flytec.h"
#include "UnitFrame6015Impl.h"

UnitFrame6015Impl::UnitFrame6015Impl(QWidget* parent, const char* name, Qt::WFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

UnitFrame6015Impl::~UnitFrame6015Impl()
{
}

void UnitFrame6015Impl::update()
{
	Flytec *pDev;
	uint uiValue;

	pDev = static_cast<Flytec*>(IGPSDevice::pInstance());
	uiValue = pDev->parRead(MemFa, UNIT_FLAGS, FtUInt16).toUInt();

	// Distance 1
  comboBox_Distance1->setCurrentIndex((uiValue & MASK_UNIT_DIST1) >> POS_UNIT_DIST1);

	// Distance 2
  comboBox_Distance2->setCurrentIndex((uiValue & MASK_UNIT_DIST2) >> POS_UNIT_DIST2);

	// Speed 1
  comboBox_Velocity1->setCurrentIndex((uiValue & MASK_UNIT_SPEED1) >> POS_UNIT_SPEED1);

	// Speed 2
  comboBox_Velocity2->setCurrentIndex((uiValue & MASK_UNIT_SPEED2) >> POS_UNIT_SPEED2);

	// Temperature
  comboBox_Temp->setCurrentIndex((uiValue & MASK_UNIT_DEG) >> POS_UNIT_DEG);

	// Pressure
  comboBox_Press->setCurrentIndex((uiValue & MASK_UNIT_PRESS) >> POS_UNIT_PRESS);

	// Time Format
  comboBox_TimeFormat->setCurrentIndex((uiValue & MASK_UNIT_TIME) >> POS_UNIT_TIME);
}

void UnitFrame6015Impl::store()
{
	Flytec *pDev;
	uint uiValue;

	pDev = static_cast<Flytec*>(IGPSDevice::pInstance());
	uiValue = pDev->parRead(MemFa, UNIT_FLAGS, FtUInt16).toUInt();
	uiValue &= ~(MASK_UNIT_DIST1 | MASK_UNIT_DIST2 | MASK_UNIT_SPEED1 | MASK_UNIT_SPEED2 | MASK_UNIT_DEG | MASK_UNIT_PRESS | MASK_UNIT_TIME);
  uiValue |= (comboBox_Distance1->currentIndex() << POS_UNIT_DIST1);
  uiValue |= (comboBox_Distance2->currentIndex() << POS_UNIT_DIST2);
  uiValue |= (comboBox_Velocity1->currentIndex() << POS_UNIT_SPEED1);
  uiValue |= (comboBox_Velocity2->currentIndex() << POS_UNIT_SPEED2);
  uiValue |= (comboBox_Temp->currentIndex() << POS_UNIT_DEG);
  uiValue |= (comboBox_Press->currentIndex() << POS_UNIT_PRESS);
  uiValue |= (comboBox_TimeFormat->currentIndex() << POS_UNIT_TIME);
	pDev->parWrite(MemFa, UNIT_FLAGS, FtUInt16, uiValue);
}
