/***************************************************************************
 *   Copyright (C) 2012 by Alex Graf                                       *
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
#include "OpenAirItem.h"

OpenAirItem::OpenAirItem()
{
}

OpenAirItem::~OpenAirItem()
{
}

void OpenAirItem::setPos(double lat, double lon)
{
  m_pos.setPos(lat, lon);
}

const LatLng& OpenAirItem::pos() const
{
  return m_pos;
}

double OpenAirItem::lat()
{
	return m_pos.lat();
}

double OpenAirItem::lon()
{
	return m_pos.lon();
}

OpenAirItem::ItemType OpenAirItem::type()
{
	return m_type;
}

void OpenAirItem::setType(ItemType type)
{
	m_type = type;
}

/*****************
Point
******************/
OpenAirItemPoint::OpenAirItemPoint(OpenAirItem::ItemType type)
{
	setType(type);
}

OpenAirItemPoint::~OpenAirItemPoint()
{
}

/*****************
Segment
******************/
OpenAirItemSeg::OpenAirItemSeg(OpenAirItem::ItemType type)
{
	setType(type);
}

OpenAirItemSeg::~OpenAirItemSeg()
{
}

void OpenAirItemSeg::setDir(bool dir)
{
	m_dir = dir;
}

bool OpenAirItemSeg::dir()
{
	return m_dir;
}

/*****************
Circle
******************/
OpenAirItemCircle::OpenAirItemCircle()
{
	setType(OpenAirItem::Circle);
}

OpenAirItemCircle::~OpenAirItemCircle()
{
}

void OpenAirItemCircle::setRadius(uint radius)
{
	m_radius = radius;
}

uint OpenAirItemCircle::radius()
{
	return m_radius;
}
