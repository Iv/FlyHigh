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

WebMapTrack::WebMapTrack()
	:WebMapItem(WebMapTrackType)
{
//	setFlag(ItemIsSelectable);
//	setAcceptedMouseButtons(Qt::LeftButton);
	setPenWidth(3.0);
	setPos(0, 0);
	m_trackToMap = false;
	m_tpsToMap = false;
	m_tri = false;
}

WebMapTrack::~WebMapTrack()
{
}

void WebMapTrack::setWayPointList(const WayPoint::WayPointListType &wpList)
{
	m_trackToMap = true;
	m_wpList = wpList;
	calcBounds();

	if(getMap()->isMapReady())
	{
		setWayPointListToMap();
	}
}

void WebMapTrack::setTurnPointList(const FlightPointList &tpList, bool tri)
{
	m_tpsToMap = true;
	m_tpList = tpList;
	m_tri = tri;

	if(getMap()->isMapReady())
	{
		setTurnPointListToMap();
	}
}

void WebMapTrack::setWayPointListToMap()
{
	PolyLineEncoder encoder;
	QString encPoints;
	QString encLevels;
	int wpNr;
	double lat;
	double lon;
	int x;
	int y;

	if(m_trackToMap && (m_wpList.size() > 1))
	{
		m_trackToMap = false;
		encoder.dpEncode(m_wpList, encPoints, encLevels);
		getMap()->setPolyLine(encPoints, encLevels, 3, "#FF0000");
	}
}

void WebMapTrack::setTurnPointListToMap()
{
	PolyLineEncoder encoder;
	QString encPoints;
	QString encLevels;
	WayPoint::WayPointListType tpList;
	int tpNr;

	if(m_tpsToMap && (m_tpList.size() == 5))
	{
		m_tpsToMap = false;

		// Marker
		getMap()->setMarker(m_tpList[0].wp.latitude(), m_tpList[0].wp.longitude(), WebMap::MarkerStart);
		getMap()->setMarker(m_tpList[1].wp.latitude(), m_tpList[1].wp.longitude(), WebMap::MarkerTp1);
		getMap()->setMarker(m_tpList[2].wp.latitude(), m_tpList[2].wp.longitude(), WebMap::MarkerTp2);
		getMap()->setMarker(m_tpList[3].wp.latitude(), m_tpList[3].wp.longitude(), WebMap::MarkerTp3);
		getMap()->setMarker(m_tpList[4].wp.latitude(), m_tpList[4].wp.longitude(), WebMap::MarkerLand);
	
		// Turnpoints
		if(m_tri)
		{
			// Triangle
			for(tpNr=1; tpNr<=3; tpNr++)
			{
				tpList.push_back(m_tpList[tpNr].wp);
			}

			tpList.push_back(m_tpList[1].wp);
			encoder.dpEncode(tpList, encPoints, encLevels);
			getMap()->setPolyLine(encPoints, encLevels, 2, "#FFFFFF");
			tpList.clear();

			// S to 1
			tpList.push_back(m_tpList[0].wp);
			tpList.push_back(m_tpList[1].wp);
			encoder.dpEncode(tpList, encPoints, encLevels);
			getMap()->setPolyLine(encPoints, encLevels, 1, "#FFFFFF");
			tpList.clear();

			// 3 to L
			tpList.push_back(m_tpList[3].wp);
			tpList.push_back(m_tpList[4].wp);
			encoder.dpEncode(tpList, encPoints, encLevels);
			getMap()->setPolyLine(encPoints, encLevels, 1, "#FFFFFF");
			tpList.clear();
		}
		else
		{
			for(tpNr=0; tpNr<m_tpList.size(); tpNr++)
			{
				tpList.push_back(m_tpList[tpNr].wp);
			}

			encoder.dpEncode(tpList, encPoints, encLevels);
			getMap()->setPolyLine(encPoints, encLevels, 2, "#FFFFFF");
			tpList.clear();
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
/**
	if(m_wpList.size() == 0)
			return;

	prepareGeometryChange();
	setWayPointListToMap();
	update();

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
