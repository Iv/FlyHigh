/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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

#include <qsqlcursor.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include "Flights.h" 

Flights::Flights(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

bool Flights::add(Flight &flight)
{
	QSqlCursor cur("Flights");
	QSqlRecord *pRec;
	
	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Number", flight.number());
	pRec->setValue("Date", flight.date());
	pRec->setValue("Time", flight.time());
	pRec->setValue("Glider", flight.glider());
	pRec->setValue("StartPt", flight.startPt());
	pRec->setValue("LandPt", flight.landPt());
	pRec->setValue("Duration", flight.duration());
	pRec->setValue("Distance", flight.distance());
	pRec->setValue("Comment", flight.comment());
	pRec->setValue("IGCFile", flight.igcData());
	cur.insert();

	DataBaseSub::setLastModified("Flights");

	return true;
}

bool Flights::delFlight(int nr)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	 
	sqls.sprintf("DELETE FROM `Flights` WHERE `Number` = '%i'", nr);
	success = query.exec(sqls);
	DataBaseSub::setLastModified("Flights");
	
	return success;
}

int Flights::newFlightNr()
{
	QString sqls = "SELECT MAX(Number) FROM Flights";
	QSqlQuery query(db());
	int newFlightNr = -1;
	
	if(query.exec(sqls) &&
		query.first())
	{
		newFlightNr = query.value(0).toInt() + 1;
	}
	
	return newFlightNr;
}

bool Flights::flightList(Flight::FlightListType &flightList)
{
	Flight flight;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM `Flights` ORDER BY `Number` DESC";
	bool success;
	
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			flight.setNumber(query.value(Number).toInt());
			flight.setDate(query.value(Date).toDate());
			flight.setTime(query.value(Time).toTime());
			flight.setGlider(query.value(Glider).toString());
			flight.setStartPt(query.value(StartPt).toString());
			flight.setLandPt(query.value(LandPt).toString());
			flight.setDuration(query.value(Duration).toInt());
			flight.setDistance(query.value(Distance).toInt());
			flight.setComment(query.value(Comment).toString());
			//flight.setIgcData(query.value(IGCFile).toByteArray());
			
			flightList.push_back(flight);
		}
	}
	
	return success;
}

bool Flights::igcFile(uint flightNr, QByteArray &arr)
{
	Flight flight;
	QSqlQuery query(db());
	QString sqls;
	bool success;
	
	sqls.sprintf("SELECT * FROM `Flights` WHERE `Number` = '%i'", flightNr);
	success = (query.exec(sqls) && query.first());
	
	if(success)
	{
		arr = query.value(IGCFile).toByteArray();
	}
	
	return success;
}
