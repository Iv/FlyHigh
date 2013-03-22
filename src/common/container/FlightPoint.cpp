/***************************************************************************
 *   Copyright (C) 2013 by Alex Graf                                       *
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
#include "FlightPoint.h"

FlightPoint::FlightPoint()
{
  m_alt = 0;
  m_elevation = 0;
}

FlightPoint::FlightPoint(const FlightPoint *pFp)
{
  *this = *pFp;
}

FlightPoint::FlightPoint(const LatLng &pos, double alt)
{
  m_pos = pos;
  m_alt = alt;
  m_elevation = 0;
}

void FlightPoint::setPos(const LatLng &pos)
{
  m_pos = pos;
}

const LatLng& FlightPoint::pos() const
{
  return m_pos;
}

void FlightPoint::setTime(const QTime &time)
{
  m_time = time;
}

const QTime& FlightPoint::time() const
{
  return m_time;
}

void FlightPoint::setAlt(double alt)
{
  m_alt = alt;
}

double FlightPoint::alt() const
{
  return m_alt;
}

void FlightPoint::setElevation(double &elevation)
{
  m_elevation = elevation;
}

double FlightPoint::elevation() const
{
  return m_elevation;
}
