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
#include "WayPoints.h"

WayPoints::WayPoints(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool WayPoints::add(WayPoint &wp)
{
  QSqlQuery query(db());
  int id;
	bool success;

  id = newId("WayPoints");
  wp.setId(id);

  success = query.prepare("INSERT INTO WayPoints(Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description, Type) "
                          "VALUES (:id, :name, :spot, :country, :lon, :lat, :altitude, :description, :type)");
  query.bindValue(":id", id);
  query.bindValue(":name", wp.name());
  query.bindValue(":spot", wp.spot());
  query.bindValue(":country", wp.country());
  query.bindValue(":lon", wp.lon());
  query.bindValue(":lat", wp.lat());
  query.bindValue(":altitude", wp.alt());
  query.bindValue(":description", wp.description());
  query.bindValue(":type", wp.type());
  success &= query.exec();

  Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("WayPoints");

	return success;
}

bool WayPoints::update(WayPoint &wp)
{
  QSqlQuery query(db());
	bool success;

  success = query.prepare("UPDATE WayPoints "
                          "SET Name=:name, Spot=:spot, Country=:country, Longitude=:lon, Latitude=:lat, Altitude=:altitude, Description=:description, Type=:type "
                          "WHERE Id=:id");
  query.bindValue(":name", wp.name());
  query.bindValue(":spot", wp.spot());
  query.bindValue(":country", wp.country());
  query.bindValue(":lon", wp.lon());
  query.bindValue(":lat", wp.lat());
  query.bindValue(":altitude", wp.alt());
  query.bindValue(":description", wp.description());
  query.bindValue(":type", wp.type());
  query.bindValue(":id", wp.id());
  success &= query.exec();

	DataBaseSub::setLastModified("WayPoints");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool WayPoints::delWayPoint(WayPoint &wp)
{
	QSqlQuery query(db());
	bool success;

  success = query.prepare("DELETE FROM WayPoints WHERE Id=:id");
  query.bindValue(":id", wp.id());
  success &= query.exec();

  DataBaseSub::setLastModified("WayPoints");
	Error::verify(success, Error::SQL_DEL);

	return success;
}

bool WayPoints::delAllWayPoints()
{
	QSqlQuery query(db());
	bool success;

	success = query.exec("DELETE * FROM WayPoints;");
	DataBaseSub::setLastModified("WayPoints");
	Error::verify(success, Error::SQL_DEL);

	return success;
}

bool WayPoints::wayPoint(int id, WayPoint &wp)
{
	QSqlQuery query(db());
	double lon;
	double lat;
	int alt;
	bool success;

  success = query.prepare("SELECT Name, Spot, Country, Longitude, Latitude, Altitude, Description, Type FROM WayPoints "
                          "WHERE Id=:id");
  query.bindValue(":id", id);
  success &= query.exec();

	if(success)
	{
	  success = query.first();

	  if(success)
	  {
      wp.setId(id);
      wp.setName(query.value(0).toString());
      wp.setSpot(query.value(1).toString());
      wp.setCountry(query.value(2).toString());
      lon = query.value(3).toDouble();
      lat = query.value(4).toDouble();
      alt = query.value(5).toInt();
      wp.setCoordinates(lat, lon, alt);
      wp.setDescription(query.value(6).toString());
      wp.setType((WayPoint::Type)query.value(7).toInt());
	  }
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	return success;
}

bool WayPoints::findWayPoint(WayPoint &wp, uint radius)
{
	QSqlQuery query(db());
	WayPoint locWp;
	double lat;
	double lon;
	uint dist;
	int id;
	bool success;
	bool found = false;

  success = query.prepare("SELECT Id, Longitude, Latitude, Name FROM WayPoints WHERE Type=:type");
  query.bindValue(":type", wp.type());
  success &= query.exec();

	if(success)
	{
		while(query.next())
		{
			lon = query.value(1).toDouble();
      lat = query.value(2).toDouble();
			locWp.setCoordinates(lat, lon, 0);
			dist = wp.distance(locWp);
			found = (dist <= radius);

			if(found)
			{
			  id = query.value(0).toInt();
        // we may reuse the query instance because we'll exit the while loop in every case
        success = query.prepare("SELECT Name, Spot, Country, Altitude, Description, Type "
                                "FROM WayPoints WHERE Id=:id");
        query.bindValue(":id", id);
        success &= (query.exec() && query.first());

        if(success)
        {
          wp.setId(id);
          wp.setName(query.value(0).toString());
          wp.setSpot(query.value(1).toString());
          wp.setCountry(query.value(2).toString());
          wp.setCoordinates(lat, lon, query.value(3).toInt());
          wp.setDescription(query.value(4).toString());
          wp.setType((WayPoint::Type)query.value(5).toInt());
        }

				break;
			}
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return found;
}

bool WayPoints::wayPointList(WayPoint::Type type, WayPoint::WayPointListType &wpList)
{
	WayPoint wp;
	QSqlQuery query(db());
	double lon;
	double lat;
	int alt;
	bool success;

  success = query.prepare("SELECT Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description, Type FROM WayPoints "
                          "WHERE Type=:type ORDER BY Country, Name, Spot ASC");
  query.bindValue(":type", type);
  success &= query.exec();

	if(success)
	{
		while(query.next())
		{
      wp.setId(query.value(0).toInt());
      wp.setName(query.value(1).toString());
      wp.setSpot(query.value(2).toString());
      wp.setCountry(query.value(3).toString());
      lon = query.value(4).toDouble();
      lat = query.value(5).toDouble();
      alt = query.value(6).toInt();
      wp.setCoordinates(lat, lon, alt);
      wp.setDescription(query.value(7).toString());
      wp.setType((WayPoint::Type)query.value(8).toInt());
			wpList.push_back(wp);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool WayPoints::checkModified()
{
  return DataBaseSub::checkModified("WayPoints");
}

bool WayPoints::setId(WayPoint &wp)
{
	QSqlQuery query(db());
	bool success;
	int id = -1;

  success = query.prepare("SELECT Id FROM WayPoints "
                          "WHERE Name=:name AND Spot=:spot AND Country=:country");
  query.bindValue(":name", wp.name());
  query.bindValue(":spot", wp.spot());
  query.bindValue(":country", wp.country());
  success &= (query.exec() && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}

	wp.setId(id);

	return id;
}
