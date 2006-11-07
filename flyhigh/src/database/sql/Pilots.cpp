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
#include "Error.h"
#include "ISql.h"
#include "Gliders.h"
#include "Pilots.h"

Pilots::Pilots(QSqlDatabase *pDB)
 :DataBaseSub(pDB)
{
}

Pilots::~Pilots()
{
}

bool Pilots::add(Pilot &pilot)
{
	QSqlCursor cur("Pilots");
	QSqlRecord *pRec;
	QSqlQuery query(db());
	bool success;

	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("FirstName", pilot.firstName());
	pRec->setValue("LastName", pilot.lastName());
	pRec->setValue("BirthDate", pilot.birthDate());
	pRec->setValue("CallSign", pilot.callSign());
	pRec->setValue("GliderId", pilot.glider().id());

	success = (cur.insert() == 1);
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

	sqls.sprintf("UPDATE `Pilots` SET `CallSign` = '%s', `GliderId` = %i WHERE `Id` = %i",
		pilot.callSign().ascii(), pilot.glider().id(), pilot.id());
	success = query.exec(sqls);
	
	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Pilots");

	return success;
}

bool Pilots::pilot(int id, Pilot &pilot)
{
	QSqlQuery query(db());
	QString sqls;
	QString dbModel;
	Glider glider;
	bool success;

	sqls.sprintf("SELECT * FROM `Pilots` WHERE `Id` = %i ", id);
	success = (query.exec(sqls) && query.first());

	if(success)
	{
		pilot.setId(query.value(Id).toInt());
		pilot.setFirstName(query.value(FirstName).toString());
		pilot.setLastName(query.value(LastName).toString());
		pilot.setBirthDate(query.value(BirthDate).toDate());
		pilot.setCallSign(query.value(CallSign).toString());
		ISql::pInstance()->pGliderTable()->glider(query.value(GliderId).toInt(), glider);
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
	QString dbModel;
	bool success;
	int id = -1;

	sqls.sprintf("SELECT * FROM `Pilots` WHERE "
		"`FirstName` = '%s' AND "
		"`LastName` = '%s' AND "
		"`BirthDate` = '%s'",
		pilot.firstName().ascii(), pilot.lastName().ascii(), pilot.birthDate().toString("yyyy-MM-dd").ascii());

	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(Id).toInt();
	}

	pilot.setId(id);
	
	return success;
}
