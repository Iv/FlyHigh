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
#ifndef OpenAir_h
#define OpenAir_h

#include <QString>
#include "OpenAirItem.h"
#include "BoundBox.h"
#include "LatLng.h"

class AirSpaceItemList;

class OpenAir
{
	public:
		enum {MaxNameSize=17};

		OpenAir();

		virtual ~OpenAir();

    int id() const;

		void setId(int id);

		const QString& name() const;

		void setName(const QString &name);

		int high() const;

		void setHigh(int high);

		int low() const;

		void setLow(int low);

		const QString& airspaceClass() const;

		void setAirspaceClass(const QString &airspaceClass);

		OpenAirItemList& airSpaceItemList();

		void setWarnDist(uint meters);

		uint warnDist() const;

		void setRemark(const QString &remark);

		const QString& remark() const;

		void createPointList(LatLngList &pointList);

		const BoundBox& boundBox() const;

	private:
    int m_id;
		QString m_name;
		QString m_airspaceClass;
		int m_high;
		int m_low;
		OpenAirItemList m_itemList;
		BoundBox m_boundBox;
		QString m_remark;
    uint m_warnDist;

		bool getNextBear(bool dir, double endBear, double prevBear, double &bear);
};

#endif
