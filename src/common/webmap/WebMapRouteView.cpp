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

#include <QWebFrame>
#include "Route.h"
#include "WebMap.h"
#include "WebMapRouteView.h"
#include "WebMapRoute.h"

WebMapRouteView::WebMapRouteView(const QString &name)
{
  QWebFrame *pFrame;
	QWidget::setWindowTitle(name);
	resize(1000, 850);

	m_pRoute = NULL;
	m_pWebMap = new WebMap(this, WebMap::MapRoute);
	m_editable = true;
	pFrame = m_pWebMap->page()->mainFrame();
	connect(m_pWebMap, SIGNAL(mapReady()), this, SLOT(mapReady()));
  connect(m_pWebMap, SIGNAL(appReady()), this, SLOT(appReady()));
	connect(m_pWebMap, SIGNAL(finished(int)), this, SLOT(finished(int)));
	connect(pFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(populateObject()));
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
	m_pWebMap->loadUrl("qrc:/route/route.html");
}

void WebMapRouteView::setEditable(bool en)
{
	m_editable = en;
}

void WebMapRouteView::resizeEvent(QResizeEvent *pEvent)
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
}

void WebMapRouteView::mapReady()
{
	m_pWebMap->getRoute()->init();
}

void WebMapRouteView::appReady()
{
  m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
	m_pWebMap->getRoute()->setEditable(m_editable);

	if(m_pRoute != NULL)
	{
		m_pWebMap->getRoute()->setName(m_pRoute->name());
		m_pWebMap->getRoute()->setTurnPointList(m_pRoute->wayPointList());
    m_pWebMap->getRoute()->setRouteToStore(m_pRoute);
	}
}

void WebMapRouteView::finished(int res)
{
	done(res);
}

void WebMapRouteView::populateObject()
{
  m_pWebMap->getRoute()->populateObject();
}
