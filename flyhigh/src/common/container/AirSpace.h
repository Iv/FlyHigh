/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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
#ifndef AirSpace_h
#define AirSpace_h

#include <qstring.h>
#include <qvaluevector.h>
#include "WayPoint.h"

class AirSpace
{
	public:
		typedef QValueVector<AirSpace> AirSpaceListType;

		AirSpace();

		const QString& name();
		void setName(const QString &name);
		const QString& high();
		void setHigh(const QString &high);
		const QString& low();
		void setLow(const QString &low);
		const QString& airspaceClass();
		void setAirspaceClass(const QString &airspaceClass);
		WayPoint::WayPointListType& edgePointList();
		
		AirSpace& operator=(const AirSpace &airspace);
	
	private:
		QString m_name;
		QString m_airspaceClass;
		QString m_high;
		QString m_low;
		WayPoint::WayPointListType m_wpList;
};

#endif
