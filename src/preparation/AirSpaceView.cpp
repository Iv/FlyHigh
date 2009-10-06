/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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
#include <qpainter.h>
#include "AirSpace.h"
#include "AirSpaceView.h"

AirSpaceView::AirSpaceView()
{
	m_pAirSpaceList = NULL;
	m_selected = -1;
}

AirSpaceView::~AirSpaceView()
{
}

void AirSpaceView::setAirSpaceList(AirSpace::AirSpaceListType *pAirSpaceList, int selected)
{
	AirSpace *pAirSpace;

	m_pAirSpaceList = pAirSpaceList;
	m_selected = selected;
	m_bbox.init();

	for(pAirSpace=pAirSpaceList->first(); pAirSpace!=NULL; pAirSpace=pAirSpaceList->next())
	{
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

#define ToInt(x) ((int)(x * 1000))

void AirSpaceView::drawAirspace()
{
	enum {Border = 5, MinSize = 50};
	QPainter paint(this);
	QRect viewRect;
	QPointArray pointList;
	BoundBox bbox;
	WayPoint::WayPointListType::const_iterator it;
	AirSpace *pAirSpace;
	AirSpace *pSelAirSpace = NULL;
	int viewHeight;
	int viewWidth;
	int ptNr;
	int spaceNr = 0;
	double lat;

	if(m_pAirSpaceList != NULL)
	{
		paint.save();
		paint.setWindow(ToInt(m_bbox.west()), ToInt(m_bbox.south()), ToInt(m_bbox.width()), ToInt(m_bbox.height()));
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

		paint.setPen(colorGroup().foreground());

		for(pAirSpace=m_pAirSpaceList->first(); pAirSpace!=NULL; pAirSpace=m_pAirSpaceList->next())
		{
			pointList.resize(pAirSpace->pointList().size());
			ptNr = 0;
		
			for(it=pAirSpace->pointList().begin(); it!=pAirSpace->pointList().end(); it++)
			{
				lat = m_bbox.north() + m_bbox.south() - (*it).latitude();
				pointList.setPoint(ptNr, ToInt((*it).longitude()), ToInt(lat));
				ptNr++;
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

		if(pSelAirSpace != NULL)
		{
			pointList.resize(pSelAirSpace->pointList().size());
			ptNr = 0;
		
			for(it=pSelAirSpace->pointList().begin(); it!=pSelAirSpace->pointList().end(); it++)
			{
				lat = m_bbox.north() + m_bbox.south() - (*it).latitude();
				pointList.setPoint(ptNr, ToInt((*it).longitude()), ToInt(lat));
				ptNr++;
			}

			paint.setPen(QColor(255, 0, 0));
			paint.drawPolyline(pointList);
		}

		paint.restore();
	}
}
