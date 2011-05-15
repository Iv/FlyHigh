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
#include <QSqlError>
#include <QVariant>

#include <QDebug>

#include "QueryStore.h"
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

	QueryStore* pQueryStore = QueryStore::pInstance();

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
		res = query.exec(pQueryStore->getQuery("migrate-drop-flights",m_ToDB));
		res = query.exec(pQueryStore->getQuery("migrate-drop-routes",m_ToDB));
		res = query.exec(pQueryStore->getQuery("migrate-drop-lastmodified",m_ToDB));
		res = query.exec(pQueryStore->getQuery("migrate-drop-pilots",m_ToDB));
		res = query.exec(pQueryStore->getQuery("migrate-drop-servicings",m_ToDB));
		res = query.exec(pQueryStore->getQuery("migrate-drop-gliders",m_ToDB));
		res = query.exec(pQueryStore->getQuery("migrate-drop-waypoints",m_ToDB));
		res = query.exec(pQueryStore->getQuery("migrate-drop-routeitems",m_ToDB));
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
								 pQueryStore->getQuery("migrate-read-routeitems",m_FromDB),
								 pQueryStore->getQuery("migrate-write-routeitems",m_ToDB)))
	{
		qDebug() << "Error in RouteItems";
		return;
	}

	if (!copyTable(tr("Copy WayPoints..."),
								 pQueryStore->getQuery("migrate-read-waypoints",m_FromDB),
								 pQueryStore->getQuery("migrate-write-waypoints",m_ToDB)))
	{
		qDebug() << "Error in WayPoints";
		return;
	}

	if (!copyTable(tr("Copy Gliders..."),
								 pQueryStore->getQuery("migrate-read-gliders",m_FromDB),
								 pQueryStore->getQuery("migrate-write-gliders",m_ToDB)))
	{
		qDebug() << "Error in Gliders";
		return;
	}

	if (!copyTable(tr("Copy Servicings..."),
								 pQueryStore->getQuery("migrate-read-servicings",m_FromDB),
								 pQueryStore->getQuery("migrate-write-servicings",m_ToDB)))
	{
		qDebug() << "Error in Servicings";
		return;
	}

	if (!copyTable(tr("Copy Pilots..."),
								 pQueryStore->getQuery("migrate-read-pilots",m_FromDB),
								 pQueryStore->getQuery("migrate-write-pilots",m_ToDB)))
	{
		qDebug() << "Error in Pilots";
		return;
	}

	/*
	if (!copyTable(tr("Copy LastModified..."),
				 pQueryStore->getQuery("migrate-read-lastmodified",m_FromDB),
				 pQueryStore->getQuery("migrate-write-lastmodified",m_ToDB)))
	{
	 qDebug() << "Error in LastModified";
	 return;
	}
	*/

	if (!copyTable(tr("Copy Routes..."),
								 pQueryStore->getQuery("migrate-read-routes",m_FromDB),
								 pQueryStore->getQuery("migrate-write-routes",m_ToDB)))
	{
		qDebug() << "Error in Routes";
		return;
	}

	if (!copyTable(tr("Copy Flights..."),
								 pQueryStore->getQuery("migrate-read-flights",m_FromDB),
								 pQueryStore->getQuery("migrate-write-flights",m_ToDB)))
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
