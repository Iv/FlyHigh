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
#include "Flytec.h"

#include <stdio.h>

DisplayFrameImpl::DisplayFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
  :QWidget(parent)
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

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

  /* page 1 */

	// screen 1
  pixmapLabel_Display_1_1->setPixmap(QPixmap(":/ft_6020_p1.png"));
	insertItems(comboBox_UserField_1_1_1);
	insertItems(comboBox_UserField_1_1_2);
	insertItems(comboBox_UserField_1_1_3);

	// screen 2
  pixmapLabel_Display_1_2->setPixmap(QPixmap(":/ft_6020_p1.png"));
	insertItems(comboBox_UserField_1_2_1);
	insertItems(comboBox_UserField_1_2_2);
	insertItems(comboBox_UserField_1_2_3);

	// screen 3
  pixmapLabel_Display_1_3->setPixmap(QPixmap(":/ft_6020_p1.png"));
	insertItems(comboBox_UserField_1_3_1);
	insertItems(comboBox_UserField_1_3_2);
	insertItems(comboBox_UserField_1_3_3);

	/* page 3 */

  // screen 1
  pixmapLabel_Display_3_1->setPixmap(QPixmap(":/ft_6020_p3.png"));
	insertItems(comboBox_UserField_3_1_1);
	insertItems(comboBox_UserField_3_1_2);
	insertItems(comboBox_UserField_3_1_3);

	// screen 2
  pixmapLabel_Display_3_2->setPixmap(QPixmap(":/ft_6020_p3.png"));
	insertItems(comboBox_UserField_3_2_1);
	insertItems(comboBox_UserField_3_2_2);
	insertItems(comboBox_UserField_3_2_3);

	// screen 3
  pixmapLabel_Display_3_3->setPixmap(QPixmap(":/ft_6020_p3.png"));
	insertItems(comboBox_UserField_3_3_1);
	insertItems(comboBox_UserField_3_3_2);
	insertItems(comboBox_UserField_3_3_3);
}

DisplayFrameImpl::~DisplayFrameImpl()
{
}

void DisplayFrameImpl::update()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// LCD Contrast
	switch(pFlytec->deviceId())
	{
	  case IFlyHighRC::DevFlytec5020:
      slider_Contrast->setValue(pFlytec->parRead(MemUndef, LCD_CONTRAST_POS, FtInt16).toInt());
    break;
    case IFlyHighRC::DevFlytec6020:
      slider_Contrast->setValue(pFlytec->parRead(MemUndef, LCD_CONTRAST_POS, FtUInt8).toUInt());
    break;
    default:
    break;
	}

	/* page 1 */

	// User Field 0
  comboBox_UserField_1_1_1->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_0_POS, FtUInt8).toUInt());
  comboBox_UserField_1_1_2->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_0_POS + 1, FtUInt8).toUInt());
  comboBox_UserField_1_1_3->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_0_POS + 2, FtUInt8).toUInt());

	// User Field 1
  comboBox_UserField_1_2_1->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_1_POS, FtUInt8).toUInt());
  comboBox_UserField_1_2_2->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_1_POS + 1, FtUInt8).toUInt());
  comboBox_UserField_1_2_3->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_1_POS + 2, FtUInt8).toUInt());

	// User Field 2
  comboBox_UserField_1_3_1->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_2_POS, FtUInt8).toUInt());
  comboBox_UserField_1_3_2->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_2_POS + 1, FtUInt8).toUInt());
  comboBox_UserField_1_3_3->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_2_POS + 2, FtUInt8).toUInt());

  /* page 3 */

  // User Field 0
  comboBox_UserField_3_1_1->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_0_POS + 3, FtUInt8).toUInt());
  comboBox_UserField_3_1_2->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_0_POS + 4, FtUInt8).toUInt());
  comboBox_UserField_3_1_3->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_0_POS + 5, FtUInt8).toUInt());

	// User Field 1
  comboBox_UserField_3_2_1->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_1_POS + 3, FtUInt8).toUInt());
  comboBox_UserField_3_2_2->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_1_POS + 4, FtUInt8).toUInt());
  comboBox_UserField_3_2_3->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_1_POS + 5, FtUInt8).toUInt());

	// User Field 2
  comboBox_UserField_3_3_1->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_2_POS + 3, FtUInt8).toUInt());
  comboBox_UserField_3_3_2->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_2_POS + 4, FtUInt8).toUInt());
  comboBox_UserField_3_3_3->setCurrentIndex(pFlytec->parRead(MemUndef, USERFIELD_2_POS + 5, FtUInt8).toUInt());
}

void DisplayFrameImpl::store()
{
  Flytec *pFlytec;

  pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());

	// LCD Contrast
	switch(pFlytec->deviceId())
	{
	  case IFlyHighRC::DevFlytec5020:
      pFlytec->parWrite(MemUndef, LCD_CONTRAST_POS, FtInt16, slider_Contrast->value());
    break;
    case IFlyHighRC::DevFlytec6020:
      pFlytec->parWrite(MemUndef, LCD_CONTRAST_POS, FtUInt8, slider_Contrast->value());
    break;
    default:
    break;
	}

	/* page 1 */

	// User Field 0
  pFlytec->parWrite(MemUndef, USERFIELD_0_POS, FtUInt8, comboBox_UserField_1_1_1->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_0_POS + 1, FtUInt8, comboBox_UserField_1_1_2->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_0_POS + 2, FtUInt8, comboBox_UserField_1_1_3->currentIndex());

	// User Field 1
  pFlytec->parWrite(MemUndef, USERFIELD_1_POS, FtUInt8, comboBox_UserField_1_2_1->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_1_POS + 1, FtUInt8, comboBox_UserField_1_2_2->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_1_POS + 2, FtUInt8, comboBox_UserField_1_2_3->currentIndex());

	// User Field 2
  pFlytec->parWrite(MemUndef, USERFIELD_2_POS, FtUInt8, comboBox_UserField_1_3_1->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_2_POS + 1, FtUInt8, comboBox_UserField_1_3_2->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_2_POS + 2, FtUInt8, comboBox_UserField_1_3_3->currentIndex());

  /* page 3 */

  // User Field 0
  pFlytec->parWrite(MemUndef, USERFIELD_0_POS + 3, FtUInt8, comboBox_UserField_3_1_1->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_0_POS + 4, FtUInt8, comboBox_UserField_3_1_2->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_0_POS + 5, FtUInt8, comboBox_UserField_3_1_3->currentIndex());

	// User Field 1
  pFlytec->parWrite(MemUndef, USERFIELD_1_POS + 3, FtUInt8, comboBox_UserField_3_2_1->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_1_POS + 4, FtUInt8, comboBox_UserField_3_2_2->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_1_POS + 5, FtUInt8, comboBox_UserField_3_2_3->currentIndex());

	// User Field 2
  pFlytec->parWrite(MemUndef, USERFIELD_2_POS + 3, FtUInt8, comboBox_UserField_3_3_1->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_2_POS + 4, FtUInt8, comboBox_UserField_3_3_2->currentIndex());
  pFlytec->parWrite(MemUndef, USERFIELD_2_POS + 5, FtUInt8, comboBox_UserField_3_3_3->currentIndex());
}

void DisplayFrameImpl::insertItems(QComboBox * pItem)
{
  Flytec *pFlytec;

	pFlytec = static_cast<Flytec*>(IGPSDevice::pInstance());
	pItem->clear();

	switch(pFlytec->deviceId())
	{
    case IFlyHighRC::DevFlytec5020:
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
    break;
    case IFlyHighRC::DevFlytec6020:
      pItem->addItem( tr( "Time" ) );
      pItem->addItem( tr( "Fl.Time" ) );
      pItem->addItem( tr( "Vario" ) );
      pItem->addItem( tr( "Alt 1" ) );
      pItem->addItem( tr( "Alt 1 (ft)" ) );
      pItem->addItem( tr( "Alt 2" ) );
      pItem->addItem( tr( "Alt 3" ) );
      pItem->addItem( tr( "FL (ft)" ) );
      pItem->addItem( tr( "QNH hPa" ) );
      pItem->addItem( tr( "Gnd Spd" ) );
      pItem->addItem( tr( "Air Spd" ) );
      pItem->addItem( tr( "Wind Spd" ) );
      pItem->addItem( tr( "Wind Comp" ) );
      pItem->addItem( tr( "Track" ) );
      pItem->addItem( tr( "Bearing" ) );
      pItem->addItem( tr( "Dist WP" ) );
      pItem->addItem( tr( "Dist goal" ) );
      pItem->addItem( tr( "Dist Toff" ) );
      pItem->addItem( tr( "Dist Cyl" ) );
      pItem->addItem( tr( "Dist therm" ) );
      pItem->addItem( tr( "L/D gnd" ) );
      pItem->addItem( tr( "L/D air" ) );
      pItem->addItem( tr( "L/D req" ) );
      pItem->addItem( tr( "Alt BG WP" ) );
      pItem->addItem( tr( "Alt BG Goal" ) );
      pItem->addItem( tr( "Temp" ) );
      pItem->addItem( tr( "SMS p/t" ) );
      pItem->addItem( QString::null );
      pItem->addItem( tr( "Start Race" ) );
      pItem->addItem( tr( "Alt GPS" ) );
      pItem->addItem( tr( "L/D goal" ) );
      pItem->addItem( tr( "Dist CTR" ) );
      pItem->addItem("FLARM");
      pItem->addItem( tr( "Dist opt WP" ) );
      pItem->addItem( tr( "XT Error" ) );
    break;
    default:
    break;
	}
}
