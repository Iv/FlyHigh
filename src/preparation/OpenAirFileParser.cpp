/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#include <QFile>
#include <QBuffer>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "AirSpace.h"
#include "AirSpaceList.h"
#include "OpenAir.h"
#include "OpenAirFileParser.h"
#include "OpenAirItem.h"

OpenAirFileParser::OpenAirFileParser()
{
}

OpenAirFileParser::~OpenAirFileParser()
{
  clearOpenAirList();
}

bool OpenAirFileParser::parse(const QString &fileName, AirSpaceList &airspaceList)
{
  QFile file(fileName);
  QTextStream inStream(&file);
  OpenAir *pOpenAir = NULL;
  AirSpace *pAirSpace;
  OpenAirList::iterator it;
	QString strValue;
  QString line;
  QByteArray byteLine;
	char* pRecord;
	int id = 0;
	float alt;
  bool success;

  airspaceList.clear();
  clearOpenAirList();
  success = file.open(QIODevice::ReadOnly | QIODevice::Text);

	if(success)
	{
    line = inStream.readLine();

    while(!line.isNull())
    {
      byteLine = line.toAscii();
      pRecord = byteLine.data();

			if(strncmp(pRecord, "AC", 2) == 0)
			{
				pOpenAir = new OpenAir();
        m_arcDir = true; // reset
        id++;
				pOpenAir->setId(id);
				m_airspaceList.push_back(pOpenAir);
				parseString(pRecord, strValue);
				pOpenAir->setAirspaceClass(strValue);
			}
			else if(strncmp(pRecord, "AN", 2) == 0)
			{
				if(pOpenAir != NULL)
				{
					parseString(pRecord, strValue);
					pOpenAir->setName(strValue);
				}
			}
			else if(strncmp(pRecord, "AH", 2) == 0)
			{
				if(pOpenAir != NULL)
				{
					parseString(pRecord, strValue);
          parseAlt(strValue, alt);
					pOpenAir->setHigh(alt);
				}
			}
			else if(strncmp(pRecord, "AL", 2) == 0)
			{
				if(pOpenAir != NULL)
				{
					parseString(pRecord, strValue);
          parseAlt(strValue, alt);
					pOpenAir->setLow(alt);
				}
			}
			else if(strncmp(pRecord, "V", 1) == 0)
			{
				parseVarAssign(pRecord);
			}
			else if(strncmp(pRecord, "DP", 2) == 0)
			{
				if(pOpenAir != NULL)
				{
					parsePoint(pRecord, pOpenAir);
				}
			}
			else if(strncmp(pRecord, "DB", 2) == 0)
			{
				if(pOpenAir != NULL)
				{
					parseArc(pRecord, pOpenAir);
				}
			}
			else if(strncmp(pRecord, "DC", 2) == 0)
			{
				if(pOpenAir != NULL)
				{
					parseCircle(pRecord, pOpenAir);
				}
			}

      line = inStream.readLine();
		}

    for(it=m_airspaceList.begin(); it!=m_airspaceList.end(); it++)
    {
      pAirSpace = new AirSpace();
      pAirSpace->setId((*it)->id());
      pAirSpace->setName((*it)->name());
      pAirSpace->setHigh((*it)->high());
      pAirSpace->setLow((*it)->low());
      pAirSpace->setAirspaceClass((*it)->airspaceClass());
      pAirSpace->setRemark((*it)->remark());
      pAirSpace->setBoundBox((*it)->boundBox());
      (*it)->createPointList(pAirSpace->pointList());
      airspaceList.push_back(pAirSpace);
    }
	}

	return success;
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

void OpenAirFileParser::parseAirspaceClass(char *pRecord, OpenAir *pOpenAir)
{
	QString str = pRecord;
	int begin;
	int end;

        begin = str.indexOf(' ') + 1;
        end = str.indexOf('\r');
	str = str.mid(begin, end-begin);

	pOpenAir->setAirspaceClass(str);
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

void OpenAirFileParser::parsePoint(char *pRecord, OpenAir *pOpenAir)
{
// DP 46:57:47 N 007:16:59 E
	OpenAirItemPoint *pPoint;
	double lat;
	double lon;

	pPoint = new OpenAirItemPoint(OpenAirItem::Point);
	parseCoordinate(pRecord + 3, lat, lon);
	pPoint->setPos(lat, lon);
	pOpenAir->push_back(pPoint);
}

void OpenAirFileParser::parseArc(char *pRecord, OpenAir *pOpenAir)
{
	QString str = pRecord;
	OpenAirItemPoint *pCenter;
	OpenAirItemSeg *pSeg;
	double beginLat;
	double beginLon;
	double endLat;
	double endLon;

  // DB 51:13:51 N 001:47:59 W, 51:13:54 N 001:42:25 W
  // DB 46:10.92N 7:13.98E 46:13.00N 7:13.33E
  // DB 48:50:07 N 002:57:55 E,48:52:28 N 003:02:09 E
	parseCoordinate(pRecord + 3, beginLat, beginLon);
  parseCoordinate(pRecord + str.indexOf(QRegExp("[EW]")) + 2, endLat, endLon);

	// center
	pCenter = new OpenAirItemPoint(OpenAirItem::Center);
	pCenter->setPos(m_arcCenterLat, m_arcCenterLon);
	pOpenAir->push_back(pCenter);

	// start
	pSeg = new OpenAirItemSeg(OpenAirItem::StartSegment);
	pSeg->setPos(beginLat, beginLon);
	pSeg->setDir(m_arcDir);
	pOpenAir->push_back(pSeg);

	//stop
	pSeg = new OpenAirItemSeg(OpenAirItem::StopSegment);
	pSeg->setPos(endLat, endLon);
	pSeg->setDir(m_arcDir);
	pOpenAir->push_back(pSeg);
}

void OpenAirFileParser::parseCircle(char *pRecord, OpenAir *pOpenAir)
{
	OpenAirItemCircle *pCircle;
	uint radius;

	pCircle = new OpenAirItemCircle();
	pCircle->setPos(m_arcCenterLat, m_arcCenterLon);
	radius = LatLng::meters(atof(pRecord + 3));
	pCircle->setRadius(radius);
	pOpenAir->push_back(pCircle);
}

bool OpenAirFileParser::parseCoordinate(char *pRecord, double &lat, double &lon)
{
  // DB 48:47.900N,007:45.200E,48:46.500N,007:49.000E
  // DB 47:28:55 N   007:23:42 E , 47:45:37 N   007:20:26 E
  QString str;
  QString coord;
  QStringList strList;
  int begin;
  int index;
  bool success;

  str = pRecord;
  index = str.indexOf(QRegExp("[NS]"));
  success = ((index > 0) && (index < str.size()));

  if(success)
  {
    coord = str.left(index);
    strList = coord.split(':');
    success = (strList.size() == 3);

    if((strList.size() == 3))
    {
      lat = strList[0].toInt() + strList[1].toInt() / 60.0 + strList[2].toInt() / 3600.0;
      success = true;
    }
    else if(strList.size() == 2)
    {
      lat = strList[0].toInt() + strList[1].toFloat() / 60.0;
      success = true;
    }

    if(str.at(index) == 'S')
    {
      lat *= -1;
    }
  }

  if(success)
  {
    begin = (index + 2);
    index = str.indexOf(QRegExp("[EW]"), begin);
    success = ((index > 0) && (index < str.size()));

    if(success)
    {
      coord = str.mid(begin, (index - begin));
      strList = coord.split(':');

      if(strList.size() == 3)
      {
        lon = strList[0].toInt() + strList[1].toInt() / 60.0 + strList[2].toInt() / 3600.0;
        success = true;
      }
      else if(strList.size() == 2)
      {
        lon = strList[0].toInt() + strList[1].toFloat() / 60.0;
        success = true;
      }

      if(str.at(index) == 'W')
      {
        lon *= -1;
      }
    }
  }

  return success;
}

bool OpenAirFileParser::parseAlt(const QString &str, float &alt)
{
  QString trimStr;
  bool success = true;

  trimStr = str.trimmed();

  // all values in feets. 1 foot = 12 inches = 30.48 cm = 0.3048 m,
  if((trimStr == "SFC") || (trimStr == "GND"))
  {
    alt = 0;
  }
  else if(trimStr.startsWith("UNLIM", Qt::CaseInsensitive))
  {
    alt = 10000;
  }
  else if(trimStr.startsWith("FL", Qt::CaseInsensitive))
  {
    // FL is in 1/100 feets
    alt = trimStr.right(trimStr.size() - 2).toFloat() * 30.48;
  }
  else if(trimStr.endsWith("FT MSL", Qt::CaseInsensitive))
  {
    alt = trimStr.left(trimStr.size() - 6).toFloat() * 0.3048;
  }
  else if(trimStr.endsWith("F AMSL", Qt::CaseInsensitive) ||
          trimStr.endsWith("FTAMSL", Qt::CaseInsensitive))
  {
    alt = trimStr.left(trimStr.size() - 5).toFloat() * 0.3048;
  }
  else if(trimStr.endsWith("FT", Qt::CaseInsensitive))
  {
    alt = trimStr.left(trimStr.size() - 2).toFloat() * 0.3048;
  }
  else if(trimStr.endsWith("ALT", Qt::CaseInsensitive) ||
          trimStr.endsWith("MSL", Qt::CaseInsensitive))
  {
    alt = trimStr.left(trimStr.size() - 3).toFloat() * 0.3048;
  }
  else if(trimStr.endsWith("FT AGL", Qt::CaseInsensitive) ||
          trimStr.endsWith("AGL", Qt::CaseInsensitive) ||
          trimStr.endsWith("M AGL", Qt::CaseInsensitive))
  {
    qDebug() << "warning: no elevation avail:" << trimStr;
    alt = 0; // alt = trimStr.toFloat() + elevation;
    success = false;
  }
  else
  {
    alt = trimStr.toFloat() * 0.3048;
  }

  alt = floor(alt / 50) * 50;

  return success;
}

void OpenAirFileParser::clearOpenAirList()
{
  OpenAirList::iterator it;

  for(it=m_airspaceList.begin(); it!=m_airspaceList.end(); it++)
  {
    delete (*it);
  }

  m_airspaceList.clear();
}
