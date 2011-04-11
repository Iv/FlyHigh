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

#include <qtabwidget.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qimage.h>
#include <qpixmap.h>
#include "PolareFrameImpl.h"

PolareFrameImpl::PolareFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
  :QWidget(parent)
{
  setupUi(this);

	// these are not supportet by 5020
  tabWidget->widget(1)->setEnabled(false);
	spinBox_Altitude1->setEnabled(false);
  pixmapLabel_Polare1->setPixmap(QPixmap(":/polare.png"));
  pixmapLabel_Polare2->setPixmap(QPixmap(":/polare.png"));
}

PolareFrameImpl::~PolareFrameImpl()
{
}

void PolareFrameImpl::update(QByteArray &arr)
{
	(void)arr;
/*
	// Polare 0
	spinBox_Sink1_0->setValue(arr[POLARE_SINK_1_0_POS]);
	spinBox_Speed1_0->setValue(arr[POLARE_SPEED_1_0_POS]);

	// Polare 1
	spinBox_Sink1_1->setValue(arr[POLARE_SINK_1_1_POS]);
	spinBox_Speed1_1->setValue(arr[POLARE_SPEED_1_1_POS]);
*/
}

void PolareFrameImpl::store(QByteArray &arr)
{
	(void)arr;
/*
	// Polare 0
	arr[POLARE_SINK_1_0_POS] = spinBox_Sink1_0->value();
	arr[POLARE_SPEED_1_0_POS] = spinBox_Speed1_0->value();

	// Polare 1
	arr[POLARE_SINK_1_1_POS] = spinBox_Sink1_1->value();
	arr[POLARE_SPEED_1_1_POS] = spinBox_Speed1_1->value();
*/
}
