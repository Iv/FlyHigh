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

#ifndef LatLng_h
#define LatLng_h

#include <QVector>

class LatLng;

typedef QVector<LatLng> LatLngList;

/**
  Container for Latitude and Longitude. All units are meters and decimal degrees
*/
class LatLng
{
  public:
    static constexpr double EarthRadius = 6371000.0;

    LatLng();

    LatLng(double lat, double lon);

    // latitude and longitude are in WGS84 (degree)
    void setPos(double lat, double lon);

    void setLat(double lat);

    double lat() const;

    void setLon(double lon);

    double lon() const;

    double distance(const LatLng &latlng) const;

    /**
      Calculates distance and bearing from this waypoint to wp. The
      calculation is done by the Haversine formula.
      @param wp distance and bearing are calculated to this waypoint
      @param dist distance in meters
      @param bear bearing in degrees
    */
    void distBear(const LatLng &latlng, double &dist, double &bear) const;

    void setMin(const LatLng &latlng);

    void setMax(const LatLng &latlng);

    bool operator==(const LatLng &latlng);

    LatLng operator+(const LatLng& right) const;

    LatLng operator-(const LatLng& right) const;

    LatLng operator*(double right) const;

    static uint distance(const LatLng &latlng1, const LatLng &latlng2);

    static double arc(double distance);

    static double meters(double nautmil);

  private:
    double m_lat;
    double m_lon;
};

#endif
