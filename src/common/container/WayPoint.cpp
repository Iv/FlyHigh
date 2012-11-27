/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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
#include "WayPoint.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

const uint WayPoint::startLandRadius = 400;

WayPoint::WayPoint()
{
	m_id = -1;
	m_name = "";
	m_desc = "";
	m_country = "";
	m_spot = "";
	m_alt = 0;
  m_type = TypeStartLand;
}

WayPoint::WayPoint(double lat, double lon)
{
	m_id = -1;
	m_name = "";
	m_desc = "";
	m_pos.setPos(lat, lon);
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
  m_pos.setPos(lat, lon);
	m_alt = alt;
}

void WayPoint::setPos(const LatLng &pos)
{
  m_pos = pos;
}

const LatLng& WayPoint::pos() const
{
  return m_pos;
}

void WayPoint::setLat(double lat)
{
  m_pos.setLat(lat);
}

double WayPoint::lat() const
{
  return m_pos.lat();
}

void WayPoint::setLon(double lon)
{
  m_pos.setLon(lon);
}

double WayPoint::lon() const
{
  return m_pos.lon();
}

void WayPoint::setAlt(int alt)
{
	m_alt = alt;
}

int WayPoint::alt() const
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

uint WayPoint::distance(const WayPoint &wp) const
{
	return distance(wp, *this);
}

void WayPoint::distBear(const WayPoint &wp, double &dist, double &bear) const
{
  m_pos.distBear(wp.pos(), dist, bear);
}

bool WayPoint::operator==(const WayPoint &wp)
{
	return (m_name == wp.m_name) && (m_spot == wp.m_spot) && (m_country == wp.m_country);
}

uint WayPoint::distance(const WayPoint &wp1, const WayPoint &wp2)
{
  return wp1.pos().distance(wp2.pos());
}

double WayPoint::arc(uint distance)
{
	double a;

	a = distance / LatLng::EarthRadius;

	return (a * 180.0 / M_PI);
}

uint WayPoint::meters(double nautmil)
{
	return (uint)round(1852.0 * nautmil);
}

void WayPoint::setMin(const WayPoint &wp)
{
  m_pos.setMin(wp.pos());
	m_alt = MIN(wp.m_alt, m_alt);
}

void WayPoint::setMax(const WayPoint &wp)
{
  m_pos.setMax(wp.pos());
	m_alt = qMax(wp.m_alt, m_alt);
}
