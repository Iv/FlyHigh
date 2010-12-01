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

#include <math.h>
#include <QApplication>
#include <QVariant>
#include <QAction>
#include <QActionGroup>
#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include "WebMapFaiSector.h"
#include "WebMapLeg.h"
#include "WebMapWidget.h"
#include "WebMapFlightView.h"
#include "WebMapTurnPoint.h"
#include "WebMapTrack.h"
#include "WebMap.h"

WebMapFlightView::WebMapFlightView(const QString &name)
{
	QWidget::setWindowTitle(name);
	resize(1000, 800);

	// Map
	m_pWebMapWidget = new WebMapWidget(this);

	m_pWebMapWidget->setGeometry(QRect(0, 0, width(), height()));

	m_pTrack = new WebMapTrack();
	m_pWebMapWidget->addItem(m_pTrack);
	connect(m_pWebMapWidget->getMap(), SIGNAL(mapReady()), this, SLOT(mapReady()));
}

WebMapFlightView::~WebMapFlightView()
{
}

void WebMapFlightView::setWayPointList(const WayPoint::WayPointListType &wpList)
{
	m_pTrack->setWayPointList(wpList);
}

void WebMapFlightView::setTurnPointList(const FlightPointList &tpList, bool tri)
{
	m_pTrack->setTurnPointList(tpList, tri);
}

void WebMapFlightView::loadMap()
{
	m_pWebMapWidget->loadMap();
}

void WebMapFlightView::resizeEvent(QResizeEvent *pEvent)
{
	if(m_pWebMapWidget != NULL)
	{
		m_pWebMapWidget->setGeometry(QRect(0, 0, width(), height()));
	}
}

void WebMapFlightView::mapReady()
{
	m_pTrack->setTurnPointListToMap();
	m_pWebMapWidget->getMap()->XCLoad();
	m_pTrack->setWayPointListToMap();
}
