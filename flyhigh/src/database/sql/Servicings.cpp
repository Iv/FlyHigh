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
#include "Servicings.h"

Servicings::Servicings(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

bool Servicings::add(Servicing &servicing)
{
	QSqlCursor cur("Servicings");
	QSqlRecord *pRec;

	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Id", newId("Servicings"));
	pRec->setValue("Glider", servicing.glider());
	pRec->setValue("Date", servicing.date());
	pRec->setValue("Responsibility", servicing.responsibility());
	pRec->setValue("Comment", servicing.comment());
	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	DataBaseSub::setLastModified("Servicings");

	return true;
}

bool Servicings::delServicing(int nr)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	 
	sqls.sprintf("DELETE FROM `Servicings` WHERE `Id` = '%i'", nr);
	success = query.exec(sqls);
	DataBaseSub::setLastModified("Servicings");
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool Servicings::servicingList(Servicing::ServicingListType &servicingList)
{
	Servicing serv;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM `Servicings` ORDER BY `Id` DESC";
	bool success;
	
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			serv.setNr(query.value(Id).toInt());
			serv.setGlider(query.value(Glider).toString());
			serv.setDate(query.value(Date).toDate());
			serv.setResponsibility(query.value(Responsibility).toString());
			serv.setComment(query.value(Comment).toString());
			
			servicingList.push_back(serv);
		}
	}
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}
