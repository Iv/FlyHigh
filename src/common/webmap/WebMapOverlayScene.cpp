/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf   *
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
#include "WebMapOverlayScene.h"
#include "WebMapOverlayItem.h"

WebMapOverlayScene::WebMapOverlayScene(QObject *pParent)
	:QGraphicsScene(pParent)
{
	m_dragMap = false;
}

WebMapOverlayScene::~WebMapOverlayScene()
{
}

void WebMapOverlayScene::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
	QPointF diff;
	QRectF rect;

	if(QGraphicsScene::mouseGrabberItem() == NULL)
	{
		rect = sceneRect();
		rect.adjust(1, 1, -1, -1);

		if(rect.contains(pEvent->scenePos()))
		{
				emit mouseSignal(pEvent);
		
				if(m_dragMap)
				{
		
					diff = (pEvent->scenePos() - m_lastMousePos);
					itemsMoveBy(diff.x(), diff.y());
				}
		
			m_lastMousePos = pEvent->scenePos();
		}
	}
	else
	{
		// drag item
		QGraphicsScene::mouseMoveEvent(pEvent);
	}
}

void WebMapOverlayScene::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
	QGraphicsScene::mousePressEvent(pEvent);

	if(QGraphicsScene::mouseGrabberItem() == NULL)
	{
		if(pEvent->button() == Qt::LeftButton)
		{
			m_dragMap = true;
			emit mouseSignal(pEvent);
		}
	}
}

void WebMapOverlayScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
	QGraphicsScene::mouseReleaseEvent(pEvent);

	if(QGraphicsScene::mouseGrabberItem() == NULL)
	{
		if(pEvent->button() == Qt::LeftButton)
		{
			m_dragMap = false;
			emit mouseSignal(pEvent);
		}
	}
}

void WebMapOverlayScene::wheelEvent(QGraphicsSceneWheelEvent *pEvent)
{
	pEvent->accept();
	emit wheelSignal(pEvent);
	updateItemsPos();
}

void WebMapOverlayScene::updateItemsPos()
{
	typedef QList<QGraphicsItem*> GraphicItemList;

	GraphicItemList list;
	GraphicItemList::iterator it;
	WebMapOverlayItem *pItem;

	list = QGraphicsScene::items();

	for(it=list.begin(); it!=list.end(); it++)
	{
		pItem = dynamic_cast<WebMapOverlayItem*>(*it);

		if(pItem != NULL)
		{
			pItem->updatePos();
		}
	}
}

void WebMapOverlayScene::itemsMoveBy(qreal dx, qreal dy)
{
	typedef QList<QGraphicsItem*> GraphicItemList;

	GraphicItemList list;
	GraphicItemList::iterator it;
	WebMapOverlayItem *pItem;

	list = QGraphicsScene::items();

	for(it=list.begin(); it!=list.end(); it++)
	{
		pItem = dynamic_cast<WebMapOverlayItem*>(*it);

		if(pItem != NULL)
		{
			pItem->moveBy(dx, dy);
		}
	}
}
