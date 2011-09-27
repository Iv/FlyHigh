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

#include <QSqlDatabase>
#include <QSqlQuery>
#include "Error.h"
#include "AirSpaces.h"

AirSpaces::AirSpaces(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool AirSpaces::add(AirSpace &airspace)
{
	(void)airspace;
/*	QSqlCursor cur("AirSpaces");
	QSqlRecord *pRec;
	QString edgePtField;
	QString edgePt;
	uint nEdgePts;
	uint edgePtNr;

	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Name", airspace.name());
	pRec->setValue("Low", airspace.low());
	pRec->setValue("High", airspace.high());
	pRec->setValue("Class", airspace.airspaceClass());

	nEdgePts = airspace.edgePointList().size();

	for(edgePtNr=0; edgePtNr<nEdgePts; edgePtNr++)
	{
		edgePtField.sprintf("EdgePoint%i", edgePtNr);
		edgePt.sprintf("%3.5f;%2.5f", airspace.edgePointList().at(edgePtNr).longitude(),
					airspace.edgePointList().at(edgePtNr).latitude());
		pRec->setValue(edgePtField, edgePt);
	}

	Error::verify(cur.insert() == 1, Error::SQL_CMD);
	DataBaseSub::setLastModified("AirSpaces");
	*/
	return true;
}

bool AirSpaces::delAirSpace(const QString &name)
{
	(void)name;
/*	QSqlQuery query(db());
	QString sqls;
	bool success;

	sqls.sprintf("DELETE FROM  AirSpaces WHERE Name='%s'", name.ascii());
	success = query.exec(sqls);
	DataBaseSub::setLastModified("AirSpaces");
	Error::verify(success, Error::SQL_CMD);

	return success;*/
	return false;
}

bool AirSpaces::airspace(const QString &name, AirSpace &airspace)
{
	(void)name;
	(void)airspace;
/*	QSqlQuery query(db());
	QString sqls;
	QString strEdgePt;
	double lat;
	double lon;
	WayPoint edgePt;
	uint edgePtNr;
	int pos;
	bool success;

	sqls.sprintf("SELECT * FROM AirSpaces WHERE Name='%s'", name.ascii());
	success = query.exec(sqls);

	if(success)
	{
		success = query.first();

		if(success)
		{
			airspace.edgePointList().clear();
			airspace.setName(query.value(Name).toString());

			for(edgePtNr=0; edgePtNr<MaxEdgePoints; edgePtNr++)
			{
				strEdgePt = query.value(EdgePoint0+edgePtNr).toString();

				if(strEdgePt == "")
				{
					break;
				}

                                pos = strEdgePt.indexOf(';');
				lon = strEdgePt.left(pos).toDouble();
				lat = strEdgePt.right(strEdgePt.length() - pos - 1).toDouble();

				edgePt.setCoordinates(lat, lon);
				airspace.edgePointList().push_back(edgePt);
			}
		}
	}
	else
	{
		Error::verify(success, Error::SQL_CMD);
	}

	return success;
*/
	return false;
}

bool AirSpaces::airspaceList(AirSpaceList &airspaceList)
{
	(void)airspaceList;
/*	AirSpace airspace;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM AirSpaces ORDER BY Name ASC";
	QString strEdgePt;
	WayPoint edgePt;
	int pos;
	double lat;
	double lon;
	uint edgePtNr;
	bool success;

	success = query.exec(sqls);

	if(success)
	{
		while(query.next())
		{
			airspace.edgePointList().clear();
			airspace.setName(query.value(Name).toString());
			airspace.setLow(query.value(Low).toString());
			airspace.setHigh(query.value(High).toString());
			airspace.setAirspaceClass(query.value(Class).toString());

			for(edgePtNr=0; edgePtNr<MaxEdgePoints; edgePtNr++)
			{
				strEdgePt = query.value(EdgePoint0+edgePtNr).toString();

				if(strEdgePt == "")
				{
					break;
				}

                                pos = strEdgePt.indexOf(';');
				lon = strEdgePt.left(pos).toDouble();
				lat = strEdgePt.right(strEdgePt.length() - pos - 1).toDouble();

				edgePt.setCoordinates(lat, lon);
				airspace.edgePointList().push_back(edgePt);
			}

			airspaceList.push_back(airspace);
		}
	}

	Error::verify(success, Error::SQL_CMD);

	return success;
*/
	return false;
}
