/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <QDateTime>
#include <QSqlQuery>
#include "DataBaseSub.h"

DataBaseSub::DataBaseSub(QSqlDatabase db)
{
	m_DB = db;
	m_lastModified = -1;
}

DataBaseSub::~DataBaseSub()
{
}

bool DataBaseSub::checkModified()
{
	return false;
}

bool DataBaseSub::checkModified(const QString &field)
{
  int dbLastMod;
  bool modified = false;

  dbLastMod = lastModified(field);

  if(m_lastModified != -1)
  {
    modified = (dbLastMod != m_lastModified);
    m_lastModified = dbLastMod;
  }
  else
  {
    m_lastModified = dbLastMod;
  }

  return modified;
}

QSqlDatabase DataBaseSub::db()
{
	return m_DB;
}

int DataBaseSub::newId(const QString &table)
{
	QString sqls = "SELECT MAX(Id) FROM " + table;
  QSqlQuery query(m_DB);
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
	QSqlQuery query(m_DB);
	QDateTime curTime;

  curTime = QDateTime::currentDateTime();
	date = curTime.toString("yyyy.MM.dd hh:mm:ss");
	m_lastModified = curTime.toTime_t();

	if(lastModified(field) > 1)
	{
    sqls = QString("UPDATE LastModified SET Time = '%1' WHERE Name = '%2'").arg(date).arg(field);
		query.exec(sqls);
	}
	else
	{
    sqls = QString("INSERT INTO LastModified (Name, Time) VALUES ('%1', '%2')").arg(field).arg(date);
		query.exec(sqls);
	}
}

int DataBaseSub::lastModified(const QString &field)
{
	QString sqls;
	QString date;
  QSqlQuery query(m_DB);
	int time = 1;

  sqls = QString("SELECT Time FROM LastModified WHERE Name = '%1'").arg(field);

	if(query.exec(sqls) && query.first())
	{
		time = query.value(0).toDateTime().toTime_t();
	}

	return time;
}
