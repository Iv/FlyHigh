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
  QString sqls;
	bool success;

  sqls = QString("INSERT INTO WayPoints(Name, Spot, Country, Longitude, "
                 "Latitude, Altitude, Description, Type) "
                 "VALUES ('%1', '%2', '%3', %4, %5, %6, '%7', %8);")
              .arg(escape(wp.name())).arg(escape(wp.spot())).arg(escape(wp.country()))
              .arg(wp.lon()).arg(wp.lat()).arg(wp.alt()).arg(escape(wp.description()))
              .arg(wp.type());

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

  sqls = QString("UPDATE WayPoints SET Name='%1', Country='%2', Spot='%3', Description='%4',"
                  "Longitude=%5, Latitude=%6, Altitude=%7, Type=%8 WHERE Id=%9;")
                  .arg(escape(wp.name())).arg(escape(wp.spot())).arg(escape(wp.country()))
                  .arg(wp.lon()).arg(wp.lat()).arg(wp.alt()).arg(escape(wp.description()))
                  .arg(wp.type())
                  .arg(wp.id());
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

	sqls = QString("DELETE FROM WayPoints WHERE Id=%1;").arg(wp.id());
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

	sqls = QString("SELECT Name, Spot, Country, Longitude, Latitude, Altitude,"
                 "Description, Type FROM WayPoints WHERE Id = %1;").arg(id);
	success = (query.exec(sqls) && query.first());

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
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	return success;
}

bool WayPoints::findWayPoint(WayPoint &wp, uint radius)
{
	QSqlQuery query(db());
	QString sqls;
	WayPoint locWp;
	double lat;
	double lon;
	uint dist;
	int id;
	bool success;
	bool found = false;

	sqls = QString("SELECT Id, Longitude, Latitude, Name FROM WayPoints WHERE Type=%1;")
                .arg(wp.type());
	success = query.exec(sqls);

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
        sqls = QString("SELECT Name, Spot, Country, Altitude, Description, Type "
                       "FROM WayPoints WHERE Id=%1;").arg(id);
        success = (query.exec(sqls) && query.first());

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
	QString sqls;
	double lon;
	double lat;
	int alt;
	bool success;

  sqls = QString("SELECT Id, Name, Spot, Country, Longitude, Latitude, Altitude, Description,"
                 "Type FROM WayPoints WHERE Type=%1 ORDER BY Country, Name, Spot ASC;")
                 .arg(type);
	success = query.exec(sqls);

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
	QString sqls;
	bool success;
	int id = -1;

  sqls = QString("SELECT Id FROM WayPoints WHERE "
                "Name='%1' AND Spot='%2' AND Country='%3';")
                .arg(escape(wp.name())).arg(escape(wp.spot())).arg(escape(wp.country()));

	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}

	wp.setId(id);

	return id;
}
