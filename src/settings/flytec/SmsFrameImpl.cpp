/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include "SmsFrameImpl.h"

SmsFrameImpl::SmsFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
	:QWidget(parent)
{
  setupUi(this);
}

SmsFrameImpl::~SmsFrameImpl()
{
}

void SmsFrameImpl::update()
{
/*
	// Recieiver number
	arr[SMS_REC_NR_POS] = 0;
	lineEdit_RecNum->setText(QString(&arr[SMS_REC_NR_POS]));

	// Competition mode
	comboBox_Mode->setCurrentItem(arr[SMS_COMP_MODE_POS]);

	// Cyclic Rate
	spinBox_Intervall->setValue(arr[SMS_CYCL_RATE_POS]);

	// Emergency
	comboBox_Emerg->setCurrentItem(arr[SMS_EMERGENCY_POS]);
*/
}

void SmsFrameImpl::store()
{
/*
	// Receiver number
	memcpy((char*)&arr[SMS_REC_NR_POS], lineEdit_RecNum->text().ascii(), lineEdit_RecNum->text().length());

	// Competition mode
	arr[SMS_COMP_MODE_POS] = comboBox_Mode->currentItem();

	// Cyclic Rate
	arr[SMS_CYCL_RATE_POS] = spinBox_Intervall->value();

	// Emergency
	arr[SMS_EMERGENCY_POS] = comboBox_Emerg->currentItem();
*/
}
