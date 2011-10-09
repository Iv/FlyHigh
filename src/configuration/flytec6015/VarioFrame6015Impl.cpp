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
#include "Flytec5020.h"
#include "VarioFrame6015Impl.h"

VarioFrame6015Impl::VarioFrame6015Impl(QWidget* parent, const char* name, Qt::WFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

VarioFrame6015Impl::~VarioFrame6015Impl()
{
}

void VarioFrame6015Impl::update()
{
	Flytec5020 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Response Delay
	uiValue = pDev->parRead(MemFa, FILT_TYPE, FtUInt8).toUInt();
	slider_RespDelay->setValue(uiValue);

	// Digital Filter Time
	uiValue = pDev->parRead(MemFa, VARIO_DIG_FK, FtUInt8).toUInt();
	spinBox_FiltTime->setValue(uiValue);

	// Min/Max Filter Time
	uiValue = pDev->parRead(MemFa, VARIO_MIN_MAX_FK, FtUInt8).toUInt();
	spinBox_FiltTimeMinMax->setValue(uiValue);

	// Min/Max Rise Reject
	uiValue = pDev->parRead(MemFa, MAX_RISE_REJ, FtUInt16).toUInt();
	spinBox_TimeRiseRej->setValue(uiValue);
}

void VarioFrame6015Impl::store()
{
	Flytec5020 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Response Delay
	uiValue = slider_RespDelay->value();
	pDev->parWrite(MemFa, FILT_TYPE, FtUInt8, uiValue);

	// Digital Filter Time
	uiValue = spinBox_FiltTime->value();
	pDev->parWrite(MemFa, VARIO_DIG_FK, FtUInt8, uiValue);

	// Min/Max Filter Time
	uiValue = spinBox_FiltTimeMinMax->value();
	pDev->parWrite(MemFa, VARIO_MIN_MAX_FK, FtUInt8, uiValue);

	// Min/Max Rise Reject
	uiValue = spinBox_TimeRiseRej->value();
	pDev->parWrite(MemFa, MAX_RISE_REJ, FtUInt16, uiValue);
}
