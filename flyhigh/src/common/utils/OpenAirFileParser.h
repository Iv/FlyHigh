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
 
#ifndef OpenAirFileParser_h
#define OpenAirFileParser_h

#include <qcstring.h>
#include "AirSpace.h"

class OpenAirFileParser
{
	public:
		OpenAirFileParser();
		
		void parse(QByteArray &openAirData);
		AirSpace::AirSpaceListType& airspaceList();
		
	private:
		typedef enum RecordType
		{
			NewAirspace, Class, Name, High, Low, Variable, Point, Arc, Circle, Unknown
		};
		
		AirSpace::AirSpaceListType m_airspaceList;
		
		RecordType recordType(char *record);
		void parseClass(char *record, AirSpace &airspace);
		void parseName(char *record, AirSpace &airspace);
		void parseHigh(char *record, AirSpace &airspace);
		void parseLow(char *record, AirSpace &airspace);
		void parsePoint(char *record, AirSpace &airspace);
		void parseArc(char *record, bool cw, AirSpace::EdgePointType &center, AirSpace &airspace);
		void parseCircle(char *record, AirSpace::EdgePointType &center, AirSpace &airspace);
		bool parseCoordinate(char *record, double &lat, double &lon);
};

#endif
