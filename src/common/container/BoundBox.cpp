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

#include <math.h>
#include "BoundBox.h"

BoundBox::BoundBox()
{
	m_isInit = false;
}

BoundBox::BoundBox(const LatLng &sw, const LatLng &ne)
{
	m_isInit = false;
  m_ne = ne;
  m_sw = sw;
}

void BoundBox::init()
{
	m_isInit = false;
}

void BoundBox::setMinMax(const LatLng &latlng)
{
	if(m_isInit)
	{
		m_sw.setMin(latlng);
		m_ne.setMax(latlng);
	}
	else
	{
		m_isInit = true;
		m_sw = latlng;
		m_ne = latlng;
	}
}

void BoundBox::setMinMax(const BoundBox &bbox)
{
	if(m_isInit)
	{
		m_sw.setMin(bbox.m_sw);
		m_ne.setMax(bbox.m_ne);
	}
	else
	{
		m_isInit = true;
		m_sw = bbox.m_sw;
		m_ne = bbox.m_ne;
	}
}

void BoundBox::setNorthEast(const LatLng &ne)
{
	m_ne = ne;
}

double BoundBox::north() const
{
	return m_ne.lat();
}

double BoundBox::east() const
{
	return m_ne.lon();
}

double BoundBox::width() const
{
	return (east() - west());
}

double BoundBox::height() const
{
	return (north() - south());
}

void BoundBox::setSouthWest(const LatLng &sw)
{
	m_sw = sw;
}

double BoundBox::south() const
{
	return m_sw.lat();
}

double BoundBox::west() const
{
	return m_sw.lon();
}

bool BoundBox::intersect(const BoundBox &bb) const
{
  bool inter;

  inter = !((bb.west() > east()) || (bb.east() < west()) ||
			(bb.north() < south()) || (bb.south() > north()));

	return inter;
}
