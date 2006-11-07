/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
 #include <stdlib.h>
#include "IDataBase.h"

IDataBase::IDataBase()
{
}

IDataBase::~IDataBase()
{
}

// flights
bool IDataBase::add(Flight &flight)
{
	(void)flight;

	return false;
}

bool IDataBase::delFlight(Flight &flight)
{
	(void)flight;

	return false;
}

int IDataBase::flightsLastModified()
{
	return 0;
}

bool IDataBase::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	(void)pilot;
	(void)flightList;

	return false;
}

bool IDataBase::flightsPerYear(FlightsPerYearListType &fpyList)
{
	(void)fpyList;

	return false;
}

bool IDataBase::loadIGCFile(Flight &flight)
{
	(void)flight;

	return false;
}

bool IDataBase::add(WayPoint &wp)
{
	(void)wp;

	return false;
}

bool IDataBase::delWayPoint(WayPoint &wp)
{
	(void)wp;

	return false;
}

int IDataBase::wayPointsLastModified()
{
	return 0;
}

bool IDataBase::wayPointList(WayPoint::WayPointListType &wpList)
{
	(void)wpList;

	return false;
}

bool IDataBase::add(Glider &glider)
{
	(void)glider;

	return false;
}

bool IDataBase::delGlider(Glider &glider)
{
	(void)glider;

	return false;
}

bool IDataBase::gliderList(Glider::GliderListType &gliderList)
{
	(void)gliderList;

	return false;
}

int IDataBase::glidersLastModified()
{
	return 0;
}

bool IDataBase::add(Route &route)
{
	(void)route;

	return false;
}

bool IDataBase::delRoute(Route &route)
{
	(void)route;

	return false;
}

int IDataBase::routesLastModified()
{
	return 0;
}

bool IDataBase::routeList(Route::RouteListType &routeList)
{
	(void)routeList;

	return false;
}

bool IDataBase::add(Pilot &pilot)
{
	(void)pilot;

	return false;
}

bool IDataBase::setId(Pilot &pilot)
{
	(void)pilot;

	return false;
}

bool IDataBase::update(Pilot &pilot)
{
	(void)pilot;

	return false;
}

bool IDataBase::pilot(int id, Pilot &pilot)
{
	(void)id;
	(void)pilot;

	return false;
}

int IDataBase::pilotsLastModified()
{
	return 0;
}

bool IDataBase::add(AirSpace &airspace)
{
	(void)airspace;

	return false;
}

bool IDataBase::delAirSpace(const QString &name)
{
	(void)name;

	return false;
}

bool IDataBase::airspace(const QString &name, AirSpace &airspace)
{
	(void)name;
	(void)airspace;

	return false;
}

int IDataBase::airspacesLastModified()
{
	return 0;
}

bool IDataBase::airspaceList(AirSpace::AirSpaceListType &airspaceList)
{
	(void)airspaceList;

	return false;
}

void IDataBase::cancel()
{
}
