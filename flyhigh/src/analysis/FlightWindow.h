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
#ifndef FlightWindow_h
#define FlightWindow_h

#include <TableWindow.h>
#include "IDataBase.h"
#include "GnuPlot.h"

class QWidget;
class QTimerEvent;

class FlightWindow : public TableWindow
{
	Q_OBJECT
	public:
		FlightWindow(QWidget* parent, const char* name, int wflags, IDataBase::SourceType src);
	
	protected:
		void timerEvent(QTimerEvent *pEvent);
		
	private slots:
		void file_update();
		void file_AddToSqlDB();
		void file_save();
		void file_delete();
		void plot_speedVsTime();
		void plot_altVsTime();
		void plot_varioVsTime();
		void plot_3d();
		
	private:
		enum Fields{Nr, Date, Time, Duration, Model, StartPt, LandPt, Distance, Comment};
		
		int m_lastModified;
		IDataBase *m_pDb;
		QString m_fileName;
		GnuPlot m_plotter;
		
		void setFlightToRow(uint row, Flight &flight);
};

#endif
