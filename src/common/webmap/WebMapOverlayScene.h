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

#ifndef WebMapOverlayScene_h
#define WebMapOverlayScene_h

#include <QGraphicsScene>
#include <QMouseEvent>

/**
	@author Alex Graf <grafal@sourceforge.net>
*/
class WebMapOverlayScene: public QGraphicsScene
{
	Q_OBJECT

	public:
		WebMapOverlayScene(QObject *pParent);

		virtual ~WebMapOverlayScene();

	signals:
		void mouseSignal(QGraphicsSceneMouseEvent *pEvent);

		void wheelSignal(QGraphicsSceneWheelEvent *pEvent);

	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent);
		
		void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
		
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

		void wheelEvent(QGraphicsSceneWheelEvent *pEvent);

	protected slots:
		void updateItemsPos();

	private:
		QPointF m_lastMousePos;
		bool m_dragMap;

		void itemsMoveBy(qreal dx, qreal dy);
};

#endif
