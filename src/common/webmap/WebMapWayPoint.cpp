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
                 " lat: %5, lon: %6, alt: %7});";
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
  spot = wp.spot();
  country = wp.country();
  lat = wp.latitude();
  lon = wp.longitude();
  alt = wp.altitude();
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

void WebMapWayPoint::saveWayPoint(int id, const QString &name, const QString &spot,
                                      const QString &country, double lat, double lon,
                                      int alt)
{
  WayPoint wp;

  wp.setId(id);
  wp.setName(name);
  wp.setSpot(spot);
  wp.setCountry(country);
  wp.setLatitude(lat);
  wp.setLongitude(lon);
  wp.setAltitude(alt);

  emit changeWayPoint(wp);
}
