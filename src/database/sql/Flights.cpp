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

Flights::Flights(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
	m_tableName = "Flights_";
	m_tableName += getenv("USER");
}

bool Flights::add(Flight &flight)
{
	QSqlCursor cur(m_tableName);
	QSqlRecord *pRec;
	
	QSqlQuery query(db());
	
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
	
	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	DataBaseSub::setLastModified(m_tableName);

	return true;
}

bool Flights::delFlight(int nr)
{
	QSqlQuery query(db());
	QString sqls;
	bool success;
	 
	sqls.sprintf("DELETE FROM `%s` WHERE `Number` = '%i'", m_tableName.ascii(), nr);
	success = query.exec(sqls);
	DataBaseSub::setLastModified(m_tableName);
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

int Flights::newFlightNr()
{
	QString sqls;
	QSqlQuery query(db());
	int newFlightNr = -1;
	
	sqls.sprintf("SELECT MAX(Number) FROM %s", m_tableName.ascii());
	
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
	QString sqls;
	bool success;
	
	sqls.sprintf("SELECT * FROM `%s` ORDER BY `Number` DESC", m_tableName.ascii());
	
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
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool Flights::flightsPerYear(FlightsPerYearListType &fpyList)
{
	QSqlQuery query(db());
	QString sqls;
	QDate now = QDate::currentDate();
	FlightsPerYearType fpy;
	bool success;
	int year;
	
	for(year=2000; year<=now.year(); year++)
	{
		sqls.sprintf("SELECT * FROM `%s` WHERE `Date` >= '%i-01-01' AND `Date` <= '%i-12-31'",
				m_tableName.ascii(), year, year);
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


bool Flights::igcFile(uint flightNr, QByteArray &arr)
{
	Flight flight;
	QSqlQuery query(db());
	QString sqls;
	bool success;
	
	sqls.sprintf("SELECT * FROM `%s` WHERE `Number` = '%i'", m_tableName.ascii(), flightNr);
	success = (query.exec(sqls) && query.first());
	
	if(success)
	{
		arr = query.value(IGCFile).toByteArray();
	}
	
	Error::verify(success, Error::SQL_CMD);
	
	return success;
}

bool Flights::setupTable()
{
	QSqlQuery query(db());
	QString sqls;
	bool success = true;
	
	if(!db()->tables().contains(m_tableName))
	{
		sqls.sprintf("CREATE TABLE `%s`("
					"`Number` int(11) NOT NULL default '0',"
					"`Date` date NOT NULL default '0000-00-00',"
					"`Time` time NOT NULL default '00:00:00',"
					"`Glider` varchar(16) NOT NULL default '0'  REFERENCES Gliders(Model),"
					"`StartPt` varchar(16) NOT NULL default '0' REFERENCES WayPoints(Name),"
					"`LandPt` varchar(16) NOT NULL default '0' REFERENCES WayPoints(Name),"
					"`Duration` int(11) NOT NULL default '0',"
					"`Distance` int(11) default '0',"
					"`Comment` varchar(200) default NULL,"
					"`IGCFile` mediumblob,"
					"PRIMARY KEY  (`Number`)"
					") TYPE=InnoDB;", m_tableName.ascii());
					
		success = query.exec(sqls);
		Error::verify(success, Error::SQL_CMD);
	}
	
	return success;
}

int Flights::lastModified()
{
	QString sqls;
	QString date;
	QSqlQuery query(db());
	int time = 1;
	
	sqls.sprintf("SELECT * FROM `LastModified` WHERE `Name` = '%s'", m_tableName.ascii());
	
	if(query.exec(sqls) &&
		query.first())
	{
		time = query.value(1).toDateTime().toTime_t();
	}
	
	return time;
}
