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
 
#ifndef Flight_h
#define Flight_h

#include <qdatetime.h>
#include <qstring.h>
#include <qvaluelist.h>

class Flight
{
	public:
		typedef QValueList<Flight> FlightListType;
		
		Flight();
		
		int number();
		void setNumber(int nr);
		const QDate& date();
		void setDate(const QDate &date);
		const QTime& time();
		void setTime(const QTime &time);
		const QString& startPt();
		void setStartPt(const QString &name);
		const QString& landPt();
		void setLandPt(const QString &name);
		int duration();
		void setDuration(int sec);
		void setDuration(const QTime &time);
		const QString& glider();
		void setGlider(const QString &name);
		QByteArray& igcData();
		/* setIgcData() makes only a shallow copy!!!
		A shared class consists of a pointer to a shared data block
		that contains a reference count and the data. */
		void setIgcData(const QByteArray &data);
		const QString& comment();
		void setComment(const QString &comm);
		
		// sets and returns the distance in meters
		uint distance();
		void setDistance(uint dist);
		
		Flight& operator=(const Flight &flight);

	private:
		int m_number;
		QDate m_date;
		QTime m_time;
		QString m_startPt;
		QString m_landPt;
		int m_duration;
		QString m_glider;
		QByteArray m_igcData;
		QString m_comment;
		uint m_distance;
};

#endif
