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
 
#include <q3sqlcursor.h>
#include <qsqldatabase.h>
//Added by qt3to4:
#include <QSqlQuery>
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
	QString sqls;
	QString value;
	uint nWps;
	uint wpNr;
	bool success;
	
	// insert route name
        sqls = QString("INSERT INTO Routes(Id, Name) VALUES(NULL, '%1');").arg(route.name());
	success = query.exec(sqls);
	Error::verify(success, Error::SQL_ADD_ROUTE_NAME);

	if(success)
	{
		setId(route);
		sqls = "INSERT INTO RouteItems(Id, RouteId, WayPointId) VALUES";
	
		// insert route items
		nWps = route.wayPointList().size();
	
		for(wpNr=0; wpNr<nWps; wpNr++)
		{
			if(wpNr != 0)
			{
				sqls += ", ";
			}
	
			value.sprintf("(NULL, %i, %i)", route.id(), route.wayPointList().at(wpNr).id());
			sqls += value;
		}
	
		sqls += ";";
		success = query.exec(sqls);
		Error::verify(success, Error::SQL_CMD);
		DataBaseSub::setLastModified("Routes");
	}
		
	return success;
}

bool Routes::delRoute(Route &route)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;

	// first delete route items
	sqls.sprintf("DELETE FROM RouteItems WHERE RouteId = %i;", route.id());
	success = query.exec(sqls);
	Error::verify(success, Error::SQL_CMD);
	
	// route
	sqls.sprintf("DELETE FROM Routes WHERE Id = %i;", route.id());
	success = query.exec(sqls);
	Error::verify(success, Error::SQL_CMD);

	DataBaseSub::setLastModified("Routes");
	
	return success;
}

bool Routes::routeList(Route::RouteListType &routeList)
{
	Route route;
	WayPoint wp;
	QSqlQuery routeQuery(db());
	QSqlQuery routeItemsQuery(db());
	QString sqls = "SELECT * FROM Routes ORDER BY Name ASC;";
	QString wpName;
	int wpId;
	bool success;
	
	success = routeQuery.exec(sqls);
	
	if(success)
	{
		while(routeQuery.next())
		{
			// route name
			route.setId(routeQuery.value(Id).toInt());
			route.setName(routeQuery.value(Name).toString());
			route.wayPointList().clear();

			// route items
			sqls.sprintf("SELECT * FROM RouteItems WHERE RouteId = %i;", route.id());
			success = routeItemsQuery.exec(sqls);
			
			if(success)
			{
				while(routeItemsQuery.next())
				{
					wpId = routeItemsQuery.value(WayPointId).toInt();
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
	QString sqls;
	QString dbModel;
	bool success;
	int id = -1;

        sqls = QString("SELECT * FROM Routes WHERE Name = '%1';").arg(route.name());
	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(Id).toInt();
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	route.setId(id);
	
	return success;
}
