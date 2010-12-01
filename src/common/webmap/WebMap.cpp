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
#include "FlightPointList.h"
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
}

WebMap::~WebMap()
{
}

void WebMap::loadMap()
{
	m_mapReady = false;
	load(QUrl("qrc:/webmap_analysis.html"));
}

void WebMap::initialize(qreal north, qreal east, qreal south, qreal west)
{
	QString code = "initialize(%1, %2, %3);";
	QWebFrame *pFrame;
	qreal lat;
	qreal lon;
	int zoom;

	getCenterAndZoom(north, east, south, west, lat, lon, zoom);
	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(lat).arg(lon).arg(zoom));
}

void WebMap::XCLoad()
{
	QString code = "XCLoad();";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code);
}

void WebMap::setTurnPointList(const FlightPointList &tpList)
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
		arg += argElem.arg(tpList.at(tpNr).wp.latitude()).arg(tpList.at(tpNr).wp.longitude());
	}

	arg += "]";
	pFrame->evaluateJavaScript(code.arg(arg));

	setFlightType("xc5");
	setMarkerDragable(false);
}

void WebMap::setWayPointList(const QString &encPoints, const QString &encLevels, uint weight, const QString &color)
{
	QString code = "setWayPointList('%1', '%2', %3, '%4');";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(encPoints).arg(encLevels).arg(weight).arg(color));
}

void WebMap::setMarkerDragable(bool en)
{
	QString code = "setMarkerDragable(%1);";
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

void WebMap::setFlightType(const QString &flightType)
{
	QString code = "setFlightType('%1');";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(flightType));
}


void WebMap::getPointFromLatLon(qreal lat, qreal lon, int &x, int &y)
{
	QString code = "getPointFromLatLon(%1, %2);";
	QWebFrame *pFrame;
	QVariant pos;
	QList<QVariant> posList;

	pFrame = page()->mainFrame();
	posList = pFrame->evaluateJavaScript(code.arg(lat).arg(lon)).toList();

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

void WebMap::zoomTo(qreal north, qreal east, qreal south, qreal west)
{
	QString code = "zoomTo(%1, %2, %3, %4);";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(north).arg(east).arg(south).arg(west));
}

void WebMap::setMarker(qreal lat, qreal lon, MarkerType type)
{
	QString code = "setMarker(%1, %2, '%3');";
	QWebFrame *pFrame;
	QString image;

	switch(type)
	{
		case MarkerStart:
			image = "img/MarkerS.png";
		break;
		case MarkerLand:
			image = "img/MarkerL.png";
		break;
		case MarkerTp1:
			image = "img/MarkerTp1.png";
		break;
		case MarkerTp2:
			image = "img/MarkerTp2.png";
		break;
		case MarkerTp3:
			image = "img/MarkerTp3.png";
		break;
		default:
			image = "http://www.google.com/intl/en_us/mapfiles/ms/micons/orange-dot.png";
		break;
	}

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(lat).arg(lon).arg(image));
}

bool WebMap::isMapReady() const
{
	return m_mapReady;
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
	QString code = "setMapSize(%1, %2);";
	QWebFrame *pFrame;

	pFrame = page()->mainFrame();
	pFrame->evaluateJavaScript(code.arg(width).arg(height));
}

void WebMap::getCenterAndZoom(qreal north, qreal east, qreal south, qreal west, qreal &lat, qreal &lon, int &zoom)
{
	double zoomLon;
	double zoomLat;

	lat = (north + south) / 2;
	lon = (east + west) / 2;
	zoomLon = log(360 * width() / (256 * (east - west))) / log(2);
	zoomLat = log(180 * height() / (256 * (north - south))) / log(2);
	zoom = QMIN((int)QMIN(floor(zoomLon), floor(zoomLat)), 17);
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
	QString replyStr(pReply->readAll());
	QStringList latLonStrList = replyStr.split(",");

	if(latLonStrList.size() == 4)
	{
		setCenter(latLonStrList[2].toFloat(), latLonStrList[3].toFloat());
		emit mapReady();
	}
}
