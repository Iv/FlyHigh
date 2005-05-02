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
#include "OpenAirFileParser.h"
#include "Vector.h"

#define MAX_REC_SIZE 100

const uint OpenAirFileParser::m_maxSeg = 16;

OpenAirFileParser::OpenAirFileParser()
{
}

void OpenAirFileParser::parse(QByteArray &openAirData)
{
	typedef enum RecordType{Unspecified, OpenAirspace, Flytec};

	QBuffer buff;
	char record[MAX_REC_SIZE];
	AirSpace airspace;
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
			else if(strncmp(record, "*\r", 2) == 0)
			{
				if(recordType != Unspecified)
				{
					if(recordType == Flytec) // Flytec removes closing edgePoint
					{
						airspace.edgePointList().push_back(airspace.edgePointList().at(0));
					}
					
					m_airspaceList.push_back(airspace);
					airspace.edgePointList().clear();
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
		parseCoordinate(record+2, m_arcCenter);
	}
}

void OpenAirFileParser::parsePoint(char *record, AirSpace &airspace)
{
// DP 46:57:47 N 007:16:59 E
	WayPoint pt;
	
	parseCoordinate(record, pt);
	airspace.edgePointList().push_back(pt);
}

void OpenAirFileParser::parseArc(char *record, AirSpace &airspace)
{
	WayPoint pt;
	WayPoint startPt;
	WayPoint endPt;
	Vector2 a;
	Vector2 b;
	Vector2 origVec;
	Vector2 rotVec;
	Vector2 center(m_arcCenter.latitude(), m_arcCenter.longitude());
	QString str = record;
	double arcA;
	double arcB;
	double arcAB;
	double segArc;
	double midR;
	uint segNr;
	uint maxSeg;

	// vector a
	parseCoordinate(record, startPt);
	a.set(startPt.latitude(), startPt.longitude());
	a -= center;
	
	// vector b
	parseCoordinate(record+str.find(',')+1, endPt);
	b.set(endPt.latitude(), endPt.longitude());
	b -= center;

	// delta arc
	arcA = a.arc();
	arcB = b.arc();
	arcAB = deltaArc(arcA, arcB);
	maxSeg = maxSegments(arcAB);

	// mid radius
	midR = (a.length() + b.length()) / 2.0;
	segArc = arcAB / maxSeg;
	origVec.setCircle(midR, a.arc());
	
	// points
	for(segNr=1; segNr<maxSeg; segNr++)
	{
		rotVec = origVec;
		rotVec.rot(segNr * segArc);
		rotVec += center;
		pt.setCoordinates(rotVec.x(), rotVec.y());
		airspace.edgePointList().push_back(pt);
	}
}

void OpenAirFileParser::parseCircle(char *record, AirSpace &airspace)
{
	Vector2 origVec;
	Vector2 rotVec;
	Vector2 center(m_arcCenter.latitude(), m_arcCenter.longitude());
	WayPoint pt;
	QString str = record;
	double segArc;
	double radiusArc;
	uint segNr;
	
	segArc = 2 * M_PI / m_maxSeg;
	radiusArc = WayPoint::arc(WayPoint::meters(atof(record)));
	origVec.set(radiusArc, 0);

	// points
	for(segNr=0; segNr<=m_maxSeg; segNr++)
	{
		rotVec = origVec;
		rotVec.rot(segNr*segArc);
		rotVec += center;
		pt.setCoordinates(rotVec.x(), rotVec.y());
		airspace.edgePointList().push_back(pt);
	}
}

bool OpenAirFileParser::parseCoordinate(char *record, WayPoint &pt)
{
	char lat[15];
	char NS[5];
	char lon[15];
	char EW[5];
	double latitude = 0.0;
	double longitude = 0.0;
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
	
	pt.setCoordinates(latitude, longitude);
	
	return success;
}

double OpenAirFileParser::deltaArc(double arcA, double arcB)
{
	double arc;
	
	if(arcA > arcB)
	{
		if(m_arcDir)
		{
			arc = 2 * M_PI - (arcA - arcB);
		}
		else
		{
			arc = arcB - arcA;
		}
	}
	else
	{
		if(m_arcDir)
		{
			arc = arcB - arcA;
		}
		else
		{
			arc = (arcB - arcA) - 2 * M_PI;
		}
	}
	
	return arc;
}

uint OpenAirFileParser::maxSegments(double arc)
{
	double absArc = fabs(arc);
	uint maxSeg = m_maxSeg / 4;
	
	if(absArc >= 3 * M_PI / 2)
	{
		maxSeg = m_maxSeg;
	}
	else if(absArc >= M_PI)
	{
		maxSeg = 3 * m_maxSeg / 2;
	}
	else if(absArc >= M_PI / 2)
	{
		maxSeg = m_maxSeg / 2;
	}

	return maxSeg;
}
