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

#ifndef _WebMapTurnPoint_h
#define _WebMapTurnPoint_h

#include <QObject>
#include "WebMapItem.h"

class QGraphicsSceneMouseEvent;
class WebMapTurnPoint;

typedef QList<WebMapTurnPoint*> WebMapTurnPointList;

class WebMapTurnPoint: public QObject, public WebMapItem
{
	Q_OBJECT

	public:
		WebMapTurnPoint();
	
		void addItem(WebMapItem *pItem);

		void setLatLon(qreal lat, qreal lon);

		qreal getLat() const;

		qreal getLon() const;

		void setText(char c);

		char getText() const;

		void setCircleRadius(qreal radius);

	signals:
		void newLatLon(const WebMapTurnPoint *pTp);

	protected:
		QRectF boundingRect() const;

		QPainterPath shape() const;

		void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);
	
		void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
	
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

		void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent);

		void updatePos();

		void updateLatLon();

	private:
		enum {Border = 5, NeedleLen = 13, Radius = 10};

		WebMapItemList m_itemList;
		qreal m_lat;
		qreal m_lon;
		qreal m_radiusMeter;
		int m_radiusPixel;
		char m_char;

		void adjustItems();

		uint getRadiusPixel();
};

#endif
