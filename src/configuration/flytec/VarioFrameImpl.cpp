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
#include <qspinbox.h>

#include "VarioFrameImpl.h"
extern "C"
{
	#include "flytec_al.h"
}

VarioFrameImpl::VarioFrameImpl(QWidget* parent, const char* name, WFlags fl)
: VarioFrame(parent,name,fl)
{
}

VarioFrameImpl::~VarioFrameImpl()
{
}

void VarioFrameImpl::update(QByteArray &arr)
{
	// Response Delay
	spinBox_RespDelay->setValue(arr[RESP_DELAY_POS] * 200);

	// Vario Mode
	comboBox_Variomode->setCurrentItem(arr[VARIOMODE_POS]);

	// Integration Time
	spinBox_ITime->setValue(arr[I_TIME_POS]);
	
	// Total Enery Compensation
	spinBox_TEC->setValue(arr[TEC_POS]);
}

void VarioFrameImpl::store(QByteArray &arr)
{
	// Response Delay
	arr[RESP_DELAY_POS] = spinBox_RespDelay->value() / 200;
	
	// Vario Mode
	arr[VARIOMODE_POS] = comboBox_Variomode->currentItem();

	// Integration Time
	arr[I_TIME_POS] = spinBox_ITime->value();
	
	// Total Enery Compensation
	arr[TEC_POS] = spinBox_TEC->value();
}




#include "VarioFrameImpl.moc"

