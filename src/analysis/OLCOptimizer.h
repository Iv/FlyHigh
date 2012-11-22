/***************************************************************************
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

/**
	Copyright (C) 2002 by Thomas Kuhlmann, Berlin. Contact: coooly @ freenet de
	Thanks to Dr.Dietrich Münchmeyer und Andreas Rieck for valuable inputs!
	adapted, aranged for better understanding and comments in english
	by Alex Graf 2005

	faster optimization with distance filtering by Alex Graf 2006
  straight distance by Alex Graf 2012
*/

#ifndef OLCOptimizer_h
#define OLCOptimizer_h

#include <QObject>
#include "IGCFileParser.h"

class OLCOptimizer: public QObject
{
	Q_OBJECT
	public:
    typedef QVector<uint> FlightPointIndexListType;

		OLCOptimizer();

		virtual ~OLCOptimizer();

		// sets the flight data for optimizing. minor and filter distances in m
		void setFlightPoints(FlightPointList &flightPoints, uint minDeltaDist, uint filterDist);

		bool optimize();

		// returns the distance in meters and the used indexes of WayPoints from data
		uint straightDist() const;

		void straightDistIndex(FlightPointIndexListType &indexList) const;

		uint straightDistance(FlightPointIndexListType &indexList) const;

		uint freeDist() const;

    void freeDistIndex(FlightPointIndexListType &indexList) const;

		uint freeDistance(FlightPointIndexListType &indexList) const;

		uint flatDist() const;

		void flatDistIndex(FlightPointIndexListType &indexList) const;

		uint flatTriangle(FlightPointIndexListType &indexList) const;

		uint faiDist() const;

		void faiDistIndex(FlightPointIndexListType &indexList) const;

		uint FAITriangle(FlightPointIndexListType &indexList) const;

		const FlightPointList& flyPointList() const;

	signals:
		void progress(int percent);

	public slots:
		void cancel();

	private:
		FlightPointList m_flightPointList;
		bool m_cancel;

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
		uint m_indexStraight[2];
		uint m_indexFree[5];
		uint m_indexFAI[5];
		uint m_indexFlat[5];
		uint m_bestFree;
		uint m_bestFAI;
		uint m_bestFlat;
		uint m_maxdist; // max distance in meters between two points
};

#endif
