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
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "AirSpaceItem.h"
#include "OpenAirFileParser.h"
#include "WayPoint.h"

#define MAX_REC_SIZE 100

OpenAirFileParser::OpenAirFileParser()
{
}

void OpenAirFileParser::parse(QByteArray &openAirData)
{
	QBuffer buff;
	char pRecord[MAX_REC_SIZE];
	AirSpace *pAirspace = NULL;
	QString strValue;
	
	m_airspaceList.clear();
	buff.setBuffer(&openAirData);

	if(buff.open(QIODevice::ReadOnly))
	{
		while(buff.readLine(pRecord, MAX_REC_SIZE) > 0)
		{
			if(strncmp(pRecord, "AC", 2) == 0)
			{
				pAirspace = new AirSpace;
				m_airspaceList.append(pAirspace);
				parseString(pRecord, strValue);
				pAirspace->setAirspaceClass(strValue);
			}
			else if(strncmp(pRecord, "AN", 2) == 0)
			{
				if(pAirspace != NULL)
				{
					parseString(pRecord, strValue);
					pAirspace->setName(strValue);
				}
			}
			else if(strncmp(pRecord, "AH", 2) == 0)
			{
				if(pAirspace != NULL)
				{
					parseString(pRecord, strValue);
					pAirspace->setHigh(strValue);
				}
			}
			else if(strncmp(pRecord, "AL", 2) == 0)
			{
				if(pAirspace != NULL)
				{
					parseString(pRecord, strValue);
					pAirspace->setLow(strValue);
				}
			}
			else if(strncmp(pRecord, "V", 1) == 0)
			{
				parseVarAssign(pRecord);
			}
			else if(strncmp(pRecord, "DP", 2) == 0)
			{
				if(pAirspace != NULL)
				{
					parsePoint(pRecord, pAirspace);
				}
			}
			else if(strncmp(pRecord, "DB", 2) == 0)
			{
				if(pAirspace != NULL)
				{
					parseArc(pRecord, pAirspace);
				}
			}
			else if(strncmp(pRecord, "DC", 2) == 0)
			{
				if(pAirspace != NULL)
				{
					parseCircle(pRecord, pAirspace);
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

void OpenAirFileParser::parseString(char *pRecord, QString &str)
{
	std::string locStr = pRecord;
	int end;

	end = locStr.rfind('\r');

	if(end < 0)
	{
		end = locStr.rfind('\n');
	}

	if(end < 0)
	{
		end = locStr.size();
	}

	str = locStr.substr(3, end - 3).c_str();
}

void OpenAirFileParser::parseHeight(char *pRecord, int &height)
{
	int strLen;
	int chNr;

	height = 0x80000000;
	pRecord += 3;
	strLen = strlen(pRecord);

	for(chNr=(strLen-1); chNr>0; chNr--)
	{
		if(isdigit(pRecord[chNr]))
		{
			height = atoi(pRecord);
			break;
		}
		else
		{
			pRecord[chNr] = 0;
		}
	}
}

void OpenAirFileParser::parseAirspaceClass(char *pRecord, AirSpace *pAirspace)
{
	QString str = pRecord;
	int begin;
	int end;
	
        begin = str.indexOf(' ') + 1;
        end = str.indexOf('\r');
	str = str.mid(begin, end-begin);

	pAirspace->setAirspaceClass(str);
}

void OpenAirFileParser::parseVarAssign(char *pRecord)
{
	pRecord += 2;

	if(strncmp(pRecord, "D=+", 3) == 0)
	{
		m_arcDir = true;
	}
	else if(strncmp(pRecord, "D=-", 3) == 0)
	{
		m_arcDir = false;
	}
	else if(strncmp(pRecord, "X=", 2) == 0)
	{
		parseCoordinate(pRecord+2, m_arcCenterLat, m_arcCenterLon);
	}
}

void OpenAirFileParser::parsePoint(char *pRecord, AirSpace *pAirspace)
{
// DP 46:57:47 N 007:16:59 E
	AirSpaceItemPoint *pPoint;
	double lat;
	double lon;
	
	pPoint = new AirSpaceItemPoint(AirSpaceItem::Point);
	parseCoordinate(pRecord + 3, lat, lon);
	pPoint->setPoint(lat, lon);
	pAirspace->airSpaceItemList().push_back(pPoint);
}

void OpenAirFileParser::parseArc(char *pRecord, AirSpace *pAirspace)
{
	QString str = pRecord;
	AirSpaceItemPoint *pCenter;
	AirSpaceItemSeg *pSeg;
	double beginLat;
	double beginLon;
	double endLat;
	double endLon;

	parseCoordinate(pRecord + 3, beginLat, beginLon);
        parseCoordinate(pRecord+str.indexOf(',')+1, endLat, endLon);

	// center
	pCenter = new AirSpaceItemPoint(AirSpaceItem::Center);
	pCenter->setPoint(m_arcCenterLat, m_arcCenterLon);
	pAirspace->airSpaceItemList().push_back(pCenter);

	// start
	pSeg = new AirSpaceItemSeg(AirSpaceItem::StartSegment);
	pSeg->setPoint(beginLat, beginLon);
	pSeg->setDir(m_arcDir);
	pAirspace->airSpaceItemList().push_back(pSeg);

	//stop
	pSeg = new AirSpaceItemSeg(AirSpaceItem::StopSegment);
	pSeg->setPoint(endLat, endLon);
	pSeg->setDir(m_arcDir);
	pAirspace->airSpaceItemList().push_back(pSeg);
}

void OpenAirFileParser::parseCircle(char *pRecord, AirSpace *pAirspace)
{
	AirSpaceItemCircle *pCircle;
	uint radius;

	pCircle = new AirSpaceItemCircle();
	pCircle->setPoint(m_arcCenterLat, m_arcCenterLon);
	radius = WayPoint::meters(atof(pRecord + 3));
	pCircle->setRadius(radius);
	pAirspace->airSpaceItemList().push_back(pCircle);
}

bool OpenAirFileParser::parseCoordinate(char *pRecord, double &latitude, double &longitude)
{
	char lat[15];
	char NS[5];
	char lon[15];
	char EW[5];
	int deg;
	int min;
	int sec;
	bool success;
	
	success = (sscanf(pRecord, "%s %1s %s %1s", lat, NS, lon, EW) == 4);
	
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
