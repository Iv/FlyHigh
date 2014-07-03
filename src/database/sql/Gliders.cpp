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

#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "Error.h"
#include "Flights.h"
#include "Gliders.h"
#include "ISql.h"

Gliders::Gliders(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool Gliders::add(Glider &glider)
{
  QSqlQuery query(db());
  bool success;

  success = query.prepare("INSERT INTO Gliders (Manufacturer, Model, Serial) "
                          "VALUES(:manufacturer, :model, :serial)");
  query.bindValue(":manufacturer", glider.manufacturer());
  query.bindValue(":model", glider.model());
  query.bindValue(":serial", glider.serial());
  success &= query.exec();

	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Gliders");

	return true;
}

bool Gliders::update(Glider &glider)
{
  QSqlQuery query(db());
	bool success;

  success = query.prepare("UPDATE Gliders SET Manufacturer=:manufacturer, Model=:model, Serial=:serial "
                          "WHERE Id=:id");
  query.bindValue(":manufacturer", glider.manufacturer());
  query.bindValue(":model", glider.model());
  query.bindValue(":serial", glider.serial());
  query.bindValue(":id", glider.id());
  success &= query.exec();

	DataBaseSub::setLastModified("Gliders");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Gliders::delGlider(Glider &glider)
{
  QSqlQuery query(db());
	bool success;

  success = query.prepare("DELETE FROM Gliders WHERE Id=:id");
  query.bindValue(":id", glider.id());
  success &= query.exec();

	DataBaseSub::setLastModified("Gliders");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Gliders::glider(const QString &modelOfGlider, Glider &glider)
{
  QSqlQuery query(db());
	QString sqls;
	QString dbModel;
	bool success = false;

	sqls = "SELECT Id, Manufacturer, Model, Serial FROM Gliders;";

	if(query.exec(sqls))
	{
		while(query.next())
		{
			dbModel = query.value(2).toString();
			success = (dbModel == modelOfGlider);

			if(success)
			{
				glider.setId(query.value(0).toInt());
				glider.setManufacturer(query.value(1).toString());
				glider.setModel(dbModel);
				glider.setSerial(query.value(3).toString());
				break;
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
	QString sqls = "SELECT Id, Manufacturer, Model, Serial FROM Gliders "
                  "ORDER BY Manufacturer, Model ASC";
	bool success;

	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
			glider.setId(query.value(0).toInt());
			glider.setManufacturer(query.value(1).toString());
			glider.setModel(query.value(2).toString());
			glider.setSerial(query.value(3).toString());
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
	bool success;

  success = query.prepare("SELECT Manufacturer, Model, Serial FROM Gliders WHERE Id=:id");
  query.bindValue(":id", id);
  success &= (query.exec() && query.first());

	if(success)
	{
		glider.setId(id);
		glider.setManufacturer(query.value(0).toString());
		glider.setModel(query.value(1).toString());
		glider.setSerial(query.value(2).toString());
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
	bool success;
	int id = -1;

  success = query.prepare("SELECT Id FROM Gliders "
                          "WHERE Manufacturer=:manufacturer AND Model=:model AND Serial=:serial");
  query.bindValue(":manufacturer", glider.manufacturer());
  query.bindValue(":model", glider.model());
  query.bindValue(":serial", glider.serial());

  success &= (query.exec() && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	glider.setId(id);

	return success;
}

bool Gliders::checkModified()
{
  return DataBaseSub::checkModified("Gliders");
}
