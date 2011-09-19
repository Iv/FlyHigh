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
#include <QDialog>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressBar>
#include <QWebFrame>
#include <math.h>
#include "WebMap.h"
#include "WebMapAirSpace.h"
#include "WebMapFlight.h"
#include "WebMapRoute.h"
#include "WebMapWayPoint.h"

#include <QDebug>

WebMap::WebMap(QWidget *pParent, MapType type)
	:QWebView(pParent)
{
	QWebFrame *pFrame;

  m_pAirSpace = NULL;
  m_pFlight = NULL;
  m_pRoute = NULL;
  m_pWayPoint = NULL;
	m_mapReady = false;
	m_mapType = type;
	m_pNetMgr = new QNetworkAccessManager(this);
	m_pProgress = new QProgressBar(this);
	m_pProgress->setGeometry(LeftWidth, 0, ProgressW, ProgressH);
	m_pProgress->show();

	pFrame = page()->mainFrame();
	pFrame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	pFrame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

	connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	connect(this, SIGNAL(loadProgress(int)), m_pProgress, SLOT(setValue(int)));
  connect(m_pNetMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(netReply(QNetworkReply*)));
	connect(pFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(populateObject()));
}

WebMap::~WebMap()
{
  if(m_pAirSpace != NULL)
  {
    delete m_pAirSpace;
  }

  if(m_pFlight != NULL)
  {
    delete m_pFlight;
  }

  if(m_pRoute != NULL)
  {
    delete m_pRoute;
  }

  if(m_pWayPoint != NULL)
  {
    delete m_pWayPoint;
  }
}

WebMapAirSpace* WebMap::getAirSpace()
{
  if(m_pAirSpace == NULL)
  {
    m_pAirSpace = new WebMapAirSpace(this);
  }

  return m_pAirSpace;
}

WebMapFlight* WebMap::getFlight()
{
  if(m_pFlight == NULL)
  {
    m_pFlight = new WebMapFlight(this);
  }

  return m_pFlight;
}

WebMapRoute* WebMap::getRoute()
{
  if(m_pRoute == NULL)
  {
    m_pRoute = new WebMapRoute(this);
  }

  return m_pRoute;
}

WebMapWayPoint* WebMap::getWayPoint()
{
  if(m_pWayPoint == NULL)
  {
    m_pWayPoint = new WebMapWayPoint(this);
  }

  return m_pWayPoint;
}

void WebMap::loadUrl(const QString &url)
{
	m_mapReady = false;
	load(QUrl(url));
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

	switch(m_mapType)
	{
	  case MapFlight:
      code = "setMapSize(%1, %2);";
      pFrame->evaluateJavaScript(code.arg(width - LeftWidth).arg(height - PlotHeight));
      code = "fl_setPlotSize(%1, %2);";
      pFrame->evaluateJavaScript(code.arg(width - LeftWidth).arg(PlotHeight));
	  break;
	  case MapRoute:
      code = "setMapSize(%1, %2);";
      pFrame->evaluateJavaScript(code.arg(width - LeftWidth).arg(height));
	  break;
	  case MapWayPoint:
      code = "setMapSize(%1, %2);";
      pFrame->evaluateJavaScript(code.arg(width - LeftWidth).arg(height));
	  break;
	  case MapAirSpace:
      code = "setMapPos(%1, %2);";
      pFrame->evaluateJavaScript(code.arg(0).arg(0));
	  	code = "setMapSize(%1, %2);";
      pFrame->evaluateJavaScript(code.arg(width).arg(height));
	  break;
	}
}

void WebMap::loadFinished(bool ok)
{
	m_mapReady = true;
	setSize(width(), height());
	emit mapReady();
	m_pProgress->hide();
}

void WebMap::netReply(QNetworkReply *pReply)
{
  QString code;
  QString replyStr(pReply->readAll());
  QWebFrame *pFrame;
  int id;

  id = m_netReqList.front().id;
  code = m_netReqList.front().callback + "(%1, %2);";
  m_netReqList.pop_front();
	pFrame = page()->mainFrame();
  pFrame->evaluateJavaScript(code.arg(id).arg(replyStr));
}

void WebMap::populateObject()
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

void WebMap::setLine(int line)
{
  emit lineChanged(line);
}

void WebMap::netRequest(int id, const QString &request, const QString &callback)
{
  QNetworkRequest netReq(request);
  NetRequest netReqPar;

  netReqPar.id = id;
  netReqPar.callback = callback;
  m_netReqList.push_back(netReqPar);
  m_pNetMgr->get(netReq);
}
