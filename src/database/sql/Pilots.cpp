/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "Error.h"
#include "ISql.h"
#include "Gliders.h"
#include "Pilots.h"

Pilots::Pilots(QSqlDatabase DB)
 :DataBaseSub(DB)
{
}

Pilots::~Pilots()
{
}

bool Pilots::add(Pilot &pilot)
{
  QSqlQuery query(db());
  QString sqls;
	bool success;

  sqls = QString("INSERT INTO Pilots (FirstName, LastName, BirthDate, CallSign, GliderId) "
                 "VALUES('%1', '%2', '%3', '%4', %5);")
                 .arg(escape(pilot.firstName())).arg(escape(pilot.lastName()))
                 .arg(pilot.birthDate().toString("yyyy-MM-dd"))
                 .arg(escape(pilot.callSign()))
                 .arg(pilot.glider().id());

  success = query.exec(sqls);
	Error::verify(success, Error::SQL_CMD);
	setId(pilot);
	DataBaseSub::setLastModified("Pilots");

	return success;
}

bool Pilots::update(Pilot &pilot)
{
  QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls = QString("UPDATE Pilots SET FirstName='%1', LastName='%2', BirthDate='%3', "
                 "CallSign='%4', GliderId=%5 WHERE Id=%6;")
                .arg(escape(pilot.firstName())).arg(escape(pilot.lastName()))
                .arg(pilot.birthDate().toString("yyyy-MM-dd"))
                .arg(escape(pilot.callSign()))
                .arg(pilot.glider().id())
                .arg(pilot.id());

	success = query.exec(sqls);
	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Pilots");

	return success;
}

bool Pilots::pilot(int id, Pilot &pilot)
{
	QSqlQuery query(db());
	QString sqls;
	Glider glider;
	bool success;

	sqls = QString("SELECT Id, FirstName, LastName, BirthDate, CallSign, GliderId "
                  "FROM Pilots WHERE Id=%1;").arg(id);
	success = (query.exec(sqls) && query.first());

	if(success)
	{
		pilot.setId(query.value(0).toInt());
		pilot.setFirstName(query.value(1).toString());
		pilot.setLastName(query.value(2).toString());
		pilot.setBirthDate(query.value(3).toDate());
		pilot.setCallSign(query.value(4).toString());
		ISql::pInstance()->pGliderTable()->glider(query.value(5).toInt(), glider);
		pilot.setGlider(glider);
	}
	else
	{
		Error::verify(success, Error::SQL_NO_PILOT_INFO);
	}

	return success;
}

bool Pilots::setId(Pilot &pilot)
{
  QSqlQuery query(db());
	QString sqls;
	bool success;
	int id = -1;

  sqls = QString("SELECT Id FROM Pilots WHERE FirstName='%1' AND LastName='%2' "
                 "AND BirthDate='%3'")
                  .arg(escape(pilot.firstName())).arg(escape(pilot.lastName()))
                  .arg(pilot.birthDate().toString("yyyy-MM-dd"));

	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}

	pilot.setId(id);

	return success;
}

bool Pilots::checkModified()
{
  return DataBaseSub::checkModified("Pilots");
}
