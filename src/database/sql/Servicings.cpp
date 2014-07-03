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
	int id;
	bool success;

  id = newId("Servicings");
  success = query.prepare("INSERT INTO Servicings (Id, GliderId, Date, Responsibility, Comment) "
                          "VALUES(:id, :glider, :date, :responsibility, :comment)");
  query.bindValue(":id", id);
  query.bindValue(":glider", servicing.glider().id());
  query.bindValue(":date", servicing.date().toString("yyyy-MM-dd"));
  query.bindValue(":responsibility", servicing.responsibility());
  query.bindValue(":comment", servicing.comment());
  success &= query.exec();

	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Servicings");

	if(servicing.id() == -1)
	{
	  servicing.setId(id);
	}

	return success;
}

bool Servicings::update(Servicing &servicing)
{
  QSqlQuery query(db());
	bool success;

  success = query.prepare("UPDATE Servicings SET GliderId=:glider, Date=:date, Responsibility=:responsibility, "
                          "Comment=:comment WHERE Id=:id");
  query.bindValue(":glider", servicing.glider().id());
  query.bindValue(":date", servicing.date().toString("yyyy-MM-dd"));
  query.bindValue(":responsibility", servicing.responsibility());
  query.bindValue(":comment", servicing.comment());
  query.bindValue(":id", servicing.id());
  success &= query.exec();

  DataBaseSub::setLastModified("Servicings");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Servicings::delServicing(Servicing &servicing)
{
	QSqlQuery query(db());
	bool success;

  success = query.prepare("DELETE FROM Servicings WHERE Id=:id");
  query.bindValue(":id", servicing.id());
  success &= query.exec();

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
