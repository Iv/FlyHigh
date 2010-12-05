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

#ifndef _WebMapFlightView_h
#define _WebMapFlightView_h

#include <QWidget>
#include "FlightPointList.h"
#include "WayPoint.h"

class Flight;
class WebMap;

class WebMapFlightView: public QWidget
{
	Q_OBJECT

	public:
		WebMapFlightView(const QString &name);

		~WebMapFlightView();

		void setTurnPointList(const WayPoint::WayPointListType &tpList);

		void setWayPointList(const WayPoint::WayPointListType &wpList);

		void setLocation(const QString &location);

		void loadMap();

	protected:
		void resizeEvent(QResizeEvent *pEvent);

	private:
		WebMap *m_pWebMap;
		WayPoint::WayPointListType m_wpList;
		WayPoint::WayPointListType m_tpList;
		QString m_location;

	public slots:
		void mapReady();
};

#endif
