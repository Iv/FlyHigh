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
#include "WebMap.h"
#include "WebMapWayPoint.h"
#include "WebMapWayPointView.h"

#include <QDebug>

WebMapWayPointView::WebMapWayPointView(const QString &name)
{
  QWebFrame *pFrame;
	QWidget::setWindowTitle(name);
	resize(1000, 850);

	m_pWpList = NULL;
	m_pWebMap = new WebMap(this, WebMap::MapWayPoint);
  pFrame = m_pWebMap->page()->mainFrame();
	connect(m_pWebMap, SIGNAL(mapReady()), this, SLOT(mapReady()));
	connect(m_pWebMap, SIGNAL(finished(int)), this, SLOT(finished(int)));
  connect(pFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(populateJavaScriptWindowObject()));
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
	m_pWebMap->loadUrl("qrc:/waypoint.html");
}

void WebMapWayPointView::resizeEvent(QResizeEvent *pEvent)
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
}

void WebMapWayPointView::saveWayPoint(int id, const QString &name, const QString &spot,
                                      const QString &country, double lat, double lon,
                                      int alt)
{
  WayPoint wp;

  wp.setId(id);
  wp.setName(name);
  wp.setSpot(spot);
  wp.setCountry(country);
  wp.setLatitude(lat);
  wp.setLongitude(lon);
  wp.setAltitude(alt);

  emit changedWayPoint(wp);
}

void WebMapWayPointView::mapReady()
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
  m_pWebMap->getWayPoint()->setWayPointList(m_pWpList);
	m_pWebMap->getWayPoint()->init();
}

void WebMapWayPointView::finished(int res)
{
	done(res);
}

void WebMapWayPointView::populateJavaScriptWindowObject()
{
	m_pWebMap->page()->mainFrame()->addToJavaScriptWindowObject("WebMapWayPointView", this);
}
