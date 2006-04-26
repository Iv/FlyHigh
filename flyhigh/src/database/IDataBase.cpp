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
bool IDataBase::delFlight(int nr)
{
	(void)nr;

	return false;
}

int IDataBase::flightsLastModified()
{
	return 0;
}

bool IDataBase::flightList(Flight::FlightListType &flightList)
{
	(void)flightList;

	return false;
}

bool IDataBase::flightsPerYear(FlightsPerYearListType &fpyList)
{
	(void)fpyList;

	return false;
}

bool IDataBase::igcFile(uint flightNr, QByteArray &arr)
{
	(void)flightNr;
	(void)arr;

	return false;
}

bool IDataBase::add(WayPoint &wp)
{
	(void)wp;

	return false;
}

bool IDataBase::delWayPoint(const QString &name)
{
	(void)name;

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

bool IDataBase::add(Route &route)
{
	(void)route;

	return false;
}

bool IDataBase::delRoute(const QString &name)
{
	(void)name;

	return false;
}

bool IDataBase::route(const QString &name, Route &route)
{
	(void)name;
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
