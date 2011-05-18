/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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
#include <qdatetime.h>
#include <q3sqlcursor.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <stdlib.h>
#include "QueryStore.h"
#include "Error.h"
#include "Upgrade.h"

const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_0 = QDateTime(QDate(2004,  3, 20), QTime(20, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_1 = QDateTime(QDate(2005,  6, 12), QTime(20, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_2 = QDateTime(QDate(2006,  3, 31), QTime(20,50));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_5_0 = QDateTime(QDate(2006, 11, 14), QTime( 0, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_8_1 = QDateTime(QDate(2011,  1, 18), QTime( 0, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_8_2 = QDateTime(QDate(2011,  2, 20), QTime( 0, 0));

Upgrade::Upgrade(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool Upgrade::setup(const QString &dbname, const QString &user, const QString &pwd)
{
	QSqlQuery query(db());
	QString sqls;
	bool res = true;
	QueryStore* pQueryStore = QueryStore::pInstance();

	// prepare db
  if (db().driverName()=="QMYSQL")
  {
    // setup mysql db
    sqls = "CREATE DATABASE `%1` DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
		res &= query.exec(sqls.arg(dbname));

    sqls = "CREATE USER '%1'@'localhost' IDENTIFIED BY '%2';";
		res &= query.exec(sqls.arg(user).arg(pwd));

    sqls = "GRANT SELECT, INSERT, UPDATE, DELETE, DROP, CREATE, ALTER ON %1.* TO '%2'@'localhost' IDENTIFIED BY '%3';";
		res &= query.exec(sqls.arg(dbname).arg(user).arg(pwd));

    sqls = "USE `%1`;";
		res &= query.exec(sqls.arg(dbname));
	}
	else if (db().driverName()=="QSQLITE")
	{
		// setup sqlite db

		// this replaces the 'CHARSET=utf8' clause in mysql
		sqls = "PRAGMA encoding = 'UTF-8'";
		res &= query.exec(sqls);
	}
	else
	{
		return false;
	}

	// create tables
	res &= query.exec(pQueryStore->getQuery("setup-create-gliders", db()));
	res &= query.exec(pQueryStore->getQuery("setup-create-pilots", db()));
	res &= query.exec(pQueryStore->getQuery("setup-create-waypoints", db()));
	res &= query.exec(pQueryStore->getQuery("setup-create-flights", db()));
	res &= query.exec(pQueryStore->getQuery("setup-create-routes", db()));
	res &= query.exec(pQueryStore->getQuery("setup-create-routeitems", db()));
	res &= query.exec(pQueryStore->getQuery("setup-create-servicings", db()));
	res &= query.exec(pQueryStore->getQuery("setup-create-lastmodified", db()));

	// finish db
	if (db().driverName()=="QMYSQL")
	{
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(1, 'Pilots', '1970-01-01 01:01:00'),"
				"(2, 'Flights', '1970-01-01 01:01:00'),"
				"(3, 'Gliders', '1970-01-01 01:01:00'),"
				"(4, 'Servicings', '1970-01-01 01:01:00'),"
				"(5, 'WayPoints', '1970-01-01 01:01:00'),"
				"(6, 'DataBaseVersion', '2011-01-18 00:00:00'),"
				"(7, 'Routes', '1970-01-01 01:01:00');";
		res &= query.exec(sqls);
	}
	else if (db().driverName()=="QSQLITE")
	{
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(1, 'Pilots', '1970-01-01 01:01:00')";
		res &= query.exec(sqls);
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(2, 'Flights', '1970-01-01 01:01:00')";
		res &= query.exec(sqls);
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(3, 'Gliders', '1970-01-01 01:01:00')";
		res &= query.exec(sqls);
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(4, 'Servicings', '1970-01-01 01:01:00')";
		res &= query.exec(sqls);
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(5, 'WayPoints', '1970-01-01 01:01:00')";
		res &= query.exec(sqls);
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(6, 'DataBaseVersion', '2011-01-18 00:00:00')";
		res &= query.exec(sqls);
		sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
				"(7, 'Routes', '1970-01-01 01:01:00')";
		res &= query.exec(sqls);
	}
	return res;
}

bool Upgrade::upgrade()
{
	bool res = true;
	QString sqls;
	QSqlQuery query(db());
	DataBaseVersion dbVers = DataBaseVersion_0_5_0;

	if(dataBaseVersion() < DataBaseVersion_0_5_0)
	{
		Q_ASSERT(false);       
	}

	if(dataBaseVersion() < DataBaseVersion_0_8_1)
	{
		sqls = "ALTER TABLE Routes ADD Type INT NULL DEFAULT 0;";
		res &= query.exec(sqls);
		setDataBaseVersion(DataBaseVersion_0_8_1);
	}
	
	if(dataBaseVersion() < DataBaseVersion_0_8_2)
	{
		DataBaseSub::setLastModified("Pilots");
		DataBaseSub::setLastModified("Flights");
		DataBaseSub::setLastModified("Gliders");
		DataBaseSub::setLastModified("Servicings");
		DataBaseSub::setLastModified("WayPoints");
		DataBaseSub::setLastModified("Routes");
		setDataBaseVersion(DataBaseVersion_0_8_2);
	}
	return res;
}

Upgrade::DataBaseVersion Upgrade::dataBaseVersion()
{
	QString sqls;
	QSqlQuery query(db());
	DataBaseVersion dbVers = DataBaseVersion_0_5_0;
	
	sqls.sprintf("SELECT Time FROM LastModified WHERE Name = 'DataBaseVersion'");
	
	if(query.exec(sqls) && query.first())
	{
		dbVers = query.value(0).toDateTime();
	}
	
	return dbVers;
}

void Upgrade::setDataBaseVersion(const DataBaseVersion &tabVers)
{
	QString sqls;
	QString version;
	QSqlQuery query(db());
	
	version = tabVers.toString("yyyy-MM-dd hh:mm:ss");
	sqls = QString("UPDATE LastModified SET Time = '%1' WHERE Name = 'DataBaseVersion'").arg(version);
	query.exec(sqls);
}
