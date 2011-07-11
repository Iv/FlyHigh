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
#include <qtabwidget.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qslider.h>

#include "DisplayFrameImpl.h"
#include "Flytec5020.h"

DisplayFrameImpl::DisplayFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
  :QWidget(parent)
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

  setupUi(this);

  // contrast max
  switch(pFlytec->deviceId())
	{
	  case IFlyHighRC::DevFlytec5020:
      slider_Contrast->setMaximum(1000);
    break;
    case IFlyHighRC::DevFlytec6020:
      slider_Contrast->setMaximum(100);
    break;
    default:
    break;
	}

	// not supported by 5020
	comboBox_Font->setEnabled(false);

	// Screen 1
  pixmapLabel_Display->setPixmap(QPixmap(":/ft_5020.png"));
	insertItems(comboBox_UserField_0_0);
	insertItems(comboBox_UserField_0_1);
	insertItems(comboBox_UserField_0_2);

	// Screen 2
  pixmapLabel_Display_2->setPixmap(QPixmap(":/ft_5020.png"));
	insertItems(comboBox_UserField_1_0);
	insertItems(comboBox_UserField_1_1);
	insertItems(comboBox_UserField_1_2);

	// Screen 3
  pixmapLabel_Display_3->setPixmap(QPixmap(":/ft_5020.png"));
	insertItems(comboBox_UserField_2_0);
	insertItems(comboBox_UserField_2_1);
	insertItems(comboBox_UserField_2_2);
}

DisplayFrameImpl::~DisplayFrameImpl()
{
}

void DisplayFrameImpl::update()
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// LCD Contrast
	switch(pFlytec->deviceId())
	{
	  case IFlyHighRC::DevFlytec5020:
      slider_Contrast->setValue(pFlytec->parRead(LCD_CONTRAST_POS, FtInt16).toInt());
    break;
    case IFlyHighRC::DevFlytec6020:
      slider_Contrast->setValue(pFlytec->parRead(LCD_CONTRAST_POS, FtUInt8).toUInt());
    break;
    default:
    break;
	}

	// User Field 0
  comboBox_UserField_0_0->setCurrentIndex(pFlytec->parRead(USERFIELD_0_POS, FtUInt8).toUInt());
  comboBox_UserField_0_1->setCurrentIndex(pFlytec->parRead(USERFIELD_0_POS + 1, FtUInt8).toUInt());
  comboBox_UserField_0_2->setCurrentIndex(pFlytec->parRead(USERFIELD_0_POS + 2, FtUInt8).toUInt());

	// User Field 1
  comboBox_UserField_1_0->setCurrentIndex(pFlytec->parRead(USERFIELD_1_POS, FtUInt8).toUInt());
  comboBox_UserField_1_1->setCurrentIndex(pFlytec->parRead(USERFIELD_1_POS + 1, FtUInt8).toUInt());
  comboBox_UserField_1_2->setCurrentIndex(pFlytec->parRead(USERFIELD_1_POS + 2, FtUInt8).toUInt());

	// User Field 2
  comboBox_UserField_2_0->setCurrentIndex(pFlytec->parRead(USERFIELD_2_POS, FtUInt8).toUInt());
  comboBox_UserField_2_1->setCurrentIndex(pFlytec->parRead(USERFIELD_2_POS + 1, FtUInt8).toUInt());
  comboBox_UserField_2_2->setCurrentIndex(pFlytec->parRead(USERFIELD_2_POS + 2, FtUInt8).toUInt());
}

void DisplayFrameImpl::store()
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// LCD Contrast
	switch(pFlytec->deviceId())
	{
	  case IFlyHighRC::DevFlytec5020:
      pFlytec->parWrite(LCD_CONTRAST_POS, FtInt16, slider_Contrast->value());
    break;
    case IFlyHighRC::DevFlytec6020:
      pFlytec->parWrite(LCD_CONTRAST_POS, FtUInt8, slider_Contrast->value());
    break;
    default:
    break;
	}

	// User Field 0
  pFlytec->parWrite(USERFIELD_0_POS, FtUInt8, comboBox_UserField_0_0->currentIndex());
  pFlytec->parWrite(USERFIELD_0_POS + 1, FtUInt8, comboBox_UserField_0_1->currentIndex());
  pFlytec->parWrite(USERFIELD_0_POS + 2, FtUInt8, comboBox_UserField_0_2->currentIndex());

	// User Field 1
  pFlytec->parWrite(USERFIELD_1_POS, FtUInt8, comboBox_UserField_1_0->currentIndex());
  pFlytec->parWrite(USERFIELD_1_POS + 1, FtUInt8, comboBox_UserField_1_1->currentIndex());
  pFlytec->parWrite(USERFIELD_1_POS + 2, FtUInt8, comboBox_UserField_1_2->currentIndex());

	// User Field 2
  pFlytec->parWrite(USERFIELD_2_POS, FtUInt8, comboBox_UserField_2_0->currentIndex());
  pFlytec->parWrite(USERFIELD_2_POS + 1, FtUInt8, comboBox_UserField_2_1->currentIndex());
  pFlytec->parWrite(USERFIELD_2_POS + 2, FtUInt8, comboBox_UserField_2_2->currentIndex());
}

void DisplayFrameImpl::insertItems(QComboBox * pItem)
{
	pItem->clear();
  pItem->addItem( tr( "Wind spd" ) );
  pItem->addItem( tr( "Time" ) );
  pItem->addItem( tr( "Fl.Time" ) );
  pItem->addItem( tr( "Gnd spd" ) );
  pItem->addItem( tr( "Speed diff" ) );
  pItem->addItem( tr( "Dist WP" ) );
  pItem->addItem( tr( "Bearing" ) );
  pItem->addItem( tr( "Track" ) );
  pItem->addItem( tr( "Temp" ) );
  pItem->addItem( tr( "Alt 2" ) );
  pItem->addItem( tr( "Alt 3" ) );
  pItem->addItem( tr( "QNH hPa" ) );
  pItem->addItem( tr( "L/D Gnd" ) );
  pItem->addItem( tr( "L/D air" ) );
  pItem->addItem( tr( "L/D Req" ) );
  pItem->addItem( tr( "Dist to ^" ) );
  pItem->addItem( QString::null );
  pItem->addItem( tr( "Alt @ BG" ) );
  pItem->addItem( tr( "FL (ft)" ) );
  pItem->addItem( tr( "Air spd" ) );
  pItem->addItem( tr( "Alt @ goal" ) );
  pItem->addItem( tr( "Dist goal" ) );
  pItem->addItem( tr( "Vario" ) );
  pItem->addItem( tr( "Alt 1" ) );
  pItem->addItem( tr( "SMS p/t" ) );
  pItem->addItem( tr( "Dist toff" ) );
  pItem->addItem( tr( "Dist cyl" ) );
  pItem->addItem( tr( "L/D rel goal" ) );
  pItem->addItem( tr( "Race start" ) );
}
