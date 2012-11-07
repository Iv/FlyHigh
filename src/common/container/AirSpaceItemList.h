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
#ifndef AirSpaceItemList_h
#define AirSpaceItemList_h

#include <QVector>

class AirSpaceItem;

class AirSpaceItemList
{
	public:
    typedef QVector<AirSpaceItem*> AirSpaceItemListType;

    typedef AirSpaceItemListType::iterator iterator;

    typedef AirSpaceItemListType::const_iterator const_iterator;

		AirSpaceItemList();

		virtual ~AirSpaceItemList();

		void push_back(AirSpaceItem *pItem);

		void clear();

		int size() const;

		AirSpaceItem* at(int pos);

		iterator begin();

		const_iterator begin() const;

		iterator end();

		const_iterator end() const;

	private:
    AirSpaceItemListType m_itemList;
};

#endif
