/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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

class WayPoint
{
	public:
		typedef QValueVector<WayPoint> WayPointListType;
		typedef QValueVector<int> WayPointIdListType;
		static const uint startLandRadius; // in meters
		
		WayPoint();

		const QString& name();
		void setName(const QString &name);
		const QString& description();
		void setDescription(const QString &desc);
		void setWayPoint(const WayPoint &wp);
		// latitude and longitude are in WGS84 (degree)
		void setCoordinates(double lat, double lon, int alt);
		double latitude();
		double longitude();
		int altitude();
		
		// distance to an other WayPoint in meters
		uint distance(WayPoint &wp);
		
		WayPoint& operator=(const WayPoint &wp);
		
	private:
		QString m_name;
		QString m_desc;
		double m_lat;
		double m_lon;
		int m_alt;
};

#endif
