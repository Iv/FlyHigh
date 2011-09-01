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
#include "WebMapWayPointView.h"

WebMapWayPointView::WebMapWayPointView(const QString &name)
{
	QWidget::setWindowTitle(name);
	resize(1000, 850);

	m_pWpList = NULL;
	m_editItem = 0;
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

void WebMapWayPointView::setEditItem(uint nr)
{
  m_editItem = nr;
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
  setWayPointList();
	load();
}

void WebMapWayPointView::finished(int res)
{
	done(res);
}

void WebMapWayPointView::load()
{
	QString code = "wp_load();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMapWayPointView::setWayPointList()
{
	QString code = "wp_pushWayPoint('%1', %2, %3, %4);";
	QWebFrame *pFrame;
	uint itemNr;
	uint listSize;
	QString name;
	float lat;
	float lon;
	int alt;

	listSize = m_pWpList->size();

	if(listSize > 0)
	{
    pFrame = m_pWebMap->page()->mainFrame();

		for(itemNr=0; itemNr<listSize; itemNr++)
		{
      name = m_pWpList->at(itemNr).name();
		  lat = m_pWpList->at(itemNr).latitude();
		  lon = m_pWpList->at(itemNr).longitude();
      alt = m_pWpList->at(itemNr).altitude();
      pFrame->evaluateJavaScript(code.arg(name).arg(lat).arg(lon).arg(alt));
		}
	}
}
