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

#ifndef WebMapRoute_h
#define WebMapRoute_h

#include <QObject>
#include "FlightPointList.h"
#include "Route.h"
#include "WayPoint.h"

class WebMap;

class WebMapRoute: public QObject
{
	Q_OBJECT

	public:
		WebMapRoute(WebMap *pWebMap);

		~WebMapRoute();

		void init();

		void populateObject();

    void setName(const QString &name);

    QString name() const;

    Route::Type type() const;

		void setTurnPointList(const WayPoint::WayPointListType &tpList);

		void turnPointList(WayPoint::WayPointListType &tpList) const;

    void setEditable(bool en);

/**
    void setRouteToStore(Route *pRoute);

	protected slots:
    void beginSaveRoute();

    void saveRoute(int id, const QString &name, int type);

		void saveWayPoint(const QString &name, double lat, double lon, int alt);

    void endSaveRoute();
*/

	private:
		WebMap *m_pWebMap;
		Route *m_pRoute;
};

#endif
