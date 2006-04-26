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
 
#include <qsqldatabase.h> 
#include "AirSpaces.h"
#include "Error.h"
#include "ISql.h" 
#include "WayPoints.h"
#include "Gliders.h"
#include "Flights.h"
#include "Routes.h"
#include "Servicings.h"
#include "Upgrade.h"

ISql* ISql::m_pInst = NULL;

ISql::ISql()
{
	m_pDefaultDB = QSqlDatabase::addDatabase("QMYSQL3");
	
	setName("flyhigh");
	setUserName("flyhigh");
	setPassword("flyhigh");
	setHostName("localhost");
	setPort(3306);
	
	m_pAirSpaces = new AirSpaces(m_pDefaultDB);
	m_pWayPoints = new WayPoints(m_pDefaultDB);
	m_pGliders = new Gliders(m_pDefaultDB);
	m_pFlights = new Flights(m_pDefaultDB);
	m_pRoutes = new Routes(m_pDefaultDB);
	m_pServicings = new Servicings(m_pDefaultDB);
}

ISql::~ISql()
{
	m_pDefaultDB->close();
	
	delete m_pAirSpaces;
	delete m_pWayPoints;
	delete m_pGliders;
	delete m_pFlights;
	delete m_pRoutes;
	delete m_pServicings;
}

void ISql::setName(const QString &name)
{
	m_pDefaultDB->setDatabaseName(name);
}

void ISql::setUserName(const QString &userName)
{
	m_pDefaultDB->setUserName(userName);
}

void ISql::setPassword(const QString &passwd)
{
	m_pDefaultDB->setPassword(passwd);
}

void ISql::setHostName(const QString &hostName)
{
	m_pDefaultDB->setHostName(hostName);
}

void ISql::setPort(int port)
{
	m_pDefaultDB->setPort(port);
}

bool ISql::open()
{
	bool success;
	
	success = m_pDefaultDB->open();
	Error::verify(success, Error::SQL_OPEN);
	setupTables();
	
	return success;
}

ISql* ISql::pInstance()
{
	if(m_pInst == NULL)
	{
		m_pInst = new ISql();
	}
	
	return m_pInst;
}

bool ISql::add(WayPoint &wp)
{
	return m_pWayPoints->add(wp);
}

bool ISql::delWayPoint(const QString &name)
{
	return m_pWayPoints->delWayPoint(name);
}

bool ISql::wayPoint(const QString &name, WayPoint &wp)
{
	return m_pWayPoints->wayPoint(name, wp);
}

bool ISql::findWayPoint(WayPoint &wp, uint radius)
{
	return m_pWayPoints->findWayPoint(wp, radius);
}

int ISql::wayPointsLastModified()
{
	return m_pWayPoints->lastModified("WayPoints");
}

bool ISql::wayPointList(WayPoint::WayPointListType &wpList)
{
	return m_pWayPoints->wayPointList(wpList);
}

bool ISql::add(Glider &glider)
{
	return m_pGliders->add(glider);
}

bool ISql::glider(const QString &model, Glider &glider)
{
	return m_pGliders->glider(model, glider);
}

bool ISql::gliderList(Glider::GliderListType &gliderList)
{
	return m_pGliders->gliderList(gliderList);
}

int ISql::glidersLastModified()
{
	return m_pGliders->lastModified("Gliders");
}

bool ISql::add(Flight &flight)
{
	return m_pFlights->add(flight);
}

bool ISql::delFlight(int nr)
{
	return m_pFlights->delFlight(nr);
}

int ISql::newFlightNr()
{
	return m_pFlights->newFlightNr();
}

bool ISql::flightList(Flight::FlightListType &flightList)
{
	return m_pFlights->flightList(flightList);
}

bool ISql::flightsPerYear(FlightsPerYearListType &fpyList)
{
	return m_pFlights->flightsPerYear(fpyList);
}

bool ISql::igcFile(uint flightNr, QByteArray &arr)
{
	return m_pFlights->igcFile(flightNr, arr);
}

int ISql::flightsLastModified()
{
	return m_pFlights->lastModified();
}

bool ISql::add(Route &route)
{
	return m_pRoutes->add(route);
}

bool ISql::delRoute(const QString &name)
{
	return m_pRoutes->delRoute(name);
}

bool ISql::route(const QString &name, Route &route)
{
	return m_pRoutes->route(name, route);
}

int ISql::routesLastModified()
{
	return m_pRoutes->lastModified("Routes");
}

bool ISql::routeList(Route::RouteListType &routeList)
{
	return m_pRoutes->routeList(routeList);
}

bool ISql::add(AirSpace &airspace)
{
	return m_pAirSpaces->add(airspace);
}

bool ISql::delAirSpace(const QString &name)
{
	return m_pAirSpaces->delAirSpace(name);
}

bool ISql::airspace(const QString &name, AirSpace &airspace)
{
	return m_pAirSpaces->airspace(name, airspace);
}

int ISql::airspacesLastModified()
{
	return m_pAirSpaces->lastModified("AirSpaces");
}

bool ISql::airspaceList(AirSpace::AirSpaceListType &airspaceList)
{
	return m_pAirSpaces->airspaceList(airspaceList);
}

bool ISql::add(Servicing &serv)
{
	return m_pServicings->add(serv);
}

bool ISql::delServicing(int nr)
{
	return m_pServicings->delServicing(nr);
}

bool ISql::servicingList(Servicing::ServicingListType &servicingList)
{
	return m_pServicings->servicingList(servicingList);
}

int ISql::servicingsLastModified()
{
	return m_pServicings->lastModified("Servicings");
}

void ISql::setupTables()
{
/*	Upgrade upgrade(m_pDefaultDB);
	QString sqls;
	int tabVers_0_3_2 = QDateTime::fromString(m_TableVersion_0_3_2, Qt::ISODate).toTime_t();
	int tabVers_0_3_3 = QDateTime::fromString(m_TableVersion_0_3_3, Qt::ISODate).toTime_t();
	bool created;
	
	if(m_pFlights->setupTable())
	
	if(upgrade.tableVersion() < tabVers_0_3_2)
	{
		upgrade.upgradeFrom_0_3_1_to_0_3_2();
	}
	
	created = m_pFlights->createTable();
	created &= m_pAirSpaces->createTable();
	created &= m_pWayPoints->createTable();
	created &= m_pGliders->createTable();
	created &= m_pFlights->createTable();
	created &= m_pRoutes->createTable();
	created &= m_pServicings->createTable();
	
	if(created)
	{
		upgrade.setTableVersion(m_TableVersion_0_3_3);
	}
	else if(upgrade.tableVersion() == tabVers_0_3_2)
	{
		upgrade.upgradeFrom_0_3_2_to_0_3_3();
	}*/
}


