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

#ifndef WayPoint_h
#define WayPoint_h

#include <QString>
#include <QVector>
#include "LatLng.h"

/****************************************
COMMON
distance in meters, arc in degrees
****************************************/

class WayPoint
{
	public:
    typedef enum Type{TypeStartLand = 1, TypeBuoy = 2, TypeTurnPoint = 4}Type;

		typedef QVector<WayPoint> WayPointListType;

		typedef QVector<int> WayPointIdListType;

		typedef QVector<int> AltListType;

		static const uint startLandRadius; // in meters

		static const double earthRadius; // in meters

		WayPoint();

		WayPoint(double lat, double lon);

		int id() const;

		void setId(int id);

		const QString& name() const;

		void setName(const QString &name);

    void fullName(QString& name) const;

		void setSpot(const QString &spot);

		const QString& spot() const;

		void setCountry(const QString &country);

		const QString& country() const;

		void setDescription(const QString &desc);

    const QString& description() const;

		// latitude and longitude are in WGS84 (degree)
		void setCoordinates(double lat, double lon, int alt = 0);

		void setPos(const LatLng &pos);

		const LatLng& pos() const;

		void setLat(double lat);

		double lat() const;

		void setLon(double lon);

		double lon() const;

		void setAlt(int alt);

		int alt() const;

		void setType(Type type);

		Type type() const;

		uint distance(const WayPoint &wp) const;

		/**
			Calculates distance and bearing from this waypoint to wp. The
			calculation is done by the Haversine formula.
			@param wp distance and bearing are calculated to this waypoint
			@param dist distance in meters
			@param bear bearing in degrees
		*/
		void distBear(const WayPoint &wp, double &dist, double &bear) const;

		static uint distance(const WayPoint &wp1, const WayPoint &wp2);

		static double arc(uint distance);

		static uint meters(double nautmil);

		void setMin(const WayPoint &wp);

		void setMax(const WayPoint &wp);

		bool operator==(const WayPoint &wp);

	private:
		int m_id;
		QString m_name;
		QString m_desc;
		QString m_country;
		QString m_spot;
		LatLng m_pos;
		int m_alt;
    Type m_type;
};

#endif
