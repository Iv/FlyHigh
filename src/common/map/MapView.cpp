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

#include "MapView.h"
#include <qpainter.h>
#include <qpoint.h>
#include "SwissMap100.h"
#include "Map.h"
#include "MapTile.h"
#include "ProgressDlg.h"

MapView::MapView(QWidget *parent, const char *name, int wFlags)
	:QScrollView(parent, name, wFlags)
{
	m_pMap = NULL;
	m_maxAlt = 0;
	m_minAlt = 0;
}

MapView::~MapView()
{
	if(m_pMap != NULL)
	{
		delete m_pMap;
	}
}

void MapView::shwoWayPointList(WayPoint::WayPointListType &wpList)
{
	ProgressDlg progDlg(this);
	QRect rect;
	double n;
	double e;
	double s;
	double w;
	int listSize;
	int wpNr;

	m_wpList = wpList;
	listSize = wpList.size();

	if(listSize > 0)
	{
		n = wpList[0].latitude();
		e = wpList[0].longitude();
		s = wpList[0].latitude();
		w = wpList[0].longitude();
		m_maxAlt = wpList[0].altitude();
		m_minAlt = wpList[0].altitude();
	
		for(wpNr=0; wpNr<listSize; wpNr++)
		{
			// latitude
			if(wpList[wpNr].latitude() > n)
			{
				n = wpList[wpNr].latitude();
			}
			else if(wpList[wpNr].latitude() < s)
			{
				s = wpList[wpNr].latitude();
			}
			
			// longitude
			if(wpList[wpNr].longitude() > e)
			{
				e = wpList[wpNr].longitude();
			}
			else if(wpList[wpNr].longitude() < w)
			{
				w = wpList[wpNr].longitude();
			}

			// altitude
			if(wpList[wpNr].altitude() > m_maxAlt)
			{
				m_maxAlt = wpList[wpNr].altitude();
			}
			else if(wpList[wpNr].altitude() < m_minAlt)
			{
				m_minAlt = wpList[wpNr].altitude();
			}
		}

		// map
		m_pMap = new SwissMap100("/var/opt/flyhigh/swissmap100/");
		progDlg.beginProgress("reading map tiles...", m_pMap);
		m_pMap->loadMap(n, e, s, w);
		progDlg.endProgress();

		// waypoints
		recalcWayPoints();
		m_pMap->pixRect(rect);
		resizeContents(rect.width(), rect.height());
		repaintContents();
	}
}

void MapView::keyPressEvent(QKeyEvent * e)
{
	QRect rect;

	switch(e->key())
	{
		case Qt::Key_Plus:
			m_pMap->setZoom(m_pMap->zoom() - 1);
			recalcWayPoints();
			m_pMap->pixRect(rect);
			resizeContents(rect.width(), rect.height());
			repaintContents();
		break;
		case Qt::Key_Minus:
			m_pMap->setZoom(m_pMap->zoom() + 1);
			recalcWayPoints();
			m_pMap->pixRect(rect);
			resizeContents(rect.width(), rect.height());
			repaintContents();
		break;
	}
}

void MapView::drawContents(QPainter* p, int cx, int cy, int cw, int ch)
{
	int rowNr;
	int rows;
	int colNr;
	int cols;
	int pts;
	int wpNr;
	int alt;
	MapTile *pTile;
	int xOffset = 0;
	int yOffset = 0;
	int pixX;
	int pixY;
	QColor color;
	uint hue;

	// map
	rows = m_pMap->tileMatrix().size();
	m_pMap->pixTileSize(pixX, pixY);

	for(rowNr=0; rowNr<rows; rowNr++)
	{
		cols = m_pMap->tileMatrix()[rowNr].size();

		if((yOffset > (cy - pixY)) && (yOffset < (cy + ch + pixY)))
		{
			for(colNr=0; colNr<cols; colNr++)
			{
				if((xOffset > (cx - pixX)) && (xOffset < (cx + cw + pixX)))
				{
					pTile = m_pMap->tileMatrix()[rowNr][colNr];

					if(pTile != NULL)
					{
						p->drawImage(xOffset, yOffset, pTile->image());
					}
				}
				xOffset += pixX;
			}
		}
		xOffset = 0;
		yOffset += pixY;
	}

	// waypoints
	pts = m_wayPoints.size();

	if(pts > 2)
	{

 
		// start and end point
		p->setPen(QPen(Qt::green, 10));
		p->drawEllipse(m_wayPoints[0].x(), m_wayPoints[0].y(), 5, 5);
		p->setPen(QPen(Qt::red, 10));
		p->drawEllipse(m_wayPoints[pts-1].x(), m_wayPoints[pts-1].y(), 5, 5);

		// other points
		for(wpNr=1; wpNr<(pts-2); wpNr++)
		{
			alt = m_wpList[wpNr].altitude();
			hue = (alt - m_minAlt) * (100 - 0) / (m_maxAlt - m_minAlt);
			color.setHsv(hue, 255, 255 );
			p->setPen(QPen(color, 4));
			p->drawLine(m_wayPoints[wpNr], m_wayPoints[wpNr+1]);
		}
	}
}

void MapView::recalcWayPoints()
{
	int listSize;
	int wpNr;
	double lat;
	double lon;
	QPoint pt;

	listSize = m_wpList.size();
	m_wayPoints.resize(listSize);

	for(wpNr=0; wpNr<listSize; wpNr++)
	{
		lat = m_wpList[wpNr].latitude();
		lon = m_wpList[wpNr].longitude();
		m_pMap->LLtoPix(lat, lon, pt);
		m_wayPoints.setPoint(wpNr, pt);
	}
}
