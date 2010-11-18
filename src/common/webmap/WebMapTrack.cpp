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
#include "PolyLineEncoder.h"
#include "WebMap.h"
#include "WebMapTrack.h"
#include "WebMapTurnPoint.h"

WebMapTrack::WebMapTrack(const WayPoint::WayPointListType &wpList)
	:WebMapItem(WebMapTrackType)
{
//	setFlag(ItemIsSelectable);
//	setAcceptedMouseButtons(Qt::LeftButton);
	m_wpList = wpList;
	setPenWidth(3.0);
	setPos(0, 0);
}

WebMapTrack::~WebMapTrack()
{
}

void WebMapTrack::calcWayPoints()
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
/**
		m_wayPoints.resize(listSize);
	
		for(wpNr=0; wpNr<listSize; wpNr++)
		{
			lat = m_wpList[wpNr].latitude();
			lon = m_wpList[wpNr].longitude();
			getMap()->getPointFromLatLon(lat, lon, x, y);
			m_wayPoints[wpNr] = QPointF(x, y);
		}
*/
		PolyLineEncoder encoder;
		QString encPoints;
		QString encLevels;

		encoder.dpEncode(m_wpList, encPoints, encLevels);
		getMap()->setPolyLine(encPoints, encLevels);

		getMap()->setMarker(m_wpList[0].latitude(), m_wpList[0].longitude(), WebMap::MarkerStart);
		getMap()->setMarker(m_wpList[listSize - 1].latitude(), m_wpList[listSize - 1].longitude(), WebMap::MarkerEnd);
	}
}

void WebMapTrack::calcBounds()
{
	int listSize;
	int wpNr;
	double lat;
	double lon;

	listSize = m_wpList.size();

	if(listSize > 1)
	{
		m_north = m_wpList[0].latitude();
		m_east = m_wpList[0].longitude();
		m_south = m_wpList[0].latitude();
		m_west = m_wpList[0].longitude();
	
		for(wpNr=0; wpNr<listSize; wpNr++)
		{
			lat = m_wpList[wpNr].latitude();
			lon = m_wpList[wpNr].longitude();

			// latitude
			if(lat > m_north)
			{
				m_north = lat;
			}
			else if(lat < m_south)
			{
				m_south = lat;
			}
			
			// longitude
			if(lon > m_east)
			{
				m_east = lon;
			}
			else if(lon < m_west)
			{
				m_west = lon;
			}
		}
	}
}

qreal WebMapTrack::getNorth() const
{
	return m_north;
}

qreal WebMapTrack::getEast() const
{
	return m_east;
}

qreal WebMapTrack::getSouth() const
{
	return m_south;
}

qreal WebMapTrack::getWest() const
{
	return m_west;
}

void WebMapTrack::adjust()
{
	if(m_wpList.size() == 0)
			return;

	prepareGeometryChange();
	calcWayPoints();
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
	pPainter->drawPolyline(m_wayPoints);
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

QVariant WebMapTrack::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == QGraphicsItem::ItemPositionChange)
	{
//		calcWayPoints();

printf("pos = %i, %i\n", scenePos().x(), scenePos().y());
	}

	return QGraphicsItem::itemChange(change, value);
}

void WebMapTrack::updatePos()
{
/**
printf("begin calcWayPoints()\n");
	calcWayPoints();
printf("end calcWayPoints()\n");
	setPos(0, 0);

	update();
*/
}
