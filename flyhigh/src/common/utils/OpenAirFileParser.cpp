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

const uint OpenAirFileParser::m_maxSeg = 8;

OpenAirFileParser::OpenAirFileParser()
{
}

void OpenAirFileParser::parse(QByteArray &openAirData)
{
	QBuffer buff;
	QString str;
	char record[MAX_REC_SIZE];
	AirSpace airspace;
	
	buff.setBuffer(openAirData);

	if(buff.open(IO_ReadOnly))
	{
		while(buff.readLine(record, MAX_REC_SIZE) > 0)
		{
			str = record;
			
			if(str.left(2) == "AC")
			{
				parseAirspaceClass(record, airspace);
			}
			else if(str.left(2) == "AN")
			{
				parseName(record, airspace);
			}
			else if(str.left(2) == "AH")
			{
				parseHigh(record, airspace);
			}
			else if(str.left(2) == "AL")
			{
				parseLow(record, airspace);
			}
			else if(str.left(1) == "V")
			{
				parseVarAssign(record);
			}
			else if(str.left(2) == "DP")
			{
				parsePoint(record, airspace);
			}
			else if(str.left(2) == "DB")
			{
				parseArc(record, airspace);
			}
			else if(str.left(2) == "DC")
			{
				parseCircle(record, airspace);
			}
			else if((str.at(0) == '*') && (str.at(1) == '\r'))
			{
				m_airspaceList.push_back(airspace);
				airspace.edgePointList().clear();
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

void OpenAirFileParser::parseName(char *record, AirSpace &airspace)
{
	QString str = record;
	int begin;
	int end;
	
	begin = str.find(' ') + 1;
	end = str.find('\r');
	str = str.mid(begin, end-begin);

	airspace.setName(str);
}

void OpenAirFileParser::parseHigh(char *record, AirSpace &airspace)
{
	QString str = record;
	int begin;
	int end;
	
	begin = str.find(' ') + 1;
	end = str.find('\r');
	str = str.mid(begin, end-begin);

	airspace.setHigh(str);
}

void OpenAirFileParser::parseLow(char *record, AirSpace &airspace)
{
	QString str = record;
	int begin;
	int end;
	
	begin = str.find(' ') + 1;
	end = str.find('\r');
	str = str.mid(begin, end-begin);

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
	
	pos = str.find(' ') + 1;
	type = str.at(pos);
	
	if(type == 'D')
	{
			pos = str.find('=') + 1;
			m_arcDir = (str.at(pos) == '+'); // math direction
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
	double arcA;
	double arcB;
	double arcAB;
	double segArc;
	uint segNr;
	uint maxSeg;
	char pos;
	
	// vector a
	parseCoordinate(record, startPt);
	a.set(startPt.lat, startPt.lon);
	a -= centerVec;
	
	// vector b
	pos = str.find(',');
	parseCoordinate(&record[pos], endPt);
	b.set(endPt.lat, endPt.lon);
	b -= centerVec;
	
	// delta arc
	arcA = a.arc();
	arcB = b.arc();
	arcAB = deltaArc(arcA, arcB);
	maxSeg = maxSegments(arcAB);
	
	// start point
	segArc = arcAB / maxSeg;
	airspace.edgePointList().push_back(startPt);
	
	// 1st point
	//a *= 1 / cos(segArc / 2); // scale
/*	resVec = a;
	resVec.rot(segArc / 2);
	resVec += centerVec;
	pt.lat = resVec.x();
	pt.lon = resVec.y();
	airspace.edgePointList().push_back(pt);
	*/
	// other points
	for(segNr=0; segNr<maxSeg; segNr++)
	{
		resVec = a;
		resVec.rot(segArc / 2 + segNr * segArc);
		resVec += centerVec;
		pt.lat = resVec.x();
		pt.lon = resVec.y();
		airspace.edgePointList().push_back(pt);
	}
	
	// end point
//	airspace.edgePointList().push_back(endPt);
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
	int begin;
	int end;
	
	begin = str.find(' ') + 1;
	end = str.find('\r');
	str = str.mid(begin, end-begin);
	
	segArc = 2 * M_PI / m_maxSeg;
	radius = str.toDouble() * nautmil;
	startVec.setCircle(radius, 0);
	startVec *= 1 / cos(segArc / 2); // scale

	// points
	for(segNr=0; segNr<=m_maxSeg; segNr++)
	{
		resVec = startVec;
		resVec.rot(segNr*segArc);
		resVec += centerVec;
		pt.lat = resVec.x();
		pt.lon = resVec.y();
		airspace.edgePointList().push_back(pt);
	}
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
				if(isdigit(*pRider))
				{
					if(sscanf(pRider, "%2d%*c%2d%*c%2d%*c%c", &deg, &minute, &sec, &dir) == 4)
					{
						pt.lat = deg + minute / 60.0 + sec / 3600.0;
						
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
				if(isdigit(*pRider))
				{
					if(sscanf(pRider, "%3d%*c%2d%*c%2d%*c%c", &deg, &minute, &sec, &dir) == 4)
					{
						pt.lon = deg + minute / 60.0 + sec / 3600.0;
						
						if(dir == 'W')
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
