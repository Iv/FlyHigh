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
#include <qspinbox.h>

#include "VelocityFrameImpl.h"
#include "Flytec5020.h"

VelocityFrameImpl::VelocityFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
  :QWidget(parent)
{
  setupUi(this);
}

VelocityFrameImpl::~VelocityFrameImpl()
{
}

void VelocityFrameImpl::update()
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// best L/D
	spinBox_BestLD->setValue(pFlytec->parRead(BEST_LD_POS, FtUInt8).toUInt());

	// speed best L/D
	spinBox_SpeedBestLD->setValue(pFlytec->parRead(SPEED_BEST_LD_POS, FtUInt8).toUInt());

	// Windweel Gain
	spinBox_Windweel->setValue(pFlytec->parRead(SPEED_GAIN_WHEEL_POS, FtUInt8).toUInt());

	// Stall Speed
	spinBox_Stallspeed->setValue(pFlytec->parRead(STALL_SPEED_POS, FtUInt8).toUInt());

	// Stall Altitude
	spinBox_Stallaltitude->setValue(pFlytec->parRead(STALL_ALT_POS, FtUInt16).toUInt());
}

void VelocityFrameImpl::store()
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// best L/D
	pFlytec->parWrite(BEST_LD_POS, FtUInt8, spinBox_BestLD->value());

	// speed best L/D
	pFlytec->parWrite(SPEED_BEST_LD_POS, FtUInt8, spinBox_SpeedBestLD->value());

	// Windweel Gain
	pFlytec->parWrite(SPEED_GAIN_WHEEL_POS, FtUInt8, spinBox_Windweel->value());

	// Stall Speed
	pFlytec->parWrite(STALL_SPEED_POS, FtUInt8, spinBox_Stallspeed->value());

	// Stall Altitude
	pFlytec->parWrite(STALL_ALT_POS, FtUInt16, spinBox_Stallaltitude->value());
}
