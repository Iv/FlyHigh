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
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressBar>
#include <QTimer>
#include <QWebFrame>
#include <math.h>
#include "WebMap.h"

WebMap::WebMap(QWidget *pParent)
	:QWebView(pParent)
{
	QWebFrame *pFrame;

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
}

WebMap::~WebMap()
{
}

void WebMap::getPointFromLatLon(qreal lat, qreal lng, int &x, int &y)
{
	QString code = "getPointFromLatLon(%1, %2);";
	QWebFrame *pFrame;
	QVariant pos;
	QList<QVariant> posList;

	pFrame = page()->mainFrame();
	posList = pFrame->evaluateJavaScript(code.arg(lat).arg(lng)).toList();

	if(posList.size() == 2)
	{
		x = posList[0].toInt();
		y = posList[1].toInt();
	}
}

void WebMap::getLatLonFromPoint(int x, int y, qreal &lat, qreal &lon)
{
	QString code = "getLatLonFromPoint(%1, %2);";
	QWebFrame *pFrame;
	QVariant pos;
	QList<QVariant> latLonList;

	pFrame = page()->mainFrame();
	latLonList = pFrame->evaluateJavaScript(code.arg(x).arg(y)).toList();

	if(latLonList.size() == 2)
	{
		lat = latLonList[0].toDouble();
		lon = latLonList[1].toDouble();
	}
}

void WebMap::gotoLocation(const QString &local)
{
	QString requestStr(tr("http://maps.google.com/maps/geo?q=%1&output=%2&key=%3")
			.arg(local)
			.arg("csv")
			.arg("GOOGLE_MAPS_KEY") );

	m_pNetMgr->get(QNetworkRequest(requestStr));
}

void WebMap::mouseSlot(QGraphicsSceneMouseEvent *pEvent)
{
	QPoint pos((int)pEvent->scenePos().x(), (int)pEvent->scenePos().y());

	switch(pEvent->type())
	{
		case QEvent::GraphicsSceneMouseMove:
		{
			QMouseEvent event(QEvent::MouseMove, pos, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
			QWebView::mouseMoveEvent(&event);
		}
		break;
		case QEvent::GraphicsSceneMousePress:
		{
			QMouseEvent event(QEvent::MouseButtonPress, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
			QWebView::mousePressEvent(&event);
		}
		break;
		case QEvent::GraphicsSceneMouseRelease:
		{
			QMouseEvent event(QEvent::MouseButtonRelease, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
			QWebView::mouseReleaseEvent(&event);
		}
		break;
		default:
		break;
	}
}

void WebMap::wheelSlot(QGraphicsSceneWheelEvent *pEvent)
{
	if(pEvent->delta() > 0)
	{
		zoomIn();
	}
	else if(pEvent->delta() < 0)
	{
		zoomOut();
	}
}

void WebMap::resizeEvent(QResizeEvent *pEvent)
{
	QWebView::resizeEvent(pEvent);
	setSize(width(), height());
}

void WebMap::zoomIn()
{
	QString code = "zoomIn();";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMap::zoomOut()
{
	QString code = "zoomOut();";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMap::setCenter(qreal lat, qreal lon)
{
	QString code = "setCenter(%1, %2);";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(lat).arg(lon));
}

void WebMap::setSize(uint width, uint height)
{
	QString code = "setSize(%1, %2);";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(width).arg(height));
}

void WebMap::loadFinished(bool ok)
{
	emit mapReady();
	m_pProgress->hide();
}

void WebMap::replyFinished(QNetworkReply *pReply)
{
	QString replyStr(pReply->readAll());
	QStringList latLonStrList = replyStr.split(",");

	if(latLonStrList.size() == 4)
	{
		setCenter(latLonStrList[2].toFloat(), latLonStrList[3].toFloat());
		emit mapReady();
	}
}

#if 0
void WebMap::emitBounds()
{
	typedef QWebMap<QString, QVariant> BoundsWebMap;

	enum {NofBounds = 4};

	QWebFrame *pFrame;
	QVariant ret;
	BoundsWebMap mapNeSw;
	BoundsWebMap mapLatLon;
	BoundsWebMap::iterator itNeSw;
	BoundsWebMap::iterator itLatLon;
	qreal bounds[NofBounds];
	uint boundNr = 0;

	pFrame = page()->mainFrame();
	ret = pFrame->evaluateJavaScript("map.getBounds();");
	mapNeSw = ret.toWebMap();

	for(itNeSw=mapNeSw.begin(); itNeSw!=mapNeSw.end(); itNeSw++)
	{
		if(boundNr >= NofBounds)
		{
			break;
		}

		mapLatLon = (*itNeSw).toWebMap();

		for(itLatLon=mapLatLon.begin(); itLatLon!=mapLatLon.end(); itLatLon++)
		{
			bounds[boundNr] = (*itLatLon).toDouble() * 180 / M_PI;
			boundNr++;
		}
	}

	emit newBounds(bounds[0], bounds[1], bounds[2], bounds[3]);
}

void WebMap::getBounds()
{
	QWebFrame *pFrame;
	QVariant ret;
	QString str = "hello";

	pFrame = page()->mainFrame();

/*
ret = pFrame->evaluateJavaScript("test()");
str = ret.toString();
printf("return %s\n", str.toAscii().data());
*/

/*
	ret = pFrame->evaluateJavaScript("test2();");

	foreach (QVariant value, map)
	{
		printf("return %f\n", value.toDouble());
	}
*/

	ret = pFrame->evaluateJavaScript("map.getBounds();");

	QWebMap<QString, QVariant> mapNESW = ret.toWebMap();

	foreach(QVariant value, mapNESW)
	{
		QWebMap<QString, QVariant> mapLatLon = value.toWebMap();

		foreach(QVariant latLon, mapLatLon)
		{
			printf("return %f\n", latLon.toDouble() * 180 / M_PI);
		}
	}
}
#endif
