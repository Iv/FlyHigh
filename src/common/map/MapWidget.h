/***************************************************************************
 *   Copyright (C) 2012 by Alex Graf                                       *
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

#ifndef _MapWidget_h
#define _MapWidget_h

#include <QPoint>
#include <QVector>
#include <QWidget>
#include "WayPoint.h"

class QPaintEvent;
class QScrollArea;
class Map;

class MapWidget: public QWidget
{
  public:
    MapWidget(QScrollArea *pScrollArea);

    ~MapWidget();

    /**
      Shows waypoints on map.
    */
    void showWayPointList(WayPoint::WayPointListType &wpList);

    /**
      Zooms map out. Does not care about same location.
    */
    void zoomOut();

    /**
      Zooms map in. Does not care about same location.
    */
    void zoomIn();

    /**
      Zooms map at pt. This means that pt is on the same location after zoom.
      @param pt Point to where to zoom.
      @param in true = zoom in
    */
    void zoom(const QPoint &pt, bool in);

  protected:
    void paintEvent(QPaintEvent *pEvent);

  private:
    typedef QVector<QPoint> PointArray;

    QScrollArea *m_pScrollArea;
    Map *m_pMap;
    QPoint m_prevMousePos;
		WayPoint::WayPointListType m_wpList;
		PointArray m_wayPoints;
		int m_maxAlt;
		int m_minAlt;

		void zoom(bool in);

		void recalcWayPoints();
};

#endif
