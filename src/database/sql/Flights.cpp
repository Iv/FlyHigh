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
  int id;
	bool success;

  id = newId("Flights");
  success = query.prepare("INSERT INTO Flights(Id, Number, PilotId, Date, Time, GliderId, StartPtId, "
                          "LandPtId, Duration, Distance, PhotoPath, Comment, IGCFile) "
                          "VALUES(:id, :number, :pilot, :date, :time, :glider, :start, :land, :duration, :distance, :photopath, :comment, :igcdata)");
  query.bindValue(":id",id);
  query.bindValue(":number",flight.number());
  query.bindValue(":pilot",flight.pilot().id());
  query.bindValue(":date",flight.date().toString("yyyy-MM-dd"));
  query.bindValue(":time",flight.time().toString("hh:mm:ss"));
  query.bindValue(":glider",flight.glider().id());
  query.bindValue(":start",flight.startPt().id());
  query.bindValue(":land",flight.landPt().id());
  query.bindValue(":duration",flight.duration());
  query.bindValue(":distance",flight.distance());
  query.bindValue(":photopath",flight.photoPath());
  query.bindValue(":comment",flight.comment());
  query.bindValue(":igcdata", flight.igcData());
  success &= query.exec();
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
	bool success;

  success = query.prepare("UPDATE Flights SET Number=:number, PilotId=:pilot, Date=:date, Time=:time, GliderId=:glider, "
                          "StartPtId=:start, LandPtId=:land, Duration=:duration, Distance=:distance, PhotoPath=:photopath, Comment=:comment "
                          "WHERE Id=:id");
  query.bindValue(":number",flight.number());
  query.bindValue(":pilot",flight.pilot().id());
  query.bindValue(":date",flight.date().toString("yyyy-MM-dd"));
  query.bindValue(":time",flight.time().toString("hh:mm:ss"));
  query.bindValue(":glider",flight.glider().id());
  query.bindValue(":start",flight.startPt().id());
  query.bindValue(":land",flight.landPt().id());
  query.bindValue(":duration",flight.duration());
  query.bindValue(":distance",flight.distance());
  query.bindValue(":photopath",flight.photoPath());
  query.bindValue(":comment",flight.comment());
  query.bindValue(":id",flight.id());
  success &= query.exec();

	DataBaseSub::setLastModified("Flights");
	Error::verify(success, Error::SQL_CMD);

	return success;
}

bool Flights::delFlight(Flight &flight)
{
  QSqlQuery query(db());
	bool success;

  success = query.prepare("DELETE FROM Flights WHERE PilotId=:pilot AND Number=:number");
  query.bindValue(":pilot",flight.pilot().id());
  query.bindValue(":number",flight.number());
  success &= query.exec();

	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Flights");

	return success;
}

int Flights::newFlightNr(Pilot &pilot)
{
  QSqlQuery query(db());
	int newFlightNr = -1;

  query.prepare("SELECT MAX(Number) FROM Flights WHERE PilotId=:id");
  query.bindValue(":id", pilot.id());

  if(query.exec() && query.first())
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
	bool success;
	Glider glider;
	WayPoint wayPoint;

  success = query.prepare("SELECT Id, Number, PilotId, Date, Time, GliderId, StartPtId, LandPtId, "
                          "Duration, Distance, PhotoPath, Comment FROM Flights WHERE PilotId=:id ORDER BY Number DESC");
  query.bindValue(":id", pilot.id());
  success &= query.exec();

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
	QDate now = QDate::currentDate();
	FlightsPerYearType fpy;
	bool success = false;
	int year;

  success = query.prepare("SELECT Duration FROM Flights "
                          "WHERE PilotId=:pilot AND Date>=:from AND Date<=:to");
  query.bindValue(":pilot", pilot.id());

	for(year=2000; year<=now.year(); year++)
	{
    query.bindValue(":from", QDate(year,1,1).toString("yyyy-MM-dd"));
    query.bindValue(":to", QDate(year,12,31).toString("yyyy-MM-dd"));
    success &= query.exec();

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
	bool success;

  success = query.prepare("SELECT IGCFile FROM Flights WHERE PilotId=:pilot AND Number=:number");
  query.bindValue(":pilot", flight.pilot().id());
  query.bindValue(":number", flight.number());
  success &= (query.exec() && query.first());

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
	uint airtime = 0;
	uint flights = 0;
	bool success;

  success = query.prepare("SELECT Duration FROM Flights WHERE GliderId=:glider");
  query.bindValue(":glider", glider.id());
  success &= query.exec();

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
	bool success;
	int id = -1;

  success = query.prepare("SELECT Id FROM Flights WHERE Number=:number AND PilotId=:pilot");
  query.bindValue(":number", flight.number());
  query.bindValue(":pilot", flight.pilot().id());
  success &= (query.exec() && query.first());

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
