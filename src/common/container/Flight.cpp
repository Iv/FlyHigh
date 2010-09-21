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

#include "Flight.h"
 
Flight::Flight()
{
	m_id = -1;
	m_number = -1;
	m_duration = -1;
	m_comment = "";
	m_distance = 0;
}

int Flight::id()
{
	return m_id;
}

void Flight::setId(int id)
{
	m_id = id;
}

Pilot& Flight::pilot()
{
	return m_pilot;
}

void Flight::setPilot(Pilot &pilot)
{
	m_pilot = pilot;
}

int Flight::number()
{
	return m_number;
}

void Flight::setNumber(int nr)
{
	m_number = nr;
}

const QDate& Flight::date()
{
	return m_date;
}

void Flight::setDate(const QDate &date)
{
	m_date = date;
}

const QTime& Flight::time()
{
	return m_time;
}

void Flight::setTime(const QTime &time)
{
	m_time = time;
}

WayPoint& Flight::startPt()
{
	return m_startPt;
}

void Flight::setStartPt(WayPoint &wp)
{
	m_startPt = wp;
}

WayPoint& Flight::landPt()
{
	return m_landPt;
}

void Flight::setLandPt(WayPoint &wp)
{
	m_landPt = wp;
}

int Flight::duration()
{
	return m_duration;
}

void Flight::setDuration(int sec)
{
	m_duration = sec;
}

void Flight::setDuration(const QTime &time)
{
	m_duration = time.hour() * 3600 + time.minute() * 60 + time.second();
}

Glider& Flight::glider()
{
	return m_glider;
}

void Flight::setGlider(Glider &glider)
{
	m_glider = glider;
}

const QByteArray& Flight::igcData() const
{
	return m_igcData;
}

void Flight::setIgcData(const QByteArray &data)
{
	m_igcData = data;
}

const QString& Flight::comment()
{
	return m_comment;
}

void Flight::setComment(const QString &comm)
{
	m_comment = comm;
}

uint Flight::distance()
{
	return m_distance;
}
 
void Flight::setDistance(uint dist)
{
	m_distance = dist;
}

Flight& Flight::operator=(const Flight &flight)
{
	m_id = flight.m_id;
	m_number = flight.m_number;
	m_pilot = flight.m_pilot;
	m_date = flight.m_date;
	m_time = flight.m_time;
	m_startPt = flight.m_startPt;
	m_landPt = flight.m_landPt;
	m_duration = flight.m_duration;
	m_glider = flight.m_glider;
	m_igcData = flight.m_igcData;
	m_comment = flight.m_comment;
	m_distance = flight.m_distance;
	
	return *this;
}
