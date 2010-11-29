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

#ifndef _WebMapTrack_h
#define _WebMapTrack_h

#include <QPolygonF>
#include "FlightPointList.h"
#include "WebMapItem.h"
#include "WayPoint.h"

class WebMapTrack: public WebMapItem
{
	public:
		WebMapTrack();

		~WebMapTrack();

		void setWayPointList(const WayPoint::WayPointListType &wpList);

		void setTurnPointList(const FlightPointList &tpList, bool tri);

		void setWayPointListToMap();

		void setTurnPointListToMap();

		qreal getNorth() const;

		qreal getEast() const;

		qreal getSouth() const;

		qreal getWest() const;

	protected:
		void adjust();

		QRectF boundingRect() const;

		QPainterPath shape() const;

		void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);

	private:
		WayPoint::WayPointListType m_wpList;
		FlightPointList m_tpList;
		qreal m_north;
		qreal m_east;
		qreal m_south;
		qreal m_west;
		bool m_trackToMap;
		bool m_tpsToMap;
		bool m_tri;

		void calcBounds();
};

#endif
