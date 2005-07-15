/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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

#ifndef GarminFmt_h
#define GarminFmt_h

#include "Flight.h"
#include "Route.h"
#include "WayPoint.h"

#include "gps.h"
#include <stdio.h>

class GarminFmt
{
	public:
		GarminFmt();
		
		void GPS_Fmt_Print_Track(GPS_PTrack *trk, int32 n, Flight::FlightListType &flightList);
		void GPS_Fmt_Print_Waypoint(GPS_PWay *way, int32 n, WayPoint::WayPointListType &wpList);
		void GPS_Fmt_Print_Route(GPS_PWay *way, int32 n, Route::RouteListType &routeList);
		
	private:
		void setBrecord(GPS_PTrack trk, QString &line);
		void setWayPoint(GPS_PWay way, WayPoint &wp);
};

#endif
