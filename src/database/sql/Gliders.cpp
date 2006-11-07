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

#include <qsqlcursor.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include "Error.h"
#include "Flights.h"
#include "Gliders.h"
#include "ISql.h"

Gliders::Gliders(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

bool Gliders::add(Glider &glider)
{
	QSqlCursor cur("Gliders");
	QSqlRecord *pRec;

	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Manufacturer", glider.manufacturer());
	pRec->setValue("Model", glider.model());
	pRec->setValue("Serial", glider.serial());
	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	setGliderId(glider);
	DataBaseSub::setLastModified("Gliders");

	return true;
}

bool Gliders::delGlider(Glider &glider)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	 
	sqls.sprintf("DELETE FROM `Gliders` WHERE `Id` = '%i'", glider.id());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("Gliders");
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool Gliders::glider(const QString &modelOfGlider, Glider &glider)
{
	QSqlQuery query(db());
	QString sqls;
	QString dbModel;
	bool success;
	
	sqls.sprintf("SELECT * FROM `Gliders`");
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			dbModel = query.value(Manufacturer).toString() + " " + query.value(Model).toString();
			
			if(dbModel == modelOfGlider)
			{
				glider.setId(query.value(Id).toInt());
				glider.setManufacturer(query.value(Manufacturer).toString());
				glider.setModel(query.value(Model).toString());
				glider.setSerial(query.value(Serial).toString());
			}
		}
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}
	
	return success;
}

bool Gliders::gliderList(Glider::GliderListType &gliderList)
{
	Glider glider;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM `Gliders` ORDER BY `Manufacturer`, `Model` ASC";
	bool success;
	
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			glider.setId(query.value(Id).toInt());
			glider.setManufacturer(query.value(Manufacturer).toString());
			glider.setModel(query.value(Model).toString());
			glider.setSerial(query.value(Serial).toString());
			ISql::pInstance()->pFlightTable()->setFlightStatistic(glider);
			gliderList.push_back(glider);
		}
	}
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool Gliders::glider(int id, Glider &glider)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	
	sqls.sprintf("SELECT * FROM `Gliders` WHERE `Id` = %i", id);
	success = (query.exec(sqls) && query.first());
	
	if(success)
	{
		glider.setId(query.value(Id).toInt());
		glider.setManufacturer(query.value(Manufacturer).toString());
		glider.setModel(query.value(Model).toString());
		glider.setSerial(query.value(Serial).toString());
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}
	
	return success;
}

bool Gliders::setGliderId(Glider &glider)
{
	QSqlQuery query(db());
	QString sqls;
	QString dbModel;
	bool success;
	int id = -1;

	sqls.sprintf("SELECT * FROM `Gliders` WHERE "
		"`Manufacturer` = '%s' AND "
		"`Model` = '%s' AND "
		"`Serial` = '%s'",
		glider.manufacturer().ascii(), glider.model().ascii(), glider.serial().ascii());

	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(Id).toInt();
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	glider.setId(id);
	
	return success;
}
