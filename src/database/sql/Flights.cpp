/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                         *
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

#include <qdatetime.h>
#include <qsqlcursor.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <stdlib.h>
#include "Error.h"
#include "Flights.h"
#include "Glider.h"
#include "Gliders.h"
#include "Pilots.h"
#include "WayPoint.h"
#include "WayPoints.h"
#include "ISql.h"

Flights::Flights(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

bool Flights::add(Flight &flight)
{
	QSqlCursor cur("Flights");
	QSqlRecord *pRec;
	QSqlQuery query(db());
	bool success;

	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Number", flight.number());
	pRec->setValue("PilotId", flight.pilot().id());
	pRec->setValue("Date", flight.date());
	pRec->setValue("Time", flight.time());
	pRec->setValue("GliderId", flight.glider().id());
	pRec->setValue("StartPtId", flight.startPt().id());
	pRec->setValue("LandPtId", flight.landPt().id());
	pRec->setValue("Duration", flight.duration());
	pRec->setValue("Distance", flight.distance());
	pRec->setValue("Comment", flight.comment());
	pRec->setValue("IGCFile", flight.igcData());
	
	success = (cur.insert() == 1);
	Error::verify(success, Error::SQL_CMD);
	DataBaseSub::setLastModified("Flights");
	setId(flight);

	return success;
}

bool Flights::delFlight(Flight &flight)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls.sprintf("DELETE FROM `Flights` WHERE `PilotId` = %i AND `Number` = %i", flight.pilot().id(), flight.number());
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
	
	sqls.sprintf("SELECT MAX(Number) FROM `Flights` WHERE `PilotId` = %i", pilot.id());
	
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

	sqls.sprintf("SELECT * FROM `Flights` WHERE `PilotId` = '%i' ORDER BY `Number` DESC;", pilot.id());
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			flight.setId(query.value(Id).toInt());
			flight.setNumber(query.value(Number).toInt());
			ISql::pInstance()->pPilotTable()->pilot(query.value(PilotId).toInt(), flightPilot);
			flight.setPilot(flightPilot);
			flight.setDate(query.value(Date).toDate());
			flight.setTime(query.value(Time).toTime());
			ISql::pInstance()->pGliderTable()->glider(query.value(GliderId).toInt(), glider);
			flight.setGlider(glider);
			ISql::pInstance()->pWayPointTable()->wayPoint(query.value(StartPtId).toInt(), wayPoint);
			flight.setStartPt(wayPoint);
			ISql::pInstance()->pWayPointTable()->wayPoint(query.value(LandPtId).toInt(), wayPoint);
			flight.setLandPt(wayPoint);
			flight.setDuration(query.value(Duration).toInt());
			flight.setDistance(query.value(Distance).toInt());
			flight.setComment(query.value(Comment).toString());
			//flight.setIgcData(query.value(IGCFile).toByteArray());
			
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
		sqls.sprintf("SELECT * FROM `Flights` WHERE "
			"`PilotId` = %i AND "
			"`Date` >= '%i-01-01' AND "
			"`Date` <= '%i-12-31';",
				pilot.id(), year, year);
		success = query.exec(sqls);
	
		if(success)
		{
			fpy.nFlights = 0;
			fpy.airTimeSecs = 0;
			fpy.year = year;
			
			while(query.next())
			{
				fpy.nFlights++;
				fpy.airTimeSecs += query.value(Duration).toInt();
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

	sqls.sprintf("SELECT * FROM `Flights` WHERE `PilotId` = '%i' AND `Number` = '%i';", flight.pilot().id(), flight.number());
	success = (query.exec(sqls) && query.first());
	
	if(success)
	{
		flight.igcData() = query.value(IGCFile).toByteArray();
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
	
	sqls.sprintf("SELECT * FROM `Flights` WHERE `GliderId` = '%i';", glider.id());
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			flights++;
			airtime += query.value(Duration).toInt();
		}

		glider.setAirtime(airtime);
		glider.setFlights(flights);
	}
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool Flights::setId(Flight &flight)
{
	QSqlQuery query(db());
	QString sqls;
	QString dbModel;
	bool success;
	int id = -1;

	sqls.sprintf("SELECT * FROM `Flights` WHERE "
		"`Number` = '%i' AND "
		"`PilotId` = '%i';",
		flight.number(), flight.pilot().id());

	success = (query.exec(sqls) && query.first());

	if(success)
	{
		id = query.value(Id).toInt();
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	flight.setId(id);
	
	return success;
}
