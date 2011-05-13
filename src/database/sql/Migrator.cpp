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

#include <QSqlQuery>
#include <QSqlDriver>
#include <QVariant>

#include <QDebug>

#include "Upgrade.h"
#include "Migrator.h"

Migrator::Migrator()
{
}

Migrator::~Migrator()
{
}

void Migrator::stopProcessing()
{
	m_stopProcessing=true;
}

void Migrator::copyDatabases(DatabaseParameters fromDBParameters, DatabaseParameters toDBParameters)
{
	m_stopProcessing = false;

	// create source connection
	m_FromDB = QSqlDatabase::addDatabase(fromDBParameters.dBType(),"MigrationFromDatabase");
	if (fromDBParameters.isMySQL())
	{
		m_FromDB.setDatabaseName(fromDBParameters.dBName());
		m_FromDB.setUserName(fromDBParameters.dBUserName());
		m_FromDB.setPassword(fromDBParameters.dBPassword());
		m_FromDB.setHostName(fromDBParameters.dBHostName());
		m_FromDB.setPort(fromDBParameters.dBPort());
	}
	else if (fromDBParameters.isSQLite())
	{
		m_FromDB.setDatabaseName(fromDBParameters.dBFile());
	}

	if (!m_FromDB.open())
	{
		emit finished(Migrator::failed, tr("Error while opening the source database."));
		return;
	}

	// create target connection
	m_ToDB = QSqlDatabase::addDatabase(toDBParameters.dBType(),"MigrationToDatabase");
	if (toDBParameters.isMySQL())
	{
		m_ToDB.setDatabaseName(toDBParameters.dBName());
		m_ToDB.setUserName(toDBParameters.dBUserName());
		m_ToDB.setPassword(toDBParameters.dBPassword());
		m_ToDB.setHostName(toDBParameters.dBHostName());
		m_ToDB.setPort(toDBParameters.dBPort());
	}
	else if (toDBParameters.isSQLite())
	{
		m_ToDB.setDatabaseName(toDBParameters.dBFile());
	}

	if (!m_ToDB.open())
	{
		emit finished(Migrator::failed, tr("Error while opening the target database."));
		return;
	}

	// Delete all tables
	{
		bool res = true;
		emit stepStarted(tr("Clean Target..."));

		QSqlQuery query(m_ToDB);
		QString sqls;
		// flights
		sqls = "DROP TABLE IF EXISTS `Flights`";
		res &= query.exec(sqls);
		// routes
		sqls = "DROP TABLE IF EXISTS `Routes`";
		res &= query.exec(sqls);
		// LastModified
		sqls = "DROP TABLE IF EXISTS `LastModified`";
		res &= query.exec(sqls);
		// pilots
		sqls = "DROP TABLE IF EXISTS `Pilots`";
		res &= query.exec(sqls);
		// servicings
		sqls = "DROP TABLE IF EXISTS `Servicings`";
		res &= query.exec(sqls);
		// gliders
		sqls = "DROP TABLE IF EXISTS `Gliders`";
		res &= query.exec(sqls);
		// waypoints
		sqls = "DROP TABLE IF EXISTS `WayPoints`";
		res &= query.exec(sqls);
		// route items
		sqls = "DROP TABLE IF EXISTS `RouteItems`";
		res &= query.exec(sqls);

		if (!res)
		{
			emit finished(Migrator::failed, tr("Error while scrubbing the target database."));
			m_FromDB.close();
			m_ToDB.close();
			return;
		}
	}

	// then create the schema
	{
		bool res = true;
		emit stepStarted(tr("Create Schema..."));

		Upgrade creator(m_ToDB);
		if (toDBParameters.isMySQL())
		{
			res &= creator.setup(toDBParameters.dBName(),toDBParameters.dBUserName(),toDBParameters.dBPassword());
		}
		else if (toDBParameters.isSQLite())
		{
			res &= creator.setup(toDBParameters.dBFile(),toDBParameters.dBUserName(),toDBParameters.dBPassword());
		}
		res &= creator.upgrade();

		if (!res)
		{
			emit finished(Migrator::failed, tr("Error while creating the database schema."));
			m_FromDB.close();
			m_ToDB.close();
			return;
		}
	}

	// copy tables
	if (!copyTable(tr("Copy RouteItems..."),
								 "SELECT Id, RouteId, WayPointId FROM RouteItems",
								 "INSERT INTO RouteItems (Id, RouteId, WayPointId) VALUES (?,?,?)"))
	{
		qDebug() << "Error";
		return;
	}

	if (!copyTable(tr("Copy WayPoints..."),
								 "SELECT Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description FROM WayPoints",
								 "INSERT INTO WayPoints (Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description) VALUES (?,?,?,?,?,?,?,?)"))
	{
		qDebug() << "Error in WayPoints";
		return;
	}

	if (!copyTable(tr("Copy Gliders..."),
								 "SELECT Id, Manufacturer, Model, Serial FROM Gliders",
								 "INSERT INTO Gliders (Id, Manufacturer, Model, Serial) VALUES (?,?,?,?)"))
	{
		qDebug() << "Error in Gliders";
		return;
	}

	if (!copyTable(tr("Copy Servicings..."),
								 "SELECT Id, GliderId, Date, Responsibility, Comment FROM Servicings",
								 "INSERT INTO Servicings (Id, GliderId, Date, Responsibility, Comment) VALUES (?,?,?,?,?)"))
	{
		qDebug() << "Error in Servicings";
		return;
	}

	if (!copyTable(tr("Copy Pilots..."),
								 "SELECT Id, FirstName, LastName, BirthDate, CallSign, GliderId FROM Pilots",
								 "INSERT INTO Pilots (Id, FirstName, LastName, BirthDate, CallSign, GliderId) VALUES (?,?,?,?,?,?)"))
	{
		qDebug() << "Error in Pilots";
		return;
	}

	/*
	if (!copyTable(tr("Copy LastModified..."),
			 "SELECT Id, Name, Time FROM LastModified",
			 "INSERT INTO LastModified (Id, Name, Time) VALUES (?,?,?)"))
	{
	 qDebug() << "Error in LastModified";
	 return;
	}
	*/

	if (!copyTable(tr("Copy Routes..."),
								 "SELECT Id, Name, Type FROM Routes",
								 "INSERT INTO Routes (Id, Name, Type) VALUES (?,?,?)"))
	{
		qDebug() << "Error in Routes";
		return;
	}

	if (!copyTable(tr("Copy Flights..."),
								 "SELECT Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile FROM Flights",
								 "INSERT INTO Flights (Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)"))
	{
		qDebug() << "Error in Flights";
		return;
	}

	m_FromDB.close();
	m_ToDB.close();

	emit finished(Migrator::success, QString());
}

void Migrator::handleClosing(bool isStopThread)
{
	if (isStopThread)
	{
		emit finished(Migrator::canceled, "");
	}

	m_FromDB.close();
	m_ToDB.close();
}

bool Migrator::copyTable(const QString& name, const QString& fromAct, const QString& toAct)
{
	// route items
	emit stepStarted(name);
	bool res = true;
	int resultcounter = 0;
	int resultsize;

	// this acutally runs the query!
	QSqlQuery fetch(fromAct,m_FromDB);

	if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
	{
		resultsize = fetch.size();
	}
	else
	{
		// The sql driver does not return query result size
		// we try something different...
		fetch.last();
		resultsize = fetch.at()<0 ? 0 : fetch.at();
		// set the result pointer _before_ the first record
		fetch.seek(-1);
	}

	while(fetch.next())
	{
		emit smallStepStarted(++resultcounter, resultsize);

		QSqlQuery insert(m_ToDB);
		insert.prepare(toAct);

		int cols = fetch.record().count();
		for(int i=0; i<cols; ++i)
		{
			insert.addBindValue(fetch.value(i));
		}
		res &= insert.exec();
	}

	if (m_stopProcessing || !res)
	{
		// error or aborted
		handleClosing(m_stopProcessing);
		return false;
	}

	// success
	return res;
}
