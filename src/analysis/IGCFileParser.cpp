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

#include <qbuffer.h>
#include <qfile.h>
#include "IGCFileParser.h"

IGCFileParser::IGCFileParser()
{
	m_model = "";
	m_prevAlt = 0;
}

void IGCFileParser::parse(const QByteArray &igcData)
{
	#define MAX_REC_SIZE 50
	QBuffer buff;
	char record[MAX_REC_SIZE];

	m_flightPointList.clear();
	buff.setBuffer(const_cast<QByteArray*>(&igcData));

	if(buff.open(QIODevice::ReadOnly))
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

const QString& IGCFileParser::pilot()
{
	return m_pilot;
}

const QString& IGCFileParser::model()
{
	return m_model;
}

const QString& IGCFileParser::gliderId()
{
	return m_gliderId;
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
	int y;
	int m;
	int d;

	if(strncmp(record, "HFPLTPILOT", 10) == 0) // Pilot
	{
		colonValue(record, m_pilot);
	}
	else if(strncmp(record, "HFGTYGLIDERTYPE", 15) == 0) // Glider
	{
		colonValue(record, m_model);
/*		// HFGTYGLIDERTYPE:Spirit
                start = rec.indexOf(':') + 1;
		length = rec.length() - 2; // without \r\n

		// strip white space @ end
		for(end=length;end>0;end--)
		{
			if(rec[end-1] != ' ')
			{
				break;
			}
		}
		m_model = rec.mid(start, end-start);*/
	}
	else if(strncmp(record, "HFGIDGLIDERID", 13) == 0) // Glider ID
	{
		colonValue(record, m_gliderId);
	}
	else if(strncmp(record, "HFDTE", 5) == 0) // Date
	{
		// HFDTE221204
		if(sscanf(record, "%*5c%2d%2d%2d", &d, &m, &y) == 3)
		{
			y += 2000;
      m_date.setDate(y, m, d);
		}
	}
}

void IGCFileParser::parseBRecord(const char *record, bool gpsAlt)
{
	FlightPoint *pFlightPoint;
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
    pFlightPoint = new FlightPoint();

		// time
		pFlightPoint->setTime(QTime(hh, mm, ss));

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

		pFlightPoint->setPos(LatLng(lat, lon));

		// altitude
		if(gpsAlt)
		{
		  if(valid == 'A')
		  {
        alt = altGPS;
        m_prevAlt = altGPS;
		  }
		  else
		  {
        alt = m_prevAlt;
		  }
		}
		else
		{
			alt = altPress;
		}

    pFlightPoint->setAlt(alt);
		m_flightPointList.push_back(pFlightPoint);
	}
}

void IGCFileParser::colonValue(const char *record, QString &str)
{
	QString rec = record;
	int length;
	int start;
	int end;

  start = rec.indexOf(':') + 1;
	length = rec.length() - 2; // without \r\n

	// strip white space @ end
	for(end=length;end>0;end--)
	{
		if(rec[end-1] != ' ')
		{
			break;
		}
	}

	str = rec.mid(start, end-start);
}
