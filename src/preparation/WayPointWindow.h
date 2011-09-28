/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#ifndef WayPointWindow_h
#define WayPointWindow_h

#include "TableWindow.h"
#include "IDataBase.h"

class QWidget;
class WebMapWayPointView;

class WayPointWindow: public TableWindow
{
	Q_OBJECT

	public:
		WayPointWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags, IDataBase::SourceType src);

	protected:
		void selectionChanged();

	private slots:
		void file_update();

		void file_AddToSqlDB();

		void file_AddToGps();

		void file_delete();

		void file_addNewWp();

		void file_Edit();

		void file_editWebMap();

		void file_viewWebMap();

		void wayPointViewFinished(int res);

		void wayPointChanged(const WayPoint &wp);

		void wayPointChanged(int id);

	private:
		enum Fields{Name, Country, Spot, Longitude, Latitude, Altitude, Description};

		WayPoint::WayPointListType m_wpList;
		WebMapWayPointView *m_pWayPointView;
		IDataBase *m_pDb;
		int m_lastModified;
		bool m_externSelect;

    void setWpToRow(uint row, WayPoint &wp);
};

#endif
