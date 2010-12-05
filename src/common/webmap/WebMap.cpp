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
#include "WebMap.h"

WebMap::WebMap(QWidget *pParent)
	:QWebView(pParent)
{
	QWebFrame *pFrame;

	m_mapReady = false;
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
	QString code = "setMapSize(%1, %2);";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(width).arg(height));
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

	printf("setOk %i\n", ok);
}
