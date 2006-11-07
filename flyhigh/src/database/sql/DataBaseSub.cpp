/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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
#include "DataBaseSub.h"

DataBaseSub::DataBaseSub(QSqlDatabase *pDB)
{
	m_pDB = pDB;
}

DataBaseSub::~DataBaseSub()
{
}

QSqlDatabase* DataBaseSub::db()
{
	return m_pDB;
}

int DataBaseSub::newId(const QString &table)
{
	QString sqls = "SELECT MAX(Id) FROM " + table;
	QSqlQuery query(db());
	int newid = -1;
	
	if(query.exec(sqls) &&
		query.first())
	{
		newid = query.value(0).toInt() + 1;
	}
	
	return newid;
}

void DataBaseSub::setLastModified(const QString &field)
{
	QString sqls;
	QString date;
	QSqlQuery query(db());
	
	date = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
	
	if(lastModified(field) > 1)
	{
		sqls.sprintf("UPDATE `LastModified` SET `Time` = '%s' WHERE `Name` = '%s'", 
										date.ascii(), field.ascii());
		query.exec(sqls);
	}
	else
	{
		sqls.sprintf("INSERT INTO `LastModified` (`Name`, `Time`) VALUES ('%s', '%s')", 
										field.ascii(), date.ascii());
		query.exec(sqls);
	}
}

int DataBaseSub::lastModified(const QString &field)
{
	QString sqls;
	QString date;
	QSqlQuery query(db());
	int time = 1;
	
	sqls.sprintf("SELECT `Time` FROM `LastModified` WHERE `Name` = '%s'", field.ascii());
	
	if(query.exec(sqls) && query.first())
	{
		time = query.value(0).toDateTime().toTime_t();
	}
	
	return time;
}
