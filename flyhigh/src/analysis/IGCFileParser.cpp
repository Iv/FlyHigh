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
 
#include <qbuffer.h>
#include <qfile.h>
#include "IGCFileParser.h"

IGCFileParser::IGCFileParser()
{
	m_model = "";
}

void IGCFileParser::parse(QByteArray &igcData)
{
	#define MAX_REC_SIZE 50
	QBuffer buff;
	char record[MAX_REC_SIZE];
	
#ifdef _DEBUG
	QFile debugFile("debugFile.igc");
	debugFile.open(IO_WriteOnly);
	debugFile.writeBlock(igcData);
	debugFile.close();
#endif

	buff.setBuffer(igcData);

	if(buff.open(IO_ReadOnly))
	{
		while(buff.readLine(record, MAX_REC_SIZE) > 0)
		{
			switch(*record)
			{
				case 'H':
					parseHRecord(record);
				break;
				case 'B':
					parseBRecord(record, true);
				break;
				default:
				break;
			}
		}
		buff.close();
	}
}

const QString& IGCFileParser::model()
{
	return m_model;
}

const QDate& IGCFileParser::date()
{
	return m_date;
}

FlightPointList& IGCFileParser::flightPointList()
{
	return m_flightPointList;
}

void IGCFileParser::parseHRecord(const char *record)
{
	QString rec = record;
	QString subType = rec.mid(2, 3);
	int y;
	int m;
	int d;
	int start;
	int end;
	uint length;
	
	if(subType == "GTY") // Glider
	{
		// HFGTYGLIDERTYPE:Spirit
		start = rec.find(':') + 1;
		length = rec.length() - 2; // without \r\n
		
		// strip white space @ end
		for(end=length;end>0;end--)
		{
			if(rec[end-1] != ' ')
			{
				break;
			}
		}
		m_model = rec.mid(start, end-start);
	}
	else if(subType == "DTE") // Date
	{
		// HFDTE221204
		if(sscanf(record,
				"%*5c%2d%2d%2d",
				&d, &m, &y) == 3)
		{
			y += 2000;
			m_date.setYMD(y, m, d);
		}
	}
}

void IGCFileParser::parseBRecord(const char *record, bool gpsAlt)
{
	FlightPointList::FlightPointType flightPoint;
	double lat;
	double lon;
	int alt;
	int hh;
	int mm;
	int ss;
	int latdeg;
	int latminute;
	int latmindec;
	int londeg;
	int lonminute;
	int lonmindec;
	int altPress;
	int altGPS;
	char northsouth;
	char eastwest;
	char valid;

	if(sscanf(record,
			"%*c%2d%2d%2d%2d%2d%3d%c%3d%2d%3d%c%c%5d%5d",
			&hh, &mm, &ss, &latdeg, &latminute, &latmindec, &northsouth, &londeg,
			&lonminute, &lonmindec, &eastwest, &valid, &altPress, &altGPS) == 14)
	{
		// time
		flightPoint.time.setHMS(hh, mm, ss);
		
		// latitude
		lat = ((double) latdeg + ((double) latminute / 60.0) + ((double) latmindec / 60000.0));
		
		if(northsouth == 'S')
		{
			lat *= -1.0;
		}
		
		// longitude
		lon = ((double) londeg + ((double) lonminute / 60.0) + ((double) lonmindec / 60000.0));
		
		if(eastwest == 'W')
		{
			lon *= -1.0;
		}
		
		// altitude
		if(gpsAlt)
		{
			alt = altGPS;
		}
		else
		{
			alt = altPress;
		}

		flightPoint.wp.setCoordinates(lat, lon, alt);
		m_flightPointList.add(flightPoint);
	}
}
