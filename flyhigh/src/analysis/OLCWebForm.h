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

#ifndef OLCWebForm_h
#define OLCWebForm_h

#include <qdatetime.h>
#include <qstring.h>
#include "FlightPointList.h"
#include "Flight.h"
#include "WayPoint.h"

class OLCWebForm
{
	public:
		OLCWebForm();

		void setFlight(Flight &flight);
	void setDeparture(const FlightPointList::FlightPointType &dep);
		void set1stWayPoint(const WayPoint &wp);
		void set2ndWayPoint(const WayPoint &wp);
		void set3rdWayPoint(const WayPoint &wp);
		void setFinish(const FlightPointList::FlightPointType &fin);
		
		bool save(const QString & name);
		void olcFileName(QString &fileName); // without file ending (setPilot() and setDate() must be called before!)
		
	private:
		Flight m_flight;
		FlightPointList::FlightPointType m_departure;
		WayPoint m_1stWayPoint;
		WayPoint m_2ndWayPoint;
		WayPoint m_3rdWayPoint;
		FlightPointList::FlightPointType m_finish;
		
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
		void streamWayPoint(QTextStream& s, uint wpNr, WayPoint &wp);
		void streamFinishTime(QTextStream& s);
		void streamComment(QTextStream& s);
		void streamSubmit(QTextStream& s);
		
		char getOLCchar(int value);
};

#endif
