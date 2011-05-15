/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#include <QSqlDatabase>
#include <QDebug>
#include "QueryStore.h"

// initialize statics
QueryStore* QueryStore::m_pInst = NULL;

QueryStore* QueryStore::pInstance()
{
	if(m_pInst == NULL)
	{
		m_pInst = new QueryStore();
	}

	return m_pInst;
}

QueryStore::QueryStore()
{
	init();
}

void QueryStore::init()
{
	m_QueryMap.insert("migrate-read-routeitems",
										"SELECT Id, RouteId, WayPointId FROM RouteItems");
	m_QueryMap.insert("migrate-write-routeitems",
										"INSERT INTO RouteItems (Id, RouteId, WayPointId) VALUES (?,?,?)");
	m_QueryMap.insert("migrate-read-waypoints",
										"SELECT Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description FROM WayPoints");
	m_QueryMap.insert("migrate-write-waypoints",
										"INSERT INTO WayPoints (Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description) VALUES (?,?,?,?,?,?,?,?)");
	m_QueryMap.insert("migrate-read-gliders",
										"SELECT Id, Manufacturer, Model, Serial FROM Gliders");
	m_QueryMap.insert("migrate-write-gliders",
										"INSERT INTO Gliders (Id, Manufacturer, Model, Serial) VALUES (?,?,?,?)");
	m_QueryMap.insert("migrate-read-servicings",
										"SELECT Id, GliderId, Date, Responsibility, Comment FROM Servicings");
	m_QueryMap.insert("migrate-write-servicings",
										"INSERT INTO Servicings (Id, GliderId, Date, Responsibility, Comment) VALUES (?,?,?,?,?)");
	m_QueryMap.insert("migrate-read-lastmodified",
										"SELECT Id, Name, Time FROM LastModified");
	m_QueryMap.insert("migrate-write-lastmodified",
										"INSERT INTO LastModified (Id, Name, Time) VALUES (?,?,?)");
	m_QueryMap.insert("migrate-read-pilots",
										"SELECT Id, FirstName, LastName, BirthDate, CallSign, GliderId FROM Pilots");
	m_QueryMap.insert("migrate-write-pilots",
										"INSERT INTO Pilots (Id, FirstName, LastName, BirthDate, CallSign, GliderId) VALUES (?,?,?,?,?,?)");
	m_QueryMap.insert("migrate-read-routes",
										"SELECT Id, Name, Type FROM Routes");
	m_QueryMap.insert("migrate-write-routes",
										"INSERT INTO Routes (Id, Name, Type) VALUES (?,?,?)");
	m_QueryMap.insert("migrate-read-flights",
										"SELECT Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile FROM Flights");
	m_QueryMap.insert("migrate-write-flights",
										"INSERT INTO Flights (Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");

	// does not work with sqlite: http://bugreports.qt.nokia.com/browse/QTBUG-8689
	/*m_QueryMap.insert("migrate-drop-tables",
										"DROP TABLE IF EXISTS Flights; "
										"DROP TABLE IF EXISTS Routes; "
										"DROP TABLE IF EXISTS LastModified; "
										"DROP TABLE IF EXISTS Pilots; "
										"DROP TABLE IF EXISTS Servicings; "
										"DROP TABLE IF EXISTS Gliders; "
										"DROP TABLE IF EXISTS WayPoints; "
										"DROP TABLE IF EXISTS RouteItems");*/

	m_QueryMap.insert("migrate-drop-flights",
										"DROP TABLE IF EXISTS Flights");
	m_QueryMap.insert("migrate-drop-routes",
										"DROP TABLE IF EXISTS Routes");
	m_QueryMap.insert("migrate-drop-lastmodified",
										"DROP TABLE IF EXISTS LastModified");
	m_QueryMap.insert("migrate-drop-pilots",
										"DROP TABLE IF EXISTS Pilots");
	m_QueryMap.insert("migrate-drop-servicings",
										"DROP TABLE IF EXISTS Servicings");
	m_QueryMap.insert("migrate-drop-gliders",
										"DROP TABLE IF EXISTS Gliders");
	m_QueryMap.insert("migrate-drop-waypoints",
										"DROP TABLE IF EXISTS WayPoints");
	m_QueryMap.insert("migrate-drop-routeitems",
										"DROP TABLE IF EXISTS RouteItems");

}

QString QueryStore::getQuery(const QString& name, const QSqlDatabase& db) const
{
	// check if there is a common query
	TQueryMap::const_iterator iter=m_QueryMap.constFind(name);
	if (iter!=m_QueryMap.constEnd())
	{
		return iter.value();
	}
	else
	{
		// check for mysql query
		if (db.driverName()=="QMYSQL")
		{
			iter = m_QueryMapMysql.constFind(name);
			if (iter!=m_QueryMapMysql.constEnd())
			{
				return iter.value();
			}
		}
		else if (db.driverName()=="QSQLITE")
		{
			iter = m_QueryMapSqlite.constFind(name);
			if (iter!=m_QueryMapSqlite.constEnd())
			{
				return iter.value();
			}
		}
	}

	// somethings' wrong...
	qDebug() << "query '" << name << "' not found for driver " << db.driverName();

	return "";
}
