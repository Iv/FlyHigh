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
#ifndef OpenAirItem_h
#define OpenAirItem_h

#include <QVector>
#include "LatLng.h"

class OpenAirItem;

typedef QVector<OpenAirItem*> OpenAirItemList;

class OpenAirItem
{
	public:
		typedef enum ItemType{Point, Circle, Center, StartSegment, StopSegment}ItemType;

		OpenAirItem();

		virtual ~OpenAirItem();

		void setPos(double lat, double lon);

		const LatLng& pos() const;

		double lat();

		double lon();

		ItemType type();

	protected:
		void setType(ItemType type);

	private:
		ItemType m_type;
		LatLng m_pos;
};

class OpenAirItemPoint: public OpenAirItem
{
	public:
		OpenAirItemPoint(OpenAirItem::ItemType type);

		~OpenAirItemPoint();
};

class OpenAirItemSeg: public OpenAirItem
{
	public:
		OpenAirItemSeg(OpenAirItem::ItemType type);

		~OpenAirItemSeg();

		void setDir(bool dir);

		bool dir();

	private:
		bool m_dir; // + = true = clockwise
};

class OpenAirItemCircle: public OpenAirItem
{
	public:
		OpenAirItemCircle();

		~OpenAirItemCircle();

		void setRadius(uint radius);

		uint radius();

	private:
		uint m_radius;
};

#endif
