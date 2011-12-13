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

#ifndef WayPoints_h
#define WayPoints_h

#include <qstring.h>
#include "DataBaseSub.h"
#include "WayPoint.h"

class QSqlDatabase;

class WayPoints: public DataBaseSub
{
	public:
		WayPoints(QSqlDatabase DB);

		bool add(WayPoint &wp);

		bool update(WayPoint &wp);

		bool delWayPoint(WayPoint &wp);

		bool delAllWayPoints();

		bool wayPoint(int id, WayPoint &wp);

		// searches a WayPoint around radius from lat and lon of wp
		// fills values of wp and returns database ID
		// radius in meters
		bool findWayPoint(WayPoint &wp, uint radius);

		bool wayPointList(WayPoint::Type type, WayPoint::WayPointListType &wpList);

		bool checkModified();

	private:
		enum Elements
		{
			Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description, Type
		};

		bool setId(WayPoint &wp);
};

#endif
