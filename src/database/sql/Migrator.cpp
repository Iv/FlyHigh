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
	{
		// route items
		emit stepStarted(tr("Copy RouteItems..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, RouteId, WayPointId FROM RouteItems",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;
		while(fetch.next())
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO RouteItems (Id, RouteId, WayPointId) VALUES (:Id, :RouteId, :WayPointId)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":RouteId",fetch.value(1));
			insert.bindValue(":WayPointId",fetch.value(2));
			res &= insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
	}

	{
		// waypoints
		emit stepStarted(tr("Copy WayPoints..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description FROM WayPoints",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;
		while(fetch.next())
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO WayPoints (Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description) VALUES (:Id, :Name, :Spot, :Country, :Longitude, :Latitude, :Altitude, :Description)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":Name",fetch.value(1));
			insert.bindValue(":Spot",fetch.value(2));
			insert.bindValue(":Country",fetch.value(3));
			insert.bindValue(":Longitude",fetch.value(4));
			insert.bindValue(":Latitude",fetch.value(5));
			insert.bindValue(":Altitude",fetch.value(6));
			insert.bindValue(":Description",fetch.value(7));
			res &= insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
	}
	{
		// gliders
		emit stepStarted(tr("Copy Gliders..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, Manufacturer, Model, Serial FROM Gliders",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;
		while(fetch.next())
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO Gliders (Id, Manufacturer, Model, Serial) VALUES (:Id, :Manufacturer, :Model, :Serial)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":Manufacturer",fetch.value(1));
			insert.bindValue(":Model",fetch.value(2));
			insert.bindValue(":Serial",fetch.value(3));
			res &= insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
	}
	{
		// servicings
		emit stepStarted(tr("Copy Servicings..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, GliderId, Date, Responsibility, Comment FROM Servicings",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;
		while(fetch.next())
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO Servicings (Id, GliderId, Date, Responsibility, Comment) VALUES (:Id, :GliderId, :Date, :Responsibility, :Comment)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":GliderId",fetch.value(1));
			insert.bindValue(":Date",fetch.value(2));
			insert.bindValue(":Responsibility",fetch.value(3));
			insert.bindValue(":Comment",fetch.value(4));
			res &= insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
	}
	{
		// pilots
		emit stepStarted(tr("Copy Pilots..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, FirstName, LastName, BirthDate, CallSign, GliderId FROM Pilots",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;
		while(fetch.next())
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO Pilots (Id, FirstName, LastName, BirthDate, CallSign, GliderId) VALUES (:Id, :FirstName, :LastName, :BirthDate, :CallSign, :GliderId)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":FirstName",fetch.value(1));
			insert.bindValue(":LastName",fetch.value(2));
			insert.bindValue(":BirthDate",fetch.value(3));
			insert.bindValue(":CallSign",fetch.value(4));
			insert.bindValue(":GliderId",fetch.value(5));
			res &= insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
	}
	{
		// last modified?
		emit stepStarted(tr("Copy LastModified..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, Name, Time FROM LastModified",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;

		//		while(fetch.next())
		// insert would fail, already set in setup()/upgrade()
		while(false)
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO LastModified (Id, Name, Time) VALUES (:Id, :Name, :Time)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":Name",fetch.value(1));
			insert.bindValue(":Time",fetch.value(2));
			res &= insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
	}
	{
		// routes
		emit stepStarted(tr("Copy Routes..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, Name, Type FROM Routes",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;
		while(fetch.next())
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO Routes (Id, Name, Type) VALUES (:Id, :Name, :Type)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":Name",fetch.value(1));
			insert.bindValue(":Type",fetch.value(2));
			res &= insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
	}

	{
		// flights
		emit stepStarted(tr("Copy Flights..."));
		bool res = true;
		int resultsize;

		// this acutally runs the query!
		QSqlQuery fetch("SELECT Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile FROM Flights",
										m_FromDB);

		if (fetch.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			resultsize = fetch.size();
		}
		else
		{
			qDebug() << "The sql driver does not return query result size";
			// we try something different...
			fetch.last();
			resultsize = fetch.at()<0 ? 0 : fetch.at();
			fetch.first();
		}

		int resultcounter = 0;
		while(fetch.next())
		{
			emit smallStepStarted(++resultcounter, resultsize);

			QSqlQuery insert(m_ToDB);
			insert.prepare("INSERT INTO Flights (Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, Duration, Distance, Comment, IGCFile) VALUES (:Id, :Number, :PilotId, :Date, :Time, :GliderId, :StartPtId, :LandPtId, :Duration, :Distance, :Comment, :IGCFile)");
			insert.bindValue(":Id",fetch.value(0));
			insert.bindValue(":Number",fetch.value(1));
			insert.bindValue(":PilotId",fetch.value(2));
			insert.bindValue(":Date",fetch.value(3));
			insert.bindValue(":Time",fetch.value(4));
			insert.bindValue(":GliderId",fetch.value(5));
			insert.bindValue(":StartPtId",fetch.value(6));
			insert.bindValue(":LandPtId",fetch.value(7));
			insert.bindValue(":Duration",fetch.value(8));
			insert.bindValue(":Distance",fetch.value(9));
			insert.bindValue(":Comment",fetch.value(10));
			insert.bindValue(":IGCFile",fetch.value(11));
			insert.exec();
		}

		if (m_stopProcessing || !res)
		{
			handleClosing(m_stopProcessing);
			return;
		}
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

