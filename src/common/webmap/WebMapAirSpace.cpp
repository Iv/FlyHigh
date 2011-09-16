/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#include <QObject>
#include <QWebFrame>
#include "AirSpace.h"
#include "WebMap.h"
#include "WebMapAirSpace.h"

#include <QDebug>

WebMapAirSpace::WebMapAirSpace(WebMap *pWebMap)
{
	m_pWebMap = pWebMap;
}

WebMapAirSpace::~WebMapAirSpace()
{
}

void WebMapAirSpace::init()
{
	QString code = "as_init();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMapAirSpace::pushAirSpace(AirSpace *pAirSpace)
{
	QString code = "as_pushAirSpace([%1], [%2], {id: %3});";
	QString value = "%1";
	QWebFrame *pFrame;
	uint wpNr;
	uint wpListSize;
	QString strLat = "";
	QString strLon = "";
	float lat;
	float lon;
  float endLat;
	float endLon;
  int id;
	bool first = true;

  pFrame = m_pWebMap->page()->mainFrame();
  pAirSpace->createPointList();
  wpListSize = pAirSpace->pointList().size();

  if(wpListSize > 0)
  {
    for(wpNr=0; wpNr<wpListSize; wpNr++)
    {
      if(!first)
      {
        strLat += ",";
        strLon += ",";
      }

      first = false;
      lat = pAirSpace->pointList().at(wpNr).latitude();
      lon = pAirSpace->pointList().at(wpNr).longitude();
      strLat += value.arg(lat);
      strLon += value.arg(lon);
    }

    lat = pAirSpace->pointList().at(0).latitude();
    lon = pAirSpace->pointList().at(0).longitude();
    endLat = pAirSpace->pointList().at(wpListSize - 1).latitude();
    endLon = pAirSpace->pointList().at(wpListSize - 1).longitude();

    if((lat != endLat) || (lon !=endLon))
    {
      strLat += ",";
      strLon += ",";
      strLat += value.arg(lat);
      strLon += value.arg(lon);
    }
  }

  id = pAirSpace->id();
  pFrame->evaluateJavaScript(code.arg(strLat).arg(strLon).arg(id));
}

void WebMapAirSpace::selectAirSpace(int nr)
{
	QString code = "as_selectAirSpace(%1);";
	QWebFrame *pFrame;

  pFrame = m_pWebMap->page()->mainFrame();
  pFrame->evaluateJavaScript(code.arg(nr));
}
