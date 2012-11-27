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
	QString code = "rt_init();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMapRoute::setName(const QString &name)
{
	QString code = "rt_setName('%1');";
	QString locName;
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	locName = name;
	locName.replace(QRegExp("('|\")"), "\\\\1");
	pFrame->evaluateJavaScript(code.arg(name));
}

QString WebMapRoute::name() const
{
	QString code = "rt_getName();";
	QWebFrame *pFrame;
	QString name;

	pFrame = m_pWebMap->page()->mainFrame();
	name = pFrame->evaluateJavaScript(code).toString();

	return name;
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
		arg += argElem.arg(tpList.at(tpNr).pos().lat()).arg(tpList.at(tpNr).pos().lon());
	}

	arg += "]";
	pFrame->evaluateJavaScript(code.arg(arg));
}

void WebMapRoute::turnPointList(WayPoint::WayPointListType &tpList) const
{
  QString code = "rt_getTurnPts();";
  QString wpName;
  QWebFrame *pFrame;
  QVariantList tpArray;
  QVariantList tpLatLng;
  QVariantList::iterator it;
  WayPoint wp;
  int wpNr = 0;

	wpName = "%1_" + name();
	tpList.clear();
  pFrame = m_pWebMap->page()->mainFrame();
  tpArray = pFrame->evaluateJavaScript(code).toList();

  for(it=tpArray.begin(); it!=tpArray.end(); it++)
  {
    tpLatLng = (*it).toList();

    if(tpLatLng.size() == 3)
    {
      wp.setName(wpName.arg(wpNr));
      wp.setLat(tpLatLng[0].toFloat());
      wp.setLon(tpLatLng[1].toFloat());
      wp.setAlt(tpLatLng[2].toInt());
      wp.setType(WayPoint::TypeTurnPoint);
      tpList.push_back(wp);
      wpNr++;
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

Route::Type WebMapRoute::type() const
{
	QString code = "rt_getType();";
	QWebFrame *pFrame;
	int type;

	pFrame = m_pWebMap->page()->mainFrame();
	type = pFrame->evaluateJavaScript(code).toInt();

	return (Route::Type)type;
}
