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
#include "WebMapOverlay.h"
#include "WebMapOverlayItem.h"
#include "WebMapOverlayScene.h"
#include "WebMapWidget.h"

WebMapWidget::WebMapWidget(QWidget *pParent)
	:QWidget(pParent)
{
	m_pMap = new WebMap(this);
	m_pMap->load(QUrl("qrc:/index.html"));
	m_pOverlay = new WebMapOverlay(this);
	connect(m_pOverlay->getScene(), SIGNAL(mouseSignal(QGraphicsSceneMouseEvent*)), m_pMap, SLOT(mouseSlot(QGraphicsSceneMouseEvent*)));
	connect(m_pOverlay->getScene(), SIGNAL(wheelSignal(QGraphicsSceneWheelEvent*)), m_pMap, SLOT(wheelSlot(QGraphicsSceneWheelEvent*)));
	connect(m_pMap, SIGNAL(mapReady()), m_pOverlay->getScene(), SLOT(updateItemsPos()));
}

WebMapWidget::~WebMapWidget()
{
}

void WebMapWidget::addItem(WebMapOverlayItem *pItem)
{
	pItem->setMapOverlay(m_pOverlay);
	pItem->setMap(m_pMap);
	m_pOverlay->addItem(pItem);
}

void WebMapWidget::removeItem(WebMapOverlayItem *pItem)
{
	pItem->setMapOverlay(NULL);
	pItem->setMap(NULL);
	m_pOverlay->removeItem(pItem);
}

void WebMapWidget::clear()
{
	m_pOverlay->clear();
}

void WebMapWidget::resizeEvent(QResizeEvent *pEvent)
{
	(void)pEvent;

	m_pMap->setGeometry(0, 0, width(), height());
	m_pOverlay->setGeometry(0, 0, width(), height());
}
