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

#ifndef WebMapOverlayItem_h
#define WebMapOverlayItem_h

#include <QGraphicsItem>

class WebMap;
class WebMapOverlay;
class WebMapOverlayItem;

typedef QList<WebMapOverlayItem*> WebMapOverlayItemList;

/**
	@author Alex Graf <grafal@sourceforge.net>
*/
class WebMapOverlayItem: public QGraphicsItem
{
	public:
		typedef enum ItemType{WebMapTurnPointType = UserType + 1, WebMapLegType, WebMapFaiSectorType}ItemType;

		WebMapOverlayItem(ItemType type);

		virtual ~WebMapOverlayItem();

		int type() const;

		void setColor(const QColor &color);

		const QColor& getColor() const;

	protected:
		virtual void adjust();

		virtual void updatePos();

		void setMap(WebMap *pMap);

		WebMap* getMap();

		void setMapOverlay(WebMapOverlay *pOverlay);

		WebMapOverlay* getMapOverlay();

	private:
		ItemType m_type;
		QColor m_color;
		WebMap *m_pMap;
		WebMapOverlay *m_pOverlay;

		friend class WebMapWidget;
		friend class WebMapTurnPoint;
		friend class WebMapOverlayScene;
};

#endif
