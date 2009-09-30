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
	m_pAirSpace = NULL;
}

AirSpaceView::~AirSpaceView()
{
}

void AirSpaceView::setAirSpace(const AirSpace *pAirSpace)
{
	m_pAirSpace = pAirSpace;
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
	int viewHeight;
	int viewWidth;
	int ptNr = 0;
	double lat;

	if(m_pAirSpace != NULL)
	{
		bbox = m_pAirSpace->boundBox();
	
		paint.setBrush(colorGroup().foreground());
		paint.save();
		paint.setWindow(ToInt(bbox.west()), ToInt(bbox.south()), ToInt(bbox.width()), ToInt(bbox.height()));
		viewRect = paint.viewport();
	
		if(bbox.width() > bbox.height())
		{
			if(bbox.width() == 0)
			{
				viewHeight = MinSize;
			}
			else
			{
				viewHeight = (int)(viewRect.width() * bbox.height() / bbox.width());
			}

			paint.setViewport(viewRect.left() + Border, (viewRect.height() - viewHeight) / 2 + Border,
					viewRect.width() - 2 * Border, viewHeight - 2 * Border);
		}
		else
		{
			if(bbox.height() == 0)
			{
				viewWidth = MinSize;
			}
			else
			{
				viewWidth = (int)(viewRect.height() * bbox.width() / bbox.height());
				paint.setViewport((viewRect.width() - viewWidth) / 2 + Border, viewRect.top() + Border,
						viewWidth - 2 * Border, viewRect.height() - 2 * Border);
			}
		}
	
		pointList.resize(m_pAirSpace->pointList().size());
	
		for(it=m_pAirSpace->pointList().begin(); it!=m_pAirSpace->pointList().end(); it++)
		{
			lat = bbox.north() + bbox.south() - (*it).latitude();
			pointList.setPoint(ptNr, ToInt((*it).longitude()), ToInt(lat));
			ptNr++;
		}
	
		paint.drawPolyline(pointList);
		paint.restore();
	}
}
