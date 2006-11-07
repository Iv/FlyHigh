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
#ifndef ServicingWindow_h
#define ServicingWindow_h

#include "Servicing.h"
#include "TableWindow.h"

class ISql;

class ServicingWindow: public TableWindow
{
	Q_OBJECT

	public:
		ServicingWindow(QWidget* parent, const char* name, int wflags);
		
	protected:
		bool periodicalUpdate();
	
	private slots:
		void file_update();
		void file_new();
		void file_delete();
	
	private:
		enum Fields{Glider, Date, Responsibility, Comment};
		
		Servicing::ServicingListType m_servicingsList;
		int m_lastModified;
		ISql *m_pDb;
};

#endif
