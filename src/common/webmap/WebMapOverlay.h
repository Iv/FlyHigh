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
#ifndef WebMapOverlay_h
#define WebMapOverlay_h

#include <QGraphicsView>

class WebMapOverlayItem;
class WebMapScene;
class WebMapTurnPoint;

/**
	@author Alex Graf <grafal@sourceforge.net>
*/
class WebMapOverlay: public QGraphicsView
{
	public:
		WebMapOverlay(QWidget *pParent);

		virtual ~WebMapOverlay();

		void addItem(WebMapOverlayItem *pItem);

		void removeItem(WebMapOverlayItem *pItem);

		void clear();

	protected:
		void resizeEvent(QResizeEvent *pEvent);

		WebMapScene* getScene();

	private:
		WebMapTurnPoint *m_pCenterTurnPoint;

	friend class WebMapWidget;
};

#endif
