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
#include <math.h>
#include "WebMapFaiSector.h"
#include "WebMapTurnPoint.h"

WebMapFaiSector::WebMapFaiSector(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapTurnPoint *pTp3)
	:WebMapOverlayItem(WebMapFaiSectorType)
{
	m_pTp1 = pTp1;
	m_pTp2 = pTp2;
	m_pTp3 = pTp3;

	m_pTp1->addItem(this);
	m_pTp2->addItem(this);
	m_pTp3->addItem(this);

	setAcceptedMouseButtons(0);
	adjust();
}

WebMapFaiSector::~WebMapFaiSector()
{
}

void WebMapFaiSector::adjust()
{
	if(!m_pTp1 || !m_pTp2 || !m_pTp3)
			return;

	prepareGeometryChange();
	getWebMapFaiSector(m_pTp1, m_pTp2, m_pTp3, m_faiSector);
	update();
}

QRectF WebMapFaiSector::boundingRect() const
{
	QRectF boundRect;
	qreal penWidth;
	qreal extra;

	if(!m_pTp1 || !m_pTp2 || !m_pTp3)
			return QRectF();

	penWidth = 1;
	extra = penWidth / 2.0;
	boundRect = m_faiSector.boundingRect();
	boundRect.normalized();
	boundRect.adjusted(-extra, -extra, extra, extra);

	return boundRect;
}

void WebMapFaiSector::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
	if(!m_pTp1 || !m_pTp2 || !m_pTp3)
			return;

	pPainter->setPen(QPen(m_pTp3->getColor(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	pPainter->drawPolygon(m_faiSector);
}

int WebMapFaiSector::orient2dTri(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapTurnPoint *pTp3)
{
	return ((pTp2->x() - pTp1->x()) * (pTp3->y() - pTp1->y()) -
					(pTp3->x() - pTp1->x()) * (pTp2->y() - pTp1->y()));

/*
	return ((latlng2.lng() - latlng1.lng()) * (latlng3.lat() - latlng1.lat()) -
				(latlng3.lng() - latlng1.lng()) * (latlng2.lat() - latlng1.lat()));
*/
}

void WebMapFaiSector::distBearTo(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, qreal &dist, qreal &bear)
{
	QLineF line(mapFromItem(pTp1, 0, 0), mapFromItem(pTp2, 0, 0));
	qreal angle;

	dist = line.length();
	bear = -atan2(line.dy(), line.dx());
	bear += M_PI / 2;

	if(bear > (2 * M_PI))
	{
		bear -= (2 * M_PI);
	}
}

QPointF WebMapFaiSector::latLngAt(WebMapTurnPoint *pTp1, qreal bearing, qreal distance)
{
/*
	qreal lat;
	qreal lng;

	lat = asin(sin(latlng.latRadians()) * cos(distance) + cos(latlng.latRadians()) * sin(distance) * cos(bearing));
	lng = latlng.lngRadians() + atan2(sin(bearing) * sin(distance) * cos(latlng.latRadians()), cos(distance) - sin(latlng.latRadians()) * sin(lat));

	return new QPointF(180.0 * lat / Math.PI, 180.0 * lng / Math.PI);
*/
}

QPointF WebMapFaiSector::latLngAtXY(WebMapTurnPoint *pTp1, qreal x, qreal y)
{
	QPointF movedCoord;

	movedCoord = mapFromItem(pTp1, 0, 0);
	movedCoord += QPointF(x, y);

	return movedCoord;

//	return latLngAt(pTp1, atan2(x, y), sqrt(x*x + y*y) / R);
}

void WebMapFaiSector::getWebMapFaiSector(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapTurnPoint *pTp3, QPolygonF &faiSector)
{
	const qreal deltaAngle = 1;
	qreal bear;
	qreal cosBear;
	qreal sinBear;
	qreal ap;
	qreal bp;
	qreal cp;
	qreal a;
	qreal b;
	qreal c;
	qreal x;
	qreal y;
	qreal rotX;
	qreal rotY;
	bool cw;

	faiSector.clear();
	distBearTo(pTp1, pTp2, c, bear);
 	bear -= M_PI / 2;

	// the orientation of the triangle
	cw = (orient2dTri(pTp1, pTp2, pTp3) < 0);

	if(!cw)
	{
		bear = -bear;
	}

	cosBear = cos(bear);
	sinBear = sin(bear);

	/* calculate the sectors */

	// case 1: c is minimal, a and b variable
	cp = 28.0;

	for(ap=28.0; ap<44.0; ap+=deltaAngle)
	{
		bp = 100.0 - ap - cp;
		a = c * ap / cp;
		b = c * bp / cp;
		x = (b * b + c * c - a * a) / (2 * c);
		y = sqrt(b * b - x * x);

		// rotation
		rotX = x * cosBear - y * sinBear;
		rotY = x * sinBear + y * cosBear;

		if(cw)
		{
			rotY = -rotY;
		}

		// translation
		faiSector.push_back(latLngAtXY(pTp1, rotX, rotY));
//		latlngs.push(latLngAtXY(latlng1, rotX, rotY));
	}

	// case 2: b is minimal, a and c variable
	bp = 28.0;

	for(cp=28.0; cp<44.0; cp+=deltaAngle)
	{
		ap = 100.0 - bp - cp;
		a = c * ap / cp;
		b = c * bp / cp;
		x = (b * b + c * c - a * a) / (2 * c);
		y = sqrt(b*b - x*x);

		// rotation
		rotX = x * cosBear - y * sinBear;
		rotY = x * sinBear + y * cosBear;

		if(cw)
		{
			rotY = -rotY;
		}

		// translation
		faiSector.push_back(latLngAtXY(pTp1, rotX, rotY));
//		latlngs.push(latLngAtXY(latlng1, rotX, rotY));
	}

	// case 3: a ist minimal, b and c variable
	ap = 28.0;

	for(cp=44.0; cp>=28.0; cp-=deltaAngle)
	{
		bp = 100.0 - ap - cp;
		a = c * ap / cp;
		b = c * bp / cp;
		x = (b * b + c * c - a * a) / (2.0 * c);
		y = sqrt(b*b - x*x);

		// rotation
		rotX = x * cosBear - y * sinBear;
		rotY = x * sinBear + y * cosBear;

		if(cw)
		{
			rotY = -rotY;
		}

		// translation
		faiSector.push_back(latLngAtXY(pTp1, rotX, rotY));
//		latlngs.push(latLngAtXY(latlng1, rotX, rotY));
	}

#if 0
	var bear;
	var cosBear;
	var sinBear;
	var latlngs = [];
	var cw;
	var ap;
	var bp;
	var cp;
	var a;
	var b;
	var c = latlng1.distanceFrom(latlng2);
	var x;
	var y;
	var rotX;
	var rotY;
	var deltaAngle = 1;

	// the orientation of the triangle
	cw = (orient2dTri(latlng1, latlng2, latlng3) < 0);

	if(cw)
	{
		bear = (initialBearingTo(latlng1, latlng2) - Math.PI / 2);
	}
	else
	{
		bear = -(initialBearingTo(latlng1, latlng2) - Math.PI / 2);
	}

	cosBear = Math.cos(bear);
	sinBear = Math.sin(bear);

	/* calculate the sectors */

	// case 1: c is minimal, a and b variable
	cp = 28.0;

	for(ap=28; ap<44; ap+=deltaAngle)
	{
		bp = 100.0 - ap - cp;
		a = c * ap / cp;
		b = c * bp / cp;
		x = (b*b + c*c - a*a) / (2 * c);
		y = Math.sqrt(b*b - x*x);

		// rotation
		rotX = x * cosBear - y * sinBear;
		rotY = x * sinBear + y * cosBear;

		if(cw)
		{
			rotY = -rotY;
		}

		// translation
		latlngs.push(latLngAtXY(latlng1, rotX, rotY));
	}

	// case 2: b is minimal, a and c variable
	bp = 28.0;

	for(cp=28; cp<44; cp+=deltaAngle)
	{
		ap = 100.0 - bp - cp;
		a = c * ap / cp;
		b = c * bp / cp;
		x = (b*b + c*c - a*a) / (2 * c);
		y = Math.sqrt(b*b - x*x);

		// rotation
		rotX = x * cosBear - y * sinBear;
		rotY = x * sinBear + y * cosBear;

		if(cw)
		{
			rotY = -rotY;
		}

		// translation
		latlngs.push(latLngAtXY(latlng1, rotX, rotY));
	}

	// case 3: a ist minimal, b and c variable
	ap = 28.0;

	for(cp=44; cp>=28; cp-=deltaAngle)
	{
		bp = 100.0 - ap - cp;
		a = c * ap / cp;
		b = c * bp / cp;
		x = (b*b + c*c - a*a) / (2 * c);
		y = Math.sqrt(b*b - x*x);

		// rotation
		rotX = x * cosBear - y * sinBear;
		rotY = x * sinBear + y * cosBear;

		if(cw)
		{
			rotY = -rotY;
		}

		// translation
		latlngs.push(latLngAtXY(latlng1, rotX, rotY));
	}

	return latlngs;
#endif
}
