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

#ifndef WebMap_h
#define WebMap_h

#include <QVector>
#include <QWebView>

class QNetworkAccessManager;
class QProgressBar;
class WebMapAirSpace;
class WebMapFlight;
class WebMapRoute;
class WebMapWayPoint;

class WebMap: public QWebView
{
	Q_OBJECT

	public:
		typedef enum MarkerType{MarkerStart, MarkerLand, MarkerTp1, MarkerTp2, MarkerTp3, MarkerDefault}MarkerType;

		typedef enum MapType{MapFlight, MapRoute, MapWayPoint, MapAirSpace}MapType;

		WebMap(QWidget *pParent, MapType type);

		~WebMap();

		WebMapAirSpace* getAirSpace();

		WebMapFlight* getFlight();

		WebMapRoute* getRoute();

		WebMapWayPoint* getWayPoint();

		void loadUrl(const QString &url);

		bool isMapReady() const;

		void setSize(uint width, uint height);

	signals:
		void appReady();

		void mapReady();

		void finished(int result);

		void lineChanged(int line);

	protected:
		void resizeEvent(QResizeEvent *pEvent);

	protected slots:
		void setOk(bool ok);

		void setLine(int line);

		void netRequest(int id, const QString &request, const QString &callback);

	private:
		enum {ProgressW = 200, ProgressH = 15, LeftWidth = 280, PlotHeight = 140, Margin = 5};

		typedef struct NetRequest
		{
      int id;
      QString callback;
		}NetRequest;

		typedef QVector<NetRequest> NetRequestList;

    WebMapAirSpace *m_pAirSpace;
    WebMapFlight *m_pFlight;
    WebMapRoute *m_pRoute;
    WebMapWayPoint *m_pWayPoint;
		QNetworkAccessManager *m_pNetMgr;
		QProgressBar *m_pProgress;
		NetRequestList m_netReqList;
    MapType m_mapType;
		bool m_mapReady;

	private slots:
		void loadFinished(bool ok);

		void netReply(QNetworkReply *pReply);

		void populateObject();
};

#endif
