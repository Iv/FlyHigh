/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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
#ifndef RouteWindow_h
#define RouteWindow_h

#include "TableWindow.h"
#include "IDataBase.h"
#include "Route.h"

class QWidget;

class RouteWindow : public TableWindow
{
	Q_OBJECT
	
	public:
		RouteWindow(QWidget* parent, const char* name, int wflags, IDataBase::SourceType src);

	protected:
		void timerEvent(QTimerEvent *pEvent);
		
	private slots:
		void file_new();
		void file_delete();
		void file_update();
		void file_AddToGPS();
		void file_AddToSqlDB();
		
	private:
		enum Fields{Name, WayPoint1};
		
		IDataBase *m_pDb;
		int m_lastModified;
		
		void setRouteToRow(uint row, Route &route);
};

#endif