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

#ifndef OLCWebForm_h
#define OLCWebForm_h

#include <qdatetime.h>
#include <qstring.h>
#include <QTextStream>
#include "FlightPointList.h"
#include "Flight.h"
#include "WayPoint.h"

class OLCWebForm
{
	public:
		OLCWebForm();

		void setFlight(Flight &flight);

    void setDeparture(const FlightPoint &dep);

		void set1stTurnPoint(const FlightPoint &tp);

		void set2ndTurnPoint(const FlightPoint &tp);

		void set3rdTurnPoint(const FlightPoint &tp);

		void setFinish(const FlightPoint &fin);

		bool save(const QString & name);

		void olcFileName(QString &fileName); // without file ending (setPilot() and setDate() must be called before!)

	private:
		Flight m_flight;
		FlightPoint m_departure;
		FlightPoint m_1stTurnPoint;
		FlightPoint m_2ndTurnPoint;
		FlightPoint m_3rdTurnPoint;
		FlightPoint m_finish;

		void streamHead(QTextStream& s);

		void streamName(QTextStream& s);

		void streamBirth(QTextStream& s);

		void streamTakeoffLoc(QTextStream& s);

		void streamCallsign(QTextStream& s);

		void streamTrack(QTextStream& s);

		void streamFlightDate(QTextStream& s);

		void streamModel(QTextStream& s);

		void streamTakeOffTime(QTextStream& s);

		void streamDepartureTime(QTextStream& s);

		void streamFlightPoint(QTextStream& s, uint fpNr, FlightPoint &fp);

		void streamFinishTime(QTextStream& s);

		void streamComment(QTextStream& s);

		void streamSubmit(QTextStream& s);

		char getOLCchar(int value);
};

#endif
