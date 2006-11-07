/***************************************************************************
 *   Copyright (C) 2006 by Alex Graf                                     *
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
#ifndef Pilots_h
#define Pilots_h

#include "DataBaseSub.h"
#include "Pilot.h"

/**
@author Alex Graf
*/
class Pilots: public DataBaseSub
{
	public:
		Pilots(QSqlDatabase *pDB);
		~Pilots();

		bool add(Pilot &pilot);
		bool update(Pilot &pilot);
		bool pilot(int id, Pilot &pilot);
		bool setId(Pilot &pilot);

	private:
		enum Elements
		{
			Id, FirstName, LastName, BirthDate, CallSign, GliderId
		};
};

#endif
