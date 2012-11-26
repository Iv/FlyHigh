/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#ifndef AirSpaces_h
#define AirSpaces_h

#include "DataBaseSub.h"
#include "AirSpace.h"

class QSqlDatabase;
class AirSpaceList;

class AirSpaces: public DataBaseSub
{
	public:
		AirSpaces(QSqlDatabase DB);

		bool add(AirSpace &airspace);

		bool updateAirSpace(const AirSpace &airspace);

		bool delAirSpace(const AirSpace &airspace);

		bool airspace(const QString &name, AirSpace &airspace);

		bool airspaceList(AirSpaceList &airspaceList);

		bool checkModified();

  signals:
		void changed();

	private:
		bool airSpaceItems(int id, LatLngList &itemList);
};

#endif
