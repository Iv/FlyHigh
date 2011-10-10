/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
 #include <stdlib.h>
#include "IDataBase.h"

IDataBase::IDataBase()
{
}

IDataBase::~IDataBase()
{
}

bool IDataBase::open()
{
	return false;
}

void IDataBase::close()
{
}

// flights
bool IDataBase::add(Flight &flight)
{
	return false;
}

bool IDataBase::delFlight(Flight &flight)
{
	return false;
}

int IDataBase::flightsLastModified()
{
	return 0;
}

bool IDataBase::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	return false;
}

bool IDataBase::loadIGCFile(Flight &flight)
{
	return false;
}

bool IDataBase::add(WayPoint &wp)
{
	return false;
}

bool IDataBase::add(WayPoint::WayPointListType &wpList)
{
	return false;
}

bool IDataBase::update(WayPoint &wp)
{
  return false;
}

bool IDataBase::update(WayPoint::WayPointListType &wpList)
{
	return false;
}

bool IDataBase::delWayPoint(WayPoint &wp)
{
	return false;
}

bool IDataBase::delWayPoints(WayPoint::WayPointListType &wpList)
{
	return false;
}

bool IDataBase::delAllWayPoints()
{
	return false;
}

int IDataBase::wayPointsLastModified()
{
	return 0;
}

bool IDataBase::wayPointList(WayPoint::WayPointListType &wpList)
{
	return false;
}

bool IDataBase::add(Glider &glider)
{
	return false;
}

bool IDataBase::delGlider(Glider &glider)
{
	return false;
}

bool IDataBase::gliderList(Glider::GliderListType &gliderList)
{
	return false;
}

int IDataBase::glidersLastModified()
{
	return 0;
}

bool IDataBase::add(Route &route)
{
	return false;
}

bool IDataBase::delRoute(Route &route)
{
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
	return false;
}

bool IDataBase::setId(Pilot &pilot)
{
	return false;
}

bool IDataBase::update(Pilot &pilot)
{
	return false;
}

bool IDataBase::pilot(int id, Pilot &pilot)
{
	return false;
}

int IDataBase::pilotsLastModified()
{
	return 0;
}

bool IDataBase::add(AirSpace &airspace)
{
	return false;
}

bool IDataBase::delAirSpace(AirSpace &airspace)
{
	return false;
}

bool IDataBase::airspace(const QString &name, AirSpace &airspace)
{
	return false;
}

int IDataBase::airspacesLastModified()
{
	return 0;
}

bool IDataBase::airspaceList(AirSpaceList &airspaceList)
{
	return false;
}

void IDataBase::cancel()
{
}
