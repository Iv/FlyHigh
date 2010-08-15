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
#include <qtabwidget.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qslider.h>
#include "DisplayFrame6015Impl.h"
#include "Flytec6015.h"
#include "Images.h"

DisplayFrame6015Impl::DisplayFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: DisplayFrame6015(parent,name,fl)
{
}

DisplayFrame6015Impl::~DisplayFrame6015Impl()
{
}

void DisplayFrame6015Impl::update(QByteArray &arr)
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Alti, Line 4, Alt2, Alt3
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();
	comboBox_AltDisp->setCurrentItem((uiValue & MASK_ALT_DISP) >> POS_ALT_DISP);
	comboBox_Line4->setCurrentItem((uiValue & MASK_LINE4_DISP) >> POS_LINE4_DISP);
	comboBox_Alt2Mode->setCurrentItem((uiValue & MASK_ALT2_MODE) >> POS_ALT2_MODE);
	comboBox_Alt3Mode->setCurrentItem((uiValue & MASK_ALT3_MODE) >> POS_ALT3_MODE);
}

void DisplayFrame6015Impl::store(QByteArray &arr)
{
	Flytec6015 *pDev;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Alti, Line 4, Alt2, Alt3
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();
	uiValue &= ~(MASK_ALT_DISP | MASK_LINE4_DISP | MASK_ALT2_MODE | MASK_ALT3_MODE);
	uiValue |= (comboBox_AltDisp->currentItem() << POS_ALT_DISP);
	uiValue |= (comboBox_Line4->currentItem() << POS_LINE4_DISP);
	uiValue |= (comboBox_Alt2Mode->currentItem() << POS_ALT2_MODE);
	uiValue |= (comboBox_Alt3Mode->currentItem() << POS_ALT3_MODE);
	pDev->memoryWrite(MemFa, DIV_FLAGS, UInt16, uiValue);
}

#include "DisplayFrame6015Impl.moc"

