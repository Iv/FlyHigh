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
#include "CorrFrameImpl.h"
#include "Flytec.h"

CorrFrameImpl::CorrFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
	:QWidget(parent)
{
  setupUi(this);
}

CorrFrameImpl::~CorrFrameImpl()
{
}

void CorrFrameImpl::update()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// correction of A1
  spinBox_QNHCorrA1->setValue(pFlytec->parRead(MemUndef, QNH_CORR_A1_POS, FtInt16).toInt());

	// correction of A2
  spinBox_QNHCorrA1->setValue(pFlytec->parRead(MemUndef, QNH_CORR_A2_POS, FtInt16).toInt());

/*
	// temprature correction
	i8value = (char)arr[TMP_CORR_POS];
	spinBox_TempCorr->setValue(i8value);
*/
}

void CorrFrameImpl::store()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// correction of A1
	pFlytec->parWrite(MemUndef, QNH_CORR_A1_POS, FtInt16, spinBox_QNHCorrA1->value());

	// correction of A2
	pFlytec->parWrite(MemUndef, QNH_CORR_A2_POS, FtInt16, spinBox_QNHCorrA1->value());

/*
	// temprature correction
	arr[TMP_CORR_POS] = spinBox_TempCorr->value();
*/
}
