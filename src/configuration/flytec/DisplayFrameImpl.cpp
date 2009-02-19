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

#include "DisplayFrameImpl.h"
#include "Images.h"
extern "C"
{
	#include "flytec_al.h"
}

DisplayFrameImpl::DisplayFrameImpl(QWidget* parent, const char* name, WFlags fl)
: DisplayFrame(parent,name,fl)
{
	// not supported by 5020
	comboBox_Font->setEnabled(false);
	
	// Screen 1
	pixmapLabel_Display->setPixmap(Images::pInstance()->getImage("ft_5020.png"));
	insertItems(comboBox_UserField_0_0);
	insertItems(comboBox_UserField_0_1);
	insertItems(comboBox_UserField_0_2);
	
	// Screen 2
	pixmapLabel_Display_2->setPixmap(Images::pInstance()->getImage("ft_5020.png"));
	insertItems(comboBox_UserField_1_0);
	insertItems(comboBox_UserField_1_1);
	insertItems(comboBox_UserField_1_2);
	
	// Screen 3
	pixmapLabel_Display_3->setPixmap(Images::pInstance()->getImage("ft_5020.png"));
	insertItems(comboBox_UserField_2_0);
	insertItems(comboBox_UserField_2_1);
	insertItems(comboBox_UserField_2_2);
}

DisplayFrameImpl::~DisplayFrameImpl()
{
}

void DisplayFrameImpl::update(QByteArray &arr)
{
	int16_t i16value;

	i16value = arr[LCD_CONTRAST_POS] << 8;
	i16value += arr[LCD_CONTRAST_POS+1];
	slider_Contrast->setValue(i16value);

	comboBox_UserField_0_0->setCurrentItem(arr[USERFIELD_0_POS]);
	comboBox_UserField_0_1->setCurrentItem(arr[USERFIELD_0_POS+1]);
	comboBox_UserField_0_2->setCurrentItem(arr[USERFIELD_0_POS+2]);

	comboBox_UserField_1_0->setCurrentItem(arr[USERFIELD_1_POS]);
	comboBox_UserField_1_1->setCurrentItem(arr[USERFIELD_1_POS+1]);
	comboBox_UserField_1_2->setCurrentItem(arr[USERFIELD_1_POS+2]);
	
	comboBox_UserField_2_0->setCurrentItem(arr[USERFIELD_2_POS]);
	comboBox_UserField_2_1->setCurrentItem(arr[USERFIELD_2_POS+1]);
	comboBox_UserField_2_2->setCurrentItem(arr[USERFIELD_2_POS+2]);
}

void DisplayFrameImpl::store(QByteArray &arr)
{
	int16_t i16value;

	// LCD Contrast
	i16value = slider_Contrast->value();
	arr[LCD_CONTRAST_POS] = (u_char)(i16value >> 8);
	arr[LCD_CONTRAST_POS+1] = (u_char)(i16value & 0xFF);
	
	// User Field 0
	arr[USERFIELD_0_POS] = comboBox_UserField_0_0->currentItem();
	arr[USERFIELD_0_POS+1] = comboBox_UserField_0_1->currentItem();
	arr[USERFIELD_0_POS+2] = comboBox_UserField_0_2->currentItem();
	
	// User Field 1
	arr[USERFIELD_1_POS] = comboBox_UserField_1_0->currentItem();
	arr[USERFIELD_1_POS+1] = comboBox_UserField_1_1->currentItem();
	arr[USERFIELD_1_POS+2] = comboBox_UserField_1_2->currentItem();

	// User Field 2
	arr[USERFIELD_2_POS] = comboBox_UserField_2_0->currentItem();
	arr[USERFIELD_2_POS+1] = comboBox_UserField_2_1->currentItem();
	arr[USERFIELD_2_POS+2] = comboBox_UserField_2_2->currentItem();
}

void DisplayFrameImpl::insertItems( QComboBox * pItem )
{
	pItem->clear();
	pItem->insertItem( tr( "Wind spd" ) );
	pItem->insertItem( tr( "Time" ) );
	pItem->insertItem( tr( "Fl.Time" ) );
	pItem->insertItem( tr( "Gnd spd" ) );
	pItem->insertItem( tr( "Speed diff" ) );
	pItem->insertItem( tr( "Dist WP" ) );
	pItem->insertItem( tr( "Bearing" ) );
	pItem->insertItem( tr( "Track" ) );
	pItem->insertItem( tr( "Temp" ) );
	pItem->insertItem( tr( "Alt 2" ) );
	pItem->insertItem( tr( "Alt 3" ) );
	pItem->insertItem( tr( "QNH hPa" ) );
	pItem->insertItem( tr( "L/D Gnd" ) );
	pItem->insertItem( tr( "L/D air" ) );
	pItem->insertItem( tr( "L/D Req" ) );
	pItem->insertItem( tr( "Dist to ^" ) );
	pItem->insertItem( QString::null );
	pItem->insertItem( tr( "Alt @ BG" ) );
	pItem->insertItem( tr( "FL (ft)" ) );
	pItem->insertItem( tr( "Air spd" ) );
	pItem->insertItem( tr( "Alt @ goal" ) );
	pItem->insertItem( tr( "Dist goal" ) );
	pItem->insertItem( tr( "Vario" ) );
	pItem->insertItem( tr( "Alt 1" ) );
	pItem->insertItem( tr( "SMS p/t" ) );
	pItem->insertItem( tr( "Dist toff" ) );
	pItem->insertItem( tr( "Dist cyl" ) );
	pItem->insertItem( tr( "L/D rel goal" ) );
	pItem->insertItem( tr( "Race start" ) );
}



#include "DisplayFrameImpl.moc"

