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
#include "WayPoint.h"

const uint WayPoint::startLandRadius = 500;
const double WayPoint::earthRadius = 6371000.0;
 
WayPoint::WayPoint()
{
	m_name = "";
	m_desc = "";
	m_lat = 0;
	m_lon = 0;
	m_alt = 0;
}

const QString& WayPoint::name()
{
	return m_name;
}

void WayPoint::setName(const QString &name)
{
		m_name = name;
}

const QString& WayPoint::description()
{
	return m_desc;
}

void WayPoint::setDescription(const QString &desc)
{
	m_desc = desc;
}

void WayPoint::setWayPoint(const WayPoint &wp)
{
	*this = wp;
}

void WayPoint::setCoordinates(double lat, double lon, int alt)
{
	m_lat = lat;
	m_lon = lon;
	m_alt = alt;
}

double WayPoint::latitude()
{
	return m_lat;
}

double WayPoint::longitude()
{
	return m_lon;
}

int WayPoint::altitude()
{
	return m_alt;
}

uint WayPoint::distance(WayPoint &wp)
{
	return distance(wp, *this);
}

WayPoint& WayPoint::operator=(const WayPoint &wp)
{
	m_name = wp.m_name;
	m_desc = wp.m_desc;
	m_lat = wp.m_lat;
	m_lon = wp.m_lon;
	m_alt = wp.m_alt;
	
	return *this;
}

uint WayPoint::distance(WayPoint &wp1, WayPoint &wp2)
{
	double dist;
	double angle; // in radians
	double lat1; // in radians
	double lat2; // in radians
	double deltaLong; // in radians
	
	// P1, L1 Latitude and Longitude of Point 1
	// P2, L2 Latitude and Longitude of Point 2
	lat1 = wp1.latitude() * M_PI / 180;
	lat2 = wp2.latitude() * M_PI / 180;
	deltaLong = (wp2.longitude() - wp1.longitude()) * M_PI / 180;
	
	// cos E = sin P1 * sin P2 + cos P1 * cos P2 * cos (L2 - L1)
	angle = sinl(lat1) * sinl(lat2) + cosl(lat1) * cosl(lat2) * cosl(deltaLong);

	// FAI Earth Radius is 6371 km
	dist = earthRadius * acosl(angle);
	
	return (uint)round(dist);
}

double WayPoint::arc(uint distance)
{
	double a;
	
	a = distance / earthRadius;
	
	return (a * 180.0 / M_PI);
}

uint WayPoint::meters(double nautmil)
{
	return (uint)round(1852.0 * nautmil);
}
