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
 
#include <qsqlcursor.h>
#include <qsqldatabase.h>
#include "Error.h"
#include "Routes.h"

Routes::Routes(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

bool Routes::add(Route &route)
{
	QSqlCursor cur("Routes");
	QSqlRecord *pRec;
	QString wpField;
	QString wpName;
	uint nWps;
	uint wpNr;
	
	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Name", route.name());

	nWps = route.wayPointList().size();

	for(wpNr=0; wpNr<nWps; wpNr++)
	{
		wpField.sprintf("WayPoint%i", wpNr);
		wpName = *(route.wayPointList().at(wpNr));
		pRec->setValue(wpField, wpName);
	}

	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	DataBaseSub::setLastModified("Routes");
	
	return true;
}

bool Routes::delRoute(const QString &name)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	 
	sqls.sprintf("DELETE FROM `Routes` WHERE `Name` = '%s'", name.ascii());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("Routes");
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool Routes::route(const QString &name, Route &route)
{
	QSqlQuery query(db());
	QString sqls;
	QString wpName;
	uint wpNr;
	bool success;
	
	sqls.sprintf("SELECT * FROM `Routes` WHERE `Name` = '%s'", name.ascii());
	success = query.exec(sqls);
	
	if(success)
	{
		success = query.first();
		
		if(success)
		{
			route.wayPointList().clear();
			route.setName(query.value(Name).toString());
			
			for(wpNr=0; wpNr<MaxWayPoints; wpNr++)
			{
				wpName = query.value(wpNr+1).toString();
				
				if(wpName == "")
				{
					break;
				}
				
				route.wayPointList().push_back(wpName);
			}
		}
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}
	
	return success;
}

bool Routes::routeList(Route::RouteListType &routeList)
{
	Route route;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM Routes ORDER BY Name ASC";
	QString wpName;
	uint wpNr;
	bool success;
	
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			route.wayPointList().clear();
			route.setName(query.value(Name).toString());
			
			for(wpNr=0; wpNr<MaxWayPoints; wpNr++)
			{
				wpName = query.value(wpNr+1).toString();
				
				if(wpName == "")
				{
					break;
				}
				
				route.wayPointList().push_back(wpName);
			}

			routeList.push_back(route);
		}
	}
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}
