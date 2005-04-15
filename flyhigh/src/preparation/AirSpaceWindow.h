/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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
#ifndef AirSpaceWindow_h
#define AirSpaceWindow_h

#include "AirSpace.h"
#include "IDataBase.h"
#include "IAirSpaceForm.h"
#include "TableWindow.h"

class AirSpaceWindow: public TableWindow
{
	Q_OBJECT
	
	public:
		AirSpaceWindow(QWidget* parent, const char* name, int wflags, IDataBase::SourceType src);
	
	protected:
		void timerEvent(QTimerEvent *pEvent);
		
	protected slots:
		void currentChanged(int row, int col);
	
	private slots:
		void file_new();
		void file_import();
		void file_delete();
		void file_update();
		void file_AddToGPS();
		void file_AddToSqlDB();
		
	private:
		enum Fields{Name, High, Low, Class};
		
		IDataBase *m_pDb;
		int m_lastModified;
		IAirSpaceForm m_curAirSpace;
		AirSpace::AirSpaceListType m_airSpaceList;
		
		void setAirSpaceToRow(uint row, AirSpace &airspace);
		void plotAirSpace(AirSpace &airspace, const QString& title);
};

#endif
