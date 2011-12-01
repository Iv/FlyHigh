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

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <stdlib.h>
#include <QSqlError>
#include "QueryExecutor.h"
#include "DatabaseParameters.h"
#include "Error.h"
#include "Upgrade.h"

const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_0 = QDateTime(QDate(2004,  3, 20), QTime(20, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_1 = QDateTime(QDate(2005,  6, 12), QTime(20, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_2 = QDateTime(QDate(2006,  3, 31), QTime(20,50));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_5_0 = QDateTime(QDate(2006, 11, 14), QTime( 0, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_8_1 = QDateTime(QDate(2011,  1, 18), QTime( 0, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_8_2 = QDateTime(QDate(2011,  2, 20), QTime( 0, 0));
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_9_2 = QDateTime(QDate(2011,  12, 1), QTime( 0, 0));
// don't forget to replace new version string on setup-set-lastmodified statement!

Upgrade::Upgrade(QSqlDatabase DB)
	:DataBaseSub(DB)
	,m_pExecutor(new QueryExecutor())
{
}

bool Upgrade::setup(const DatabaseParameters& params)
{
	bool res = true;
	QueryExecutor::TReplaceMap replacements;
	QSqlQuery query;
	QSqlQuery user;

	// we need db name, username and password as replacement tokens
	replacements["%dbname"] = params.dBName();
	replacements["%username"] = params.dBUserName();
	replacements["%password"] = params.dBPassword();

	// create the db
	query = m_pExecutor->executeQuery("setup-create-db",
                                    QueryExecutor::TBindMap(),
                                    replacements,
                                    db());

	// only mysql cares about user management and permissions
	if(params.isMySQL())
	{
		// check if there's already a user
		user = m_pExecutor->executeQuery("setup-get-user",
                                     QueryExecutor::TBindMap(),
                                     replacements,
                                     db());

		// if the user already exists, it won't be touched
		// which means the user keeps his former password
		if(user.size() == 0)
		{
			// no user with this name
			m_pExecutor->executeQuery("setup-create-user",
																QueryExecutor::TBindMap(),
																replacements,
																db());
		}

		// grant permissions unconditionally, the db is probably new
		m_pExecutor->executeQuery("setup-privileges",
															QueryExecutor::TBindMap(),
															replacements,
															db());

		// and the 'USE' clause
		m_pExecutor->executeQuery("setup-finalize",
															QueryExecutor::TBindMap(),
															replacements,
															db());
	}

	if(query.lastError().type()!=QSqlError::NoError)
	{
		// something failed
		return false;
	}

	// create tables
	m_pExecutor->executeQuery("setup-create-gliders", db());
	m_pExecutor->executeQuery("setup-create-pilots", db());
	m_pExecutor->executeQuery("setup-create-waypoints", db());
	m_pExecutor->executeQuery("setup-create-flights", db());
	m_pExecutor->executeQuery("setup-create-routes", db());
	m_pExecutor->executeQuery("setup-create-routeitems", db());
	m_pExecutor->executeQuery("setup-create-servicings", db());
	m_pExecutor->executeQuery("setup-create-lastmodified", db());

	// finalize db setup
	replacements.clear();
	replacements["%versiontimestamp"] = DataBaseVersion_0_9_2.toString("yyyy-MM-dd hh:mm:ss");
	m_pExecutor->executeQuery("setup-set-lastmodified",
														QueryExecutor::TBindMap(),
														replacements,
														db());

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

	if(dataBaseVersion() < DataBaseVersion_0_9_2)
	{
	  m_pExecutor->executeQuery("upgrade-waypoints-add-type-and-radius", db());
		setDataBaseVersion(DataBaseVersion_0_9_2);
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
