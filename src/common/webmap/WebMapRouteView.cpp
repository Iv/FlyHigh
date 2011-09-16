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
#include "WebMapRoute.h"

WebMapRouteView::WebMapRouteView(const QString &name)
{
	QWidget::setWindowTitle(name);
	resize(1000, 850);

	m_pRoute = NULL;
	m_pWebMap = new WebMap(this, WebMap::MapRoute);
	m_tpDrag = true;
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
	m_pWebMap->loadUrl("qrc:/webmap_route.html");
}

void WebMapRouteView::setTurnPointsDragable(bool drag)
{
	m_tpDrag = drag;
}

void WebMapRouteView::resizeEvent(QResizeEvent *pEvent)
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
}

void WebMapRouteView::mapReady()
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
	m_pWebMap->getRoute()->setTurnPointsDragable(m_tpDrag);

	if(m_pRoute != NULL)
	{
		m_pWebMap->getRoute()->setName(m_pRoute->name());
		m_pWebMap->getRoute()->setTurnPointList(m_pRoute->wayPointList());

		switch(m_pRoute->type())
		{
			case Route::Free:
				m_pWebMap->getRoute()->setFlightType("xc2");
			break;
			case Route::Free1Tp:
				m_pWebMap->getRoute()->setFlightType("xc3");
			break;
			case Route::Free2Tp:
				m_pWebMap->getRoute()->setFlightType("xc4");
			break;
			case Route::Free3Tp:
				m_pWebMap->getRoute()->setFlightType("xc5");
			break;
			case Route::FlatOrFaiTri:
				m_pWebMap->getRoute()->setFlightType("xc3c");
			break;
			default:
				switch(m_pRoute->wayPointList().size())
				{
					case 2:
						m_pWebMap->getRoute()->setFlightType("xc2");
					break;
					case 3:
						m_pWebMap->getRoute()->setFlightType("xc3");
					break;
					case 4:
						m_pWebMap->getRoute()->setFlightType("xc4");
					break;
					case 5:
						m_pWebMap->getRoute()->setFlightType("xc5");
					break;
					default:
						m_pWebMap->getRoute()->setFlightType("xc5");
					break;
				}
			break;
		}
	}

	m_pWebMap->XCLoad();
}

void WebMapRouteView::finished(int res)
{
	QString type;

	if(m_pRoute != NULL)
	{
		m_pWebMap->getRoute()->getTurnPointList(m_pRoute->wayPointList());
		m_pRoute->setName(m_pWebMap->getRoute()->getName());
		type = m_pWebMap->getRoute()->getFlightType();

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

	done(res);
}
