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

const QString Upgrade::m_TableVersion_0_3_2 = "2005.06.12 20:00:00"; // equal version_0_3_2
const QString Upgrade::m_TableVersion_0_3_3 = "2006.03.31 20:50:00"; // equal version_0_3_3

Upgrade::Upgrade(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

int Upgrade::tableVersion()
{
	QString sqls;
	QString date;
	QSqlQuery query(db());
	int time = 1;
	
	sqls.sprintf("SELECT * FROM `LastModified` WHERE `Name` = 'TableVersion'");
	
	if(query.exec(sqls) && query.first())
	{
		time = query.value(1).toDateTime().toTime_t();
	}
	
	return time;
}

void Upgrade::setTableVersion(const QString &tabVers)
{
	QString sqls;
	QSqlQuery query(db());
	
	if(tableVersion() > 1)
	{
		sqls.sprintf("UPDATE `LastModified` SET `Time` = '%s' WHERE `Name` = 'TableVersion'", 
				tabVers.ascii());
		query.exec(sqls);
	}
	else
	{
		sqls.sprintf("INSERT INTO `LastModified` (`Name`, `Time`) VALUES ('TableVersion', '%s')",
				tabVers.ascii());
		query.exec(sqls);
	}
}

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

void Upgrade::upgradeFrom_0_3_1_to_0_3_2()
{
	QString sqls;
	QSqlQuery query(db());
	
	QString tableName = "Flights_";
	tableName += getenv("USER");
	
	sqls.sprintf("RENAME TABLE `Flights` TO `%s`", tableName.ascii());
	query.exec(sqls);
	
	setTableVersion(m_TableVersion_0_3_2);
}

void Upgrade::upgradeFrom_0_3_2_to_0_3_3()
{
}
