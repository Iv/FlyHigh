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
#ifndef AirSpaceItem_h
#define AirSpaceItem_h

#include <qvaluevector.h>

class AirSpaceItem
{
	public:
		typedef QValueVector<AirSpaceItem*> AirSpaceItemListType;
		typedef enum ItemType{Point, Circle, Center, StartSegment, StopSegment};

		AirSpaceItem();
		virtual ~AirSpaceItem();

		void setPoint(double lat, double lon);
		double lat();
		double lon();
		ItemType type();
	
	protected:
		void setType(ItemType type);

	private:
		ItemType m_type;
		double m_lat;
		double m_lon;
};

class AirSpaceItemPoint: public AirSpaceItem
{
	public:
		AirSpaceItemPoint(AirSpaceItem::ItemType type);
		~AirSpaceItemPoint();

//		virtual AirSpaceItem& operator=(const AirSpaceItem &airSpaceItem);
};

class AirSpaceItemSeg: public AirSpaceItem
{
	public:
		AirSpaceItemSeg(AirSpaceItem::ItemType type);
		~AirSpaceItemSeg();
		
		void setDir(bool dir);
		bool dir();
//		virtual AirSpaceItem& operator=(const AirSpaceItem &airSpaceItem);
		
	private:
		bool m_dir; // + = true = clockwise
};

class AirSpaceItemCircle: public AirSpaceItem
{
	public:
		AirSpaceItemCircle();
		~AirSpaceItemCircle();

		void setRadius(uint radius);
		uint radius();
//		virtual AirSpaceItem& operator=(const AirSpaceItem &airSpaceItem);
	
	private:
		uint m_radius;
};

#endif
