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

#include "Route.h"
#include "WebMap.h"
#include "WebMapRouteView.h"

WebMapRouteView::WebMapRouteView(const QString &name)
{
	QWidget::setWindowTitle(name);
	resize(1000, 800);

	m_pRoute = NULL;
	m_pWebMap = new WebMap(this);
	connect(m_pWebMap, SIGNAL(mapReady()), this, SLOT(mapReady()));
	connect(m_pWebMap, SIGNAL(finished(int)), this, SLOT(finished(int)));
}

WebMapRouteView::~WebMapRouteView()
{
	delete m_pWebMap;
}

void WebMapRouteView::setRoute(Route *pRoute)
{
	m_pRoute = pRoute;
}

void WebMapRouteView::loadMap()
{
	m_pWebMap->loadMap("qrc:/webmap_route.html");
}

void WebMapRouteView::resizeEvent(QResizeEvent *pEvent)
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
}

void WebMapRouteView::mapReady()
{
	m_pWebMap->setTurnPointsDragable(true);

	if(m_pRoute != NULL)
	{
		m_pWebMap->setTurnPointList(m_pRoute->wayPointList());
		m_pWebMap->setLocation(m_pRoute->name());

		switch(m_pRoute->wayPointList().size())
		{
			case 2:
				m_pWebMap->setFlightType("xc2");
			break;
			case 3:
				m_pWebMap->setFlightType("xc3");
			break;
			case 4:
				m_pWebMap->setFlightType("xc4");
			break;
			case 5:
				m_pWebMap->setFlightType("xc5");
			break;
		}
	}

	m_pWebMap->XCLoad();
}

void WebMapRouteView::finished(int res)
{
	if(m_pRoute != NULL)
	{
		m_pWebMap->getTurnPointList(m_pRoute->wayPointList());
		m_pRoute->setName(m_pWebMap->getLocation());
	}

	done(res);
}
