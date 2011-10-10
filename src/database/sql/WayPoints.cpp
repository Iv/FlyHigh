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
#include "WayPoints.h"

WayPoints::WayPoints(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool WayPoints::add(WayPoint &wp)
{
  QSqlQuery query(db());
  QString sqls;
	bool success;

  sqls = QString("INSERT INTO WayPoints(Id, Name, Spot, Country, Longitude,"
                                        "Latitude, Altitude, Description)"
                 "VALUES (NULL, '%1', '%2', '%3', %4, %5, %6, '%7');")
              .arg(wp.name()).arg(wp.spot()).arg(wp.country()).arg(wp.longitude())
              .arg(wp.latitude()).arg(wp.altitude()).arg(wp.description());

	success = query.exec(sqls);
  Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("WayPoints");
	setId(wp);

	return success;
}

bool WayPoints::update(WayPoint &wp)
{
  QSqlQuery query(db());
  QString sqls;
	bool success;

  sqls = QString("UPDATE WayPoints SET Name='%1',Country='%2',Spot='%3',Description='%4',"
          "Longitude=%5,Latitude=%6,Altitude=%7 WHERE Id=%8;")
          .arg(wp.name()).arg(wp.country()).arg(wp.spot()).arg(wp.description())
          .arg(wp.longitude()).arg(wp.latitude()).arg(wp.altitude()).arg(wp.id());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("WayPoints");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool WayPoints::delWayPoint(WayPoint &wp)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls = QString("DELETE FROM WayPoints WHERE Id = %1;").arg(wp.id());
	success = query.exec(sqls);
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
	QString sqls;
	double lon;
	double lat;
	int alt;
	bool success;

	sqls = QString("SELECT * FROM WayPoints WHERE Id = %1;").arg(id);
	success = (query.exec(sqls) && query.first());

	if(success)
	{
		wp.setId(id);
		wp.setName(query.value(Name).toString());
		wp.setSpot(query.value(Spot).toString());
		wp.setCountry(query.value(Country).toString());
		wp.setDescription(query.value(Description).toString());
		lon = query.value(Longitude).toDouble();
		lat = query.value(Latitude).toDouble();
		alt = query.value(Altitude).toInt();
		wp.setCoordinates(lat, lon, alt);
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
	QString sqls = "SELECT * FROM WayPoints;";
	WayPoint locWp;
	double lat;
	double lon;
	uint dist;
	bool success;
	bool found = false;

	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
			lat = query.value(Latitude).toDouble();
			lon = query.value(Longitude).toDouble();
			locWp.setCoordinates(lat, lon, 0);
			dist = wp.distance(locWp);
			found = (dist <= radius);

			if(found)
			{
				wp.setId(query.value(Id).toInt());
				wp.setName(query.value(Name).toString());
				wp.setSpot(query.value(Spot).toString());
				wp.setCountry(query.value(Country).toString());
				wp.setDescription(query.value(Description).toString());
				wp.setCoordinates(lat, lon, query.value(Altitude).toInt());
				break;
			}
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return found;
}

bool WayPoints::wayPointList(WayPoint::WayPointListType &wpList)
{
	WayPoint wp;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM WayPoints ORDER BY Country, Name, Spot ASC;";
	double lon;
	double lat;
	int alt;
	bool success;

	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
      wp.setId(query.value(Id).toInt());
      wp.setName(query.value(Name).toString());
      wp.setSpot(query.value(Spot).toString());
      wp.setCountry(query.value(Country).toString());
      wp.setDescription(query.value(Description).toString());
      lon = query.value(Longitude).toDouble();
      lat = query.value(Latitude).toDouble();
      alt = query.value(Altitude).toInt();
			wp.setCoordinates(lat, lon, alt);
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
	QString sqls;
	bool success;
	int id = -1;

  sqls = QString("SELECT Id FROM WayPoints WHERE "
          "Name = '%1' AND "
          "Spot = '%2' AND "
          "Country = '%3'").arg(wp.name(),wp.spot(),wp.country());

	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}

	wp.setId(id);

	return id;
}
