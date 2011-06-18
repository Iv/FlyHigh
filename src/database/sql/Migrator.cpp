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

#include "QueryExecutor.h"
#include "Upgrade.h"
#include "Migrator.h"

Migrator::Migrator()
	: m_pExecutor(new QueryExecutor())
{
}

Migrator::~Migrator()
{
	delete m_pExecutor;
	m_pExecutor = NULL;
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
		emit stepStarted(tr("Clean Target..."));
		QSqlQuery res = m_pExecutor->executeQuery("migrate-drop-tables",m_ToDB);

		if (res.lastError().type()!=QSqlError::NoError)
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
								 "migrate-read-routeitems",
								 "migrate-write-routeitems"))
	{
		qDebug() << "Error in RouteItems";
		return;
	}

	if (!copyTable(tr("Copy WayPoints..."),
								 "migrate-read-waypoints",
								 "migrate-write-waypoints"))
	{
		qDebug() << "Error in WayPoints";
		return;
	}

	if (!copyTable(tr("Copy Gliders..."),
								 "migrate-read-gliders",
								 "migrate-write-gliders"))
	{
		qDebug() << "Error in Gliders";
		return;
	}

	if (!copyTable(tr("Copy Servicings..."),
								 "migrate-read-servicings",
								 "migrate-write-servicings"))
	{
		qDebug() << "Error in Servicings";
		return;
	}

	if (!copyTable(tr("Copy Pilots..."),
								 "migrate-read-pilots",
								 "migrate-write-pilots"))
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
								 "migrate-read-routes",
								 "migrate-write-routes"))
	{
		qDebug() << "Error in Routes";
		return;
	}

	if (!copyTable(tr("Copy Flights..."),
								 "migrate-read-flights",
								 "migrate-write-flights"))
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


	// read table
	QSqlQuery fetch = m_pExecutor->executeQuery(fromAct,m_FromDB);

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

	// create a list of the column names
	int cols = fetch.record().count();
	QStringList columnames;
	for(int i=0; i<cols; ++i)
	{
		columnames.append(fetch.record().fieldName(i));
	}

	// loop over results
	while(fetch.next())
	{
		emit smallStepStarted(++resultcounter, resultsize);

		// create bindings
		QueryExecutor::TBindMap bindings;
		int i=0;
		for(QStringList::const_iterator iter = columnames.constBegin(); iter != columnames.constEnd(); ++iter)
		{
			QString col = *iter;
			// prepend ':' to column names
			bindings[col.insert(0,':')] = fetch.value(i);
			i++;
		}

		// write to new table
		QSqlQuery insert = m_pExecutor->executeQuery(toAct,bindings,m_ToDB);

		if (insert.lastError().type()!=QSqlError::NoError)
		{
			qDebug() << "Errors while inserting";
			res = false;
		}
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
