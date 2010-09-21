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
//Added by qt3to4:
#include <Q3TextStream>
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
		
		void streamHead(Q3TextStream& s);
		void streamName(Q3TextStream& s);
		void streamBirth(Q3TextStream& s);
		void streamTakeoffLoc(Q3TextStream& s);
		void streamCallsign(Q3TextStream& s);
		void streamTrack(Q3TextStream& s);
		void streamFlightDate(Q3TextStream& s);
		void streamModel(Q3TextStream& s);
		void streamTakeOffTime(Q3TextStream& s);
		void streamDepartureTime(Q3TextStream& s);
		void streamWayPoint(Q3TextStream& s, uint wpNr, WayPoint &wp);
		void streamFinishTime(Q3TextStream& s);
		void streamComment(Q3TextStream& s);
		void streamSubmit(Q3TextStream& s);
		
		char getOLCchar(int value);
};

#endif
