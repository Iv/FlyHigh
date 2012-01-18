/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
#ifndef FlightExpWindow_h
#define FlightExpWindow_h

#include "TableWindow.h"

class QWidget;
class ISql;

class FlightExpWindow: public TableWindow
{
	Q_OBJECT
	public:
		FlightExpWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags);

	private slots:
		void file_update();

	private:
		enum Fields{Year, NrFlights, Airtime};

		int m_lastModified;
		ISql *m_pDb;
};

#endif
