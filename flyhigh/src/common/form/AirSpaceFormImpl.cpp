/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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
 
#include <qbrush.h>
#include <qheader.h>
#include <qpainter.h>
#include <qtable.h>
#include <qtextedit.h>
#include "AirSpace.h"
#include "AirSpaceFormImpl.h"

AirSpaceFormImpl::AirSpaceFormImpl(QWidget* parent, const QString &caption, AirSpace *pAirSpace)
	:AirSpaceForm(parent, caption)
{
	QHeader *pHeader;
	
	AirSpaceForm::setCaption(caption);
	tableEdgePoints->setColumnWidth(Use, 20);
	tableEdgePoints->setColumnWidth(Longitude, 95);
	tableEdgePoints->setColumnReadOnly(Longitude, true);
	tableEdgePoints->setColumnWidth(Latitude, 95);
	tableEdgePoints->setColumnReadOnly(Latitude, true);
	
	pHeader = tableEdgePoints->horizontalHeader();
	pHeader->setLabel(Use, "");
	pHeader->setLabel(Longitude, "Longitude\n[°,min]");
	pHeader->setLabel(Latitude, "Latitude\n[°,min]");

	setAirSpace(pAirSpace);
}

void AirSpaceFormImpl::setAirSpace(AirSpace *pAirSpace)
{
	QTableItem *pTabItem;
	uint sentNr;
	uint maxSent = 1;
	
	if(pAirSpace != NULL)
	{
		textEditComment->setText("Hallo");
		
		pTabItem = new QCheckTableItem(tableEdgePoints, "");
		
		for(sentNr=0; sentNr<maxSent; sentNr++)
		{
			tableEdgePoints->insertRows(sentNr);
			tableEdgePoints->setItem(sentNr, Use, pTabItem);
		}
		
	}
}

void AirSpaceFormImpl::paintEvent(QPaintEvent *pEvent)
{
	QPainter paint(this);

	QBrush b1(Qt::blue);
	QBrush b2(Qt::green, Qt::Dense6Pattern); // green 12% fill
	QBrush b3(Qt::NoBrush); // void brush
	QBrush b4(Qt::CrossPattern); // black cross pattern

	paint.setPen(Qt::red);
	paint.setBrush(b1);
	paint.drawRect(290, 100, 350, 350);
	/*
	paint.setBrush( b2 );
	paint.drawRoundRect( 10, 150, 200, 100, 20, 20 );
	paint.setBrush( b3 );
	paint.drawEllipse( 250, 10, 200, 100 );
	paint.setBrush( b4 );
	paint.drawPie( 250, 150, 200, 100, 45*16, 90*16 );
	*/
	
	AirSpaceForm::paintEvent(pEvent);
}

#include "AirSpaceFormImpl.moc"
