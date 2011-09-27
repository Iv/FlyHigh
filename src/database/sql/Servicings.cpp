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

#include <q3sqlcursor.h>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "Error.h"
#include "Glider.h"
#include "Gliders.h"
#include "ISql.h"
#include "Servicings.h"

Servicings::Servicings(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool Servicings::add(Servicing &servicing)
{
	Q3SqlCursor cur("Servicings");
	QSqlRecord *pRec;

	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Id", newId("Servicings"));
	pRec->setValue("GliderId", servicing.glider().id());
	pRec->setValue("Date", servicing.date());
	pRec->setValue("Responsibility", servicing.responsibility());
	pRec->setValue("Comment", servicing.comment());
	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	DataBaseSub::setLastModified("Servicings");
	emit changed();

	return true;
}

bool Servicings::delServicing(Servicing &servicing)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls.sprintf("DELETE FROM Servicings WHERE Id = %i", servicing.id());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("Servicings");
	Error::verify(success, Error::SQL_CMD);
	emit changed();

	return success;
}

bool Servicings::servicingList(Servicing::ServicingListType &servicingList)
{
	Servicing servicing;
	Glider glider;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM Servicings ORDER BY Id DESC";
	bool success;

	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
			servicing.setId(query.value(Id).toInt());
			ISql::pInstance()->pGliderTable()->glider(query.value(GliderId).toInt(), glider);
			servicing.setGlider(glider);
			servicing.setDate(query.value(Date).toDate());
			servicing.setResponsibility(query.value(Responsibility).toString());
			servicing.setComment(query.value(Comment).toString());

			servicingList.push_back(servicing);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}
