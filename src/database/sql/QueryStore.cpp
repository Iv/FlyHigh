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

void QueryStore::deleteInstance()
{
  if(m_pInst != NULL)
  {
    delete m_pInst;
    m_pInst = NULL;
  }
}

QueryStore::QueryStore()
{
	init();
}

QueryStore::~QueryStore()
{
  m_QueryMap.clear();
}

void QueryStore::init()
{
	addQuery("migrate-read-routeitems",
					 "common",
					 "SELECT Id, RouteId, WayPointId FROM RouteItems");
	addQuery("migrate-write-routeitems",
					 "common",
					 "INSERT INTO RouteItems (Id, RouteId, WayPointId) VALUES (:Id, :RouteId, :WayPointId)");
	addQuery("migrate-read-waypoints",
					 "common",
					 "SELECT Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description, Type FROM WayPoints");
	addQuery("migrate-write-waypoints",
					 "common",
					 "INSERT INTO WayPoints (Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description, Type) VALUES (:Id, :Name, :Spot, :Country, :Longitude, :Latitude, :Altitude, :Description, :Type)");
	addQuery("migrate-read-gliders",
					 "common",
					 "SELECT Id, Manufacturer, Model, Serial FROM Gliders");
	addQuery("migrate-write-gliders",
					 "common",
					 "INSERT INTO Gliders (Id, Manufacturer, Model, Serial) VALUES (:Id, :Manufacturer, :Model, :Serial)");
	addQuery("migrate-read-servicings",
					 "common",
					 "SELECT Id, GliderId, Date, Responsibility, Comment FROM Servicings");
	addQuery("migrate-write-servicings",
					 "common",
					 "INSERT INTO Servicings (Id, GliderId, Date, Responsibility, Comment) VALUES (:Id, :GliderId, :Date, :Responsibility, :Comment)");
	addQuery("migrate-read-lastmodified",
					 "common",
					 "SELECT Id, Name, Time FROM LastModified");
	addQuery("migrate-write-lastmodified",
					 "common",
					 "INSERT INTO LastModified (Id, Name, Time) VALUES (:Id, :Name, :Time)");
	addQuery("migrate-read-pilots",
					 "common",
					 "SELECT Id, FirstName, LastName, BirthDate, CallSign, GliderId FROM Pilots");
	addQuery("migrate-write-pilots",
					 "common",
					 "INSERT INTO Pilots (Id, FirstName, LastName, BirthDate, CallSign, GliderId) VALUES (:Id, :FirstName, :LastName, :BirthDate, :CallSign, :GliderId)");
  addQuery("migrate-read-accounts",
           "common",
           "SELECT Id, Username, Password, Type, PilotId, Comment FROM Accounts");
  addQuery("migrate-write-accounts",
           "common",
           "INSERT INTO Accounts (Id, Username, Password, Type, PilotId, Comment) VALUES (:Id, :Username, :Password, :Type, :PilotId, :Comment)");
  addQuery("migrate-read-routes",
					 "common",
					 "SELECT Id, Name, Type FROM Routes");
	addQuery("migrate-write-routes",
					 "common",
					 "INSERT INTO Routes (Id, Name, Type) VALUES (:Id, :Name, :Type)");
	addQuery("migrate-read-flights",
					 "common",
           "SELECT Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile, PhotoPath FROM Flights");
	addQuery("migrate-write-flights",
					 "common",
           "INSERT INTO Flights (Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile, PhotoPath) VALUES (:Id, :Number, :PilotId, :Date, :Time, :GliderId, :StartPtId, :LandPtId, :Duration, :Distance, :Comment, :IGCFile, :PhotoPath)");
  addQuery("migrate-read-airspaceitems",
           "common",
           "SELECT Id, AirSpaceId, Longitude, Latitude FROM AirSpaceItems");
  addQuery("migrate-write-airspaceitems",
           "common",
           "INSERT INTO AirSpaceItems (Id, AirSpaceId, Longitude, Latitude) VALUES (:Id, :AirSpaceId, :Longitude, :Latitude)");
  addQuery("migrate-read-airspaces",
           "common",
           "SELECT Id, Name, Class, Lower, Upper, Comment FROM AirSpaces");
  addQuery("migrate-write-airspaces",
           "common",
           "INSERT INTO AirSpaces (Id, Name, Class, Lower, Upper, Comment) VALUES (:Id, :Name, :Class, :Lower, :Upper, :Comment)");
  addQuery("migrate-read-accounts",
           "common",
           "SELECT Id, Username, Password, Type, PilotId, Comment FROM Accounts");
  addQuery("migrate-write-accounts",
           "common",
           "INSERT INTO Accounts (Id, Username, Password, Type, PilotId, Comment) VALUES (:Id, :Username, :Password, :Type, :PilotId, :Comment)");

	// multiple statements separated by ';' does not work with sqlite: http://bugreports.qt.nokia.com/browse/QTBUG-8689
	// therefore we need to use a list of statements
	QStringList droptables;
	droptables << "DROP TABLE IF EXISTS Flights" <<
								"DROP TABLE IF EXISTS RouteItems" <<
								"DROP TABLE IF EXISTS Routes" <<
								"DROP TABLE IF EXISTS LastModified" <<
                "DROP TABLE IF EXISTS Accounts" <<
                "DROP TABLE IF EXISTS Pilots" <<
								"DROP TABLE IF EXISTS Servicings" <<
								"DROP TABLE IF EXISTS Gliders" <<
                "DROP TABLE IF EXISTS WayPoints" <<
                "DROP TABLE IF EXISTS AirSpaceItems" <<
                "DROP TABLE IF EXISTS AirSpaces";
	addQuery("migrate-drop-tables",
					 "common",
					 droptables);

	addQuery("setup-create-gliders",
					 "QMYSQL",
					 "CREATE TABLE `Gliders`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`Manufacturer` VARCHAR(16) CHARACTER SET utf8 NULL DEFAULT '',"
					 "`Model` VARCHAR(16) CHARACTER SET utf8 NOT NULL,"
					 "`Serial` VARCHAR(16) CHARACTER SET utf8 NULL DEFAULT '',"
					 "PRIMARY KEY(`Id`),"
					 "UNIQUE KEY(`Manufacturer`, `Model`, `Serial`)"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-gliders",
					 "QSQLITE",
					 "CREATE TABLE `Gliders`"
					 "("
					 "`Id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
					 "`Manufacturer` VARCHAR(16) DEFAULT '',"
					 "`Model` VARCHAR(16) NOT NULL,"
					 "`Serial` VARCHAR(16) DEFAULT '')");

	addQuery("setup-create-pilots",
					 "QMYSQL",
					 "CREATE TABLE `Pilots`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`FirstName` VARCHAR(16) CHARACTER SET utf8 NOT NULL,"
					 "`LastName` VARCHAR(16) CHARACTER SET utf8 NOT NULL,"
					 "`BirthDate` DATE NULL DEFAULT '0000-00-00',"
					 "`CallSign` VARCHAR(16) CHARACTER SET utf8 NOT NULL,"
					 "`GliderId` INT NULL DEFAULT '0',"
					 "PRIMARY KEY(`Id`),"
					 "UNIQUE KEY byPilot(`FirstName`, `LastName`, `BirthDate`),"
					 "FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-pilots",
					 "QSQLITE",
					 "CREATE TABLE `Pilots`"
					 "("
					 "`Id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
					 "`FirstName` VARCHAR(16) NOT NULL,"
					 "`LastName` VARCHAR(16) NOT NULL,"
					 "`BirthDate` DATE DEFAULT '0000-00-00',"
					 "`CallSign` VARCHAR(16) NOT NULL,"
					 "`GliderId` INTEGER DEFAULT '0',"
					 "CONSTRAINT Pilots_fk1 FOREIGN KEY (`GliderId`)"
					 " REFERENCES `Gliders` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE)");

	addQuery("setup-create-waypoints",
					 "QMYSQL",
					 "CREATE TABLE `WayPoints`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`Name` VARCHAR(16) NOT NULL,"
					 "`Spot` VARCHAR(16) NULL DEFAULT '',"
					 "`Country` VARCHAR(2) NULL DEFAULT '',"
					 "`Longitude` FLOAT NOT NULL,"
					 "`Latitude` FLOAT NOT NULL,"
					 "`Altitude` INT NOT NULL,"
					 "`Description` VARCHAR(200) NULL DEFAULT '',"
					 "`Type` INT NOT NULL,"
					 "PRIMARY KEY(`Id`),"
					 "UNIQUE KEY byWayPoints(`Name`, `Spot`, `Country`)"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-waypoints",
					 "QSQLITE",
					 "CREATE TABLE `WayPoints`"
					 "("
					 "`Id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
					 "`Name` VARCHAR(16) NOT NULL,"
					 "`Spot` VARCHAR(16) DEFAULT '',"
					 "`Country` VARCHAR(2) DEFAULT '',"
					 "`Longitude` FLOAT NOT NULL,"
					 "`Latitude` FLOAT NOT NULL,"
					 "`Altitude` INTEGER NOT NULL,"
					 "`Description` VARCHAR(200) DEFAULT '',"
           "`Type` INTEGER NOT NULL)");

	addQuery("setup-create-flights",
					 "QMYSQL",
					 "CREATE TABLE `Flights`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`Number` INT NOT NULL,"
					 "`PilotId` INT NOT NULL,"
					 "`Date` DATE NOT NULL,"
					 "`Time` TIME NOT NULL,"
					 "`GliderId` INT NOT NULL,"
					 "`StartPtId` INT NOT NULL,"
					 "`LandPtId` INT NOT NULL,"
					 "`Duration` INT NOT NULL,"
					 "`Distance` INT NOT NULL,"
					 "`Comment` VARCHAR(500) NULL DEFAULT '',"
					 "`IGCFile` MEDIUMBLOB,"
           "`PhotoPath` VARCHAR(500) DEFAULT '',"
					 "PRIMARY KEY(`Id`),"
					 "UNIQUE KEY byFlight(`PilotId`, `Number`),"
					 "FOREIGN KEY(`PilotId`) REFERENCES Pilots(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "FOREIGN KEY(`StartPtId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "FOREIGN KEY(`LandPtId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-flights",
					 "QSQLITE",
					 "CREATE TABLE `Flights`"
					 "("
					 "`Id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
					 "`Number` INTEGER NOT NULL,"
					 "`PilotId` INTEGER NOT NULL,"
					 "`Date` DATE NOT NULL,"
					 "`Time` TIME NOT NULL,"
					 "`GliderId` INTEGER NOT NULL,"
					 "`StartPtId` INTEGER NOT NULL,"
					 "`LandPtId` INTEGER NOT NULL,"
					 "`Duration` INTEGER NOT NULL,"
					 "`Distance` INTEGER NOT NULL,"
					 "`Comment` VARCHAR(500) DEFAULT '',"
					 "`IGCFile` MEDIUMBLOB,"
           "`PhotoPath` VARCHAR(500) DEFAULT '',"
					 "CONSTRAINT Flights_fk1 FOREIGN KEY (`PilotId`)"
					 " REFERENCES `Pilots` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "CONSTRAINT Flights_fk2 FOREIGN KEY (`GliderId`)"
					 " REFERENCES `Gliders` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "CONSTRAINT Flights_fk3 FOREIGN KEY (`StartPtId`)"
					 " REFERENCES `WayPoints` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "CONSTRAINT Flights_fk4 FOREIGN KEY (`LandPtId`)"
					 " REFERENCES `WayPoints` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE)");

	addQuery("setup-create-routes",
					 "QMYSQL",
					 "CREATE TABLE `Routes`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`Name` VARCHAR(16) NOT NULL,"
					 "`Type` INT NULL DEFAULT 0,"
					 "PRIMARY KEY(`Id`),"
					 "UNIQUE KEY byRoutes(`Name`)"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-routes",
					 "QSQLITE",
					 "CREATE TABLE `Routes`"
					 "("
					 "`Id` INTEGER PRIMARY KEY AUTOINCREMENT,"
					 "`Name` VARCHAR(16) NOT NULL,"
					 "`Type` INTEGER DEFAULT 0)");

	addQuery("setup-create-routeitems",
					 "QMYSQL",
					 "CREATE TABLE `RouteItems`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`RouteId` INT NOT NULL,"
					 "`WayPointId` INT NOT NULL,"
					 "PRIMARY KEY(`Id`),"
					 "FOREIGN KEY(`RouteId`) REFERENCES Routes(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "FOREIGN KEY(`WayPointId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-routeitems",
					 "QSQLITE",
					 "CREATE TABLE `RouteItems`"
					 "("
					 "`Id` INTEGER PRIMARY KEY AUTOINCREMENT,"
					 "`RouteId` INTEGER NOT NULL,"
					 "`WayPointId` INTEGER NOT NULL,"
					 "CONSTRAINT RouteItems_fk1 FOREIGN KEY (`RouteId`)"
					 " REFERENCES `Routes` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
					 "CONSTRAINT RouteItems_fk2 FOREIGN KEY (`WayPointId`)"
					 " REFERENCES `WayPoints` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE)");

	addQuery("setup-create-servicings",
					 "QMYSQL",
					 "CREATE TABLE `Servicings`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`GliderId` INT NOT NULL,"
					 "`Date` DATE NOT NULL,"
					 "`Responsibility` VARCHAR(16) NOT NULL,"
					 "`Comment` VARCHAR(200) NULL DEFAULT '',"
					 "PRIMARY KEY(`Id`),"
					 "FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-servicings",
					 "QSQLITE",
					 "CREATE TABLE `Servicings`"
					 "("
					 "`Id` INTEGER PRIMARY KEY AUTOINCREMENT,"
					 "`GliderId` INTEGER NOT NULL,"
					 "`Date` DATE NOT NULL,"
					 "`Responsibility` VARCHAR(16) NOT NULL,"
					 "`Comment` VARCHAR(200) DEFAULT '',"
					 "CONSTRAINT Servicings_fk1 FOREIGN KEY (`GliderId`)"
					 " REFERENCES `Gliders` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE)");

	addQuery("setup-create-lastmodified",
					 "QMYSQL",
					 "CREATE TABLE `LastModified`"
					 "("
					 "`Id` INT NULL AUTO_INCREMENT,"
					 "`Name` VARCHAR(16) NOT NULL,"
					 "`Time` datetime NOT NULL,"
					 "PRIMARY KEY(`Id`),"
					 "UNIQUE KEY (`Name`)"
					 ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
	addQuery("setup-create-lastmodified",
					 "QSQLITE",
					 "CREATE TABLE `LastModified`"
					 "("
					 "`Id` INTEGER PRIMARY KEY AUTOINCREMENT,"
					 "`Name` VARCHAR(16) NOT NULL,"
					 "`Time` datetime NOT NULL)");

  addQuery("setup-create-airspaces",
           "QMYSQL",
           "CREATE TABLE `AirSpaces`"
           "("
           "`Id` INT NULL AUTO_INCREMENT,"
           "`Name` VARCHAR(16) NOT NULL,"
           "`Class` VARCHAR(16) NOT NULL,"
           "`Lower` INT,"
           "`Upper` INT,"
           "`Comment` VARCHAR(500) DEFAULT '',"
           "PRIMARY KEY(`Id`),"
           "UNIQUE KEY (`Name`)"
           ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
  addQuery("setup-create-airspaces",
           "QSQLITE",
           "CREATE TABLE `AirSpaces`"
           "("
           "`Id` INTEGER PRIMARY KEY AUTOINCREMENT,"
           "`Name` VARCHAR(16) NOT NULL,"
           "`Class` VARCHAR(16) NOT NULL,"
           "`Lower` INTEGER,"
           "`Upper` INTEGER,"
           "`Comment` VARCHAR(500) DEFAULT '')");

  addQuery("setup-create-airspaceitems",
           "QMYSQL",
           "CREATE TABLE `AirSpaceItems`"
           "("
           "`Id` INT NULL AUTO_INCREMENT,"
           "`AirSpaceId` INT NOT NULL,"
           "`Longitude` FLOAT,"
           "`Latitude` FLOAT,"
           "PRIMARY KEY(`Id`),"
           "FOREIGN KEY(`AirSpaceId`) REFERENCES AirSpaces(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
           ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
  addQuery("setup-create-airspaceitems",
           "QSQLITE",
           "CREATE TABLE `AirSpaceItems`"
           "("
           "`Id` INTEGER PRIMARY KEY AUTOINCREMENT,"
           "`AirSpaceId` INTEGER NOT NULL,"
           "`Longitude` FLOAT,"
           "`Latitude` FLOAT,"
           "CONSTRAINT AirSpaceItems_fk1 FOREIGN KEY(`AirSpaceId`)"
           " REFERENCES `AirSpaces` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE)");

  addQuery("setup-create-accounts",
           "QMYSQL",
           "CREATE TABLE `Accounts`"
           "("
           "`Id` INT NULL AUTO_INCREMENT,"
           "`Username` VARCHAR(100) NOT NULL,"
           "`Password` VARCHAR(100) NOT NULL,"
           "`Type` INT NULL DEFAULT 0,"
           "`PilotId` INT NOT NULL,"
           "`Comment` VARCHAR(500) DEFAULT '',"
           "PRIMARY KEY(`Id`),"
           "FOREIGN KEY(`PilotId`) REFERENCES Pilots(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
           ")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;");
  addQuery("setup-create-accounts",
           "QSQLITE",
           "CREATE TABLE `Accounts`"
           "("
           "`Id` INTEGER PRIMARY KEY AUTOINCREMENT,"
           "`Username` VARCHAR(100) NOT NULL,"
           "`Password` VARCHAR(100) NOT NULL,"
           "`Type` INTEGER DEFAULT 0,"
           "`PilotId` INTEGER NOT NULL,"
           "`Comment` VARCHAR(500) DEFAULT '',"
           "CONSTRAINT Accounts_fk1 FOREIGN KEY (`PilotId`)"
           " REFERENCES `Pilots` (`Id`) ON DELETE RESTRICT ON UPDATE CASCADE)");

	addQuery("setup-create-db",
					 "QMYSQL",
					 "CREATE DATABASE IF NOT EXISTS `%dbname` DEFAULT CHARSET=utf8 COLLATE=utf8_bin");
	addQuery("setup-create-db",
					 "QSQLITE",
					 "PRAGMA encoding = 'UTF-8'");

	addQuery("setup-create-user",
					 "QMYSQL",
					 "CREATE USER '%username'@'%' IDENTIFIED BY '%password'");

	addQuery("setup-privileges",
					 "QMYSQL",
           "GRANT SELECT, INSERT, UPDATE, DELETE, DROP, CREATE, ALTER ON `%dbname`.* TO '%username'@'%'");

	addQuery("setup-get-user",
					 "QMYSQL",
					 "SELECT user FROM mysql.user where user='%username'");

	addQuery("setup-finalize",
					 "QMYSQL",
					 "USE `%dbname`");

	// missing for sqlite: setup-finalize, setup-get-user setup-privileges, setup-create-user

	addQuery("setup-set-lastmodified",
					 "QMYSQL",
					 "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
									 "(1, 'Pilots',          '1970-01-01 01:01:00'),"
									 "(2, 'Flights',         '1970-01-01 01:01:00'),"
									 "(3, 'Gliders',         '1970-01-01 01:01:00'),"
									 "(4, 'Servicings',      '1970-01-01 01:01:00'),"
									 "(5, 'WayPoints',       '1970-01-01 01:01:00'),"
									 "(6, 'DataBaseVersion', '%versiontimestamp'),"
                   "(7, 'Routes',          '1970-01-01 01:01:00'),"
                   "(8, 'AirSpaces',       '1970-01-01 01:01:00'),"
                   "(9, 'Accounts',        '1970-01-01 01:01:00')");
	QStringList setlastmod;
	setlastmod << "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (1, 'Pilots',          '1970-01-01 01:01:00')" <<
								"INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (2, 'Flights',         '1970-01-01 01:01:00')" <<
								"INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (3, 'Gliders',         '1970-01-01 01:01:00')" <<
								"INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (4, 'Servicings',      '1970-01-01 01:01:00')" <<
								"INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (5, 'WayPoints',       '1970-01-01 01:01:00')" <<
								"INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (6, 'DataBaseVersion', '%versiontimestamp'  )" <<
                "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (7, 'Routes',          '1970-01-01 01:01:00')" <<
                "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (8, 'AirSpaces',       '1970-01-01 01:01:00')" <<
                "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES (9, 'Accounts',        '1970-01-01 01:01:00')";
  addQuery("setup-set-lastmodified",
					 "QSQLITE",
					 setlastmod);

  // upgrades
	addQuery("upgrade-waypoints-add-type",
					 "QMYSQL",
					 "ALTER TABLE `WayPoints` "
              "ADD `Type` INT NOT NULL DEFAULT 1");
	addQuery("upgrade-waypoints-add-type",
					 "QSQLITE",
					 "ALTER TABLE `WayPoints` "
              "ADD `Type` INTEGER NOT NULL DEFAULT 1");
  addQuery("upgrade-flights-add-photopath",
           "common",
           "ALTER TABLE `Flights` "
              "ADD `PhotoPath` VARCHAR(500) DEFAULT ''");


}

void QueryStore::addQuery(const QString& name, const QString& driver, const Query& query)
{
	// check if there is already a map for this driver
	TDriverMap::iterator iter=m_QueryMap.find(driver);
	if (iter==m_QueryMap.end())
	{
		// no, insert a map for this driver first
		iter = m_QueryMap.insert(driver,TQueryMap());
	}
	// insert query
	iter->insert(name,query);
}

void QueryStore::addQuery(const QString& name, const QString& driver, const QString& sql)
{
	// create a query and propagate call
	Query query;
	query.addStatement(sql);
	addQuery(name,driver,query);
}

Query QueryStore::getQuery(const QString& name, const QSqlDatabase& db) const
{
	// check if there is a common query
	Query query = getQuery(name, "common");

	if (query.getStatements().size() > 0)
	{
		// found
		return query;
	}

	// try with the dedicated driver name
	query = getQuery(name,db.driverName());

	// issue a warning
	if (query.getStatements().empty())
	{
		qDebug() << "query '" << name << "' not found";
	}

	return query;
}

Query QueryStore::getQuery(const QString& name, const QString& driver) const
{
	// find the query map first
	TDriverMap::const_iterator driveriter=m_QueryMap.constFind(driver);
	if (driveriter==m_QueryMap.constEnd())
	{
		qDebug() << "driver name'" << driver << "' is unknown";
		// return an empty query
		return Query();
	}

	// find query
	TQueryMap::const_iterator queryiter = driveriter->constFind(name);
	if (queryiter!=driveriter->constEnd())
	{
		return queryiter.value();
	}

	// return an empty query
	return Query();
}
