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
#include "WayPoint.h"

class OLCWebForm
{
	public:
		OLCWebForm();
		
		void setPilot(const QString &name);
		void setPilotBirth(const QDate &birth);
		void setTakeOffLoc(const QString &takeoff);
		void setCallSign(const QString &callsign);
		void setDate(const QDate &date);
		void setGlider(const QString &name);
		void setDeparture(const FlightPointList::FlightPointType &dep);
		void set1stWayPoint(const WayPoint &wp);
		void set2ndWayPoint(const WayPoint &wp);
		void set3rdWayPoint(const WayPoint &wp);
		void setFinish(const FlightPointList::FlightPointType &fin);
		void setComment(const QString &comment);
		
		bool save(const QString & name);
		void olcFileName(QString &fileName); // without file ending (setPilot() and setDate() must be called before!)
		
	private:
		QString m_firstName;
		QString m_surName;
		QDate m_birth;
		QString m_takeoffLoc;
		QString m_callsign;
		QDate m_date;
		QString m_glider;
		FlightPointList::FlightPointType m_departure;
		WayPoint m_1stWayPoint;
		WayPoint m_2ndWayPoint;
		WayPoint m_3rdWayPoint;
		FlightPointList::FlightPointType m_finish;
		QString m_comment;
		
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
