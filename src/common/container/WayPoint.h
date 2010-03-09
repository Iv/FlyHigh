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
 
#ifndef WayPoint_h
#define WayPoint_h

#include <qstring.h>
#include <qvaluevector.h>

/****************************************
COMMON
distance in meters, arc in degrees
****************************************/

class WayPoint
{
	public:
		typedef QValueVector<WayPoint> WayPointListType;
		typedef QValueVector<int> WayPointIdListType;
		static const uint startLandRadius; // in meters
		static const double earthRadius; // in meters
		
		WayPoint();

		int id() const;

		void setId(int id);

		const QString& name() const;

		void setName(const QString &name);

		void setSpot(const QString &spot);

		const QString& spot() const;

		void setCountry(const QString &country);

		const QString& country() const;

		void fullName(QString& name) const;

		const QString& description() const;

		void setDescription(const QString &desc);

		// latitude and longitude are in WGS84 (degree)
		void setCoordinates(double lat, double lon, int alt=0);

		void setLatitude(double lat);

		double latitude() const;

		void setLongitude(double lon);

		double longitude() const;

		void setAltitude(int alt);

		int altitude() const;
		
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
		double m_lat;
		double m_lon;
		int m_alt;
		QString m_country;
		QString m_spot;
};

#endif
