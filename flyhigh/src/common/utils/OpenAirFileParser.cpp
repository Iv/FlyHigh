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

#define MAX_REC_SIZE 50

OpenAirFileParser::OpenAirFileParser()
{
}

void OpenAirFileParser::parse(QByteArray &openAirData)
{
	QBuffer buff;
	QString str;
	char record[MAX_REC_SIZE];
	int pos;
	AirSpace airspace;
	
	buff.setBuffer(openAirData);

	if(buff.open(IO_ReadOnly))
	{
		while(buff.readLine(record, MAX_REC_SIZE) > 0)
		{
			pos = str.find(' ');
			str = str.left(pos);
			
			if(str == "*")
			{
//				type = NewAirspace;
			}
			else if(str == "AC")
			{
				parseAirspaceClass(record, airspace);
			}
			else if(str == "AN")
			{
				parseName(record, airspace);
			}
			else if(str == "AH")
			{
				parseHigh(record, airspace);
			}
			else if(str == "AL")
			{
				parseLow(record, airspace);
			}
			else if(str == "V")
			{
				parseVarAssign(record);
			}
			else if(str == "DP")
			{
				parsePoint(record, airspace);
			}
			else if(str == "DB")
			{
				parseArc(record, airspace);
			}
			else if(str == "DC")
			{
				parseCircle(record, airspace);
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
	int pos;
	
	pos = str.find(' ');
	str = str.right(pos);

	airspace.setAirspaceClass(str);
}

void OpenAirFileParser::parseName(char *record, AirSpace &airspace)
{
	QString str = record;
	int pos;
	
	pos = str.find(' ');
	str = str.right(pos);

	airspace.setName(str);
}

void OpenAirFileParser::parseHigh(char *record, AirSpace &airspace)
{
	QString str = record;
	int pos;
	
	pos = str.find(' ');
	str = str.right(pos);

	airspace.setHigh(str);
}

void OpenAirFileParser::parseLow(char *record, AirSpace &airspace)
{
	QString str = record;
	int pos;
	
	pos = str.find(' ');
	str = str.right(pos);

	airspace.setLow(str);
}

void OpenAirFileParser::parseVarAssign(char *record)
{
/*
V D=+
V X=46:09:46 N   008:50:13 E
*/
	QString str = record;
	int pos;
	QChar type;
	
	pos = str.find(' ');
	type = str.at(pos+1);
	
	if(type == 'D')
	{
			pos = str.find('=');
			m_arcDir = (str.at(pos+1) == '-'); // math direction
	}
	else if(type == 'X')
	{
		parseCoordinate(record, m_arcCenter);
	}
}

void OpenAirFileParser::parsePoint(char *record, AirSpace &airspace)
{
// DP 46:57:47 N 007:16:59 E
	AirSpace::EdgePointType pt;
	
	parseCoordinate(record, pt);
	airspace.edgePointList().push_back(pt);
}

void OpenAirFileParser::parseArc(char *record, AirSpace &airspace)
{
/*V D=+
V X=45:54:15 N   008:49:30 E
DB  45:53:03 N 008:52:59 E,  45:55:49 N 008:46:20 E*/
	AirSpace::EdgePointType pt;
	AirSpace::EdgePointType startPt;
	AirSpace::EdgePointType endPt;
	Vector2 a;
	Vector2 b;
	Vector2 resVec;
	Vector2 centerVec(m_arcCenter.lat, m_arcCenter.lon);
	QString str = record;
	double abArc;
	double segArc;
	uint segNr;
	uint maxSeg;
	char pos;
	
	// begin vector
	parseCoordinate(record, startPt);
	a.set(startPt.lat, startPt.lon);
	a -= centerVec;
	
	// end vector
	pos = str.find(',');
	parseCoordinate(&record[pos], endPt);
	b.set(endPt.lat, endPt.lon);
	b -= centerVec;
	
	abArc = arc(a, b);
	
	if(abArc <= M_PI_4)
	{
		maxSeg = 2;
	}
	else
	{
		maxSeg = 4;
	}

	// start point
	airspace.edgePointList().push_back(startPt);
	
	// 1st point
	segArc = abArc / maxSeg;
	a *= 1 / cos(segArc / 2); // scale
	a.rot(segArc / 2);
	resVec = a + centerVec;
	pt.lat = resVec.x();
	pt.lon = resVec.y();
	airspace.edgePointList().push_back(pt);
	
	// other points
	for(segNr=1; segNr<maxSeg; segNr++)
	{
		a.rot(segArc);
		resVec = a + centerVec;
		pt.lat = resVec.x();
		pt.lon = resVec.y();
		airspace.edgePointList().push_back(pt);
	}
	
	// end point
	airspace.edgePointList().push_back(endPt);
}

void OpenAirFileParser::parseCircle(char *record, AirSpace &airspace)
{
/*
V X=58:16:18 N   026:29:46 E 
DC 2.00    
*/
	Vector2 startVec;
	Vector2 a;
	Vector2 resVec;
	Vector2 centerVec(m_arcCenter.lat, m_arcCenter.lon);
	AirSpace::EdgePointType pt;
	QString str = record;
	double segArc;
	double radius;
	const double nautmil = 1.852;
	uint segNr;
	uint maxSeg = 16;
	int pos;
	
	segArc = 2 * M_PI / maxSeg;
	pos = str.find(' ');
	radius = str.right(pos).toDouble() * nautmil;
	startVec.setCircle(radius, 0.0);
	startVec *= 1 / cos(segArc / 2); // scale
	
	// first point
	a = startVec;
	resVec = a + centerVec;
	pt.lat = resVec.x();
	pt.lon = resVec.y();
	airspace.edgePointList().push_back(pt);

	// other points
	for(segNr=1; segNr<maxSeg; segNr++)
	{
		a.rot(segArc);
		resVec = a + centerVec;
		pt.lat = resVec.x();
		pt.lon = resVec.y();
		airspace.edgePointList().push_back(pt);
	}

	// last point
	a = startVec;
	resVec = a + centerVec;
	pt.lat = resVec.x();
	pt.lon = resVec.y();
	airspace.edgePointList().push_back(pt);
}

bool OpenAirFileParser::parseCoordinate(char *record, AirSpace::EdgePointType &pt)
{
	typedef enum ParseStateType{Lat, Lon};
	ParseStateType state = Lat;
	int deg;
	int minute;
	int sec;
	char dir;
	char *pRider = record;
	uint charNr;
	
	for(charNr=0; charNr<MAX_REC_SIZE; charNr++)
	{
		switch(state)
		{
			case Lat:
				if(isalnum(*pRider))
				{
					if(sscanf(pRider, "%2d*c%2d%*c%2d%*c%c", &deg, &minute, &sec, &dir) == 4)
					{
						pt.lat = deg + minute / 60.0 + sec / 60000.0;
						
						if(dir == 'S')
						{
							pt.lat *= -1;
						}
						
						pRider += 10;
						state = Lon;
					}
					else
					{
						return false;
					}
				}
			break;
			case Lon:
				if(isalnum(*pRider))
				{
					if(sscanf(pRider, "%3d*c%2d%*c%2d%*c%c", &deg, &minute, &sec, &dir) == 4)
					{
						pt.lon = deg + minute / 60.0 + sec / 60000.0;
						
						if(dir == 'E')
						{
							pt.lon *= -1;
						}
						
						return true;
					}
					else
					{
						return false;
					}
				}
			break;
		}
		pRider++;
	}
	
	return false;
// DP 46:02:46 N 008:53:48 E
// DP 47:42:35 N   011:11:40 E 
// V X=45:54:15 N   008:49:30 E
}

/*
*
AC CTR
AN LUGANO CTR
AL SFC
AH 6500ALT
DP 46:02:46 N 008:53:48 E
V D=+
V X=46:01:26 N   008:56:14 E
DB  46:02:46 N 008:53:48 E,  46:00:17 N 008:58:52 E
DP 46:00:17 N 008:58:52 E
DP 45:53:03 N 008:52:59 E
V D=+
V X=45:54:15 N   008:49:30 E
DB  45:53:03 N 008:52:59 E,  45:55:49 N 008:46:20 E
DP 45:55:49 N 008:46:20 E
DP 46:02:46 N 008:53:48 E
*/

