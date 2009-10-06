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
 
#ifndef BoundBox_h
#define BoundBox_h

#include "WayPoint.h"

class BoundBox
{
	public:
		BoundBox();

		void init();

		void setMinMax(const WayPoint &wp);

		void setMinMax(const BoundBox &bbox);

		void setNorthEast(const WayPoint &ne);

		double north() const;

		double west() const;

		void setSouthWest(const WayPoint &sw);

		double south() const;

		double east() const;

		double width() const;

		double height() const;

		bool intersect(const BoundBox &bb) const;
		
	private:
		WayPoint m_sw;
		WayPoint m_ne;
		bool m_isInit;
};

#endif
