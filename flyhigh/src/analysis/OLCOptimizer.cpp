/***************************************************************************
	Copyright (C) 2002 by Thomas Kuhlmann, Berlin. Contact: coooly @ freenet de
	Thanks to Dr.Dietrich Münchmeyer und Andreas Rieck for valuable inputs!
	adapted, aranged for better understanding and comments in english
	by Alex Graf 2005
 
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
 
#include "OLCOptimizer.h"

OLCOptimizer::OLCOptimizer()
{
	dminindex = NULL;
	m_pDistanceMatrix = NULL;
	maxenddist = NULL;
	maxendpunkt = NULL;
}

OLCOptimizer::~OLCOptimizer()
{
	cleanup();
}

uint OLCOptimizer::freeDistance(FlightPointIndexListType &turnpointList)
{
	memcpy(turnpointList, m_indexFree, sizeof(m_indexFree));
	
	return m_bestFree;
}

uint OLCOptimizer::FAITriangle(FlightPointIndexListType &turnpointList)
{
	memcpy(turnpointList, m_indexFAI, sizeof(m_indexFree));

	return m_bestFAI;
}

uint OLCOptimizer::flatTriangle(FlightPointIndexListType &turnpointList)
{
	memcpy(turnpointList, m_indexFlat, sizeof(m_indexFree));

	return m_bestFlat;
}

FlightPointList& OLCOptimizer::flyPointList()
{
	return m_flightPointList;
}

void OLCOptimizer::cleanup()
{
	if(dminindex != NULL)
	{
		delete [] dminindex;
		dminindex = NULL;
	}
	
	if(m_pDistanceMatrix != NULL)
	{
		delete [] dminindex;
		dminindex = NULL;
	}
	
	if(maxenddist != NULL)
	{
		delete [] maxenddist;
		maxenddist = NULL;
	}
	
	if(maxendpunkt != NULL)
	{
		delete [] maxendpunkt;
		maxendpunkt = NULL;
	}
}

/*
calc all distances between all points and save in distance matrix.
the calculation is done with doubles, the result is rounded to full meters and
stoerd as integers. Because calculation after is much more faster.
*/
void OLCOptimizer::initdmval()
{
	uint nFlightPts = m_flightPointList.size();
	double sinlat[nFlightPts];
	double coslat[nFlightPts];
	double lonrad[nFlightPts];
	double latrad;
	double sli;
	double cli;
	double lri;
	uint j;
	uint dist;
	uint cmp;
	uint wpNr;
	const double CONST_PI_DIV_180 = (M_PI)/((double)180.0);
	
	m_pDistanceMatrix = new int[nFlightPts*nFlightPts];
	cmp = nFlightPts + 1;

	for(wpNr=0; wpNr<nFlightPts; wpNr++) 
	{ // all deltas to radians and save sin/cos
		lonrad[wpNr] = m_flightPointList[wpNr].wp.longitude() * CONST_PI_DIV_180;
		latrad = m_flightPointList[wpNr].wp.latitude() * CONST_PI_DIV_180;
		sinlat[wpNr] = sin(latrad);
		coslat[wpNr] = cos(latrad);
		m_pDistanceMatrix[cmp*wpNr] = 0; // fill diagonal of matrix with 0
	}

	m_maxdist = 0;
	cmp = nFlightPts - 1;

	// this for not backwards!
	for(uint i=0;i<cmp;i++)
	{
		sli = sinlat[i];
		cli = coslat[i];
		lri = lonrad[i];
		
		for(j=i+1; j<nFlightPts; j++) 
		{
			dist = (int)(6371000.0*acos(sli*sinlat[j] + cli*coslat[j]* cos(lri-lonrad[j]))+((double)0.5));  
			m_pDistanceMatrix[i+nFlightPts*j] = dist;
			
			if(dist > m_maxdist)
			{
				m_maxdist = dist;
			}
		}
	}
}
    
/*
calc smallest distance between all points x and y with x<=i and y>=j
for all x<=i, y>=j: dmin(i,j) <= dmin(x,y) and
distance[dmini[i][j]][dminj[i][j]] <= distance[x][y]
*/
#define dmini(x,y) dminindex[(x)+nFlightPts*(y)]
#define dminj(x,y) dminindex[(y)+nFlightPts*(x)]
#define dmin(x,y) m_pDistanceMatrix[(y)+nFlightPts*(x)]

void OLCOptimizer::initdmin()
{
	uint i;
	uint j;
	uint d;
	uint mini;
	uint minj = 0;
	uint minimum = m_maxdist;
	uint nFlightPts = m_flightPointList.size();
	
	dminindex = new int[nFlightPts*nFlightPts];
	
	// handle 1st row separately
	for(j=nFlightPts-1; j>0; j--)
	{
		d = m_pDistanceMatrix[0+nFlightPts*j];
		
		// d<minimum if equal point before should be found
		if(d < minimum)
		{
			minimum = d; 
			minj = j;
		}
		dmin(0,j)  = minimum;
		dmini(0,j) = 0;
		dminj(0,j) = minj;
	}
	
	// inherit following rows from there before
	for(i=1;i<nFlightPts-1;i++)
	{
		j = nFlightPts-1; // handle last col for init of minimum separatelly
		minimum = dmin(i-1,j);
		mini = dmini(i-1,j);
		minj = dminj(i-1,j);
		d = m_pDistanceMatrix[i+nFlightPts*j];
		
		if(d<minimum)
		{
			minimum = d;
			mini = i;
			minj = j;
		}
		
		dmin(i,j)  = minimum;
		dmini(i,j) = mini;
		dminj(i,j) = minj;
		
		// these cols from tail to front
		for(j=nFlightPts-2; j>i; j--)
		{ 
			d = m_pDistanceMatrix[i+nFlightPts*j];
			
			// current point better then minimum before?
			// d<minimum if equal point before should be found
			if(d<minimum)
			{
				minimum = d;
				mini = i;
				minj = j;
			}
			
			d=dmin(i-1,j);
			
			// is minimum before this col is better?
			if(d < minimum)
			{
				minimum = d;
				mini = dmini(i-1,j);
				minj = dminj(i-1,j);
			}
			dmin(i,j)  = minimum;
			dmini(i,j) = mini;
			dminj(i,j) = minj;
		}
	}
}
    
void OLCOptimizer::initmaxend()
{
	uint nFlightPts = m_flightPointList.size();
	uint w3, i, f, maxf, besti;
	
	maxenddist = new int[nFlightPts];
	maxendpunkt = new int[nFlightPts];
           
	for(w3=nFlightPts-1; w3>1; w3--)
	{
		maxf = 0;
		
		for(i=besti=nFlightPts-1; i>=w3; i--) 
		{
			f = m_pDistanceMatrix[w3+nFlightPts*i];
			
			if(f > maxf) 
			{
				maxf = f;
				besti = i;
			}
		}
		maxenddist[w3]  = maxf;
		maxendpunkt[w3] = besti;
	}
}

void OLCOptimizer::setFlightPoints(FlightPointList &flightPoints, double minDeltaSpeed)
{
	uint index;
	uint lastIndex = (flightPoints.size() - 2);
	double speed;
	
	m_flightPointList.clear();
		
	for(index=0; index<lastIndex; index++)
	{
		speed = flightPoints.speedH(index, index+1);

		if(speed > minDeltaSpeed)
		{
			m_flightPointList.add(flightPoints[index]);
		}
	}
}
    
bool OLCOptimizer::optimize()
{
	uint nFlightPts;
	uint i4cmp, i2cmp;
	uint i1, i2, i3, i4;
	uint i, a, b, c, d, e, u, w, tmp, aplusb, c25, d5;
	
	nFlightPts = m_flightPointList.size();
	i4cmp = nFlightPts - 2;
	i2cmp = nFlightPts - 2;
		
	if(nFlightPts < 5)
	{
		// not enough points!
		return false;
	}
	
	cleanup();
	initdmval();
	initdmin();
	initmaxend();
		
	m_indexFree[0] = 0;
	m_indexFree[1] = 0;
	m_indexFree[2] = 0;
	m_indexFree[3] = 0;
	m_indexFree[4] = 0;
	m_bestFree = 0;
	m_bestFAI = 0;
	m_bestFlat = 0;
		
	for(i2=0; i2<i2cmp; i2++)
	{
		for(i=i1=e=0; i<i2; i++)
		{ // optimize 1st turnpoint for free distance separately
			tmp = m_pDistanceMatrix[i+nFlightPts*i2];
		
			if(tmp > e)
			{
				e = tmp;
				i1 = i;
			}
		} // e, i1 contains for this i2 the best value
		
		i4cmp = i2 + 2;
		
		for(i4=nFlightPts; --i4>=i4cmp;)
		{ // optimize 3th turnpoint from tail
			c = m_pDistanceMatrix[i2+nFlightPts*i4];
			c25 = c * 25;
			d = dmin(i2, i4);
			d5 = d * 5;
			aplusb = 0;
			
			for(i=i3=i2+1; i<i4; i++)
			{ // optimize 2nd turnpoint for free distance separately
				a = m_pDistanceMatrix[i2+nFlightPts*i];
				b = m_pDistanceMatrix[i+nFlightPts*i4];
				tmp = a + b;
				
				if(tmp > aplusb)
				{ // finds greatest a + b (and triangle too)
					aplusb = tmp;
					i3 = i;
				}
				
				u = tmp + c;
				
				if(d5 <= u)
				{ // triangle found 5*d<= a+b+c
					if((c25>=(tmp=u*7)) && (a*25>=tmp) && (b*25>=tmp))
					{ // FAI Triangle
						w = u - d;
						
						if(w > m_bestFAI)
						{ // this FAI Triangle is greater
							m_indexFAI[0] = dmini(i2, i4);
							m_indexFAI[1] = i2;
							m_indexFAI[2] = i;
							m_indexFAI[3] = i4;
							m_indexFAI[4] = dminj(i2, i4);
							m_bestFAI = w;
						}
					}
					else
					{ // Flat Triangle
						w = u - d;
						
						if(w > m_bestFlat)
						{
							m_indexFlat[0] = dmini(i2, i4);
							m_indexFlat[1] = i2;
							m_indexFlat[2] = i;
							m_indexFlat[3] = i4;
							m_indexFlat[4] = dminj(i2, i4);
							m_bestFlat = w;
						}
					}
				}
			} // aplusb, i3 contains for this i2 and i4 the best value
			
			tmp = maxenddist[i4] + aplusb + e;
			
			if(tmp > m_bestFree)
			{
				m_indexFree[0] = i1;
				m_indexFree[1] = i2;
				m_indexFree[2] = i3;
				m_indexFree[3] = i4;
				m_indexFree[4] = maxendpunkt[i4];
				m_bestFree = tmp;
			}
		}
	}
	
	return true;
}
