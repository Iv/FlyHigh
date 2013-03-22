/***************************************************************************
 *   Copyright (C) 2012 by Alex Graf                                       *
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

#ifndef BoundBox_h
#define BoundBox_h

#include "LatLng.h"

class BoundBox
{
	public:
		BoundBox();

		BoundBox(const LatLng &sw, const LatLng &ne);

		void init();

		void setMinMax(const LatLng &wp);

		void setMinMax(const BoundBox &bbox);

		void setNorthEast(const LatLng &ne);

		const LatLng& northEast() const;

		double north() const;

		double west() const;

		void setSouthWest(const LatLng &sw);

    const LatLng& southWest() const;

		double south() const;

		double east() const;

		double width() const;

		double height() const;

		bool intersect(const BoundBox &bb) const;

		bool isInside(const LatLng &latlng) const;

	private:
		LatLng m_sw;
		LatLng m_ne;
		bool m_isInit;
};

#endif
