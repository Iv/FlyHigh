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

#include "WebMap.h"
#include "WebMapWayPointView.h"

WebMapWayPointView::WebMapWayPointView(const QString &name)
{
	QWidget::setWindowTitle(name);
	resize(1000, 850);

	m_pWpList = NULL;
	m_pWebMap = new WebMap(this, WebMap::MapWayPoint);
	connect(m_pWebMap, SIGNAL(mapReady()), this, SLOT(mapReady()));
	connect(m_pWebMap, SIGNAL(finished(int)), this, SLOT(finished(int)));
}

WebMapWayPointView::~WebMapWayPointView()
{
	delete m_pWebMap;
}

void WebMapWayPointView::setWayPointList(WayPoint::WayPointListType *pWpList)
{
	m_pWpList = pWpList;
}

void WebMapWayPointView::loadMap()
{
	m_pWebMap->loadMap("qrc:/waypoint.html");
}

void WebMapWayPointView::resizeEvent(QResizeEvent *pEvent)
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
}

void WebMapWayPointView::mapReady()
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));

	if(m_pWpList != NULL)
	{
		m_pWebMap->setWayPointList(*m_pWpList);
	}

	m_pWebMap->XCLoad();
}

void WebMapWayPointView::finished(int res)
{
/**
	QString type;

	if(m_pRoute != NULL)
	{
		m_pWebMap->getTurnPointList(m_pRoute->wayPointList());
		m_pRoute->setName(m_pWebMap->getName());
		type = m_pWebMap->getFlightType();

		if(type == "xc2")
		{
			m_pRoute->setType(Route::Free);
		}
		else if(type == "xc3")
		{
			m_pRoute->setType(Route::Free1Tp);
		}
		else if(type == "xc4")
		{
			m_pRoute->setType(Route::Free2Tp);
		}
		else if(type == "xc5")
		{
			m_pRoute->setType(Route::Free3Tp);
		}
		else if(type == "xc3c")
		{
			m_pRoute->setType(Route::FlatOrFaiTri);
		}
		else
		{
			m_pRoute->setType(Route::Undefined);
		}
	}
*/
	done(res);
}
