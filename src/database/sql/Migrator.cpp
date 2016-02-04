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

#include <QDebug>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QSemaphore>
#include "ISql.h"
#include "DatabaseUtilities.h"
#include "QueryExecutor.h"
#include "Upgrade.h"
#include "Migrator.h"

Migrator::Migrator()
	: m_pExecutor(new QueryExecutor())
	, m_pCredSemaphore(new QSemaphore(0))
{
}

Migrator::~Migrator()
{
	delete m_pExecutor;
	m_pExecutor = NULL;
	delete m_pCredSemaphore;
	m_pCredSemaphore = NULL;
}

void Migrator::stopProcessing()
{
	m_stopProcessing=true;
}

void Migrator::handleCredentialsEntered(QString root, QString pwd, bool ok)
{
	m_Root=root;
	m_Pwd=pwd;
	m_CredOk=ok;
	m_pCredSemaphore->release();
}

void Migrator::copyDatabases(DatabaseParameters fromDBParameters, DatabaseParameters toDBParameters)
{
	m_stopProcessing = false;
	bool res = true;
	QString err;

	// reject overwriting own db
	if (fromDBParameters.match(toDBParameters))
	{
		emit finished(Migrator::failed, tr("Source and target are equal. Refusing migration!"));
		return;
	}

	// create source connection
	m_FromDB = QSqlDatabase::addDatabase(fromDBParameters.dBType(),"MigrationFromDatabase");
	fromDBParameters.apply(m_FromDB);

	if (!m_FromDB.open())
	{
		emit finished(Migrator::failed, tr("Error while opening the source database."));
		return;
	}

	// create target connection
	m_ToDB = QSqlDatabase::addDatabase(toDBParameters.dBType(),"MigrationToDatabase");
	toDBParameters.apply(m_ToDB);
	bool freshdb=false;

	if (!m_ToDB.open())
	{
		// the target db may not yet exist
		if (toDBParameters.isMySQL())
		{
			m_Root = "";
			m_Pwd = "";
			m_CredOk = true;

			emit requestCredentials();
			// wait for user input
			m_pCredSemaphore->acquire();
		}
		if (m_CredOk)
		{
			res = DatabaseUtilities::createDb(toDBParameters,m_Root,m_Pwd);
			res = m_ToDB.open();
			freshdb=true;
		}
		else
		{
			res = false;
		}

		if (!res)
		{
			qDebug() << "Opening target db failed. Error: " << m_ToDB.lastError().databaseText();
			emit finished(Migrator::failed, tr("Error while opening the target database."));
			m_FromDB.close();
			return;
		}
	}

	// Delete all tables
	// no need if db was created above
	if (res && !freshdb)
	{
		emit stepStarted(tr("Clean Target..."));
		QSqlQuery del = m_pExecutor->executeQuery("migrate-drop-tables",m_ToDB);

		if (del.lastError().type()!=QSqlError::NoError)
		{
			err = tr("Error while scrubbing the target database.");
			res = false;
		}
	}

	// then create the schema
	// no need if db was created above
	if (res && !freshdb)
	{
		emit stepStarted(tr("Create Schema..."));
		Upgrade creator(m_ToDB);
		res &= creator.setup(toDBParameters);
		res &= creator.upgrade();

		if (!res)
		{
			err = tr("Error while creating the database schema.");
		}
	}

	if (res && !copyTable(tr("Copy Routes..."),
								 "migrate-read-routes",
								 "migrate-write-routes"))
	{
		err = tr("Error while migrating Routes");
		res = false;
	}

	if (res && !copyTable(tr("Copy WayPoints..."),
								 "migrate-read-waypoints",
								 "migrate-write-waypoints"))
	{
		err = tr("Error while migrating WayPoints");
		res = false;
	}

	// copy tables
	if (res && !copyTable(tr("Copy RouteItems..."),
								 "migrate-read-routeitems",
								 "migrate-write-routeitems"))
	{
		err =  tr("Error while migrating RouteItems");
		res = false;
	}

  if (res && !copyTable(tr("Copy AirSpaces..."),
                 "migrate-read-airspaces",
                 "migrate-write-airspaces"))
  {
    err =  tr("Error while migrating AirSpaces");
    res = false;
  }

  if (res && !copyTable(tr("Copy AirSpaceItems..."),
                 "migrate-read-airspaceitems",
                 "migrate-write-airspaceitems"))
  {
    err =  tr("Error while migrating AirSpaceItems");
    res = false;
  }

  if (res && !copyTable(tr("Copy Gliders..."),
								 "migrate-read-gliders",
								 "migrate-write-gliders"))
	{
		err = tr("Error while migrating Gliders");
		res = false;
	}

	if (res && !copyTable(tr("Copy Servicings..."),
								 "migrate-read-servicings",
								 "migrate-write-servicings"))
	{
		err = tr("Error while migrating Servicings");
		res = false;
	}

	if (res && !copyTable(tr("Copy Pilots..."),
								 "migrate-read-pilots",
								 "migrate-write-pilots"))
	{
		err = tr("Error while migrating Pilots");
		res = false;
	}

  if (res && !copyTable(tr("Copy Accounts..."),
                 "migrate-read-accounts",
                 "migrate-write-accounts"))
  {
    err = tr("Error while migrating Accounts");
    res = false;
  }

	if (res && !copyTable(tr("Copy Flights..."),
								 "migrate-read-flights",
								 "migrate-write-flights"))
	{
		err = tr("Error while migrating Flights");
		res = false;
	}

	m_FromDB.close();
	m_ToDB.close();

	if (res)
	{
		emit finished(Migrator::success, QString());
	}
	else
	{
		emit finished(Migrator::failed, err);
	}
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

	if (fetch.lastError().type()!=QSqlError::NoError)
	{
		// continueing is useless
		qDebug() << "running query '" << fromAct << "' failed.";
		emit finished(Migrator::failed, tr("Error while reading table."));
		handleClosing(m_stopProcessing);
		return false;
	}

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
			qDebug() << "running query '" << toAct << "' failed";
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
