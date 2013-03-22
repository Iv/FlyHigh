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

#ifndef FlightPointList_h
#define FlightPointList_h

#include <QDateTime>
#include <QVector>
#include "FlightPoint.h"
#include "WayPoint.h"

class BoundBox;

class FlightPointList
{
	public:
    typedef QVector<FlightPoint*> FlighPointListType;

    typedef FlighPointListType::iterator iterator;

		typedef QVector<float> SogListType;

		typedef QVector<float> VarioListType;

		FlightPointList(bool dataOwner = true);

		~FlightPointList();

		void setDataOwner(bool dataOwner);

		void push_back(FlightPoint *pFlightPoint);

		void clear();

		uint size() const;

		int firstValidFlightData() const;

		int lastValidFlightData() const;

		// returns the speed in m/s
		double speedH(uint index1, uint index2) const;

		double speedV(uint index1, uint index2) const;

		// returns duration in s
		int duration(uint index1, uint index2) const;

		FlightPoint* operator[] (int index);

		const FlightPoint* operator[] (int index) const;

    FlightPoint* at(int index);

		const FlightPoint* at(int index) const;

		iterator begin();

		iterator end();

		void simplify(FlightPointList &fpList) const;

		void boundBox(BoundBox &bbox);

	private:
    typedef QVector<bool> MarkerBuffer;

    FlighPointListType m_flightPointList;
    bool m_dataOwner;

    void douglasPeucker(const FlighPointListType &ptList, int begin, int end,
                        MarkerBuffer &marker, float epsilon) const;
};

#endif
