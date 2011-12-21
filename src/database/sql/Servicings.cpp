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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
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
	QSqlQuery query(db());
	QString sqls;
	bool success;

  sqls = QString("INSERT INTO Servicings (Id, GliderId, Date, Responsibility, Comment) "
                 "VALUES(%1, %2, '%3', '%4', '%5');")
                 .arg(newId("Servicings")).arg(servicing.glider().id())
                 .arg(servicing.date().toString("yyyy-MM-dd"))
                 .arg(servicing.responsibility()).arg(servicing.comment());

  success = query.exec(sqls);
	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Servicings");

	return success;
}

bool Servicings::delServicing(Servicing &servicing)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls = QString("DELETE FROM Servicings WHERE Id=%1").arg(servicing.id());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("Servicings");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Servicings::servicingList(Servicing::ServicingListType &servicingList)
{
	Servicing servicing;
	Glider glider;
	QSqlQuery query(db());
	QString sqls;
	bool success;

  sqls = "SELECT Id, GliderId, Date, Responsibility, Comment FROM Servicings "
         "ORDER BY Id DESC";
	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
			servicing.setId(query.value(0).toInt());
			ISql::pInstance()->pGliderTable()->glider(query.value(1).toInt(), glider);
			servicing.setGlider(glider);
			servicing.setDate(query.value(2).toDate());
			servicing.setResponsibility(query.value(3).toString());
			servicing.setComment(query.value(4).toString());
			servicingList.push_back(servicing);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Servicings::checkModified()
{
  return DataBaseSub::checkModified("Servicings");
}
