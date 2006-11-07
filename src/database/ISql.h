/***************************************************************************
 *   Copyright (C) 2006 by Alex Graf                                     *
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
#ifndef ISql_h
#define ISql_h

#include <qstring.h>
#include <qvaluelist.h>
#include "AirSpace.h"
#include "ContainerDef.h"
#include "IDataBase.h"
#include "WayPoint.h"
#include "Flight.h"
#include "Glider.h"
#include "Route.h"
#include "Pilot.h"
#include "Servicing.h"

class QSqlDatabase;
//class AirSpaces;
class WayPoints;
class Gliders;
class Flights;
class Routes;
class Servicings;
class Pilots;

/**
@author Alex Graf
*/
class ISql: public IDataBase
{
	public:
		static ISql* pInstance();
		~ISql();

		void setName(const QString &name);
		void setUserName(const QString &userName);
		void setPassword(const QString &passwd);
		void setHostName(const QString &hostName);
		void setPort(int port);
		
		bool add(WayPoint &wp);
		bool delWayPoint(WayPoint &wp);
		bool findWayPoint(WayPoint &wp, uint radius);
		bool wayPointList(WayPoint::WayPointListType &wpList);
		int wayPointsLastModified();

		bool add(Glider &glider);
		bool glider(const QString &model, Glider &glider);
		bool gliderList(Glider::GliderListType &gliderList);
		int glidersLastModified();

		bool add(Flight &flight);
		bool delFlight(Flight &flight);
		int newFlightNr();
		bool flightList(Pilot &pilot, Flight::FlightListType &flightList);
		bool flightsPerYear(FlightsPerYearListType &fpyList);
		bool loadIGCFile(Flight &flight);
		int flightsLastModified();
		
		bool add(Route &route);
		bool delRoute(Route &route);
		int routesLastModified();
		bool routeList(Route::RouteListType &routeList);
		
/* not longer supported
		bool add(AirSpace &airspace);
		bool delAirSpace(AirSpace &airspace);
		bool airspace(const QString &name, AirSpace &airspace);
		int airspacesLastModified();
		bool airspaceList(AirSpace::AirSpaceListType &airspaceList);
*/
		bool add(Servicing &serv);
		bool delServicing(Servicing &servicing);
		bool servicingList(Servicing::ServicingListType &servicingList);
		int servicingsLastModified();

		bool add(Pilot &pilot);
		bool update(Pilot &pilot);
		bool pilot(int id, Pilot &pilot);
		bool setId(Pilot &pilot);
		int pilotsLastModified();
		
		bool open();

	protected:
//		AirSpaces* pAirSpaceTable();
		WayPoints* pWayPointTable();
		Gliders* pGliderTable();
		Flights* pFlightTable();
		Servicings* pServicingTable();
		Pilots* pPilotTable();

	private:
		static ISql* m_pInst;
//		AirSpaces* m_pAirSpaces;
		WayPoints* m_pWayPoints;
		Gliders* m_pGliders;
		Flights* m_pFlights;
		Routes* m_pRoutes;
		Servicings* m_pServicings;
		Pilots* m_pPilots;
		QSqlDatabase* m_pDefaultDB;

		ISql();

		void setupTables();
		int tableVersion();
		void setTableVersion();

//		friend class AirSpaces;
		friend class WayPoints;
		friend class Gliders;
		friend class Flights;
		friend class Servicings;
		friend class Pilots;
		friend class Routes;
};

#endif
