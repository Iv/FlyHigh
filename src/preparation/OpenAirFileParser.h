/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#ifndef OpenAirFileParser_h
#define OpenAirFileParser_h

#include "OpenAir.h"

class AirSpaceList;
class OpenAir;

class OpenAirFileParser
{
	public:
    typedef enum Unit{Feet, Meter}Unit;

		OpenAirFileParser();

		~OpenAirFileParser();

		void setDefaultUnit(Unit unit);

		bool parse(const QString &fileName, AirSpaceList &airspaceList);

	private:
    typedef QList<OpenAir*> OpenAirList;

    OpenAirList m_airspaceList;
		double m_arcCenterLat;
		double m_arcCenterLon;
		Unit m_defUnit;
		bool m_arcDir;

		void parseString(char *pRecord, QString &str);

		void parseAirspaceClass(char *pRecord, OpenAir *pOpenAir);

		void parseVarAssign(char *pRecord);

		void parsePoint(char *pRecord, OpenAir *pOpenAir);

		void parseArc(char *pRecord, OpenAir *pOpenAir);

		void parseCircle(char *pRecord, OpenAir *pOpenAir);

		bool parseCoordinate(char *pRecord, double &lat, double &lon);

		bool parseAlt(const QString &str, float &alt);

		void clearOpenAirList();
};

#endif
