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
		WebMap(QWidget *pParent);

		~WebMap();

		void getPointFromLatLon(qreal lat, qreal lon, int &x, int &y);

		void getLatLonFromPoint(int x, int y, qreal &lat, qreal &lon);

		void gotoLocation(const QString &local);

	signals:
		void mapReady();

	public slots:
		void mouseSlot(QGraphicsSceneMouseEvent *pEvent);

		void wheelSlot(QGraphicsSceneWheelEvent *pEvent);

	protected:
		void resizeEvent(QResizeEvent *pEvent);

	private:
		enum {ProgressW = 200, ProgressH = 15};

		QNetworkAccessManager *m_pNetMgr;
		QProgressBar *m_pProgress;

		void zoomIn();

		void zoomOut();

		void setCenter(qreal lat, qreal lon);

		void setSize(uint width, uint height);

	private slots:
		void loadFinished(bool ok);

		void replyFinished(QNetworkReply *pReply);
};

#endif
