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
  connect(pFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(populateObject()));
  connect(m_pWebMap->getWayPoint(), SIGNAL(changeWayPoint(const WayPoint&)), this,
          SLOT(saveWayPoint(const WayPoint&)));
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

void WebMapWayPointView::setWayPointList()
{
	int itemNr;
	int listSize;

	listSize = m_pWpList->size();

	if(listSize > 0)
	{
		for(itemNr=0; itemNr<listSize; itemNr++)
		{
      m_pWebMap->getWayPoint()->pushWayPoint(m_pWpList->at(itemNr));
		}
	}
}

void WebMapWayPointView::mapReady()
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));

  if(m_pWpList != NULL)
  {
    setWayPointList();
  }

	m_pWebMap->getWayPoint()->init();
}

void WebMapWayPointView::finished(int res)
{
	done(res);
}

void WebMapWayPointView::populateObject()
{
  m_pWebMap->getWayPoint()->populateObject();
}

void WebMapWayPointView::saveWayPoint(const WayPoint &wp)
{
  emit updateWayPoint(wp);
}
