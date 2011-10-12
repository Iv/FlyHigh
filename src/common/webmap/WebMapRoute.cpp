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
}

WebMapRoute::~WebMapRoute()
{
}

void WebMapRoute::init()
{
	QString code = "rt_init();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMapRoute::setName(const QString &name)
{
	QString code = "rt_setName('%1');";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(name));
}

QString WebMapRoute::getName()
{
	QString code = "rt_getName();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();

	return pFrame->evaluateJavaScript(code).toString();
}

void WebMapRoute::setTurnPointList(const WayPoint::WayPointListType &tpList)
{
	QString code = "rt_setTurnPointList('%1');";
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

void WebMapRoute::getTurnPointList(WayPoint::WayPointListType &tpList)
{
	QString code = "rt_getTurnPointList();";
	QWebFrame *pFrame;
	QVariantList list;
	QVariantList::iterator it;
	QVariantList::iterator latLngIt;
	WayPoint wp;
	QString name;
	int wpNr = 0;

	pFrame = m_pWebMap->page()->mainFrame();
	list = pFrame->evaluateJavaScript(code).toList();
	tpList.clear();

	name = getName();
	name += "_%1";

	for(it=list.begin(); it!=list.end(); it++)
	{
		if((*it).toList().size() == 2)
		{
			wp.setName(name.arg(wpNr));
			wp.setLatitude((*it).toList().at(0).toDouble());
			wp.setLongitude((*it).toList().at(1).toDouble());
			wpNr++;
			tpList.push_back(wp);
		}
	}
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
	QString code = "rt_setLocation('%1');";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(loc));
}

QString WebMapRoute::getLocation()
{
	QString code = "rt_getLocation();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();

	return pFrame->evaluateJavaScript(code).toString();
}

void WebMapRoute::setFlightType(const QString &flightType)
{
	QString code = "rt_setFlightType('%1');";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(flightType));
}

QString WebMapRoute::getFlightType() const
{
	QString code = "rt_getFlightType();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();

	return pFrame->evaluateJavaScript(code).toString();
}
