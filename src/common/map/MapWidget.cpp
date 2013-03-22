/***************************************************************************
 *   Copyright (C) 2012 by Alex Graf                                       *
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

#include <QPainter>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>
#include "MapWidget.h"
#include "ProgressDlg.h"
#include "SwissMap100.h"

MapWidget::MapWidget(QScrollArea *pScrollArea)
{
  m_pMap = NULL;
  m_maxAlt = 0;
	m_minAlt = 0;
  m_pScrollArea = pScrollArea;
}

MapWidget::~MapWidget()
{
	if(m_pMap != NULL)
	{
		delete m_pMap;
	}
}

void MapWidget::showFlightPointList(const FlightPointList &fpList)
{
	ProgressDlg progDlg(this);
	QRect rect;
	double n;
	double e;
	double s;
	double w;
	int listSize;
	int wpNr;

	listSize = fpList.size();

	if(listSize > 0)
	{
		n = fpList[0]->pos().lat();
		e = fpList[0]->pos().lon();
		s = fpList[0]->pos().lat();
		w = fpList[0]->pos().lon();
		m_maxAlt = fpList[0]->alt();
		m_minAlt = fpList[0]->alt();

		for(wpNr=0; wpNr<listSize; wpNr++)
		{
		  m_fpList.push_back(new FlightPoint(fpList[wpNr]));

			// latitude
			if(fpList[wpNr]->pos().lat() > n)
			{
				n = fpList[wpNr]->pos().lat();
			}
			else if(fpList[wpNr]->pos().lat() < s)
			{
				s = fpList[wpNr]->pos().lat();
			}

			// longitude
			if(fpList[wpNr]->pos().lon() > e)
			{
				e = fpList[wpNr]->pos().lon();
			}
			else if(fpList[wpNr]->pos().lon() < w)
			{
				w = fpList[wpNr]->pos().lon();
			}

			// altitude
			if(fpList[wpNr]->alt() > m_maxAlt)
			{
				m_maxAlt = fpList[wpNr]->alt();
			}
			else if(fpList[wpNr]->alt() < m_minAlt)
			{
				m_minAlt = fpList[wpNr]->alt();
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
    resize(rect.width(), rect.height());
	}
}

void MapWidget::zoomOut()
{
  zoom(false);
}

void MapWidget::zoomIn()
{
  zoom(true);
}

void MapWidget::zoom(const QPoint &pt, bool in)
{
  QPoint ptAbs;
  double lat;
  double lon;
  int scrollX;
  int scrollY;

  scrollX = m_pScrollArea->horizontalScrollBar()->value();
  scrollY = m_pScrollArea->verticalScrollBar()->value();
  ptAbs.setX(pt.x() + scrollX);
  ptAbs.setY(pt.y() + scrollY);
  m_pMap->pixToLL(ptAbs, lat, lon);

  zoom(in);

  m_pMap->LLtoPix(lat, lon, ptAbs);
  scrollX = ptAbs.x() - pt.x();
  scrollY = ptAbs.y() - pt.y();
  m_pScrollArea->horizontalScrollBar()->setValue(scrollX);
  m_pScrollArea->verticalScrollBar()->setValue(scrollY);
}

void MapWidget::paintEvent(QPaintEvent *pEvent)
{
  QPainter painter(this);
	QColor color;
	MapTile *pTile;
	int rowNr;
	int rows;
	int colNr;
	int cols;
	int pts;
	int wpNr;
	int alt;
	int xOffset = 0;
	int yOffset = 0;
	int pixX;
	int pixY;
  int cx;
  int cy;
  int ch;
  int cw;

  if(m_pMap != NULL)
  {
    cx = m_pScrollArea->horizontalScrollBar()->value();
    cy = m_pScrollArea->verticalScrollBar()->value();
    ch = m_pScrollArea->widget()->height();
    cw = m_pScrollArea->widget()->width();

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
              painter.drawImage(xOffset, yOffset, pTile->image());
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
      painter.setPen(QPen(Qt::green, 10));
      painter.drawEllipse(m_wayPoints[0].x(), m_wayPoints[0].y(), 5, 5);
      painter.setPen(QPen(Qt::red, 10));
      painter.drawEllipse(m_wayPoints[pts-1].x(), m_wayPoints[pts-1].y(), 5, 5);

      // other points
      for(wpNr=1; wpNr<(pts-2); wpNr++)
      {
        alt = m_fpList[wpNr]->alt();

        if(alt < 0) color = QColor(0, 0, 0); // black
        else if(alt < 500) color = QColor(125, 0, 0); // brown
        else if(alt < 1000) color = QColor(255, 0, 0); // red
        else if(alt < 1500) color = QColor(255, 125, 0); // orange
        else if(alt < 2000) color = QColor(255, 200, 0); // dark orange
        else if(alt < 2500) color = QColor(255, 255, 0); // yellow
        else if(alt < 3000) color = QColor(230, 255, 0); // yellow-green
        else if(alt < 3500) color = QColor(160, 255, 0); // light green
        else if(alt < 4000) color = QColor(50, 255, 0); // green
        else color = QColor(255, 255, 255); // white

        painter.setPen(QPen(color, 4));
        painter.drawLine(m_wayPoints[wpNr], m_wayPoints[wpNr+1]);
      }
    }
  }
}

void MapWidget::zoom(bool in)
{
  QRect rect;

  if(in)
  {
    m_pMap->setZoom(m_pMap->zoom() - 1);
  }
  else
  {
    m_pMap->setZoom(m_pMap->zoom() + 1);
  }

  recalcWayPoints();
  m_pMap->pixRect(rect);
  resize(rect.width(), rect.height());
}

void MapWidget::recalcWayPoints()
{
	int listSize;
	int wpNr;
	double lat;
	double lon;
	QPoint pt;

	listSize = m_fpList.size();
	m_wayPoints.resize(listSize);

	for(wpNr=0; wpNr<listSize; wpNr++)
	{
		lat = m_fpList[wpNr]->pos().lat();
		lon = m_fpList[wpNr]->pos().lon();
		m_pMap->LLtoPix(lat, lon, pt);
		m_wayPoints[wpNr] = pt;
	}
}
