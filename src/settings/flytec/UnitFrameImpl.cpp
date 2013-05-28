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
#include <stdint.h>
#include "UnitFrameImpl.h"
#include "Flytec.h"

UnitFrameImpl::UnitFrameImpl(QWidget* parent, const char* name, Qt::WindowFlags fl)
  :QWidget(parent)
{
  setupUi(this);
}

UnitFrameImpl::~UnitFrameImpl()
{
}

void UnitFrameImpl::update()
{
  Flytec *pFlytec;
	uint8_t ch;
	int index;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());
 	ch = pFlytec->parRead(MemUndef, UNITS_POS, FtUInt8).toUInt();

	// Distance
	index = (ch & 0x01);
  comboBox_Distance->setCurrentIndex(index);

	// Velocity 1
	ch >>= 1;
	index = (ch & 0x01);
  comboBox_Velocity1->setCurrentIndex(index);

	// Velocity 2
	ch >>= 1;
	index = (ch & 0x03);
  comboBox_Velocity2->setCurrentIndex(index);

	// Temperature
	ch >>= 2;
	index = (ch & 0x01);
  comboBox_Temp->setCurrentIndex(index);
}

void UnitFrameImpl::store()
{
  Flytec *pFlytec;
	uint8_t ch = 0;
	int index;

	pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// Distance
  index = comboBox_Distance->currentIndex();
	ch |= index;

	// Velocity 1
  index = comboBox_Velocity1->currentIndex();
	index <<= 1;
	ch |= index;

	// Velocity 2
  index = comboBox_Velocity2->currentIndex();
	index <<= 2;
	ch |= index;

	// Temperature
  index = comboBox_Temp->currentIndex();
	index <<= 4;
	ch |= index;

	// store
	pFlytec->parWrite(MemUndef, UNITS_POS, FtUInt8, ch);
}
