/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <QKeyEvent>
#include <QScrollArea>
#include <QScrollBar>
#include "MapView.h"
#include "MapWidget.h"

#include <QDebug>

MapView::MapView(QWidget *parent, const char *name, Qt::WindowFlags wFlags)
{
  QWidget *pWidget;

  m_pScrollArea = new QScrollArea();
  pWidget = new MapWidget(m_pScrollArea);
  m_pScrollArea->setWidget(pWidget);
  setCentralWidget(m_pScrollArea);
  resize(1000, 850);

  pWidget->show();

m_pScrollArea->viewport()->installEventFilter(this);
}

MapView::~MapView()
{
}

void MapView::showWayPointList(WayPoint::WayPointListType &wpList)
{
  MapWidget *pWidget;

  pWidget = static_cast<MapWidget*>(m_pScrollArea->widget());
  pWidget->showWayPointList(wpList);
}

void MapView::keyPressEvent(QKeyEvent *pEvent)
{
  MapWidget *pWidget;

  pWidget = static_cast<MapWidget*>(m_pScrollArea->widget());

	switch(pEvent->key())
	{
		case Qt::Key_Plus:
      pWidget->zoomIn();
		break;
		case Qt::Key_Minus:
      pWidget->zoomOut();
		break;
		default:
		break;
	}
}

bool MapView::eventFilter(QObject *pObject, QEvent *pEvent)
{
  QWheelEvent *pWheelEvent;
  QMouseEvent *pMouseEvent;
  bool used = false;

  switch(pEvent->type())
  {
    case QEvent::Wheel:
      pWheelEvent = static_cast<QWheelEvent*>(pEvent);
      wheel(pWheelEvent);
      used = true;
    break;
    case QEvent::MouseMove:
      pMouseEvent = static_cast<QMouseEvent*>(pEvent);
      mouseMove(pMouseEvent);
      used = true;
    break;
    case QEvent::MouseButtonPress:
      pMouseEvent = static_cast<QMouseEvent*>(pEvent);
      m_prevMousePos = pMouseEvent->pos();
      used = true;
    break;
    default:
    break;
  }

  return used;
}

void MapView::mouseMove(QMouseEvent *pEvent)
{
  QPoint delta;
  int value;

  delta = m_prevMousePos - pEvent->pos();
  m_prevMousePos = pEvent->pos();
  value = m_pScrollArea->horizontalScrollBar()->value() + delta.x();
  m_pScrollArea->horizontalScrollBar()->setValue(value);
  value = m_pScrollArea->verticalScrollBar()->value() + delta.y();
  m_pScrollArea->verticalScrollBar()->setValue(value);
}

void MapView::wheel(QWheelEvent *pEvent)
{
  MapWidget *pWidget;

  pWidget = static_cast<MapWidget*>(m_pScrollArea->widget());
  pWidget->zoom(pEvent->pos(), (pEvent->delta() > 0));
}

