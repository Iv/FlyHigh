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
#include <qsqlcursor.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <stdlib.h>
#include "Error.h"
#include "Upgrade.h"

const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_0 = QDateTime(QDate(2004, 3, 20), QTime(20, 0)); // equal version_0_3_0
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_1 = QDateTime(QDate(2005, 6, 12), QTime(20, 0)); // equal version_0_3_1
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_3_2 = QDateTime(QDate(2006, 3, 31), QTime(20, 50)); // equal version_0_3_2
const Upgrade::DataBaseVersion Upgrade::DataBaseVersion_0_5_0 = QDateTime(QDate(2006, 11, 14), QTime(0, 0)); // equal version_0_5_0


Upgrade::Upgrade(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

void Upgrade::upgrade()
{
	if(dataBaseVersion() < DataBaseVersion_0_5_0)
	{
		ASSERT(false);
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
	
	version = tabVers.toString("yyyy-MM-dd hh:mm:ss").ascii();
	sqls.sprintf("UPDATE LastModified SET Time = '%s' WHERE Name = 'DataBaseVersion'",
				version.ascii());
	query.exec(sqls);
}
