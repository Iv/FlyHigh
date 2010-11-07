/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your pOption) any later version.                                  *
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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <math.h>
#include <stdio.h>
#include "WebMapTurnPoint.h"
#include "WebMapItem.h"
#include "WebMap.h"

WebMapTurnPoint::WebMapTurnPoint()
	:WebMapItem(WebMapTurnPointType)
{
	m_lat = 0;
	m_lon = 0;
	m_char = '0';
	m_radiusMeter = 0;
	m_radiusPixel = 0;
	setFlag(ItemIsMovable);
	setCacheMode(DeviceCoordinateCache);
	setZValue(1);
	setAcceptedMouseButtons(Qt::LeftButton);
}

void WebMapTurnPoint::addItem(WebMapItem *pItem)
{
	m_itemList.push_back(pItem);
}

void WebMapTurnPoint::setLatLon(qreal lat, qreal lon)
{
	m_lat = lat;
	m_lon = lon;
	updatePos();

	emit newLatLon(this);
}

qreal WebMapTurnPoint::getLat() const
{
	return m_lat;
}

qreal WebMapTurnPoint::getLon() const
{
	return m_lon;
}

void WebMapTurnPoint::setText(char c)
{
	m_char = c;
}

char WebMapTurnPoint::getText() const
{
	return m_char;
}

void WebMapTurnPoint::setCircleRadius(qreal radius)
{
	qreal EarthRadius = 6371000.0;
	qreal circleLat;
//	qreal circleLon;
	int x;
	int y;

// radius muss anhand Skalierung berechnet werden
	m_radiusMeter = radius;
	m_radiusPixel = getRadiusPixel();
	update();
}

QRectF WebMapTurnPoint::boundingRect() const
{
	/// korrektes bounding rect

//QRectF radiusRect(-(m_radiusPixel + Border), -(m_radiusPixel + Border), 2 * (m_radiusPixel + Border), 2 * (m_radiusPixel + Border));

	if(m_radiusPixel > 50)
	{
printf("m_radiusPixel %i\n", m_radiusPixel);
	}

	QRectF radiusRect(-(m_radiusPixel + Border), -(m_radiusPixel + Border), 2 * (m_radiusPixel + Border), 2 * (m_radiusPixel + Border));
	QRectF pointerRect(-(Radius + Border), -(2 * Radius + NeedleLen + Border),
					2 * (Radius + Border), 2 * (Radius + Border) + NeedleLen);

	return radiusRect.united (pointerRect);
/*
	return QRectF(-(Radius + Border), -(2 * Radius + NeedleLen + Border),
					2 * (Radius + Border), 2 * (Radius + Border) + NeedleLen);
*/
}

QPainterPath WebMapTurnPoint::shape() const
{
	QPainterPath path;

	path.addEllipse(-Radius, -(2 * Radius + NeedleLen), 2 * Radius, 2 * Radius);

	return path;
}

void WebMapTurnPoint::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
	QPolygonF needle;
	QRect circleRect(-Radius, -(2 * Radius + NeedleLen), 2 * Radius, 2 * Radius);
	qreal pixelRad;

	if(pOption->state & QStyle::State_Sunken)
	{
		QRadialGradient hiGrad(QPointF(0, circleRect.center().y()), Radius + Border);
		hiGrad.setColorAt(0, QColor(0, 0, 0, 255));
		hiGrad.setColorAt(0.7, QColor(255, 255, 255, 100));
		hiGrad.setColorAt(1, QColor(255, 255, 255, 0));
		pPainter->setBrush(hiGrad);
		pPainter->setPen(Qt::NoPen);
		pPainter->drawEllipse(circleRect.adjusted(-Border, -Border, Border, Border));
	}

	// Nadel
	needle.push_back(QPointF(-2, -15));
	needle.push_back(QPointF(0, 0));
	needle.push_back(QPointF(2, -15));
	pPainter->setPen(Qt::NoPen);
	pPainter->setBrush(QBrush(getColor().darker(200)));
	pPainter->drawPolygon(needle);

	// Kreis
	QRadialGradient circleGrad(QPointF(-Radius / 2, -(2 * Radius - Radius / 2 + NeedleLen)),
						2 * Radius);
	circleGrad.setColorAt(0, QColor(255, 255, 255));
	circleGrad.setColorAt(0.6, getColor().lighter(100));
	circleGrad.setColorAt(0.8, getColor());
	circleGrad.setColorAt(1, getColor().darker(200));
	pPainter->setBrush(circleGrad);
	pPainter->setPen(QPen(QBrush(getColor().darker(200)), 1));
	pPainter->drawEllipse(circleRect);

	// Text
	pPainter->setPen(Qt::black);
	pPainter->setFont(QFont("Arial", 16, QFont::Bold));
	pPainter->drawText(circleRect, Qt::AlignCenter, QChar(m_char));

	// FAI Kreis
	if(m_radiusPixel > 0)
	{
		QRect faiCircleRect(-m_radiusPixel, -m_radiusPixel, 2 * m_radiusPixel, 2 * m_radiusPixel);
		pPainter->setPen(getColor());
		pPainter->setBrush(Qt::NoBrush);
		pPainter->drawEllipse(faiCircleRect);
	}
}

void WebMapTurnPoint::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
	update();
	QGraphicsItem::mousePressEvent(pEvent);
}

void WebMapTurnPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
	update();
	QGraphicsItem::mouseReleaseEvent(pEvent);
}

void WebMapTurnPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
	updateLatLon();
	QGraphicsItem::mouseMoveEvent(pEvent);
}

void WebMapTurnPoint::updatePos()
{
	int x;
	int y;

	m_radiusPixel = getRadiusPixel();
	getMap()->getPointFromLatLon(m_lat, m_lon, x, y);
	setPos(x, y);
	adjustItems();
}

void WebMapTurnPoint::updateLatLon()
{
	getMap()->getLatLonFromPoint(pos().x(), pos().y(), m_lat, m_lon);
//	printf("WebMapTurnPoint new lat = %f new lon = %f\n", m_lat, m_lon);
	adjustItems();

	emit newLatLon(this);
}

void WebMapTurnPoint::adjustItems()
{
	WebMapItemList::iterator it;

	for(it=m_itemList.begin(); it!=m_itemList.end(); it++)
	{
		(*it)->adjust();
	}
}

uint WebMapTurnPoint::getRadiusPixel()
{
	qreal EarthRadius = 6371000.0;
	qreal circleLat;
//	qreal circleLon;
	uint radiusPixel = 0;
	int x;
	int y;

	if(m_radiusMeter > 0)
	{
		circleLat = (m_radiusMeter / EarthRadius) * 180 / M_PI;
	//	circleLon = circleLat / cos(m_lat * M_PI / 180);
	
		getMap()->getPointFromLatLon(m_lat + circleLat, m_lon, x, y);
	
		QLineF line(pos(), QPointF(x, y));
	
		radiusPixel = (uint)line.length();
	
		if(radiusPixel > 1000)
		{
			radiusPixel = 0;
		}
	}

	return radiusPixel;
}
