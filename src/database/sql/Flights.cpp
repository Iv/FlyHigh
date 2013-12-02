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

#include <QDateTime>
#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "Error.h"
#include "Flights.h"
#include "Glider.h"
#include "Gliders.h"
#include "Pilots.h"
#include "WayPoint.h"
#include "WayPoints.h"
#include "ISql.h"

Flights::Flights(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool Flights::add(Flight &flight)
{
  QSqlQuery query(db());
  QString sqls;
  int id;
	bool success;

  id = newId("Flights");
  sqls = QString("INSERT INTO Flights(Id, Number, PilotId, Date, Time, GliderId, StartPtId, "
                 "LandPtId, Duration, Distance, PhotoPath, Comment, IGCFile) "
                 "VALUES(%1, %2, %3, '%4', '%5', %6, %7, %8, %9, %10, '%11', '%12', :igcdata);")
                 .arg(id).arg(flight.number()).arg(flight.pilot().id())
                 .arg(flight.date().toString("yyyy-MM-dd")).arg(flight.time().toString("hh:mm:ss"))
                 .arg(flight.glider().id()).arg(flight.startPt().id()).arg(flight.landPt().id())
                 .arg(flight.duration()).arg(flight.distance()).arg(flight.photoPath())
                 .arg(escape(flight.comment()));

  query.prepare(sqls);
  query.bindValue(":igcdata", flight.igcData());
	success = query.exec();
	DataBaseSub::setLastModified("Flights");
	Error::verify(success, Error::SQL_CMD);

  if(flight.id() == -1)
  {
    flight.setId(id);
  }

	return success;
}

bool Flights::updateFlight(Flight &flight)
{
  QSqlQuery query(db());
  QString sqls;
	bool success;

  sqls = QString("UPDATE Flights SET Number=%1, PilotId=%2, Date='%3', Time='%4', GliderId=%5, "
                 "StartPtId=%6, LandPtId=%7, Duration=%8, Distance=%9, PhotoPath='%10', Comment='%11' "
                 "WHERE Id=%12;")
                 .arg(flight.number()).arg(flight.pilot().id()).arg(flight.date().toString("yyyy-MM-dd"))
                 .arg(flight.time().toString("hh:mm:ss"))
                 .arg(flight.glider().id()).arg(flight.startPt().id()).arg(flight.landPt().id())
                 .arg(flight.duration()).arg(flight.distance()).arg(flight.photoPath())
                 .arg(escape(flight.comment())).arg(flight.id());

	success = query.exec(sqls);
	DataBaseSub::setLastModified("Flights");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Flights::delFlight(Flight &flight)
{
  QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls = QString("DELETE FROM Flights WHERE PilotId=%1 AND Number=%2")
                  .arg(flight.pilot().id()).arg(flight.number());
	success = query.exec(sqls);
	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Flights");

	return success;
}

int Flights::newFlightNr(Pilot &pilot)
{
	QString sqls;
  QSqlQuery query(db());
	int newFlightNr = -1;

	sqls = QString("SELECT MAX(Number) FROM Flights WHERE PilotId=%1").arg(pilot.id());

	if(query.exec(sqls) && query.first())
	{
		newFlightNr = query.value(0).toInt() + 1;
	}

	return newFlightNr;
}

bool Flights::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	Pilot flightPilot;
	Flight flight;
  QSqlQuery query(db());
	QString sqls;
	bool success;
	Glider glider;
	WayPoint wayPoint;

	sqls = QString("SELECT Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, "
                  "Duration, Distance, PhotoPath, Comment FROM Flights WHERE PilotId=%1 ORDER BY Number DESC;")
                .arg(pilot.id());

	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
			flight.setId(query.value(0).toInt());
			flight.setNumber(query.value(1).toInt());
			ISql::pInstance()->pPilotTable()->pilot(query.value(2).toInt(), flightPilot);
			flight.setPilot(flightPilot);
			flight.setDate(query.value(3).toDate());
			flight.setTime(query.value(4).toTime());
			ISql::pInstance()->pGliderTable()->glider(query.value(5).toInt(), glider);
			flight.setGlider(glider);
			ISql::pInstance()->pWayPointTable()->wayPoint(query.value(6).toInt(), wayPoint);
			flight.setStartPt(wayPoint);
			ISql::pInstance()->pWayPointTable()->wayPoint(query.value(7).toInt(), wayPoint);
			flight.setLandPt(wayPoint);
			flight.setDuration(query.value(8).toInt());
			flight.setDistance(query.value(9).toInt());
			flight.setPhotoPath(query.value(10).toString());
			flight.setComment(query.value(11).toString());
			//flight.setIgcData(query.value(12).toByteArray());

			flightList.push_back(flight);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Flights::flightsPerYear(Pilot &pilot, FlightsPerYearListType &fpyList)
{
  QSqlQuery query(db());
	QString sqls;
	QDate now = QDate::currentDate();
	FlightsPerYearType fpy;
	bool success = false;
	int year;

	for(year=2000; year<=now.year(); year++)
	{
		sqls = QString("SELECT Duration FROM Flights WHERE PilotId = %1 AND "
                  "Date >= '%2-01-01' AND Date <= '%3-12-31';")
                  .arg(pilot.id()).arg(year).arg(year);

		success = query.exec(sqls);

		if(success)
		{
			fpy.nFlights = 0;
			fpy.airTimeSecs = 0;
			fpy.year = year;

			while(query.next())
			{
				fpy.nFlights++;
				fpy.airTimeSecs += query.value(0).toInt();
			}

			if(fpy.nFlights > 0)
			{
				fpyList.push_back(fpy);
			}
		}
		else
		{
			Error::verify(success, Error::SQL_CMD);
			break;
		}
	}

	return success;
}


bool Flights::loadIGCFile(Flight &flight)
{
  QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls = QString("SELECT IGCFile FROM Flights WHERE PilotId=%1 AND Number=%2;")
                .arg(flight.pilot().id()).arg(flight.number());
	success = (query.exec(sqls) && query.first());

	if(success)
	{
		flight.setIgcData(query.value(0).toByteArray());
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Flights::setFlightStatistic(Glider &glider)
{
  QSqlQuery query(db());
	QString sqls;
	uint airtime = 0;
	uint flights = 0;
	bool success;

	sqls = QString("SELECT Duration FROM Flights WHERE GliderId=%1;").arg(glider.id());
	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
			flights++;
			airtime += query.value(0).toInt();
		}

		glider.setAirtime(airtime);
		glider.setFlights(flights);
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Flights::checkModified()
{
	return DataBaseSub::checkModified("Flights");
}

bool Flights::setId(Flight &flight)
{
  QSqlQuery query(db());
	QString sqls;
	QString dbModel;
	bool success;
	int id = -1;

	sqls = QString("SELECT Id FROM Flights WHERE Number=%1 AND PilotId=%2;")
                .arg(flight.number()).arg(flight.pilot().id());

	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(0).toInt();
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	flight.setId(id);

	return success;
}
