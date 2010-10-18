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
#include "WebMapLeg.h"
#include "WebMapTurnPoint.h"

WebMapLeg::WebMapLeg(WebMapTurnPoint *pSrcTp, WebMapTurnPoint *pDstTp)
	:WebMapItem(WebMapLegType)
{
	m_arrowSize = 10;
	m_arrowEn = true;
	m_glowEn = false;
	m_pSrcTp = pSrcTp;
	m_pDstTp = pDstTp;
	m_pSrcTp->addItem(this);
	m_pDstTp->addItem(this);

	setFlag(ItemIsSelectable);
	setAcceptedMouseButtons(Qt::LeftButton);
	adjust();
}

WebMapLeg::~WebMapLeg()
{
}

void WebMapLeg::enableArrow(bool en)
{
	m_arrowEn = en;
	update();
}

void WebMapLeg::enableGlow(bool en)
{
	m_glowEn = en;
	update();
}

void WebMapLeg::adjust()
{
	if(!m_pSrcTp || !m_pDstTp)
			return;

	prepareGeometryChange();
	m_srcPoint = mapFromItem(m_pSrcTp, 0, 0);
	m_dstPoint = mapFromItem(m_pDstTp, 0, 0);
	update();
}

QRectF WebMapLeg::boundingRect() const
{
	if(!m_pSrcTp || !m_pDstTp)
			return QRectF();

	qreal penWidth = 1;
	qreal extra = (penWidth + m_arrowSize) / 2.0;

	return QRectF(m_srcPoint, QSizeF(m_dstPoint.x() - m_srcPoint.x(),
																		m_dstPoint.y() - m_srcPoint.y()))
			.normalized()
			.adjusted(-extra, -extra, extra, extra);
}

QPainterPath WebMapLeg::shape() const
{
	QPainterPath path;

	QLineF line(m_srcPoint, m_dstPoint);

	path.addPolygon(QPolygonF() << line.p1() << line.p2());

	return path;
}

void WebMapLeg::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
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
}

void WebMapLeg::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
	update();
	QGraphicsItem::mousePressEvent(pEvent);
}

void WebMapLeg::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
	update();
	QGraphicsItem::mouseReleaseEvent(pEvent);
}
