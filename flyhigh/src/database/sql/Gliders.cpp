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
#include "Gliders.h" 

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
	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	
	DataBaseSub::setLastModified("'Gliders'");

	return true;
}

bool Gliders::glider(const QString &model, Glider &glider)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	
	sqls.sprintf("SELECT * FROM `Gliders` WHERE `Model` = '%s'", model.ascii());
	success = query.exec(sqls);
	
	if(success)
	{
		success = query.first();
		
		if(success)
		{
			glider.setManufacturer(query.value(Manufacturer).toString());
			glider.setModel(query.value(Model).toString());
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
	QString sqls = "SELECT * FROM Gliders ORDER BY Model ASC";
	bool success;
	
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			glider.setManufacturer(query.value(Manufacturer).toString());
			glider.setModel(query.value(Model).toString());

			gliderList.push_back(glider);
		}
	}
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}
