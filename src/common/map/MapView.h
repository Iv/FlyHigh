//Added by qt3to4:
#include <QKeyEvent>
/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                         *
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

#ifndef _MapView_h
#define _MapView_h

class Map;

#include <vector>
#include <q3scrollview.h>
#include <qpixmap.h>
#include <q3pointarray.h>
#include "WayPoint.h"

class MapView: public Q3ScrollView
{
	public:
          MapView(QWidget *parent=0, const char *name=0, Qt::WindowFlags wFlags=0);
		virtual ~MapView();

		void shwoWayPointList(WayPoint::WayPointListType &wpList);

	protected:
		void keyPressEvent(QKeyEvent * e);
		void drawContents(QPainter* p, int cx, int cy, int cw, int ch);

	private:
		Map *m_pMap;
		WayPoint::WayPointListType m_wpList;
		Q3PointArray m_wayPoints;
		int m_maxAlt;
		int m_minAlt;

		void recalcWayPoints();
};

#endif
