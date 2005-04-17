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
#include "Servicing.h"

// this is the interface class of database
class QSqlDatabase;
class AirSpaces;
class WayPoints;
class Gliders;
class Flights;
class Routes;
class Servicings;

class ISql: public IDataBase
{
	public:
		typedef QValueList<int> FlightNrListType;
		
		~ISql();
		static ISql* pInstance();
		
		void setName(const QString &name);
		void setUserName(const QString &userName);
		void setPassword(const QString &passwd);
		void setHostName(const QString &hostName);
		void setPort(int port);
		
		bool add(WayPoint &wp);
		bool delWayPoint(const QString &name);
		bool wayPoint(const QString &name, WayPoint &wp);
		bool findWayPoint(WayPoint &wp, uint radius);
		bool wayPointList(WayPoint::WayPointListType &wpList);
		int wayPointsLastModified();

		bool add(Glider &glider);
		bool glider(const QString &model, Glider &glider);
		bool gliderList(Glider::GliderListType &gliderList);
		int glidersLastModified();

		bool add(Flight &flight);
		bool delFlight(int nr);
		bool flight(int nr, Flight &flight);
		int newFlightNr();
		bool flightList(Flight::FlightListType &flightList);
		bool flightsPerYear(FlightsPerYearListType &fpyList);
		bool igcFile(uint flightNr, QByteArray &arr);
		int flightsLastModified();
		
		bool add(Route &route);
		bool delRoute(const QString &name);
		bool route(const QString &name, Route &route);
		int routesLastModified();
		bool routeList(Route::RouteListType &routeList);
		
		bool add(AirSpace &airspace);
		bool delAirSpace(const QString &name);
		bool airspace(const QString &name, AirSpace &airspace);
		int airspacesLastModified();
		bool airspaceList(AirSpace::AirSpaceListType &airspaceList);
		
		bool add(Servicing &serv);
		bool delServicing(int nr);
		bool servicingList(Servicing::ServicingListType &servicingList);
		int servicingsLastModified();
		
		bool open();
		
	private:
		static ISql* m_pInst;
		AirSpaces *m_pAirSpaces;
		WayPoints *m_pWayPoints;
		Gliders *m_pGliders;
		Flights *m_pFlights;
		Routes *m_pRoutes;
		Servicings *m_pServicings;
		QSqlDatabase *m_pDefaultDB;
		
		ISql();
};

#endif
