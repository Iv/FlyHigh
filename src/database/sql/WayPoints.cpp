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
#include <qsqlquery.h>
#include "Error.h"
#include "WayPoints.h"

WayPoints::WayPoints(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

bool WayPoints::add(WayPoint &wp)
{
	QSqlCursor cur("WayPoints");
	QSqlRecord *pRec;
	
	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Name", wp.name());
	pRec->setValue("Description", wp.description());
	pRec->setValue("Longitude", wp.longitude());
	pRec->setValue("Latitude", wp.latitude());
	pRec->setValue("Altitude", wp.altitude());
	cur.insert();
	
	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	DataBaseSub::setLastModified("WayPoints");
	
	return true;
}

bool WayPoints::delWayPoint(const QString &name)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	 
	sqls.sprintf("DELETE FROM `WayPoints` WHERE `Name` = '%s'", name.ascii());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("WayPoints");
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool WayPoints::wayPoint(const QString &name, WayPoint &wp)
{
	QSqlCursor cur("WayPoints");
	QString filter;
	double lon;
	double lat;
	int alt;
	bool exist;

	filter.sprintf("Name='%s'", name.ascii());
	cur.setFilter(filter);
	cur.select();	
	exist = cur.first();
	
	if(exist)
	{
		wp.setName(cur.value("Name").toString());
		wp.setDescription(cur.value("Description").toString());
		
		lon = cur.value("Longitude").toDouble();
		lat = cur.value("Latitude").toDouble();
		alt = cur.value("Altitude").toInt();
		wp.setCoordinates(lat, lon, alt);
	}

	Error::verify(exist, Error::SQL_CMD);
	
	return exist;
}

bool WayPoints::findWayPoint(WayPoint &wp, uint radius)
{
	QSqlCursor cur("WayPoints");
	WayPoint recWp;
	double recLon;
	double recLat;
	uint dist;
	bool found = false;
 
	// select all
	Error::verify(cur.select(), Error::SQL_CMD);

	while(cur.next())
	{
		recLat = cur.value("Latitude").toDouble();
		recLon = cur.value("Longitude").toDouble();
		recWp.setCoordinates(recLat, recLon, 0);
		dist = wp.distance(recWp);
		found = (dist <= radius);
		
		if(found)
		{
			wp.setCoordinates(recLat, recLon, cur.value("Altitude").toInt());
			wp.setName(cur.value("Name").toString());
			wp.setDescription(cur.value("Description").toString());
			break;
		}
	}

	return found;
}

bool WayPoints::wayPointList(WayPoint::WayPointListType &wpList)
{
	WayPoint wp;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM `WayPoints` ORDER BY `Name` ASC";
	double lon;
	double lat;
	int alt;
	bool success;
	
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			wp.setName(query.value(Name).toString());
			lon = query.value(Longitude).toDouble();
			lat = query.value(Latitude).toDouble();
			alt = query.value(Altitude).toInt();
			wp.setCoordinates(lat, lon, alt);

			wpList.push_back(wp);
		}
	}
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}
