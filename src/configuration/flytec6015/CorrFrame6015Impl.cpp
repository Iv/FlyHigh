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

CorrFrame6015Impl::CorrFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
	:CorrFrame6015(parent,name,fl)
{
}

CorrFrame6015Impl::~CorrFrame6015Impl()
{
}


void CorrFrame6015Impl::update(QByteArray &arr)
{
/*
	int16_t i16value;
	
	// correction of A1
	i16value = arr[QNH_CORR_A1_POS] << 8;
	i16value += arr[QNH_CORR_A1_POS+1];
	spinBox_QNHCorrA1->setValue(i16value);

	// correction of A2
	i16value = arr[QNH_CORR_A2_POS] << 8;
	i16value += arr[QNH_CORR_A2_POS+1];
	spinBox_QNHCorrA2->setValue(i16value);
*/
/*
	// temprature correction
	i8value = (char)arr[TMP_CORR_POS];
	spinBox_TempCorr->setValue(i8value);
*/
}

void CorrFrame6015Impl::store(QByteArray &arr)
{
/*
	int16_t i16value;
	
	// correction of A1
	i16value = spinBox_QNHCorrA1->value();
	arr[QNH_CORR_A1_POS] = (char)(i16value >> 8);
	arr[QNH_CORR_A1_POS+1] = (char)(i16value & 0xFF);

	// correction of A2
	i16value = spinBox_QNHCorrA2->value();
	arr[QNH_CORR_A2_POS] = (char)(i16value >> 8);
	arr[QNH_CORR_A2_POS+1] = (char)(i16value & 0xFF);
*/
/*
	// temprature correction
	arr[TMP_CORR_POS] = spinBox_TempCorr->value();
*/
}

#include "CorrFrame6015Impl.moc"
