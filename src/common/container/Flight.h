/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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
 
#ifndef Flight_h
#define Flight_h

#include <qdatetime.h>
#include <qstring.h>
#include <q3valuelist.h>
#include "WayPoint.h"
#include "Glider.h"
#include "Pilot.h"

class Flight
{
	public:
		typedef Q3ValueList<Flight> FlightListType;
		
		Flight();
		
		int id();
		void setId(int id);
		Pilot& pilot();
		void setPilot(Pilot &pilot);
		int number();
		void setNumber(int nr);
		const QDate& date();
		void setDate(const QDate &date);
		const QTime& time();
		void setTime(const QTime &time);
		WayPoint& startPt();
		void setStartPt(WayPoint &wp);
		WayPoint& landPt();
		void setLandPt(WayPoint &wp);
		int duration();
		void setDuration(int sec);
		void setDuration(const QTime &time);
		Glider& glider();
		void setGlider(Glider &glider);
		const QByteArray& igcData() const;
		void setIgcData(const QByteArray &data);
		const QString& comment();
		void setComment(const QString &comm);
		
		// sets and returns the distance in meters
		uint distance();
		void setDistance(uint dist);
		
		Flight& operator=(const Flight &flight);

	private:
		int m_id;
		Pilot m_pilot;
		int m_number;
		QDate m_date;
		QTime m_time;
		WayPoint m_startPt;
		WayPoint m_landPt;
		int m_duration;
		Glider m_glider;
		QByteArray m_igcData;
		QString m_comment;
		uint m_distance;
};

#endif
