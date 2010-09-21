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
#include <q3table.h>
#include <q3header.h>
#include <qpainter.h>
#include <q3table.h>
#include <q3textedit.h>
//Added by qt3to4:
#include <Q3PointArray>
#include <QCloseEvent>
#include <QPaintEvent>
#include "AirSpace.h"
#include "AirSpaceFormImpl.h"

AirSpaceFormImpl::AirSpaceFormImpl(QWidget* parent, const QString &caption, AirSpace *pAirSpace)
	:AirSpaceForm(parent, caption),
	m_drawRect(290, 10, 350, 350)
{
	Q3Header *pHeader;
	
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
	QString str;
	Q3CheckTableItem *pTabItem;
	uint ptNr;
	uint maxPts;

	m_pAirSpace = pAirSpace;
	tableEdgePoints->setNumRows(0); // clear table
	
	if(pAirSpace != NULL)
	{
		AirSpaceForm::setCaption(m_pAirSpace->name());
		maxPts = m_pAirSpace->edgePointList().size();
		
		for(ptNr=0; ptNr<maxPts; ptNr++)
		{
			tableEdgePoints->insertRows(ptNr);
			pTabItem = new Q3CheckTableItem(tableEdgePoints, "");
			pTabItem->setChecked(true);
			tableEdgePoints->setItem(ptNr, Use, pTabItem);
			
			str.sprintf("%.5f", m_pAirSpace->edgePointList().at(ptNr).longitude());
			tableEdgePoints->setText(ptNr, Longitude, str);
			
			str.sprintf("%.5f", m_pAirSpace->edgePointList().at(ptNr).latitude());
			tableEdgePoints->setText(ptNr, Latitude, str);
		}
		repaint();
	}
}

void AirSpaceFormImpl::tableChanged(int row, int col)
{
	repaint();
}

void AirSpaceFormImpl::closeEvent(QCloseEvent * e)
{
	// prevent from closing
}

void AirSpaceFormImpl::paintEvent(QPaintEvent *pEvent)
{
	QPainter paint(this);
	Q3PointArray edgePts;
	QRect boundRect;
	Q3CheckTableItem *pTabItem;
	uint ptNr;
	uint maxPts;
	uint nPts = 0;
	int lat;
	int lon;
	int dx;
	int dy;
	double sx;
	double sy;
	
	if(m_pAirSpace != NULL)
	{
		// fill points to point array
		maxPts = m_pAirSpace->edgePointList().size();
		edgePts.resize(maxPts);
			
		for(ptNr=0; ptNr<maxPts; ptNr++)
		{
			pTabItem = (Q3CheckTableItem*)tableEdgePoints->item(ptNr, Use);
		
			if(pTabItem->isChecked())
			{
				lat = (int)(m_pAirSpace->edgePointList().at(ptNr).longitude() * 1000);
				lon = -(int)(m_pAirSpace->edgePointList().at(ptNr).latitude() * 1000); // y axis is inverse
				edgePts.setPoint(nPts, lat, lon);
				nPts++;
			}
		}
		
		edgePts.resize(nPts);
		
		// calc translation and scale
		boundRect = edgePts.boundingRect();
		dx = boundRect.left() - m_drawRect.left();
		dy = boundRect.top() - m_drawRect.top();
		sx = (double)(boundRect.right() - boundRect.left()) / (double)(m_drawRect.right() - m_drawRect.left());
		sy=  (double)(boundRect.bottom() - boundRect.top()) / (double)(m_drawRect.bottom() - m_drawRect.top());
		
		// translate and scale to draw rect
		edgePts.translate(-boundRect.left(), -boundRect.top());
		scaleEdgePts(edgePts, sx, sy);
		edgePts.translate(m_drawRect.left(), m_drawRect.top());
		
		// draw
		paint.setPen(Qt::black);
		paint.drawPolyline(edgePts);
	}
	
	AirSpaceForm::paintEvent(pEvent);
}

void AirSpaceFormImpl::scaleEdgePts(Q3PointArray &edgePts, double sx, double sy)
{
	uint ptNr;
	uint maxPts;
	double scale;
	
	if(sx > sy)
	{
		scale = sx;
	}
	else
	{
		scale = sy;
	}
	
	maxPts = edgePts.size();
	
	for(ptNr=0; ptNr<maxPts; ptNr++)
	{
		edgePts[ptNr] /= scale;
	}
}

#include "moc_AirSpaceFormImpl.cxx"
