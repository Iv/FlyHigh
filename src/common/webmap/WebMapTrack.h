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
#include "WebMapItem.h"
#include "WayPoint.h"

class WebMapTrack: public WebMapItem
{
	public:
		WebMapTrack(const WayPoint::WayPointListType &wpList);

		~WebMapTrack();

	protected:
		void adjust();

		QRectF boundingRect() const;

		QPainterPath shape() const;

		void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);

		void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
	
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

	private:
		WayPoint::WayPointListType m_wpList;
		QPolygonF m_wayPoints;
		qreal m_n;
		qreal m_e;
		qreal m_s;
		qreal m_w;

/**
		QPointF m_srcPoint;
		QPointF m_dstPoint;
*/

		void recalcWayPoints();
};

#endif
