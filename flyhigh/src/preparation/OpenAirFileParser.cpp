/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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

#include <qbuffer.h>
#include <qstring.h>
#include <math.h>
#include "AirSpaceItem.h"
#include "OpenAirFileParser.h"
#include "WayPoint.h"

#define MAX_REC_SIZE 100

OpenAirFileParser::OpenAirFileParser()
{
}

void OpenAirFileParser::parse(QByteArray &openAirData)
{
	typedef enum RecordType{Unspecified, OpenAirspace, Flytec};

	QBuffer buff;
	char record[MAX_REC_SIZE];
	AirSpace airspace;
	AirSpace *pAirspace;
	RecordType recordType = Unspecified;
	
	m_airspaceList.clear();
	buff.setBuffer(openAirData);

	if(buff.open(IO_ReadOnly))
	{
		while(buff.readLine(record, MAX_REC_SIZE) > 0)
		{
			if(strncmp(record, "*## ", 4) == 0) // this is a Flytec airspace name
			{
				airspace.setName(record+4);
				recordType = Flytec;
			}
			else if(strncmp(record, "AC", 2) == 0)
			{
				if(recordType == Unspecified)
				{
					recordType = OpenAirspace;
				}
				airspace.setAirspaceClass(record+3);
			}
			else if(strncmp(record, "AN", 2) == 0)
			{
				if(recordType != Flytec) // skip AN on Flytec record 
				{
					airspace.setName(record+3);
				}
			}
			else if(strncmp(record, "AH", 2) == 0)
			{
				airspace.setHigh(record+3);
			}
			else if(strncmp(record, "AL", 2) == 0)
			{
				airspace.setLow(record+3);
			}
			else if(strncmp(record, "V", 1) == 0)
			{
				parseVarAssign(record+2);
			}
			else if(strncmp(record, "DP", 2) == 0)
			{
				parsePoint(record+3, airspace);
			}
			else if(strncmp(record, "DB", 2) == 0)
			{
				parseArc(record+3, airspace);
			}
			else if(strncmp(record, "DC", 2) == 0)
			{
				parseCircle(record+3, airspace);
			}
			else if(strncmp(record, "*", 1) == 0)
			{
				if(recordType != Unspecified)
				{
					pAirspace = new AirSpace;
					*pAirspace = airspace;
					m_airspaceList.append(pAirspace);
					airspace.airSpaceItemList().clear();
					recordType = Unspecified;
				}
			}
		}
		buff.close();
	}
}

AirSpace::AirSpaceListType& OpenAirFileParser::airspaceList()
{
	return m_airspaceList;
}

void OpenAirFileParser::parseAirspaceClass(char *record, AirSpace &airspace)
{
	QString str = record;
	int begin;
	int end;
	
	begin = str.find(' ') + 1;
	end = str.find('\r');
	str = str.mid(begin, end-begin);

	airspace.setAirspaceClass(str);
}

void OpenAirFileParser::parseVarAssign(char *record)
{
	if(strncmp(record, "D=+", 3) == 0)
	{
		m_arcDir = true;
	}
	else if(strncmp(record, "D=-", 3) == 0)
	{
		m_arcDir = false;
	}
	else if(strncmp(record, "X=", 2) == 0)
	{
		parseCoordinate(record+2, m_arcCenterLat, m_arcCenterLon);
	}
}

void OpenAirFileParser::parsePoint(char *record, AirSpace &airspace)
{
// DP 46:57:47 N 007:16:59 E
	AirSpaceItemPoint *pPoint;
	double lat;
	double lon;
	
	pPoint = new AirSpaceItemPoint(AirSpaceItem::Point);
	parseCoordinate(record, lat, lon);
	pPoint->setPoint(lat, lon);
	airspace.airSpaceItemList().push_back(pPoint);
}

void OpenAirFileParser::parseArc(char *record, AirSpace &airspace)
{
	QString str = record;
	AirSpaceItemPoint *pCenter;
	AirSpaceItemSeg *pSeg;
	double beginLat;
	double beginLon;
	double endLat;
	double endLon;

	parseCoordinate(record, beginLat, beginLon);
	parseCoordinate(record+str.find(',')+1, endLat, endLon);

	// center
	pCenter = new AirSpaceItemPoint(AirSpaceItem::Center);
	pCenter->setPoint(m_arcCenterLat, m_arcCenterLon);
	airspace.airSpaceItemList().push_back(pCenter);

	// start
	pSeg = new AirSpaceItemSeg(AirSpaceItem::StartSegment);
	pSeg->setPoint(beginLat, beginLon);
	pSeg->setDir(m_arcDir);
	airspace.airSpaceItemList().push_back(pSeg);

	//stop
	pSeg = new AirSpaceItemSeg(AirSpaceItem::StopSegment);
	pSeg->setPoint(endLat, endLon);
	pSeg->setDir(m_arcDir);
	airspace.airSpaceItemList().push_back(pSeg);
}

void OpenAirFileParser::parseCircle(char *record, AirSpace &airspace)
{
	AirSpaceItemCircle *pCircle;
	uint radius;

	pCircle = new AirSpaceItemCircle();
	pCircle->setPoint(m_arcCenterLat, m_arcCenterLon);
	radius = WayPoint::meters(atof(record));
	pCircle->setRadius(radius);
	airspace.airSpaceItemList().push_back(pCircle);
}

bool OpenAirFileParser::parseCoordinate(char *record, double &latitude, double &longitude)
{
	char lat[15];
	char NS[5];
	char lon[15];
	char EW[5];
	int deg;
	int min;
	int sec;
	bool success;
	
	success = (sscanf(record, "%s %1s %s %1s", lat, NS, lon, EW) == 4);
	
	if(success)
	{
		success = (sscanf(lat, "%d:%d:%d", &deg, &min, &sec) == 3);
		
		if(success)
		{
			latitude = deg + min / 60.0 + sec / 3600.0;
			
			if(strcmp(NS, "S") == 0)
			{
				latitude *= -1;
			}
		}
		
		success = (sscanf(lon, "%d:%d:%d", &deg, &min, &sec) == 3);
		
		if(success)
		{
			longitude = deg + min / 60.0 + sec / 3600.0;
			
			if(strcmp(EW, "W") == 0)
			{
				longitude *= -1;
			}
		}
	}
	
	return success;
}
