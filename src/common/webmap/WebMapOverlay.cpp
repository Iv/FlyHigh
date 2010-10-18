/***************************************************************************
 *   Copyright (C) 2009 by Alex Graf   *
 *   grafal@sourceforge.net   *
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

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include "WebMapOverlay.h"
#include "WebMapItem.h"
#include "WebMapScene.h"

WebMapOverlay::WebMapOverlay(QWidget *pParent)
	:QGraphicsView(pParent)
{
	WebMapScene *pScene;

	pScene = new WebMapScene(this);
	pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(pScene);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setStyleSheet("background: transparent");
	setCacheMode(CacheBackground);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	setResizeAnchor(AnchorViewCenter);
}

WebMapOverlay::~WebMapOverlay()
{
}

void WebMapOverlay::addItem(WebMapItem *pItem)
{
	scene()->addItem(pItem);
}

void WebMapOverlay::removeItem(WebMapItem *pItem)
{
	scene()->removeItem(pItem);
}

void WebMapOverlay::clear()
{
	scene()->clear();
}

void WebMapOverlay::resizeEvent(QResizeEvent *pEvent)
{
	scene()->setSceneRect(0, 0, width(), height());
}

WebMapScene* WebMapOverlay::getScene()
{
	return (WebMapScene*)scene();
}
