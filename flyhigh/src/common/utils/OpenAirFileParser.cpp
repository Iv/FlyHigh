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
#include "OpenAirFileParser.h"

#define MAX_REC_SIZE 50

OpenAirFileParser::OpenAirFileParser()
{
}

void OpenAirFileParser::parse(QByteArray &openAirData)
{
	QBuffer buff;
	char record[MAX_REC_SIZE];
	AirSpace airspace;
	
	buff.setBuffer(openAirData);

	if(buff.open(IO_ReadOnly))
	{
		while(buff.readLine(record, MAX_REC_SIZE) > 0)
		{
			switch(recordType(record))
			{
				case NewAirspace:
				break;
				case Class:
					parseClass(record, airspace);
				break;
				case High:
				break;
				case Low:
				break;
				case Variable:
				break;
				case Point:
				break;
				case Arc:
				break;
				case Circle:
				break;
				default:
				break;
			}
		}
		buff.close();
	}
}

AirSpace::AirSpaceListType& OpenAirFileParser::airspaceList()
{
	return m_airspaceList;
}

OpenAirFileParser::RecordType OpenAirFileParser::recordType(char *record)
{
	QString rec = record;
	QString strType;
	RecordType type = Unknown;
	int pos;
	
	/*AC GP
AN SCHAFFHAUSEN NORD
AL 0ALT
AH 6562ALT
DP 47:43:59 N 008:52:49 E
V D=+
*/
	
	pos = rec.find(' ');
	strType = rec.left(pos);
	
	if(strType == "*")
	{
		type = NewAirspace;
	}
	else if(strType == "AC")
	{
		type = Class;
	}
	else if(strType == "AN")
	{
		type = Name;
	}
	else if(strType == "AH")
	{
		type = High;
	}
	else if(strType == "AL")
	{
		type = Low;
	}
	else if(strType == "V")
	{
		type = Variable;
	}
	else if(strType == "DP")
	{
		type = Point;
	}
	else if(strType == "DB")
	{
		type = Arc;
	}
	else if(strType == "DC")
	{
		type = Circle;
	}
	
	return type;
}

void OpenAirFileParser::parseClass(char *record, AirSpace &airspace)
{
//AC GP
}

void OpenAirFileParser::parseName(char *record, AirSpace &airspace)
{
//AN SCHAFFHAUSEN NORD
}

void OpenAirFileParser::parseHigh(char *record, AirSpace &airspace)
{
//AH 6562ALT
}

void OpenAirFileParser::parseLow(char *record, AirSpace &airspace)
{
//AL 0ALT
}

void OpenAirFileParser::parsePoint(char *record, AirSpace &airspace)
{
// DP 46:57:47 N 007:16:59 E

}

void OpenAirFileParser::parseArc(char *record, bool cw, AirSpace::EdgePointType &center, AirSpace &airspace)
{
/*V D=+
V X=45:54:15 N   008:49:30 E
DB  45:53:03 N 008:52:59 E,  45:55:49 N 008:46:20 E*/
}

void OpenAirFileParser::parseCircle(char *record, AirSpace::EdgePointType &center, AirSpace &airspace)
{
/*V D=+
V X=45:54:15 N   008:49:30 E
DC  45:53:03 N 008:52:59 E,  45:55:49 N 008:46:20 E*/
}

bool OpenAirFileParser::parseCoordinate(char *record, double &lat, double &lon)
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
						lat = deg + minute / 60.0 + sec / 60000.0;
						
						if(dir == 'S')
						{
							lat *= -1;
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
						lon = deg + minute / 60.0 + sec / 60000.0;
						
						if(dir == 'E')
						{
							lon *= -1;
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

