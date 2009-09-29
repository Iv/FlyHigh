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
#include <qptrlist.h>
#include <qsortedlist.h>
#include "AirSpaceItem.h"
#include "BoundBox.h"
#include "WayPoint.h"

class AirSpace
{
	public:
		enum {MaxNameSize=17};
		typedef QSortedList<AirSpace> AirSpaceListType;

		AirSpace();

		virtual ~AirSpace();

		const QString& name();

		void setName(const QString &name);

		const QString& high();

		void setHigh(const QString & high);

		const QString& low();

		void setLow(const QString & low);

		const QString& airspaceClass();

		void setAirspaceClass(const QString &airspaceClass);

		AirSpaceItem::AirSpaceItemListType& airSpaceItemList();

		void setWarnDist(uint meters);

		uint warnDist();

		void setRemark(const QString &remark);

		const QString& remark();

		void createPointList();

		const WayPoint::WayPointListType& pointList() const;

		const BoundBox& boundBox() const;

		bool isInside(const WayPoint &wp) const;
		
		bool operator<(const AirSpace &airspace);

		bool operator==(const AirSpace &airspace);
	
	private:
		QString m_name;
		QString m_airspaceClass;
		QString m_high;
		QString m_low;
		AirSpaceItem::AirSpaceItemListType m_airSpaceItemList;
		WayPoint::WayPointListType m_pointList;
		BoundBox m_boundBox;
		uint m_warnDist;
		QString m_remark;
};

#endif
