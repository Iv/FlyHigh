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

const QDateTime Upgrade::m_TableVersion_0_3_0 = QDateTime(QDate(2004, 3, 20), QTime(20, 0)); // equal version_0_3_0
const QDateTime Upgrade::m_TableVersion_0_3_1 = QDateTime(QDate(2005, 6, 12), QTime(20, 0)); // equal version_0_3_1
const QDateTime Upgrade::m_TableVersion_0_3_2 = QDateTime(QDate(2006, 3, 31), QTime(20, 50)); // equal version_0_3_2

Upgrade::Upgrade(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

void Upgrade::upgrade()
{
	if(tableVersion() < m_TableVersion_0_3_2)
	{
		upgradeTo_0_3_2();
	}
}

QDateTime Upgrade::tableVersion()
{
	QString sqls;
	QString date;
	QSqlQuery query(db());
	QDateTime time = m_TableVersion_0_3_0;
	
	sqls.sprintf("SELECT * FROM `LastModified` WHERE `Name` = 'TableVersion'");
	
	if(query.exec(sqls) && query.first())
	{
		time = query.value(1).toDateTime();
	}
	
	return time;
}

void Upgrade::setTableVersion(const QDateTime &tabVers)
{
	QString sqls;
	QString version;
	QSqlQuery query(db());
	
	version = tabVers.toString("yyyy-MM-dd hh:mm:ss").ascii();
	
	if(tableVersion() > m_TableVersion_0_3_0)
	{
		sqls.sprintf("UPDATE `LastModified` SET `Time` = '%s' WHERE `Name` = 'TableVersion'", 
				version.ascii());
	}
	else
	{
		sqls.sprintf("INSERT INTO `LastModified` (`Name`, `Time`) VALUES ('TableVersion', '%s')",
				version.ascii());
	}
	
	query.exec(sqls);
}
/*
void Upgrade::testTables()
{
	QString sqls;
	QSqlQuery query(db());
	bool success;

	if(!db()->tables().contains(""))
	{
		sqls.sprintf("CREATE TABLE `%s`("
					"`Number` int(11) NOT NULL default '0',"
					"`Date` date NOT NULL default '0000-00-00',"
					"`Time` time NOT NULL default '00:00:00',"
					"`Glider` varchar(16) NOT NULL default '0'  REFERENCES Gliders(Model),"
					"`StartPt` varchar(16) NOT NULL default '0' REFERENCES WayPoints(Name),"
					"`LandPt` varchar(16) NOT NULL default '0' REFERENCES WayPoints(Name),"
					"`Duration` int(11) NOT NULL default '0',"
					"`Distance` int(11) default '0',"
					"`Comment` varchar(200) default NULL,"
					"`IGCFile` mediumblob,"
					"PRIMARY KEY  (`Number`)"
					") TYPE=InnoDB;", tableName().ascii());
					
		success = query.exec(sqls);
		Error::verify(success, Error::SQL_CMD);
	}
}
*/

void Upgrade::upgradeTo_0_3_2()
{
	QString sqls;
	QSqlQuery query(db());
	
	QString tableName = "Flights_";
	tableName += getenv("USER");
	
	sqls.sprintf("RENAME TABLE `Flights` TO `%s`", tableName.ascii());
	query.exec(sqls);
	
	setTableVersion(m_TableVersion_0_3_2);
}
