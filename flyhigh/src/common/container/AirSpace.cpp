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

#include "AirSpace.h"

AirSpace::AirSpace()
{
}

const QString& AirSpace::name()
{
	return m_name;
}

void AirSpace::setName(const QString &name)
{
	m_name = name;
}

const QString& AirSpace::high()
{
	return m_high;
}

void AirSpace::setHigh(const QString &high)
{
	m_high = high;
}

const QString& AirSpace::low()
{
	return m_low;
}

void AirSpace::setLow(const QString &low)
{
	m_low = low;
}

const QString& AirSpace::airspaceClass()
{
	return m_airspaceClass;
}

void AirSpace::setAirspaceClass(const QString &airspaceClass)
{
	m_airspaceClass = airspaceClass;
}

AirSpace::EdgePointListType& AirSpace::edgePointList()
{
	return m_edgePointList;
}

AirSpace& AirSpace::operator=(const AirSpace &airspace)
{
	m_name = airspace.m_name;
	m_edgePointList = airspace.m_edgePointList;
	
	return *this;
}
