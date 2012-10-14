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
#include "WebMap.h"
#include "WebMapRoute.h"

#include <QDebug>

WebMapRoute::WebMapRoute(WebMap *pWebMap)
{
	m_pWebMap = pWebMap;
	m_pRoute = NULL;
}

WebMapRoute::~WebMapRoute()
{
}

void WebMapRoute::init()
{
	QString code = "rt_init(720, 860);";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(m_pWebMap->width()).arg(m_pWebMap->height()));
}

void WebMapRoute::populateObject()
{
	m_pWebMap->page()->mainFrame()->addToJavaScriptWindowObject("WebMapRoute", this);
}

void WebMapRoute::setName(const QString &name)
{
	QString code = "rt_setName('%1');";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(name));
}

void WebMapRoute::setTurnPointList(const WayPoint::WayPointListType &tpList)
{
	QString code = "rt_setTurnPts(%1);";
	QString arg = "[";
	QString argElem = "[%1,%2]";
	QWebFrame *pFrame;
	int tpNr;
	bool first = true;

	pFrame = m_pWebMap->page()->mainFrame();

	for(tpNr=0; tpNr<tpList.size(); tpNr++)
	{
		if(!first)
		{
			arg += ",";
		}

		first = false;
		arg += argElem.arg(tpList.at(tpNr).latitude()).arg(tpList.at(tpNr).longitude());
	}

	arg += "]";
	pFrame->evaluateJavaScript(code.arg(arg));
}

void WebMapRoute::setEditable(bool en)
{
	QString code = "rt_setEditable(%1);";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(en));
}

void WebMapRoute::setLocation(const QString &loc)
{
/**
	QString code = "rt_setLocation('%1');";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(loc));
*/
}

QString WebMapRoute::getLocation()
{
/**
	QString code = "rt_getLocation();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();

	return pFrame->evaluateJavaScript(code).toString();
*/
  return "";
}

void WebMapRoute::setFlightType(Route::Type type)
{
/**
	QString code = "rt_setFlightType('%1');";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(type));
*/
}

void WebMapRoute::setRouteToStore(Route *pRoute)
{
  m_pRoute = pRoute;
}

void WebMapRoute::beginSaveRoute()
{
  if(m_pRoute != NULL)
  {
    m_pRoute->wayPointList().clear();
  }
}

void WebMapRoute::saveRoute(int id, const QString &name, int type)
{
  if(m_pRoute != NULL)
  {
    m_pRoute->setId(id);
    m_pRoute->setName(name);
    m_pRoute->setType((Route::Type)type);
  }
}

void WebMapRoute::saveWayPoint(const QString &name, double lat, double lon, int alt)
{
  WayPoint wp;

  if(m_pRoute != NULL)
  {
    wp.setName(name);
    wp.setLatitude(lat);
    wp.setLongitude(lon);
    wp.setAltitude(alt);
    wp.setType(WayPoint::TypeTurnPoint);
    m_pRoute->wayPointList().push_back(wp);
  }
}

void WebMapRoute::endSaveRoute()
{
  m_pRoute = NULL;
}
