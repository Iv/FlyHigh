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

#include <QWebFrame>
#include "WebMap.h"
#include "WebMapWayPoint.h"

#include <QDebug>

WebMapWayPoint::WebMapWayPoint(WebMap *pWebMap)
{
	m_pWebMap = pWebMap;
}

WebMapWayPoint::~WebMapWayPoint()
{
}

void WebMapWayPoint::init()
{
	QString code = "wp_init();";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMapWayPoint::pushWayPoint(const WayPoint &wp)
{
	QString code = "wp_pushWayPoint({id: %1, name: '%2', spot: '%3', country: '%4',"
                 " lat: %5, lng: %6, alt: %7});";
	QWebFrame *pFrame;
	QString name;
	QString country;
	QString spot;
	float lat;
	float lon;
	int id;
	int alt;

  pFrame = m_pWebMap->page()->mainFrame();
  id = wp.id();
  name = wp.name();
  name.replace(QRegExp("('|\")"), "\\\\1");
  spot = wp.spot();
  spot.replace(QRegExp("('|\")"), "\\\\1");
  country = wp.country();
  country.replace(QRegExp("('|\")"), "\\\\1");
  lat = wp.lat();
  lon = wp.lon();
  alt = wp.alt();
  pFrame->evaluateJavaScript(code.arg(id).arg(name).arg(spot).arg(country)
                             .arg(lat).arg(lon).arg(alt));
}

void WebMapWayPoint::selectWayPoint(uint id)
{
	QString code = "wp_selectWayPoint(%1);";
	QWebFrame *pFrame;

  pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(id));
}

void WebMapWayPoint::populateObject()
{
	m_pWebMap->page()->mainFrame()->addToJavaScriptWindowObject("WebMapWayPoint", this);
}

void WebMapWayPoint::setEditable(bool en)
{
	QString code = "wp_setEditable(%1);";
	QWebFrame *pFrame;

	pFrame = m_pWebMap->page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(en));
}

bool WebMapWayPoint::getNextModified(WayPoint &wp)
{
  QString code = "wp_getNextModified();";
  QWebFrame *pFrame;
	QVariantMap wpMap;
	bool valid;

	pFrame = m_pWebMap->page()->mainFrame();
	wpMap = pFrame->evaluateJavaScript(code).toMap();
	valid = !wpMap.empty();

  if(valid)
	{
    wp.setId(wpMap["id"].toInt());
    wp.setName(wpMap["name"].toString());
    wp.setSpot(wpMap["spot"].toString());
    wp.setCountry(wpMap["country"].toString());
    wp.setCoordinates(wpMap["lat"].toDouble(), wpMap["lng"].toDouble(), wpMap["alt"].toInt());
	}

	return valid;
}
