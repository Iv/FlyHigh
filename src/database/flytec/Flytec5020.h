/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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
#ifndef Flytec5020_h
#define Flytec5020_h

#include "IGPSDevice.h"

class Pilot;
class Protocol5020;
class AirSpaceList;

class Flytec5020: public IGPSDevice
{
	public:
		Flytec5020();

		virtual ~Flytec5020();

	protected:
		bool open();

		void close();

		void cancel();

		// flights
		bool flightList(Pilot &pilot, Flight::FlightListType &flightList);

		bool loadIGCFile(Flight &flight);

		// waypoints
		bool add(WayPoint &wp);

		bool delWayPoint(WayPoint &wp);

		bool delAllWayPoints();

		bool wayPointList(WayPoint::WayPointListType &wpList);

		// routes
		bool add(Route &route);

		bool routeList(Route::RouteListType &routeList);

		bool delRoute(Route &route);

				// airspaces
		bool add(AirSpace &airspace);

		bool delAirSpace(AirSpace &airspace);

		bool airspaceList(AirSpaceList &airspaceList);

	private:
		Protocol5020 *m_protocol;
		bool m_cancel;
};

#endif
