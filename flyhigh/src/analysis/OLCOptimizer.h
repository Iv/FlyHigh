/***************************************************************************
	Copyright (C) 2002 by Thomas Kuhlmann, Berlin. Contact: coooly @ freenet de
	Thanks to Dr.Dietrich Münchmeyer und Andreas Rieck for valuable inputs!
	adapted, aranged for better understanding and comments in english 
	by Alex Graf 2005

 *                                                                        
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
 
#ifndef OLCOptimizer_h
#define OLCOptimizer_h

#include <qvaluelist.h>
#include "IGCFileParser.h"

#define FLIGHT_POINT_INDEX_LIST_SIZE 5

class OLCOptimizer
{
	public:
		typedef uint FlightPointIndexListType[FLIGHT_POINT_INDEX_LIST_SIZE];

		OLCOptimizer();
		~OLCOptimizer();

		// sets the flight data for optimizing. minor speed (distances) in m/s
		void setFlightPoints(FlightPointList &flightPoints, double minDeltaSpeed);
		bool optimize();
		
		// returns the distance in meters and the used indexes of WayPoints from data
		uint freeDistance(FlightPointIndexListType &turnpointList);
		uint FAITriangle(FlightPointIndexListType &turnpointList);
		uint flatTriangle(FlightPointIndexListType &turnpointList);
		FlightPointList& flyPointList();

	private:
		FlightPointList m_flightPointList;
		
		void initmaxend();
		void initdmin();
		void initdmval();
		void cleanup();

		// optimization matrixes
		int *dminindex;
		int *m_pDistanceMatrix;
		int *maxenddist;
		int *maxendpunkt;
  
		// Indexes of best 5 points for Free Distance, FAI Triangle und Flat Triangle
		uint m_indexFree[5];
		uint m_indexFAI[5];
		uint m_indexFlat[5];
		uint m_bestFree;
		uint m_bestFAI;
		uint m_bestFlat;
		
		uint m_maxdist; // max distance in meters between two points
};

#endif
