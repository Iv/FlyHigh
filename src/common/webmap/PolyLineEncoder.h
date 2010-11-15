/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
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

#ifndef _PolyLineEncoder_h
#define _PolyLineEncoder_h

#include "WayPoint.h"

class PolyLineEncoder
{
	public:
		PolyLineEncoder(const WayPoint::WayPointListType &wpList);

		~PolyLineEncoder();

		void dpEncode(const WayPoint::WayPointListType &points);

	private:
		typedef std::vector<double> ZoomLevelBreakType;

		typedef std::vector<double> DistsType;

		typedef struct StackItem
		{
			int i;
			int j;
		}StackItem;

		static const double undefined;

		WayPoint::WayPointListType m_wpList;

		ZoomLevelBreakType m_zoomLevelBreaks;
		uint m_numLevels;
		uint m_zoomFactor;
		double m_verySmall;
		bool m_forceEndpoints;

		std::string createEncodings(const WayPoint::WayPointListType &points, DistsType &dists);

		std::string encodeLevels(const WayPoint::WayPointListType &points, DistsType &dists, double absMaxDist);

		uint computeLevel(double dd);

		std::string encodeSignedNumber(int num);

		std::string encodeNumber(uint num);

		double distance(const WayPoint &p0, const WayPoint &p1, const WayPoint &p2, double segLength);
};

#endif
