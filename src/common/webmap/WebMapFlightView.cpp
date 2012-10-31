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
#include "WebMapFlight.h"
#include "WebMapFlightView.h"
#include "WebMapRoute.h"

WebMapFlightView::WebMapFlightView(const QString &name)
{
	QWidget::setWindowTitle(name);
	resize(1000, 800);

	m_pWebMap = new WebMap(this, WebMap::MapFlight);
	m_pWebMap->getFlight()->setPlotEnable(true);
	m_location = "";
	connect(m_pWebMap, SIGNAL(mapReady()), this, SLOT(mapReady()));
  connect(m_pWebMap, SIGNAL(appReady()), this, SLOT(appReady()));
	connect(m_pWebMap, SIGNAL(finished(int)), this, SLOT(done(int)));
}

WebMapFlightView::~WebMapFlightView()
{
	delete m_pWebMap;
}

void WebMapFlightView::setTurnPointList(const WayPoint::WayPointListType &tpList)
{
	m_tpList = tpList;
}

void WebMapFlightView::setWayPointList(const WayPoint::WayPointListType &wpList)
{
	m_wpList = wpList;
}

void WebMapFlightView::setLocation(const QString &location)
{
	m_location = location;
}

void WebMapFlightView::setFlightPointList(const QDate &date, const FlightPointList &fpList)
{
  m_date = date;
	m_fpList = fpList;
}

void WebMapFlightView::setSogList(const FlightPointList::SogListType &sogList)
{
	m_sogList = sogList;
}

void WebMapFlightView::setVarioList(const FlightPointList::VarioListType &varioList)
{
	m_varioList = varioList;
}

void WebMapFlightView::loadMap()
{
	m_pWebMap->loadUrl("qrc:/flight/flight.html");
}

void WebMapFlightView::resizeEvent(QResizeEvent *pEvent)
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
}

void WebMapFlightView::mapReady()
{
	m_pWebMap->getFlight()->init();
}

void WebMapFlightView::appReady()
{
  m_pWebMap->setSize(width(), height());

  m_pWebMap->getRoute()->setName(m_location);
	m_pWebMap->getRoute()->setTurnPointList(m_tpList);
	m_pWebMap->getFlight()->setFlightPointList(m_date, m_fpList);
	m_pWebMap->getFlight()->setSogList(m_sogList);
	m_pWebMap->getFlight()->setVarioList(m_varioList);
	m_pWebMap->getFlight()->showPlot();
}
