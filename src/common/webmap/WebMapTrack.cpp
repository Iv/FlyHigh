/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
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
#include <QStyleOption>
#include <math.h>
#include "WebMap.h"
#include "WebMapTrack.h"
#include "WebMapTurnPoint.h"

WebMapTrack::WebMapTrack(const WayPoint::WayPointListType &wpList)
	:WebMapItem(WebMapTrackType)
{
//	setFlag(ItemIsSelectable);
//	setAcceptedMouseButtons(Qt::LeftButton);
	m_wpList = wpList;
	adjust();
}

WebMapTrack::~WebMapTrack()
{
}

void WebMapTrack::adjust()
{
	if(m_wayPoints.size() == 0)
			return;

	prepareGeometryChange();
	recalcWayPoints();
	update();

/**
	if(!m_pSrcTp || !m_pDstTp)
			return;

	prepareGeometryChange();

	m_srcPoint = mapFromItem(m_pSrcTp, 0, 0);
	m_dstPoint = mapFromItem(m_pDstTp, 0, 0);

	update();
*/
}

QRectF WebMapTrack::boundingRect() const
{
	qreal extra;
	
	if(m_wayPoints.size() == 0)
			return QRectF();

	extra = (getPenWidth() / 2.0);

	return m_wayPoints.boundingRect()
			.normalized()
			.adjusted(-extra, -extra, extra, extra);

/**
	if(!m_pSrcTp || !m_pDstTp)
			return QRectF();

	qreal penWidth = 1;
	qreal extra = (penWidth + m_arrowSize) / 2.0;

	return QRectF(m_srcPoint, QSizeF(m_dstPoint.x() - m_srcPoint.x(),
																		m_dstPoint.y() - m_srcPoint.y()))
			.normalized()
			.adjusted(-extra, -extra, extra, extra);
*/
}

QPainterPath WebMapTrack::shape() const
{
	QPainterPath path;
	QRectF rect;
	qreal extra;

	extra = (getPenWidth() / 2.0);

	rect = m_wayPoints.boundingRect();
	rect.adjust(-extra, -extra, extra, extra);
	path.addRect(rect);

	return path;

/**
	QPainterPath path;

	QLineF line(m_srcPoint, m_dstPoint);

	path.addPolygon(QPolygonF() << line.p1() << line.p2());

	return path;
*/
}

void WebMapTrack::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
	if(m_wayPoints.size() == 0)
			return;

	pPainter->setPen(QPen(QColor(255, 170, 0), getPenWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//	pPainter->setBrush(Qt::black);
	pPainter->drawPolygon(m_wayPoints);

/**
	QPointF arrowP1;
	QPointF arrowP2;
	double angle;

	if(!m_pSrcTp || !m_pDstTp)
			return;

	// Draw the line itself
	QLineF line(m_srcPoint, m_dstPoint);

	if(pOption->state & QStyle::State_Sunken)
	{
		pPainter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	}
	else if(m_glowEn)
	{
		pPainter->setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	}
	else
	{
		pPainter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	}

	pPainter->drawLine(line);

	// Draw the arrows if there's enough room
	if(m_arrowEn)
	{
		angle = ::acos(line.dx() / line.length());
	
		if(line.dy() >= 0)
		{
			angle = 2.0 * M_PI - angle;
		}
	
		arrowP1 = m_dstPoint + QPointF(sin(angle - M_PI / 3) * m_arrowSize,
									cos(angle - M_PI / 3) * m_arrowSize);
		arrowP2 = m_dstPoint + QPointF(sin(angle - M_PI + M_PI / 3) * m_arrowSize,
									cos(angle - M_PI + M_PI / 3) * m_arrowSize);
	
		pPainter->setBrush(Qt::black);
		pPainter->drawPolygon(QPolygonF() << line.p2() << arrowP1 << arrowP2);
	}
*/
}

void WebMapTrack::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
	update();
	QGraphicsItem::mousePressEvent(pEvent);
}

void WebMapTrack::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
	update();
	QGraphicsItem::mouseReleaseEvent(pEvent);
}

void WebMapTrack::recalcWayPoints()
{
	int listSize;
	int wpNr;
	double lat;
	double lon;
	int x;
	int y;

	listSize = m_wpList.size();

	if(listSize > 1)
	{
		m_wayPoints.resize(listSize);

		m_n = m_wpList[0].latitude();
		m_e = m_wpList[0].longitude();
		m_s = m_wpList[0].latitude();
		m_w = m_wpList[0].longitude();
	
		for(wpNr=0; wpNr<listSize; wpNr++)
		{
			lat = m_wpList[wpNr].latitude();
			lon = m_wpList[wpNr].longitude();

			// latitude
			if(lat > m_n)
			{
				m_n = lat;
			}
			else if(lat < m_s)
			{
				m_s = lat;
			}
			
			// longitude
			if(lon > m_e)
			{
				m_e = lon;
			}
			else if(lon < m_w)
			{
				m_w = lon;
			}

			getMap()->getPointFromLatLon(lat, lat, x, y);
			m_wayPoints[wpNr] = QPointF(x, y);
		}
	}
}
