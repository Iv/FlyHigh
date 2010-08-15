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

#include "CorrFrame6015Impl.h"
#include "Flytec6015.h"

CorrFrame6015Impl::CorrFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
	:CorrFrame6015(parent,name,fl)
{
}

CorrFrame6015Impl::~CorrFrame6015Impl()
{
}

void CorrFrame6015Impl::update(QByteArray &arr)
{
	Flytec6015 *pDev;
	int iValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Alt1 diff to QNH
	iValue = pDev->memoryRead(MemFa, ALT1_DIFF, Int32).toInt() / 100;
	spinBox_QNHCorrA1->setValue(iValue);

	// Offset pressure sensor
	iValue = pDev->memoryRead(MemFa, PRESS_OFFSET, Int32).toInt();
	spinBox_OffsetPress->setValue(iValue);
}

void CorrFrame6015Impl::store(QByteArray &arr)
{
	Flytec6015 *pDev;
	int iValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Alt1 diff to QNH
	iValue = spinBox_QNHCorrA1->value();
	pDev->memoryWrite(MemFa, ALT1_DIFF, Int32, iValue * 100);

	// Offset pressure sensor
	iValue = spinBox_OffsetPress->value();
	pDev->memoryWrite(MemFa, PRESS_OFFSET, Int32, iValue);
}

#include "CorrFrame6015Impl.moc"
