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
#include "AirSpace.h"
#include "AirSpaceList.h"
#include "AirSpaces.h"
#include "LatLng.h"

AirSpaces::AirSpaces(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool AirSpaces::add(AirSpace &airspace)
{
  QSqlQuery query(db());
  QString name;
  QString sqls;
  LatLngList::iterator it;
  int id;
  bool success;
  bool first = true;

  id = newId("AirSpaces");
  name = findUniqueName(airspace.name());

  success = query.prepare("INSERT INTO AirSpaces(Id, Name, Class, Lower, Upper, Comment) VALUES(:id, :name, :class, :lower, :upper, :comment)");
  query.bindValue(":id",id);
  query.bindValue(":name",name);
  query.bindValue(":class",airspace.airspaceClass());
  query.bindValue(":lower",airspace.low());
  query.bindValue(":upper",airspace.high());
  query.bindValue(":comment",airspace.remark());
  success &= query.exec();

  if(airspace.id() == -1)
  {
    airspace.setId(id);
  }

  if(success)
  {
    sqls = "INSERT INTO AirSpaceItems(AirSpaceId, Longitude, Latitude) VALUES";

    for(it=airspace.pointList().begin(); it!=airspace.pointList().end(); it++)
    {
      if(!first)
      {
        sqls += ",";
      }

      first = false;
      sqls += QString("(%1, %2, %3)").arg(id).arg((*it).lon()).arg((*it).lat());
    }

    sqls += ";";
    success &= query.exec(sqls);
  }

	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("AirSpaces");

	return success;
}

bool AirSpaces::updateAirSpace(const AirSpace &airspace)
{
  QSqlQuery query(db());
	bool success;

  success = query.prepare("UPDATE AirSpaces SET Name=:name, Class=:class, Lower=:lower, Upper=:upper, Comment=:comment WHERE ID=:id");
  query.bindValue(":name",airspace.name());
  query.bindValue(":class",airspace.airspaceClass());
  query.bindValue(":lower",airspace.low());
  query.bindValue(":upper",airspace.high());
  query.bindValue(":comment",airspace.remark());
  query.bindValue(":id",airspace.id());
  success &= query.exec();

	DataBaseSub::setLastModified("AirSpaces");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool AirSpaces::delAirSpace(const AirSpace &airspace)
{
  QSqlQuery query(db());
	bool success;

  success = query.prepare("DELETE FROM AirSpaceItems WHERE AirSpaceId=:id");
  query.bindValue(":id", airspace.id());
  success &= query.exec();
  success &= query.prepare("DELETE FROM AirSpaces WHERE Id=:id");
  query.bindValue(":id", airspace.id());
  success &= query.exec();

	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("AirSpaces");

	return success;
}

bool AirSpaces::airspace(const QString &name, AirSpace &airspace)
{
  QSqlQuery query(db());
	bool success = false;

  success = query.prepare("SELECT Id, Name, Class, Lower, Upper, Comment FROM AirSpaces WHERE Name=:name");
  query.bindValue(":name", name);
  success &= query.exec();

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
		  success = airSpaceItems(airspace.id(), airspace.pointList());
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

	sqls = QString("SELECT Id, Name, Class, Lower, Upper, Comment FROM AirSpaces ORDER BY Name;");
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
		  success = airSpaceItems(pAirSpace->id(), pAirSpace->pointList());
		  airspaceList.push_back(pAirSpace);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool AirSpaces::checkModified()
{
  return DataBaseSub::checkModified("AirSpaces");
}

bool AirSpaces::airSpaceItems(int id, LatLngList &itemList)
{
  QSqlQuery query(db());
	LatLng latlng;
	bool success = false;

  success = query.prepare("SELECT Latitude, Longitude FROM AirSpaceItems WHERE AirSpaceId=:id");
  query.bindValue(":id", id);
  success &= query.exec();

  itemList.clear();

  while(query.next())
  {
    latlng.setPos(query.value(0).toDouble(), query.value(1).toDouble());
    itemList.push_back(latlng);
  }

  return success;
}

QString AirSpaces::findUniqueName(const QString &name)
{
  QSqlQuery query(db());
	QString newName;
	QString number;
	int count = 2;
	bool found;

  newName = name.left(NameSize);
  query.prepare("SELECT Id FROM AirSpaces WHERE Name=:name");
  query.bindValue(":name", newName);
  query.exec();
	found = query.next();

  while(found)
  {
    number = QString("~%1").arg(count);
    newName = (name.left(NameSize - number.size()) + number);
    query.bindValue(":name", newName);
    query.exec();
    found = query.next();
    count++;
  }

  return newName;
}
