/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#include <QtAlgorithms>
#include "AirSpace.h"
#include "AirSpaceList.h"

AirSpaceList::AirSpaceList(bool autoclean)
{
  m_autoclean = autoclean;
}

AirSpaceList::~AirSpaceList()
{
  if(m_autoclean)
  {
    clear();
  }
}

void AirSpaceList::push_back(AirSpace *pAirSpace)
{
	return m_airSpaceList.push_back(pAirSpace);
}

void AirSpaceList::clear()
{
	AirSpaceListType::iterator it;

	for(it=m_airSpaceList.begin(); it!=m_airSpaceList.end(); it++)
	{
		delete (*it);
	}

	m_airSpaceList.clear();
}

uint AirSpaceList::size() const
{
	return m_airSpaceList.size();
}

AirSpace* AirSpaceList::operator[] (int index)
{
	return m_airSpaceList[index];
}

AirSpace* AirSpaceList::at(int index)
{
	return m_airSpaceList[index];
}

const AirSpace* AirSpaceList::at(int index) const
{
	return m_airSpaceList.at(index);
}

AirSpaceList::iterator AirSpaceList::begin()
{
	return m_airSpaceList.begin();
}

AirSpaceList::iterator AirSpaceList::end()
{
	return m_airSpaceList.end();
}

void AirSpaceList::sort()
{
	qSort(m_airSpaceList.begin(), m_airSpaceList.end(), nameSort);
}

bool AirSpaceList::nameSort(const AirSpace *pAirSpace1, const AirSpace *pAirSpace2)
{
  return pAirSpace1->name().toLower() < pAirSpace2->name().toLower();
}
