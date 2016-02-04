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
#include "LatLng.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

LatLng::LatLng()
{
  m_lat = 0;
  m_lon = 0;
}

LatLng::LatLng(double lat, double lon)
{
  m_lat = lat;
  m_lon = lon;
}

void LatLng::setPos(double lat, double lon)
{
  m_lat = lat;
  m_lon = lon;
}

void LatLng::setLat(double lat)
{
  m_lat = lat;
}

double LatLng::lat() const
{
  return m_lat;
}

void LatLng::setLon(double lon)
{
  m_lon = lon;
}

double LatLng::lon() const
{
  return m_lon;
}


double LatLng::distance(const LatLng &latlng) const
{
  return distance(latlng, *this);
}

void LatLng::distBear(const LatLng &latlng, double &dist, double &bear) const
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
  lat2 = (latlng.lat() * M_PI / 180);
  lon2 = (latlng.lon() * M_PI / 180);

  deltaLat = lat1 - lat2;
  deltaLon = lon1 -lon2;

  // distance
  angle = sin(deltaLat / 2) * sin(deltaLat / 2) +
      cos(lat1) * cos(lat2) * sin(deltaLon / 2) * sin(deltaLon / 2);
  dist = 2 * atan2(sqrt(angle), sqrt(1 - angle)) * EarthRadius;

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

void LatLng::setMin(const LatLng &latlng)
{
  m_lat = MIN(latlng.m_lat, m_lat);
  m_lon = MIN(latlng.m_lon, m_lon);
}

void LatLng::setMax(const LatLng &latlng)
{
  m_lat = MAX(latlng.m_lat, m_lat);
  m_lon = MAX(latlng.m_lon, m_lon);
}

bool LatLng::operator==(const LatLng &latlng)
{
  return (m_lat == latlng.m_lat) && (m_lon == latlng.m_lon);
}

LatLng LatLng::operator+(const LatLng& right) const
{
  return LatLng(m_lat + right.m_lat, m_lon + right.m_lon);
}

LatLng LatLng::operator-(const LatLng& right) const
{
  return LatLng(m_lat - right.m_lat, m_lon - right.m_lon);
}

LatLng LatLng::operator*(double right) const
{
  return LatLng(m_lat * right, m_lon * right);
}

uint LatLng::distance(const LatLng &latlng1, const LatLng &latlng2)
{
  double dist;
  double angle; // in radians
  double lat1; // in radians
  double lat2; // in radians
  double deltaLong; // in radians

  // P1, L1 Latitude and Longitude of Point 1
  // P2, L2 Latitude and Longitude of Point 2
  lat1 = latlng1.lat() * M_PI / 180;
  lat2 = latlng2.lat() * M_PI / 180;
  deltaLong = (latlng2.lon() - latlng1.lon()) * M_PI / 180;

  // cos E = sin P1 * sin P2 + cos P1 * cos P2 * cos (L2 - L1)
  angle = sinl(lat1) * sinl(lat2) + cosl(lat1) * cosl(lat2) * cosl(deltaLong);

  // FAI Earth Radius is 6371 km
  dist = EarthRadius * acosl(angle);

  return (uint)round(dist);
}

double LatLng::arc(double distance)
{
  double a;

  a = distance / EarthRadius;

  return (a * 180.0 / M_PI);
}

double LatLng::meters(double nautmil)
{
  return (1852.0 * nautmil);
}
