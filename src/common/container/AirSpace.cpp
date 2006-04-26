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
	m_name = "";
	m_airspaceClass = "";
	m_high = "";
	m_low = "";
	m_warnDist = 0;
	m_remark = "";
}

AirSpace::~AirSpace()
{
}

const QString& AirSpace::name()
{
	return m_name;
}

void AirSpace::setName(const QString &name)
{
	QString locName = name;
	
	locName.remove('#');
	m_name = locName.stripWhiteSpace();
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

AirSpaceItem::AirSpaceItemListType& AirSpace::airSpaceItemList()
{
	return m_airSpaceItemList;
}

void AirSpace::setWarnDist(uint meters)
{
	m_warnDist = meters;
}

uint AirSpace::warnDist()
{
	return m_warnDist;
}

void AirSpace::setRemark(const QString &remark)
{
	m_remark = remark;
}

const QString& AirSpace::remark()
{
	return m_remark;
}

AirSpace& AirSpace::operator=(const AirSpace &airspace)
{
	m_name = airspace.m_name;
	m_airspaceClass = airspace.m_airspaceClass;
	m_high = airspace.m_high;
	m_low = airspace.m_low;
	m_warnDist = airspace.m_warnDist;
	m_remark = airspace.m_remark;
	m_airSpaceItemList = airspace.m_airSpaceItemList;
	
	return *this;
}

bool AirSpace::operator<(const AirSpace &airspace)
{
	return (m_name < airspace.m_name);
}

bool AirSpace::operator==(const AirSpace &airspace)
{
	return (m_name == airspace.m_name);
}
