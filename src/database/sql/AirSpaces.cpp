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
  QString sqls;
  QString name;
  LatLngList::iterator it;
  int id;
  bool success;

  id = newId("AirSpaces");
  name = findUniqueName(airspace.name());
  sqls = QString("INSERT INTO AirSpaces(Id, Name, Class, Lower, Upper, Comment) "
                 "VALUES(%1, '%2', '%3', %4, %5, '%6');")
                 .arg(id).arg(name).arg(escape(airspace.airspaceClass()))
                 .arg(airspace.low()).arg(airspace.high()).arg(escape(airspace.remark()));
  success = query.exec(sqls);

  if(airspace.id() == -1)
  {
    airspace.setId(id);
  }

  if(success)
  {
    for(it=airspace.pointList().begin(); it!=airspace.pointList().end(); it++)
    {
      sqls = QString("INSERT INTO AirSpaceItems(AirSpaceId, Longitude, Latitude) "
                      "VALUES(%1, %2, %3);")
                      .arg(id).arg((*it).lon()).arg((*it).lat());
      success &= query.exec(sqls);
    }
  }

	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("AirSpaces");

	return success;
}

bool AirSpaces::updateAirSpace(const AirSpace &airspace)
{
  QSqlQuery query(db());
  QString sqls;
	bool success;

  sqls = QString("UPDATE AirSpaces SET Name='%1', Class='%2', Lower=%3, Upper=%4,"
                 "Comment='%5' WHERE ID=%6;")
                  .arg(escape(airspace.name())).arg(escape(airspace.airspaceClass()))
                  .arg(airspace.low()).arg(airspace.high()).arg(escape(airspace.remark()))
                 .arg(airspace.id());

	success = query.exec(sqls);
	DataBaseSub::setLastModified("AirSpaces");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool AirSpaces::delAirSpace(const AirSpace &airspace)
{
  QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls = QString("DELETE FROM AirSpaceItems WHERE AirSpaceId=%1;").arg(airspace.id());
	success = query.exec(sqls);
  sqls = QString("DELETE FROM AirSpaces WHERE Id=%1;").arg(airspace.id());
  success &= query.exec(sqls);

	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("AirSpaces");

	return success;
}

bool AirSpaces::airspace(const QString &name, AirSpace &airspace)
{
  QSqlQuery query(db());
	QString sqls;
	bool success = false;

	sqls = QString("SELECT Id, Name, Class, Lower, Upper, Comment FROM AirSpaces WHERE Name='%1';")
                  .arg(escape(name));
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
	QString sqls;
	LatLng latlng;
	bool success = false;

  sqls = QString("SELECT Latitude, Longitude FROM AirSpaceItems WHERE AirSpaceId=%1;")
              .arg(id);
  success = query.exec(sqls);
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
	QString sqls;
	QString locName;
	QString newName;
	QString number;
	int count = 2;
	bool found;

	sqls = QString("SELECT Id FROM AirSpaces WHERE Name='%1';");
	locName = escape(name);
	newName = locName.left(NameSize);
	query.exec(sqls.arg(newName));
	found = query.next();

  while(found)
  {
    number = QString("~%1").arg(count);
    newName = (locName.left(NameSize - number.size()) + number);
    query.exec(sqls.arg(newName));
    found = query.next();
    count++;
  }

  return newName;
}
