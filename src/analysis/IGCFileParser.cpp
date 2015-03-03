/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                *
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

#include <QBuffer>
#include <QFile>
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
				case 'B':
					parseBRecord(record);
				break;
				case 'H':
					parseHRecord(record);
				break;
				case 'I':
          parseIRecord(record);
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

void IGCFileParser::parseBRecord(const char *record)
{
	FlightPoint *pFlightPoint;
	IRecordHash::const_iterator it;
  QString rec;
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
	int altBaro;
	int altGnss;
	char northsouth;
	char eastwest;
	char valid;

	if(sscanf(record,
			"%*c%2d%2d%2d%2d%2d%3d%c%3d%2d%3d%c%c%5d%5d",
			&hh, &mm, &ss, &latdeg, &latminute, &latmindec, &northsouth, &londeg,
			&lonminute, &lonmindec, &eastwest, &valid, &altBaro, &altGnss) == 14)
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
    if(valid == 'A')
    {
      alt = altGnss;
      m_prevAlt = altGnss;
    }
    else
    {
      alt = m_prevAlt;
    }

    pFlightPoint->setAlt(alt);

    if(valid == 'A')
    {
      alt = altBaro;
      m_prevAltBaro = altBaro;
    }
    else
    {
      alt = m_prevAltBaro;
    }

    pFlightPoint->setAltBaro(alt);

    // extension TAS
    it = m_iRecordHash.find("TAS");

    if(it != m_iRecordHash.end())
    {
      int tas;

      rec = record;
      tas = rec.mid((*it).begin, (*it).size).toInt();
      pFlightPoint->setTrueAirSpeed(tas);
    }

		m_flightPointList.push_back(pFlightPoint);
	}
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

void IGCFileParser::parseIRecord(const char *record)
{
  QString rec = record;
  QString key;
  IRecord irec;
  int nofExt;
  int extNr;
  int pos;

  /*
    // I013638TAS
    Number of extensions	2 bytes	NN	Valid characters 0-9
    Start byte number	2 bytes	SS	Valid characters 0-9
    Finish byte number	2 bytes	FF	Valid characters 0-9
    3-letter Code	3 bytes	CCC	Alphanumeric, see para 7 for
    list of codes
  */
  pos = 1;
  nofExt = rec.mid(pos, 2).toInt();
  pos += 2;

  for(extNr=0; extNr<nofExt; extNr++)
  {
    irec.begin = rec.mid(pos, 2).toInt();
    pos += 2;
    irec.size = (rec.mid(pos, 2).toInt() - irec.begin);
    pos += 2;
    key = rec.mid(pos, 3);
    pos += 3;
    m_iRecordHash.insert(key, irec);
  }

  m_flightPointList.setHasTrueAirSpeed(m_iRecordHash.contains("TAS"));
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
