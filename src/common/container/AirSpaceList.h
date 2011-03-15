/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#ifndef AirSpaceList_h
#define AirSpaceList_h

#include <QList>

class AirSpace;

class AirSpaceList
{
	public:
		typedef QList <AirSpace*> AirSpaceListType;

		typedef AirSpaceListType::iterator iterator;

		AirSpaceList();

		void push_back(AirSpace *pAirSpace);

		void clear();

		uint size() const;

		AirSpace* operator[] (int index);

		const AirSpace* at(int index) const;

		iterator begin();

		iterator end();

		void sort();

	private:


		AirSpaceListType m_airSpaceList;
};

#endif
