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
#include "Error.h"
#include "Upgrade.h"

const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_0 = QDateTime(QDate(2004, 3, 20), QTime(20, 0)); // equal version_0_3_0
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_1 = QDateTime(QDate(2005, 6, 12), QTime(20, 0)); // equal version_0_3_1
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_2 = QDateTime(QDate(2006, 3, 31), QTime(20, 50)); // equal version_0_3_2
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_5_0 = QDateTime(QDate(2006, 11, 14), QTime(0, 0)); // equal version_0_5_0
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_8_1 = QDateTime(QDate(2011, 1, 18), QTime(0, 0)); // equal version_0_8_1

Upgrade::Upgrade(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

void Upgrade::setup(const QString &dbname, const QString &user, const QString &pwd)
{
	QSqlQuery query(db());
	QString sqls;

	sqls = "CREATE DATABASE `%1` DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls.arg(dbname));

	sqls = "CREATE USER '%1'@'localhost' IDENTIFIED BY '%2';";
	query.exec(sqls.arg(user).arg(pwd));

	sqls = "GRANT SELECT, INSERT, UPDATE, DELETE, DROP, CREATE, ALTER ON %1.* TO '%2'@'localhost' IDENTIFIED BY '%3';";
	query.exec(sqls.arg(dbname).arg(user).arg(pwd));

	sqls = "USE `%1`;";
	query.exec(sqls.arg(dbname));

	sqls = "CREATE TABLE `Gliders`"
	"("
		"`Id` INT NULL AUTO_INCREMENT,"
		"`Manufacturer` VARCHAR(16) CHARACTER SET utf8 NULL DEFAULT '',"
		"`Model` VARCHAR(16) CHARACTER SET utf8 NOT NULL,"
		"`Serial` VARCHAR(16) CHARACTER SET utf8 NULL DEFAULT '',"
		"PRIMARY KEY(`Id`),"
		"UNIQUE KEY(`Manufacturer`, `Model`, `Serial`)"
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "CREATE TABLE `Pilots`"
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
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "CREATE TABLE `WayPoints`"
	"("
		"`Id` INT NULL AUTO_INCREMENT,"
		"`Name` VARCHAR(16) NOT NULL,"
		"`Spot` VARCHAR(16) NULL DEFAULT '',"
		"`Country` VARCHAR(2) NULL DEFAULT '',"
		"`Longitude` FLOAT NOT NULL,"
		"`Latitude` FLOAT NOT NULL,"
		"`Altitude` INT NOT NULL,"
		"`Description` VARCHAR(200) NULL DEFAULT '',"
		"PRIMARY KEY(`Id`),"
		"UNIQUE KEY byWayPoints(`Name`, `Spot`, `Country`)"
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "CREATE TABLE `Flights`"
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
		"PRIMARY KEY(`Id`),"
		"UNIQUE KEY byFlight(`PilotId`, `Number`),"
		"FOREIGN KEY(`PilotId`) REFERENCES Pilots(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
		"FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
		"FOREIGN KEY(`StartPtId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
		"FOREIGN KEY(`LandPtId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "CREATE TABLE `Routes`"
	"("
		"`Id` INT NULL AUTO_INCREMENT,"
		"`Name` VARCHAR(16) NOT NULL,"
		"`Type` INT NULL DEFAULT 0,"
		"PRIMARY KEY(`Id`),"
		"UNIQUE KEY byRoutes(`Name`)"
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "CREATE TABLE `RouteItems`"
	"("
		"`Id` INT NULL AUTO_INCREMENT,"
		"`RouteId` INT NOT NULL,"
		"`WayPointId` INT NOT NULL,"
		"PRIMARY KEY(`Id`),"
		"FOREIGN KEY(`RouteId`) REFERENCES Routes(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE,"
		"FOREIGN KEY(`WayPointId`) REFERENCES WayPoints(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "CREATE TABLE `Servicings`"
	"("
		"`Id` INT NULL AUTO_INCREMENT,"
		"`GliderId` INT NOT NULL,"
		"`Date` DATE NOT NULL,"
		"`Responsibility` VARCHAR(16) NOT NULL,"
		"`Comment` VARCHAR(200) NULL DEFAULT '',"
		"PRIMARY KEY(`Id`),"
		"FOREIGN KEY(`GliderId`) REFERENCES Gliders(`Id`) ON DELETE RESTRICT ON UPDATE CASCADE"
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "CREATE TABLE `LastModified`"
	"("
		"`Id` INT NULL AUTO_INCREMENT,"
		"`Name` VARCHAR(16) NOT NULL,"
		"`Time` datetime NOT NULL,"
		"PRIMARY KEY(`Id`),"
		"UNIQUE KEY (`Name`)"
	")ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;";
	query.exec(sqls);

	sqls = "INSERT INTO `LastModified` (`Id`, `Name`, `Time`) VALUES"
	"(1, 'Pilots', '1970-01-01 00:00:00'),"
	"(2, 'Flights', '1970-01-01 00:00:00'),"
	"(3, 'Gliders', '1970-01-01 00:00:00'),"
	"(4, 'Servicings', '1970-01-01 00:00:00'),"
	"(5, 'WayPoints', '1970-01-01 00:00:00'),"
	"(6, 'DataBaseVersion', '2011-01-18 00:00:00'),"
	"(7, 'Routes', '1970-01-01 00:00:00');";
	query.exec(sqls);
}

void Upgrade::upgrade()
{
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
		query.exec(sqls);
		setDataBaseVersion(DataBaseVersion_0_8_1);
	}
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
