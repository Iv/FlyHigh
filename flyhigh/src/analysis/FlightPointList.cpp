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
 
#include "FlightPointList.h"

FlightPointList::FlightPointList()
{
}

void FlightPointList::add(FlightPointType &flightPoint)
{
	return m_flightPointList.push_back(flightPoint);
}

void FlightPointList::clear()
{
	return m_flightPointList.clear();
}

uint FlightPointList::size()
{
	return m_flightPointList.size();
}

FlightPointList::FlightPointType& FlightPointList::operator[] (int index)
{
	return m_flightPointList[index];
}

FlightPointList::FlightPointType& FlightPointList::at(int index)
{
	return m_flightPointList[index];
}

int FlightPointList::firstValidFlightData()
{
	int index = -1;
	int lastIndex = (size() - 2);
	
	for(index=0; index<lastIndex; index++)
	{
		if(speedH(index, index+1) > 2.777777)
		{
			break;
		}
	}
	
	return index;
}

int FlightPointList::lastValidFlightData()
{
	int index = -1;
	int lastIndex = (size() - 2);
	
	for(index=lastIndex; index>0; index--)
	{
		if(speedH(index, index+1)  > 0.5)
		{
			break;
		}
	}
	
	return index;
}

double FlightPointList::speedH(uint index1, uint index2)
{
	int deltaTime;
	uint nFlightPoints = size();
	double speedH = - 1.0;
	int distance;
	
	if((index1 < nFlightPoints) && (index2 < nFlightPoints))
	{
		distance = m_flightPointList[index1].wp.distance(m_flightPointList[index2].wp);
		deltaTime = m_flightPointList[index1].time.secsTo(m_flightPointList[index2].time);
		
		if(deltaTime > 0.0)
		{
			speedH = (distance / deltaTime);
		}
	}
	
	return speedH;
}

double FlightPointList::speedV(uint index1, uint index2)
{
	int deltaTime;
	uint nFlightPoints = size();
	double speedV = - 1.0;
	int deltaAlt;
	
	if((index1 < nFlightPoints) && (index2 < nFlightPoints))
	{
		deltaAlt = m_flightPointList[index2].wp.altitude() - m_flightPointList[index1].wp.altitude();
		deltaTime = m_flightPointList[index1].time.secsTo(m_flightPointList[index2].time);
		
		if(deltaTime > 0.0)
		{
			speedV = (deltaAlt / deltaTime);
		}
	}
	
	return speedV;
}

int FlightPointList::duration(uint index1, uint index2)
{
	int duration = -1;
	uint nFlightPoints = size();
	bool valid;

	valid = ((index1 < nFlightPoints) && (index2 < nFlightPoints));
	
	if(valid)
	{
		duration = m_flightPointList[index1].time.secsTo(m_flightPointList[index2].time);
	}

	return duration;
}
