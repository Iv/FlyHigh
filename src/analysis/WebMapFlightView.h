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
#ifndef WebMapFlightView_h
#define WebMapFlightView_h

#include <QWidget>
#include "IDataBase.h"

class QWidget;
class FlightPointList;
class WebMapLeg;
class WebMapWidget;
class WebMapTurnPoint;
class WebMapTrack;

/**
	@author Alex Graf <alex@grafitation.ch>
*/
class WebMapFlightView: public QWidget
{
	Q_OBJECT

	public:
		WebMapFlightView(const QString &name);

		~WebMapFlightView();

		void setWayPointList(const WayPoint::WayPointListType &wpList);

		void setTurnPointList(const FlightPointList &tpList, bool tri);

		void loadMap();

	protected:
		void resizeEvent(QResizeEvent *pEvent);

	private:
		WebMapWidget *m_pWebMapWidget;
		WebMapTrack *m_pTrack;

	private slots:
		void mapReady();
};

#endif
