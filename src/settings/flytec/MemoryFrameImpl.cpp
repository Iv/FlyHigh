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

#include "MemoryFrameImpl.h"
#include "Flytec.h"

MemoryFrameImpl::MemoryFrameImpl(QWidget* parent, const char* name, Qt::WindowFlags fl)
  :QWidget(parent)
{
  setupUi(this);
}

MemoryFrameImpl::~MemoryFrameImpl()
{
}

void MemoryFrameImpl::update()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// Recording Interval
	spinBox_Intervall->setValue(pFlytec->parRead(MemUndef, REC_INTERVAL_POS, FtUInt8).toUInt());

	// Stop Mode
	comboBox_Mode->setCurrentIndex(pFlytec->parRead(MemUndef, REC_STOP_MODE_POS, FtUInt8).toUInt());
}

void MemoryFrameImpl::store()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// Recording Interval
	pFlytec->parWrite(MemUndef, REC_INTERVAL_POS, FtUInt8, spinBox_Intervall->value());

	// Stop Mode
	pFlytec->parWrite(MemUndef, REC_STOP_MODE_POS, FtUInt8, comboBox_Mode->currentIndex());
}
