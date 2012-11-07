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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "Error.h"
#include "AirSpace.h"
#include "AirSpaceList.h"
#include "AirSpaceItem.h"
#include "AirSpaceItemList.h"
#include "AirSpaces.h"

AirSpaces::AirSpaces(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool AirSpaces::add(AirSpace &airspace)
{
  QSqlQuery query(db());
  QString sqls;
  bool success;

  sqls = QString("INSERT INTO AirSpaces(Name, Class, Lower, Upper, Comment) "
                 "VALUES('%1', '%2', %3, %4, '%5');")
                 .arg(airspace.name()).arg(airspace.airspaceClass()).arg(airspace.low())
                 .arg(airspace.high()).arg(airspace.remark());

  success = query.exec(sqls);
	Error::verify(success, Error::SQL_CMD);
	setAirSpaceId(airspace);
	DataBaseSub::setLastModified("AirSpaces");

	return success;
}

bool AirSpaces::delAirSpace(const AirSpace &airspace)
{
  QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls = QString("DELETE FROM AirSpaces WHERE Id='%1';").arg(airspace.id());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("AirSpaces");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool AirSpaces::airspace(const QString &name, AirSpace &airspace)
{
  QSqlQuery query(db());
	QString sqls;
	bool success = false;

	sqls = QString("SELECT Id, Name, Class, Lower, Upper, Comment FROM AirSpaces WHERE Name='%1';")
                  .arg(name);
  success = query.exec(sqls);

	if(success)
	{
		if(query.next())
		{
		  airspace.setId(query.value(0).toInt());
		  airspace.setName(query.value(1).toString());
		  airspace.setAirspaceClass(query.value(2).toString());
		  airspace.setLow(query.value(3).toInt());
		  airspace.setHigh(query.value(4).toInt());
		  airspace.setRemark(query.value(5).toString());
		  success = airSpaceItems(airspace.id(), airspace.airSpaceItemList());
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool AirSpaces::airspaceList(AirSpaceList &airspaceList)
{
  QSqlQuery query(db());
	QString sqls;
	AirSpace *pAirSpace;
	bool success = false;

	sqls = QString("SELECT Id, Name, Class, Lower, Upper, Comment FROM AirSpaces;");
  success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
		  pAirSpace = new AirSpace();
		  pAirSpace->setId(query.value(0).toInt());
		  pAirSpace->setName(query.value(1).toString());
		  pAirSpace->setAirspaceClass(query.value(2).toString());
		  pAirSpace->setLow(query.value(3).toInt());
		  pAirSpace->setHigh(query.value(4).toInt());
		  pAirSpace->setRemark(query.value(5).toString());
		  success = airSpaceItems(pAirSpace->id(), pAirSpace->airSpaceItemList());
		  airspaceList.push_back(pAirSpace);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool AirSpaces::setAirSpaceId(AirSpace &airspace)
{
  QSqlQuery query(db());
	QString sqls;
	QString dbModel;
	bool success;
	int id = -1;

  sqls = QString("SELECT Id FROM AirSpaces WHERE Name='%1'").arg(airspace.name());
	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	airspace.setId(id);

	return success;
}

bool AirSpaces::checkModified()
{
  return DataBaseSub::checkModified("AirSpaces");
}

bool AirSpaces::airSpaceItems(int id, AirSpaceItemList &itemList)
{
  QSqlQuery query(db());
	QString sqls;
	AirSpaceItem *pItem;
	bool success = false;

  sqls = QString("SELECT Latitude, Longitude FROM AirSpaceItems WHERE AirSpaceId=%1;")
              .arg(id);
  success = query.exec(sqls);
  itemList.clear();

  while(query.next())
  {
    pItem = new AirSpaceItem();
    pItem->setPoint(query.value(0).toDouble(), query.value(1).toDouble());
    itemList.push_back(pItem);
  }

  return success;
}
