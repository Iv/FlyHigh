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
 
#include "FlightPointList.h"

FlightPointList::FlightPointList()
{
}

void FlightPointList::add(const FlightPointType &flightPoint)
{
	return m_flightPointList.push_back(flightPoint);
}

void FlightPointList::clear()
{
	return m_flightPointList.clear();
}

uint FlightPointList::size() const
{
	return m_flightPointList.size();
}

FlightPointList::FlightPointType& FlightPointList::operator[] (int index)
{
	return m_flightPointList[index];
}

const FlightPointList::FlightPointType& FlightPointList::at(int index) const
{
	return m_flightPointList.at(index);
}

int FlightPointList::firstValidFlightData()
{
	int index = -1;
	int lastIndex = (size() - 2);
	int fpNr;
	
	for(fpNr=0; fpNr<lastIndex; fpNr++)
	{
		if(speedH(fpNr, fpNr+1) > 2.777777)
		{
			index = fpNr;
			break;
		}
	}
	
	return index;
}

int FlightPointList::lastValidFlightData()
{
	int index = -1;
	int lastIndex = (size() - 2);
	int fpNr;
	
	for(fpNr=lastIndex; fpNr>0; fpNr--)
	{
		if(speedH(fpNr, fpNr+1)  > 0.5)
		{
			index = fpNr;
			break;
		}
	}
	
	return index;
}

double FlightPointList::speedH(uint index1, uint index2)
{
	double deltaTime;
	uint nFlightPoints = size();
	double speedH = - 1.0;
	double distance;
	
	if((index1 < nFlightPoints) && (index2 < nFlightPoints))
	{
		distance = (double)m_flightPointList[index1].wp.distance(m_flightPointList[index2].wp);
		deltaTime = (double)m_flightPointList[index1].time.secsTo(m_flightPointList[index2].time);
		
		if(deltaTime > 0.0)
		{
			speedH = (distance / deltaTime);
		}
	}
	
	return speedH;
}

double FlightPointList::speedV(uint index1, uint index2)
{
	double deltaTime;
	uint nFlightPoints = size();
	double speedV = - 1.0;
	double deltaAlt;
	
	if((index1 < nFlightPoints) && (index2 < nFlightPoints))
	{
		deltaAlt = (double)m_flightPointList[index2].wp.altitude() - m_flightPointList[index1].wp.altitude();
		deltaTime = (double)m_flightPointList[index1].time.secsTo(m_flightPointList[index2].time);
		
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
