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
 
#ifndef IDataBase_h
#define IDataBase_h

#include <qdatetime.h>
#include <qvaluelist.h>
#include "Flight.h"
#include "Route.h"
#include "WayPoint.h"

class IDataBase
{
	public:
		typedef enum SourceType{GPSdevice, SqlDB};
		
		typedef struct
		{
			uint year;
			uint nFlights;
			QTime airTime;
		}FlightsPerYearType;
		typedef QValueList<FlightsPerYearType> FlightsPerYearListType;
				
		IDataBase();
		virtual ~IDataBase();

		// flights
		virtual bool delFlight(int nr);
		virtual int flightsLastModified();
		virtual bool flightList(Flight::FlightListType &flightList);
		virtual bool flightsPerYear(FlightsPerYearListType &fpyList);
		virtual bool igcFile(uint flightNr, QByteArray &arr);
		
		// waypoints
		virtual bool add(WayPoint &wp);
		virtual bool delWayPoint(const QString &name);
		virtual int wayPointsLastModified();
		virtual bool wayPointList(WayPoint::WayPointListType &wpList);
		
		// routes
		virtual bool add(Route &route);
		virtual bool delRoute(const QString &name);
		virtual bool route(const QString &name, Route &route);
		virtual int routesLastModified();
		virtual bool routeList(Route::RouteListType &routeList);
};

#endif
