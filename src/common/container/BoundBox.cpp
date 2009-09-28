/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                         *
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

void BoundBox::setMinMax(const WayPoint &wp)
{
	if(m_isInit)
	{
		m_sw.setMin(wp);
		m_ne.setMax(wp);
	}
	else
	{
		m_isInit = true;
		m_sw = wp;
		m_ne = wp;
	}
}

void BoundBox::setNorthEast(const WayPoint &ne)
{
	m_ne = ne;
}

double BoundBox::north() const
{
	return m_ne.latitude();
}

double BoundBox::east() const
{
	return m_ne.longitude();
}


double BoundBox::width() const
{
	return (east() - west());
}

double BoundBox::height() const
{
	return (north() - south());
}

void BoundBox::setSouthWest(const WayPoint &sw)
{
	m_sw = sw;
}

double BoundBox::south() const
{
	return m_sw.latitude();
}

double BoundBox::west() const
{
	return m_sw.longitude();
}

bool BoundBox::intersect(const BoundBox &bb) const
{
	return !((bb.west() > east()) || (bb.east() < west()) ||
			(bb.north() < south()) || (bb.south() > north()));
}
