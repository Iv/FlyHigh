/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "ISql.h"
#include "Error.h"
#include "Routes.h"
#include "WayPoint.h"
#include "WayPoints.h"

Routes::Routes(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool Routes::add(Route &route)
{
	QSqlQuery query(db());
	uint nWps;
	uint wpNr;
	int id;
	bool success;

	// insert route name
	id = newId("Routes");
  success = query.prepare("INSERT INTO Routes(Id, Name, Type) VALUES(:id, :name, :type)");
  query.bindValue(":id", id);
  query.bindValue(":name", route.name());
  query.bindValue(":type", route.type());
  success &= query.exec();
	Error::verify(success, Error::SQL_ADD_ROUTE_NAME);

  if(route.id() == -1)
  {
    route.setId(id);
  }

	if(success)
	{
    success = query.prepare("INSERT INTO RouteItems(RouteId, WayPointId) VALUES (:route, :waypoint)");

		// insert route items
		nWps = route.wayPointList().size();

		for(wpNr=0; wpNr<nWps; wpNr++)
		{
      query.bindValue(":route", id);
      query.bindValue(":waypoint", route.wayPointList().at(wpNr).id());
      success &= query.exec();
		}

		Error::verify(success, Error::SQL_CMD);
		DataBaseSub::setLastModified("Routes");
	}

	return success;
}

bool Routes::delRoute(Route &route)
{
	WayPoint::WayPointListType::iterator it;
	QSqlQuery query(db());
	bool success;

	// first delete route items
  success = query.prepare("DELETE FROM RouteItems WHERE RouteId=:id");
  query.bindValue(":id", route.id());
  success &= query.exec();
	Error::verify(success, Error::SQL_CMD);

	// route
  success = query.prepare("DELETE FROM Routes WHERE Id=:id");
  query.bindValue(":id", route.id());
  success &= query.exec();
	Error::verify(success, Error::SQL_CMD);

	// waypoints (delete only types not used anymore)
  success = query.prepare("DELETE FROM WayPoints WHERE Id=:id");

	for(it=route.wayPointList().begin(); it!=route.wayPointList().end(); it++)
	{
    if(it->type() == WayPoint::TypeTurnPoint)
	  {
      query.bindValue(":id", it->id());
      success &= query.exec();
	  }
	}

	Error::verify(success, Error::SQL_DEL);
	DataBaseSub::setLastModified("WayPoints");
	DataBaseSub::setLastModified("Routes");

	return success;
}

bool Routes::routeList(Route::RouteListType &routeList)
{
	Route route;
	WayPoint wp;
  QSqlQuery routeQuery(db());
	QSqlQuery routeItemsQuery(db());
	QString sqls = "SELECT Id, Name, Type FROM Routes ORDER BY Name ASC;";
	int wpId;
	bool success;

	success = routeQuery.exec(sqls);

	if(success)
	{
    success = routeItemsQuery.prepare("SELECT WayPointId FROM RouteItems WHERE RouteId=:route");
		while(routeQuery.next())
		{
			// route name
			route.setId(routeQuery.value(0).toInt());
			route.setName(routeQuery.value(1).toString());
			route.setType((Route::Type)routeQuery.value(2).toInt());
			route.wayPointList().clear();

			// route items
      routeItemsQuery.bindValue(":route", route.id());
      success &= routeItemsQuery.exec();

			if(success)
			{
				while(routeItemsQuery.next())
				{
					wpId = routeItemsQuery.value(0).toInt();
					ISql::pInstance()->pWayPointTable()->wayPoint(wpId, wp);
					route.wayPointList().push_back(wp);
				}
			}

			routeList.push_back(route);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Routes::setId(Route &route)
{
	QSqlQuery query(db());
	bool success;
	int id = -1;

  success = query.prepare("SELECT Id FROM Routes WHERE Name=:name");
  query.bindValue(":name", route.name());
  success &= (query.exec() && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	route.setId(id);

	return success;
}

bool Routes::checkModified()
{
  return DataBaseSub::checkModified("Routes");
}
