/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
#include "AirSpaceItem.h"

AirSpaceItem::AirSpaceItem()
{
}

AirSpaceItem::~AirSpaceItem()
{
}

void AirSpaceItem::setPoint(double lat, double lon)
{
	m_pos.setLatitude(lat);
	m_pos.setLongitude(lon);
}

double AirSpaceItem::lat()
{
	return m_pos.latitude();
}

double AirSpaceItem::lon()
{
	return m_pos.longitude();
}

const WayPoint& AirSpaceItem::pos()
{
	return m_pos;
}

AirSpaceItem::ItemType AirSpaceItem::type()
{
	return m_type;
}

void AirSpaceItem::setType(ItemType type)
{
	m_type = type;
}

/*****************
Point
******************/
AirSpaceItemPoint::AirSpaceItemPoint(AirSpaceItem::ItemType type)
{
	setType(type);
}

AirSpaceItemPoint::~AirSpaceItemPoint()
{
}

/*****************
Segment
******************/
AirSpaceItemSeg::AirSpaceItemSeg(AirSpaceItem::ItemType type)
{
	setType(type);
}

AirSpaceItemSeg::~AirSpaceItemSeg()
{
}

void AirSpaceItemSeg::setDir(bool dir)
{
	m_dir = dir;
}

bool AirSpaceItemSeg::dir()
{
	return m_dir;
}

/*****************
Circle
******************/
AirSpaceItemCircle::AirSpaceItemCircle()
{
	setType(AirSpaceItem::Circle);
}

AirSpaceItemCircle::~AirSpaceItemCircle()
{
}

void AirSpaceItemCircle::setRadius(uint radius)
{
	m_radius = radius;
}

uint AirSpaceItemCircle::radius()
{
	return m_radius;
}
