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

WebMapWayPointView::WebMapWayPointView(const QString &name, WayPoint::Type type)
{
	QWidget::setWindowTitle(name);
	resize(1000, 850);

  m_wpType = type;
	m_pWpList = NULL;
	m_editable = true;
	m_selItem = -1;
	m_pWebMap = new WebMap(this, WebMap::MapWayPoint);
	connect(m_pWebMap, SIGNAL(mapReady()), this, SLOT(mapReady()));
	connect(m_pWebMap, SIGNAL(appReady()), this, SLOT(appReady()));
	connect(m_pWebMap, SIGNAL(finished(int)), this, SLOT(finished(int)));
  connect(m_pWebMap, SIGNAL(lineChanged(int)), this, SIGNAL(wayPointChanged(int)));
}

WebMapWayPointView::~WebMapWayPointView()
{
	delete m_pWebMap;
}

void WebMapWayPointView::setWayPointList(WayPoint::WayPointListType *pWpList, int selItem)
{
	m_pWpList = pWpList;
	m_selItem = selItem;
}

WayPoint::WayPointListType& WebMapWayPointView::getModifiedWayPointList()
{
  return m_modWayPointList;
}

void WebMapWayPointView::selectWayPoint(uint id)
{
  m_pWebMap->getWayPoint()->selectWayPoint(id);
}

void WebMapWayPointView::loadMap()
{
	m_pWebMap->loadUrl("qrc:/waypoint/waypoint.html");
}

void WebMapWayPointView::setEditable(bool en)
{
  m_editable = en;
}

bool WebMapWayPointView::editable() const
{
  return m_editable;
}

void WebMapWayPointView::resizeEvent(QResizeEvent *pEvent)
{
	m_pWebMap->setGeometry(QRect(0, 0, width(), height()));
}

void WebMapWayPointView::mapReady()
{
	m_pWebMap->getWayPoint()->init();
}

void WebMapWayPointView::appReady()
{
	int itemNr;
	int listSize;

  m_pWebMap->setSize(width(), height());
	m_pWebMap->getWayPoint()->setEditable(m_editable);

  if(m_pWpList != NULL)
  {
    listSize = m_pWpList->size();

    for(itemNr=0; itemNr<listSize; itemNr++)
    {
      m_pWebMap->getWayPoint()->pushWayPoint(m_pWpList->at(itemNr));
    }

    m_pWebMap->getWayPoint()->selectWayPoint(m_pWpList->at(m_selItem).id());
  }
}

void WebMapWayPointView::finished(int res)
{
  WayPoint wp;

  m_modWayPointList.clear();

  if(res)
  {
    wp.setType(m_wpType); // restore type

    while(m_pWebMap->getWayPoint()->getNextModified(wp))
    {
      m_modWayPointList.push_back(wp);
    }
  }

	done(res);
}
