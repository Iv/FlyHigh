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
#ifndef FlytecDevice_h
#define FlytecDevice_h

#include <stdlib.h>
#include <qcstring.h>
#include "AirSpace.h"
#include "IGPSDevice.h"
#include "Flight.h"
#include "Route.h"
#include "WayPoint.h"

class FlytecDevice: public IGPSDevice
{
	public:
		FlytecDevice();
		
	protected:
		void open();
		void close();
		void cancel();
			
		// memory
		bool memoryRead(QByteArray &arr);
		bool memoryWrite(QByteArray &arr);
		
		bool flightList(Pilot &pilot, Flight::FlightListType &flightList);
		bool loadIGCFile(Flight &flight);

		bool add(WayPoint &wp);
		bool delWayPoint(WayPoint &wp);
		bool wayPointList(WayPoint::WayPointListType &wpList);
		
		bool add(Route &route);
		bool routeList(Route::RouteListType &routeList);
		bool delRoute(Route &route);
		
		bool add(AirSpace &airspace);
		bool airspaceList(AirSpace::AirSpaceListType &airspaceList);
		bool delAirSpace(AirSpace &airspace);
		
	private:
		bool m_cancel;
};

#endif
