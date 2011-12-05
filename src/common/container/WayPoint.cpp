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

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

const uint WayPoint::startLandRadius = 400;
const double WayPoint::earthRadius = 6371000.0;

WayPoint::WayPoint()
{
	m_id = -1;
	m_name = "";
	m_desc = "";
	m_country = "";
	m_spot = "";
	m_lat = 0;
	m_lon = 0;
	m_alt = 0;
  m_type = TypeStartLand;
  m_radius = 400;
}

WayPoint::WayPoint(double lat, double lon)
{
	m_id = -1;
	m_name = "";
	m_desc = "";
	m_lat = lat;
	m_lon = lon;
	m_alt = 0;
	m_country = "";
	m_spot = "";
}

int WayPoint::id() const
{
	return m_id;
}

void WayPoint::setId(int id)
{
	m_id = id;
}

const QString& WayPoint::name() const
{
	return m_name;
}

void WayPoint::setName(const QString &name)
{
	m_name = name;
}

void WayPoint::fullName(QString& name) const
{
	name = m_name;

	if(m_country != "")
	{
	 name += " (" + m_country + ")";
	}
}

void WayPoint::setSpot(const QString &spot)
{
	m_spot = spot;
}

const QString& WayPoint::spot() const
{
	return m_spot;
}

void WayPoint::setCountry(const QString &country)
{
	m_country = country;
}

const QString& WayPoint::country() const
{
	return m_country;
}

const QString& WayPoint::description() const
{
	return m_desc;
}

void WayPoint::setDescription(const QString &desc)
{
	m_desc = desc;
}

void WayPoint::setCoordinates(double lat, double lon, int alt)
{
	m_lat = lat;
	m_lon = lon;
	m_alt = alt;
}

void WayPoint::setLatitude(double lat)
{
	m_lat = lat;
}

double WayPoint::latitude() const
{
	return m_lat;
}

void WayPoint::setLongitude(double lon)
{
	m_lon = lon;
}

double WayPoint::longitude() const
{
	return m_lon;
}

void WayPoint::setAltitude(int alt)
{
	m_alt = alt;
}

int WayPoint::altitude() const
{
	return m_alt;
}

void WayPoint::setType(Type type)
{
  m_type = type;
}

WayPoint::Type WayPoint::type() const
{
  return m_type;
}

void WayPoint::setRadius(int radius)
{
  m_radius = radius;
}

int WayPoint::radius() const
{
  return m_radius;
}

uint WayPoint::distance(const WayPoint &wp) const
{
	return distance(wp, *this);
}

void WayPoint::distBear(const WayPoint &wp, double &dist, double &bear) const
{
	double deltaLat;
	double deltaLon;
	double angle;
	double lat1;
	double lon1;
	double lat2;
	double lon2;

	lat1 = (m_lat * M_PI / 180);
	lon1 = (m_lon * M_PI / 180);
	lat2 = (wp.latitude() * M_PI / 180);
	lon2 = (wp.longitude() * M_PI / 180);

	deltaLat = lat1 - lat2;
	deltaLon = lon1 -lon2;

	// distance
	angle = sin(deltaLat / 2) * sin(deltaLat / 2) +
			cos(lat1) * cos(lat2) * sin(deltaLon / 2) * sin(deltaLon / 2);
	dist = 2 * atan2(sqrt(angle), sqrt(1 - angle)) * earthRadius;

	// start angle
	bear = atan2(sin(deltaLon) * cos(lat2),
			cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(deltaLon));

	bear = (bear * 180 / M_PI);

	// angle from +/-180� to 0-360�
	if(bear < 0)
	{
		bear = -bear;
	}
	else if(bear > 0)
	{
		bear = 360 - bear;
	}
}

bool WayPoint::operator==(const WayPoint &wp)
{
	return (m_name == wp.m_name) && (m_spot == wp.m_spot) && (m_country == wp.m_country);
}

uint WayPoint::distance(const WayPoint &wp1, const WayPoint &wp2)
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

void WayPoint::setMin(const WayPoint &wp)
{
	m_lat = MIN(wp.m_lat, m_lat);
	m_lon = MIN(wp.m_lon, m_lon);
	m_alt = MIN(wp.m_alt, m_alt);
}

void WayPoint::setMax(const WayPoint &wp)
{
	m_lat = MAX(wp.m_lat, m_lat);
	m_lon = MAX(wp.m_lon, m_lon);
	m_alt = MAX(wp.m_alt, m_alt);
}
