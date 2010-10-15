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

#include "WebMapOverlayItem.h"

WebMapOverlayItem::WebMapOverlayItem(ItemType type)
{
	m_type = type;
	m_pMap = NULL;
	m_pOverlay = NULL;
}

WebMapOverlayItem::~WebMapOverlayItem()
{
}

int WebMapOverlayItem::type() const
{
	return m_type;
}

void WebMapOverlayItem::setColor(const QColor &color)
{
	m_color = color;
	update();
}

const QColor& WebMapOverlayItem::getColor() const
{
	return m_color;
}

void WebMapOverlayItem::adjust()
{
}

void WebMapOverlayItem::updatePos()
{
}

void WebMapOverlayItem::setMap(WebMap *pMap)
{
	m_pMap = pMap;
}

WebMap* WebMapOverlayItem::getMap()
{
	return m_pMap;
}

void WebMapOverlayItem::setMapOverlay(WebMapOverlay *pOverlay)
{
	m_pOverlay = pOverlay;
}

WebMapOverlay* WebMapOverlayItem::getMapOverlay()
{
	return m_pOverlay;
}
