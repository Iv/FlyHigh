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

#ifndef WebMap_h
#define WebMap_h

#include <QWebView>
#include "FlightPointList.h"
#include "WayPoint.h"

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QNetworkAccessManager;
class QProgressBar;

/**
	@author Alex Graf <grafal@sourceforge.net>
*/
class WebMap: public QWebView
{
	Q_OBJECT

	public:
		typedef enum MarkerType{MarkerStart, MarkerLand, MarkerTp1, MarkerTp2, MarkerTp3, MarkerDefault}MarkerType;

		WebMap(QWidget *pParent);

		~WebMap();

		void loadMap(const QString &url);

		void XCLoad();

		void setTurnPointList(const WayPoint::WayPointListType &tpList);

		void getTurnPointList(WayPoint::WayPointListType &tpList);

		void setWayPointList(const QString &encPoints, const QString &encLevels, uint weight, const QString &color);

		void setTurnPointsDragable(bool en);

		void setLocation(const QString &loc);

		QString getLocation();

		void setName(const QString &name);

		QString getName();

		void setFlightType(const QString &flightType);

		QString getFlightType() const;

		void setFlightPointList(const FlightPointList &fpList);

		void setSogList(const FlightPointList::SogListType &sogList);

		void setVarioList(const FlightPointList::VarioListType &varioList);

		void showPlot();

		void setPlotEnable(bool en);

		bool isMapReady() const;

	signals:
		void mapReady();

		void finished(int result);

	protected:
		void resizeEvent(QResizeEvent *pEvent);

	protected slots:
		void setOk(bool ok);

	private:
		enum {ProgressW = 200, ProgressH = 15, LeftWidth = 280, PlotHeight = 140};

		QNetworkAccessManager *m_pNetMgr;
		QProgressBar *m_pProgress;
		uint m_plotEn;
		bool m_mapReady;

		void setSize(uint width, uint height);

	private slots:
		void loadFinished(bool ok);

		void replyFinished(QNetworkReply *pReply);

		void populateJavaScriptWindowObject();
};

#endif
