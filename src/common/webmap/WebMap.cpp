/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
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
#include <QDialog>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressBar>
#include <QWebFrame>
#include <math.h>
#include "FlightPointList.h"
#include "PolyLineEncoder.h"
#include "WebMap.h"

WebMap::WebMap(QWidget *pParent)
	:QWebView(pParent)
{
	QWebFrame *pFrame;

	m_mapReady = false;
	m_plotEn = false;
	m_pNetMgr = new QNetworkAccessManager(this);
	m_pProgress = new QProgressBar(this);
	m_pProgress->setGeometry(0, 0, ProgressW, ProgressH);
	m_pProgress->show();

	pFrame = page()->mainFrame();
	pFrame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	pFrame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

	connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	connect(m_pNetMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	connect(this, SIGNAL(loadProgress(int)), m_pProgress, SLOT(setValue(int)));
	connect(pFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(populateJavaScriptWindowObject()));
}

WebMap::~WebMap()
{
}

void WebMap::loadMap(const QString &url)
{
	m_mapReady = false;
	load(QUrl(url));
}

void WebMap::XCLoad()
{
	QString code = "XCLoad();";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMap::setTurnPointList(const WayPoint::WayPointListType &tpList)
{
	QString code = "setTurnPointList('%1');";
	QString arg = "[";
	QString argElem = "[%1,%2]";
	QWebFrame *pFrame;
	uint tpNr;
	bool first = true;

	pFrame = page()->mainFrame();

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

void WebMap::getTurnPointList(WayPoint::WayPointListType &tpList)
{
	QString code = "getTurnPointList();";
	QWebFrame *pFrame;
	QVariantList list;
	QVariantList::iterator it;
	QVariantList::iterator latLngIt;
	WayPoint wp;
	QString name;
	int wpNr = 0;

	pFrame = page()->mainFrame();
	list = pFrame->evaluateJavaScript(code).toList();
	tpList.clear();

	name = getLocation();
	name += "%1";

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

void WebMap::setWayPointList(const QString &encPoints, const QString &encLevels, uint weight, const QString &color)
{
	QString code = "setWayPointList('%1', '%2', %3, '%4');";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(encPoints).arg(encLevels).arg(weight).arg(color));
}

void WebMap::setTurnPointsDragable(bool en)
{
	QString code = "setTurnPointsDragable(%1);";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(en));
}

void WebMap::setLocation(const QString &loc)
{
	QString code = "setLocation('%1');";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(loc));
}

QString WebMap::getLocation()
{
	QString code = "getLocation();";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();

	return pFrame->evaluateJavaScript(code).toString();
}

void WebMap::setFlightType(const QString &flightType)
{
	QString code = "setFlightType('%1');";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(flightType));
}

void WebMap::setFlightPointList(const FlightPointList &fpList)
{
	QString code;
	QWebFrame *pFrame;
	WayPoint::WayPointListType wpList;
	PolyLineEncoder encoder;
	QString encPoints;
	QString encLevels;
	QString value = "%1";
	QString strTime = "";
	QString strAlt = "";
	QString strLat = "";
	QString strLon = "";
	QTime time;
	WayPoint wp;
	uint fpNr;
	uint fpListSize;
	uint start;
	uint duration;
	int minAlt;
	int maxAlt;
	bool first = true;

	fpListSize = fpList.size();

	if(fpListSize > 0)
	{
		minAlt = fpList.at(0).wp.altitude();
		maxAlt = minAlt;
		time = fpList.at(0).time;
		start = time.hour() * 3600 + time.minute() * 60 + time.second();
		time = fpList.at(fpListSize - 1).time;
		duration = time.hour() * 3600 + time.minute() * 60 + time.second() - start;

		for(fpNr=0; fpNr<fpListSize; fpNr++)
		{
			wpList.push_back(fpList.at(fpNr).wp);
	
			if(!first)
			{
				strTime += ",";
				strAlt += ",";
				strLat += ",";
				strLon += ",";
			}

			first = false;
	
			// time
			strTime += "'";
			strTime += fpList.at(fpNr).time.toString("hh:mm:ss");
			strTime += "'";

			wp = fpList.at(fpNr).wp;

			// altitude
			minAlt = qMin(wp.altitude(), minAlt);
			maxAlt = qMax(wp.altitude(), maxAlt);
			strAlt += value.arg(wp.altitude());

			// lat, lon
			strLat += value.arg(wp.latitude());
			strLon += value.arg(wp.longitude());
		}

		minAlt = floor(minAlt / 100.0) * 100;
		maxAlt = ceil(maxAlt / 100.0) * 100;

		encoder.dpEncode(wpList, encPoints, encLevels);
		setWayPointList(encPoints, encLevels, 3, "#FF0000");
	
		pFrame = page()->mainFrame();
		code = "setFlightTime([%1], %2, %3);";
		pFrame->evaluateJavaScript(code.arg(strTime).arg(start).arg(duration));
		code = "setFlightAlt([%1], %2, %3);";
		pFrame->evaluateJavaScript(code.arg(strAlt).arg(minAlt).arg(maxAlt));
		code = "setFlightLatLon([%1], [%2]);";
		pFrame->evaluateJavaScript(code.arg(strLat).arg(strLon));
	}
}

void WebMap::setSogList(const FlightPointList::SogListType &sogList)
{
	QString code = "setSog([%1]);";
	QWebFrame *pFrame;
	QString value = "%1";
	QString strSog = "";
	uint itemNr;
	uint listSize;
	bool first = true;

	listSize = sogList.size();

	if(listSize > 0)
	{
		for(itemNr=0; itemNr<listSize; itemNr++)
		{
			if(!first)
			{
				strSog += ",";
			}

			first = false;

			// sog
			strSog += value.arg(round(sogList.at(itemNr) * 10.0) / 10.0);
		}
	
		pFrame = page()->mainFrame();
		pFrame->evaluateJavaScript(code.arg(strSog));
	}
}

void WebMap::setVarioList(const FlightPointList::VarioListType &varioList)
{
	QString code = "setVario([%1]);";
	QWebFrame *pFrame;
	QString value = "%1";
	QString strVario = "";
	uint itemNr;
	uint listSize;
	bool first = true;

	listSize = varioList.size();

	if(listSize > 0)
	{
		for(itemNr=0; itemNr<listSize; itemNr++)
		{
			if(!first)
			{
				strVario += ",";
			}

			first = false;

			// sog
			strVario += value.arg(round(varioList.at(itemNr) * 10.0) / 10.0);
		}
	
		pFrame = page()->mainFrame();
		pFrame->evaluateJavaScript(code.arg(strVario));
	}
}

void WebMap::showPlot()
{
	QString code = "showPlot();";
	QWebFrame *pFrame;

	if(m_plotEn)
	{
		pFrame = page()->mainFrame();
		pFrame->evaluateJavaScript(code);
	}
}

void WebMap::setPlotEnable(bool en)
{
	m_plotEn = en;
}

bool WebMap::isMapReady() const
{
	return m_mapReady;
}

void WebMap::resizeEvent(QResizeEvent *pEvent)
{
	QWebView::resizeEvent(pEvent);
	setSize(width(), height());
}

void WebMap::setSize(uint width, uint height)
{
	QString code;
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();

	if(m_plotEn)
	{
		code = "setMapSize(%1, %2);";
		pFrame->evaluateJavaScript(code.arg(width - LeftWidth).arg(height - PlotHeight));
		code = "setPlotSize(%1, %2);";
		pFrame->evaluateJavaScript(code.arg(width - LeftWidth).arg(PlotHeight));
	}
	else
	{
		code = "setMapSize(%1, %2);";
		pFrame->evaluateJavaScript(code.arg(width - LeftWidth).arg(height));
	}
}

void WebMap::loadFinished(bool ok)
{
	m_mapReady = true;
	setSize(width(), height());
	emit mapReady();
	m_pProgress->hide();
}

void WebMap::replyFinished(QNetworkReply *pReply)
{
/*
	QString replyStr(pReply->readAll());
	QStringList latLonStrList = replyStr.split(",");

	if(latLonStrList.size() == 4)
	{
		setCenter(latLonStrList[2].toFloat(), latLonStrList[3].toFloat());
		emit mapReady();
	}
*/
}

void WebMap::populateJavaScriptWindowObject()
{
	page()->mainFrame()->addToJavaScriptWindowObject("WebMap", this);
}

void WebMap::setOk(bool ok)
{
	if(ok)
	{
		emit finished(QDialog::Accepted);
	}
	else
	{
		emit finished(QDialog::Rejected);
	}
}
