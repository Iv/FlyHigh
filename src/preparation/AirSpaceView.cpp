/***************************************************************************
 *   Copyright (C) 2009 by Alex Graf                                     *
 *   grafal@sourceforge.net                                                         *
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
#include <QWheelEvent>
#include <Q3PointArray>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QPaintEvent>
#include "AirSpace.h"
#include "AirSpaceList.h"
#include "AirSpaceView.h"

AirSpaceView::AirSpaceView()
{
	m_pAirSpaceList = NULL;
	m_selected = -1;
	m_scale = MinScale;
	m_mouseDown = false;
}

AirSpaceView::~AirSpaceView()
{
}

void AirSpaceView::setAirSpaceList(AirSpaceList *pAirSpaceList, int selected)
{
	AirSpaceList::iterator it;
	AirSpace *pAirSpace;

	m_pAirSpaceList = pAirSpaceList;
	m_selected = selected;
	m_bbox.init();

	for(it=pAirSpaceList->begin(); it!=pAirSpaceList->end(); it++)
	{
		pAirSpace = *it;
		pAirSpace->createPointList();
		m_bbox.setMinMax(pAirSpace->boundBox());
	}

	update();
}

void AirSpaceView::setSelected(int sel)
{
	m_selected = sel;
	update();
}

void AirSpaceView::paintEvent(QPaintEvent *pEvent)
{
	(void)pEvent;
	drawAirspace();
}

void AirSpaceView::closeEvent(QCloseEvent *pEvent)
{
	pEvent->ignore();
}

void AirSpaceView::mousePressEvent(QMouseEvent *pEvent)
{
	if(pEvent->button() == Qt::LeftButton)
	{
		m_mouseDown = true;
		m_prevPos = pEvent->pos();
	}
}

void AirSpaceView::mouseReleaseEvent(QMouseEvent *pEvent)
{
	if(pEvent->button() == Qt::LeftButton)
	{
		m_mouseDown = false;
	}
}

void AirSpaceView::mouseMoveEvent(QMouseEvent *pEvent)
{
	QPoint prevOff;

	if(m_mouseDown && (pEvent->buttons() & Qt::LeftButton))
	{
		prevOff = m_offset;
		calcOffset(pEvent->pos());

		if(prevOff != m_offset)
		{
			update();
		}
	}
}

void AirSpaceView::wheelEvent(QWheelEvent * pEvent)
{
	if(pEvent->delta() > 0)
	{
		if(m_scale < MaxScale)
		{
			m_scale++;
			calcOffset(m_prevPos);
			update();
		}
	}
	else
	{
		if(m_scale > MinScale)
		{
			m_scale--;
			calcOffset(m_prevPos);
			update();
		}
	}
}

#define ToInt(x) ((int)(x * 1000))

void AirSpaceView::drawAirspace()
{
	enum {Border = 5, MinSize = 50};
	QPainter paint(this);
	QRect viewRect;
	Q3PointArray pointList;
	BoundBox bbox;
	AirSpaceList::iterator itAirSpace;
	WayPoint::WayPointListType::const_iterator it;
	AirSpace *pAirSpace;
	AirSpace *pSelAirSpace = NULL;
	QString str;
	int viewHeight;
	int viewWidth;
	int ptNr;
	int spaceNr = 0;
	double lat;
	double lon;
	double offLat;
	double offLon;

	if(m_pAirSpaceList != NULL)
	{
		paint.save();
		paint.setWindow(-ToInt(m_bbox.width() / 2), -ToInt(m_bbox.height() / 2), ToInt(m_bbox.width()), ToInt(m_bbox.height()));
		offLat = (m_bbox.north() + m_bbox.south()) / 2;
		offLon = (m_bbox.east() + m_bbox.west()) / 2;
		viewRect = paint.viewport();

		if(m_bbox.width() > m_bbox.height())
		{
			if(m_bbox.width() == 0)
			{
				viewHeight = MinSize;
			}
			else
			{
				viewHeight = (int)(viewRect.width() * m_bbox.height() / m_bbox.width());
			}

			paint.setViewport(viewRect.left() + Border, (viewRect.height() - viewHeight) / 2 + Border,
					viewRect.width() - 2 * Border, viewHeight - 2 * Border);
		}
		else
		{
			if(m_bbox.height() == 0)
			{
				viewWidth = MinSize;
			}
			else
			{
				viewWidth = (int)(viewRect.height() * m_bbox.width() / m_bbox.height());
				paint.setViewport((viewRect.width() - viewWidth) / 2 + Border, viewRect.top() + Border,
						viewWidth - 2 * Border, viewRect.height() - 2 * Border);
			}
		}

		// offset, scale and color
		paint.translate(m_offset.x() * paint.window().width() / viewRect.width(), m_offset.y() * paint.window().height() / viewRect.height() * 2.0);
		paint.scale(m_scale, m_scale);
		paint.setPen(palette().windowText().color());

		// draw airspaces
		for(itAirSpace=m_pAirSpaceList->begin(); itAirSpace!=m_pAirSpaceList->end(); itAirSpace++)
		{
			pointList.resize(pAirSpace->pointList().size());
			ptNr = 0;

			for(it=pAirSpace->pointList().begin(); it!=pAirSpace->pointList().end(); it++)
			{
				lat = m_bbox.north() + m_bbox.south() - (*it).latitude() - offLat;
				lon = (*it).longitude() - offLon;
				pointList.setPoint(ptNr, ToInt(lon), ToInt(lat));
				ptNr++;
			}

			ptNr = pointList.size() - 1;

			if((pointList.size() > 0) && (pointList[0] != pointList[ptNr]))
			{
				pointList.append(pointList[0]);
			}

			if(spaceNr == m_selected)
			{
				pSelAirSpace = pAirSpace;
			}
			else
			{
				paint.drawPolyline(pointList);
			}

			spaceNr++;
		}

		// draw hilited airspace
		if(pSelAirSpace != NULL)
		{
			pointList.resize(pSelAirSpace->pointList().size());
			ptNr = 0;

			for(it=pSelAirSpace->pointList().begin(); it!=pSelAirSpace->pointList().end(); it++)
			{
				lat = m_bbox.north() + m_bbox.south() - (*it).latitude() - offLat;
				lon = (*it).longitude() - offLon;
				pointList.setPoint(ptNr, ToInt(lon), ToInt(lat));
				ptNr++;
			}

			ptNr = pointList.size() - 1;

			if((pointList.size() > 0) && (pointList[0] != pointList[ptNr]))
			{
				pointList.append(pointList[0]);
			}

			paint.setPen(QColor(255, 0, 0));
			paint.drawPolyline(pointList);
		}

		paint.restore();

		// draw scale
		str.sprintf("Scale = %i", m_scale);
		paint.drawText(10, 20, str);
	}
}

void AirSpaceView::calcOffset(const QPoint &mousePos)
{
	int off;
	int maxOff;

	// X offset
	off = m_offset.x() - m_prevPos.x() + mousePos.x();
	maxOff = (m_scale - 1) * geometry().width() / 2;

	if(off > maxOff)
	{
		off = maxOff;
	}
	else if(off < -maxOff)
	{
		off = -maxOff;
	}

	m_offset.setX(off);

	// Y offset
	off = m_offset.y() - m_prevPos.y() + mousePos.y();
	maxOff = (m_scale - 1) * geometry().height() / 2;

	if(off > maxOff)
	{
		off = maxOff;
	}
	else if(off < -maxOff)
	{
		off = -maxOff;
	}

	m_offset.setY(off);
	m_prevPos = mousePos;
}
