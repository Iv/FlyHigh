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
#include "Error.h"
#include "ISql.h" 
#include "WayPoints.h"
#include "Gliders.h"
#include "Flights.h"
#include "Routes.h"

ISql* ISql::m_pInst = NULL;

ISql::ISql()
{
	m_pDefaultDB = QSqlDatabase::addDatabase("QMYSQL3");
	
	setName("flyhigh");
	setUserName("flyhigh");
	setPassword("flyhigh");
	setHostName("localhost");
	setPort(3306);
	
	m_pWayPoints = new WayPoints(m_pDefaultDB);
	m_pGliders = new Gliders(m_pDefaultDB);
	m_pFlights = new Flights(m_pDefaultDB);
	m_pRoutes = new Routes(m_pDefaultDB);
}

ISql::~ISql()
{
	m_pDefaultDB->close();
	
	delete m_pWayPoints;
	delete m_pGliders;
	delete m_pFlights;
	delete m_pRoutes;
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

bool ISql::igcFile(uint flightNr, QByteArray &arr)
{
	return m_pFlights->igcFile(flightNr, arr);
}

int ISql::flightsLastModified()
{
	return m_pFlights->lastModified("Flights");
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
