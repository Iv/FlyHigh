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

#include "WebMapItem.h"

WebMapItem::WebMapItem(ItemType type)
{
	m_type = type;
	m_pMap = NULL;
	m_pOverlay = NULL;
}

WebMapItem::~WebMapItem()
{
}

int WebMapItem::type() const
{
	return m_type;
}

void WebMapItem::setColor(const QColor &color)
{
	m_color = color;
	update();
}

const QColor& WebMapItem::getColor() const
{
	return m_color;
}

void WebMapItem::adjust()
{
}

void WebMapItem::updatePos()
{
}

void WebMapItem::setMap(WebMap *pMap)
{
	m_pMap = pMap;
}

WebMap* WebMapItem::getMap()
{
	return m_pMap;
}

void WebMapItem::setMapOverlay(WebMapOverlay *pOverlay)
{
	m_pOverlay = pOverlay;
}

WebMapOverlay* WebMapItem::getMapOverlay()
{
	return m_pOverlay;
}

qreal WebMapItem::getPenWidth() const
{
	return 1.0;
}
