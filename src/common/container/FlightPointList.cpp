/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include "BoundBox.h"
#include "FlightPointList.h"

FlightPointList::FlightPointList(bool dataOwner)
{
  m_dataOwner = dataOwner;
}

FlightPointList::~FlightPointList()
{
  clear();
}

void FlightPointList::setDataOwner(bool dataOwner)
{
  m_dataOwner = dataOwner;
}

void FlightPointList::push_back(FlightPoint *pFlightPoint)
{
	return m_flightPointList.push_back(pFlightPoint);
}

void FlightPointList::clear()
{
  iterator it;

  if(m_dataOwner)
  {
    for(it=begin(); it!=end(); it++)
    {
      delete *it;
    }
  }

  m_flightPointList.clear();
}

uint FlightPointList::size() const
{
	return m_flightPointList.size();
}

FlightPoint* FlightPointList::operator[] (int index)
{
	return m_flightPointList[index];
}

const FlightPoint* FlightPointList::operator[] (int index) const
{
	return m_flightPointList[index];
}

FlightPoint* FlightPointList::at(int index)
{
	return m_flightPointList.at(index);
}

const FlightPoint* FlightPointList::at(int index) const
{
	return m_flightPointList.at(index);
}

const FlightPoint* FlightPointList::at(const QTime &time) const
{
  FlighPointListType::const_iterator it;
  FlightPoint *pPoint = NULL;

  for(it=m_flightPointList.begin(); it!=m_flightPointList.end(); it++)
  {
    pPoint = *it;

    if(pPoint->time() >= time)
    {
      break;
    }
  }

  return pPoint;
}

FlightPointList::iterator FlightPointList::begin()
{
  return m_flightPointList.begin();
}

FlightPointList::iterator FlightPointList::end()
{
  return m_flightPointList.end();
}

int FlightPointList::firstValidFlightData() const
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

int FlightPointList::lastValidFlightData() const
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

double FlightPointList::speedH(uint index1, uint index2) const
{
	double deltaTime;
	uint nFlightPoints = size();
	double speedH = - 1.0;
	double distance;

	if((index1 < nFlightPoints) && (index2 < nFlightPoints))
	{
		distance = (double)m_flightPointList[index1]->pos().distance(m_flightPointList[index2]->pos());
		deltaTime = (double)m_flightPointList[index1]->time().secsTo(m_flightPointList[index2]->time());

		if(deltaTime > 0.0)
		{
			speedH = (distance / deltaTime);
		}
	}

	return speedH;
}

double FlightPointList::speedV(uint index1, uint index2) const
{
	double deltaTime;
	uint nFlightPoints = size();
	double speedV = - 1.0;
	double deltaAlt;

	if((index1 < nFlightPoints) && (index2 < nFlightPoints))
	{
		deltaAlt = (double)m_flightPointList[index2]->alt() - m_flightPointList[index1]->alt();
		deltaTime = (double)m_flightPointList[index1]->time().secsTo(m_flightPointList[index2]->time());

		if(deltaTime > 0.0)
		{
			speedV = (deltaAlt / deltaTime);
		}
	}

	return speedV;
}

int FlightPointList::duration(uint index1, uint index2) const
{
	int duration = -1;
	uint nFlightPoints = size();
	bool valid;

	valid = ((index1 < nFlightPoints) && (index2 < nFlightPoints));

	if(valid)
	{
		duration = m_flightPointList[index1]->time().secsTo(m_flightPointList[index2]->time());
	}

	return duration;
}

#define dot(u,v)   ((u).lon() * (v).lon() + (u).lat() * (v).lat())
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
//#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define dist2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
//#define dist(u,v)     norm(u-v)       // distance = norm of difference

void FlightPointList::simplify(FlightPointList &simpleList) const
{
  const double epsilon = 0.0001;

  FlighPointListType vtxBuff; // vertex buffer
  MarkerBuffer marker;
  double tol2 = (epsilon * epsilon); // tolerance squared
  int size;
  int index;
  int prev;
  int end;

  simpleList.setDataOwner(false);
  size = m_flightPointList.size();

  // STAGE 1. Vertex Reduction within tolerance of prior vertex cluster
  vtxBuff.push_back(m_flightPointList[0]);              // start at the beginning
  end = (size - 1);
  prev = 0;

  for(index=1; index<size; index++)
  {
    if(dist2(m_flightPointList[index]->pos(), m_flightPointList[prev]->pos()) < tol2)
          continue;

    vtxBuff.push_back(m_flightPointList[index]);
    prev = index;
  }

  if(prev < end)
  {
    vtxBuff.push_back(m_flightPointList[end]);
  }

  // STAGE 2. Douglas-Peucker polyline simplification
  size = vtxBuff.size();
  end = (size - 1);
  marker.resize(size);
  marker[0] = true;
  marker[end] = true;
  douglasPeucker(vtxBuff, 0, end, marker, epsilon);

  // STAGE 3. copy marked vertices to the output simplified polyline
  for(index=0; index<size; index++)
  {
    if(marker[index])
    {
      simpleList.push_back(vtxBuff[index]);
    }
  }
}

void FlightPointList::boundBox(BoundBox &bbox)
{
	uint index;

	for(index=0; index<size(); index++)
	{
    bbox.setMinMax(m_flightPointList[index]->pos());
	}
}

void FlightPointList::douglasPeucker(const FlighPointListType &ptList, int begin, int end,
                                   MarkerBuffer &marker, float epsilon) const
{
  LatLng w;
  LatLng pb; // base of perpendicular from v[i] to segment
  LatLng u = (ptList[end]->pos() - ptList[begin]->pos());   // segment direction vector
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
    w = (ptList[index]->pos() - ptList[begin]->pos());
    cw = dot(w, u);

    if(cw <= 0)
    {
      distSeg2 = dist2(ptList[index]->pos(), ptList[begin]->pos());
    }
    else if(cu <= cw)
    {
      distSeg2 = dist2(ptList[index]->pos(), ptList[end]->pos());
    }
    else
    {
      b = cw / cu;
      pb = ptList[begin]->pos() + u * b;
      distSeg2 = dist2(ptList[index]->pos(), pb);
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
