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

#define dot(u,v)   ((u).longitude() * (v).longitude() + (u).latitude() * (v).latitude())
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
//#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define dist2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
//#define dist(u,v)     norm(u-v)       // distance = norm of difference

void FlightPointList::simplify(FlightPointList &simpleList) const
{
  const double epsilon = 0.0001;

  FlighPointListType vt; // vertex buffer
  MarkerBuffer marker;
  double tol2 = (epsilon * epsilon); // tolerance squared
  int size;
  int index;
  int prev;
  int end;

  size = m_flightPointList.size();

  // STAGE 1. Vertex Reduction within tolerance of prior vertex cluster
  vt.push_back(m_flightPointList[0]);              // start at the beginning
  end = (size - 1);
  prev = 0;

  for(index=1; index<size; index++)
  {
    if(dist2(m_flightPointList[index].wp, m_flightPointList[prev].wp) < tol2)
          continue;

    vt.push_back(m_flightPointList[index]);
    prev = index;
  }

  if(prev < end)
  {
    vt.push_back(m_flightPointList[end]);
  }

  // STAGE 2. Douglas-Peucker polyline simplification
  size = vt.size();
  end = (size - 1);
  marker.resize(size);
  marker[0] = true;
  marker[end] = true;
  douglasPeucker(vt, 0, end, marker, epsilon);

  // STAGE 3. copy marked vertices to the output simplified polyline
  for(index=0; index<size; index++)
  {
    if(marker[index])
    {
      simpleList.add(vt[index]);
    }
  }
}

void FlightPointList::douglasPeucker(const FlighPointListType &ptList, int begin, int end,
                                   MarkerBuffer &marker, float epsilon) const
{
  WayPoint w;
  WayPoint pb; // base of perpendicular from v[i] to segment
  WayPoint u = (ptList[end].wp - ptList[begin].wp);   // segment direction vector
  double maxDist2 = 0;             // distance squared of farthest vertex
  double tol2 = (epsilon * epsilon);    // tolerance squared
  double cu = dot(u, u);         // segment length squared
  double b;
  double cw;
  double distSeg2; // distance v[i] to segment squared
  int index;
  int maxIndex = begin; // index of vertex farthest from segment

  if(end <= (begin + 1)) // there is nothing to simplify
      return;

  // test each vertex v[i] for max distance from S
  // compute using the Feb 2001 Algorithm's dist_Point_to_Segment()
  for(index=(begin + 1); index<end; index++)
  {
    // compute distance squared
    w = (ptList[index].wp - ptList[begin].wp);
    cw = dot(w, u);

    if(cw <= 0)
    {
      distSeg2 = dist2(ptList[index].wp, ptList[begin].wp);
    }
    else if(cu <= cw)
    {
      distSeg2 = dist2(ptList[index].wp, ptList[end].wp);
    }
    else
    {
      b = cw / cu;
      pb = ptList[begin].wp + u * b;
      distSeg2 = dist2(ptList[index].wp, pb);
    }

    // test with current max distance squared
    if(distSeg2 <= maxDist2)
        continue;

    // v[i] is a new max vertex
    maxIndex = index;
    maxDist2 = distSeg2;
  }

  if(maxDist2 > tol2) // error is worse than the tolerance
  {
    // split the polyline at the farthest vertex from segment
    marker[maxIndex] = true;      // mark v[maxIndex] for the simplified polyline
    // recursively simplify the two subpolylines at ptList[maxIndex]
    douglasPeucker(ptList, begin, maxIndex, marker, epsilon);
    douglasPeucker(ptList, maxIndex, end, marker, epsilon);
  }
}
