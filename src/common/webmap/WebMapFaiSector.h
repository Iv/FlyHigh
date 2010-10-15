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

#ifndef _WebMapFaiSector_h
#define _WebMapFaiSector_h

#include <QList>
#include "WebMapOverlayItem.h"

class WebMapTurnPoint;
class WebMapFaiSector;

typedef QList<WebMapFaiSector*> WebMapFaiSectorList;

class WebMapFaiSector: public WebMapOverlayItem
{
	public:
		WebMapFaiSector(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapTurnPoint *pTp3);

		~WebMapFaiSector();

		void adjust();

	protected:
		QRectF boundingRect() const;

		void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);

	private:
		WebMapTurnPoint *m_pTp1;
		WebMapTurnPoint *m_pTp2;
		WebMapTurnPoint *m_pTp3;
		QPolygonF m_faiSector;

		int orient2dTri(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapTurnPoint *pTp3);

		void distBearTo(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, qreal &dist, qreal &bear);

		QPointF latLngAt(WebMapTurnPoint *pTp1, qreal bearing, qreal distance);

		QPointF latLngAtXY(WebMapTurnPoint *pTp1, qreal x, qreal y);

		void getWebMapFaiSector(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapTurnPoint *pTp3, QPolygonF &faiSector);
};

#endif
