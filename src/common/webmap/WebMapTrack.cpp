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
#include <stdio.h>
#include "PolyLineEncoder.h"
#include "WebMap.h"
#include "WebMapTrack.h"
#include "WebMapTurnPoint.h"

WebMapTrack::WebMapTrack()
	:WebMapItem(WebMapTrackType)
{
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
		getMap()->setWayPointList(encPoints, encLevels, 3, "#FF0000");
	}
}

void WebMapTrack::setTurnPointListToMap()
{
	if(m_tpsToMap)
	{
		getMap()->setTurnPointList(m_tpList);
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
}

QRectF WebMapTrack::boundingRect() const
{
	return QRectF();
}

QPainterPath WebMapTrack::shape() const
{
	return QPainterPath();
}

void WebMapTrack::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
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
