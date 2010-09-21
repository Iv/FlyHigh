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
#include <qfile.h>
#include <q3textstream.h>
#include "KmlWriter.h"

KmlWriter::KmlWriter()
{
	m_triangle = false;
}

KmlWriter::~KmlWriter()
{
}

void KmlWriter::setFlight(Flight &flight)
{
	m_flight = flight;
}

void KmlWriter::setDeparture(const FlightPointList::FlightPointType &dep)
{
	m_departure = dep;
}

void KmlWriter::set1stWayPoint(const WayPoint &wp)
{
	m_1stWayPoint = wp;
}

void KmlWriter::set2ndWayPoint(const WayPoint &wp)
{
	m_2ndWayPoint = wp;
}

void KmlWriter::set3rdWayPoint(const WayPoint &wp)
{
	m_3rdWayPoint = wp;
}

void KmlWriter::setFinish(const FlightPointList::FlightPointType &fin)
{
	m_finish = fin;
}

void KmlWriter::setFlightPoints(FlightPointList &flightPoints)
{
	m_flightPointList = flightPoints;
}

void KmlWriter::setTriangle(bool triangle)
{
	m_triangle = triangle;
}

bool KmlWriter::save(const QString & name)
{
	QFile file;
	QString str;
	bool success;
	
        file.setFileName(name);
	success = file.open(QIODevice::WriteOnly);
	
	if(success)
	{
		Q3TextStream s(&file);
	
		s << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		s << "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n";

		s << "  <Folder>";
		m_flight.pilot().fullName(str);
		s << "   <name>XC flight: " << m_flight.startPt().name() << " by " << str << "</name>";

		str.sprintf("%.3f km ", (float)m_flight.distance() / 1000);
		s << "   <description>"
			<< "Flight over " << str
			<< "from " << m_flight.startPt().name() << " to " << m_flight.landPt().name() << " at "
			<< m_flight.date().toString("d MMM yyyy")
			<< "</description>\n";

		streamCoordinates(s);
		streamOLC(s);
		streamStart(s);
		streamLanding(s);

		s << "  </Folder>\n";
		s << "</kml>\n";

		file.close();
	}

	return success;
}

void KmlWriter::streamCoordinates(Q3TextStream& s)
{
	uint index;
	uint lastIndex;

	s << "   <Placemark>\n";
	s << "     <name>GPS tracklog</name>\n";
	s << "     <description></description>\n";
	s << "     <visibility>1</visibility>\n";
	s << "     <open>0</open>\n";
	s << "     <Style>\n";
	s << "       <LineStyle>\n";
	s << "         <color>ff00a5ff</color>\n";
	s << "         <width>3</width>\n";
	s << "       </LineStyle>\n";
	s << "     </Style>\n";
	s << "     <LineString>\n";
	s << "     <altitudeMode>absolute</altitudeMode>\n";
	s << "     <coordinates>\n";

	lastIndex = m_flightPointList.size();

	for(index=0; index<lastIndex; index++)
	{
		s << m_flightPointList[index].wp.longitude() << ","
			<< m_flightPointList[index].wp.latitude() << ","
			<< m_flightPointList[index].wp.altitude() << " ";

		if((index % 5) == 0)
		{
			s << "\n";
		}
	}

	s << "     </coordinates>\n";
	s << "   </LineString>\n";
	s << " </Placemark>\n";
}

void KmlWriter::streamOLC(Q3TextStream& s)
{
	s << "   <Placemark>\n";
	s << "     <name>IGC tracklog</name>\n";
	s << "     <description></description>\n";
	s << "     <visibility>1</visibility>\n";
	s << "     <open>0</open>\n";
	s << "     <Style>\n";
	s << "       <LineStyle>\n";
	s << "         <color>ffffffff</color>\n";
	s << "         <width>2</width>\n";
	s << "       </LineStyle>\n";
	s << "     </Style>\n";
	s << "     <LineString>\n";
	s << "     <altitudeMode>absolute</altitudeMode>\n";
	s << "     <coordinates>\n";

	if(!m_triangle)
	{
		s << m_departure.wp.longitude() << ","
			<< m_departure.wp.latitude() << ","
			<< m_departure.wp.altitude() << " ";
	}

	s << m_1stWayPoint.longitude() << ","
		<< m_1stWayPoint.latitude() << ","
		<< m_1stWayPoint.altitude() << " ";

	s << m_2ndWayPoint.longitude() << ","
		<< m_2ndWayPoint.latitude() << ","
		<< m_2ndWayPoint.altitude() << " ";

	s << m_3rdWayPoint.longitude() << ","
		<< m_3rdWayPoint.latitude() << ","
		<< m_3rdWayPoint.altitude() << " ";

	if(!m_triangle)
	{
		s << m_finish.wp.longitude() << ","
			<< m_finish.wp.latitude() << ","
			<< m_finish.wp.altitude() << "\n";
	}
	else
	{
		s << m_1stWayPoint.longitude() << ","
			<< m_1stWayPoint.latitude() << ","
			<< m_1stWayPoint.altitude() << " ";
	}

	s << "     </coordinates>\n";
	s << "   </LineString>\n";
	s << " </Placemark>\n";
}

void KmlWriter::streamStart(Q3TextStream& s)
{
	s << "  <Placemark>\n";
	s << "    <name>Start</name>\n";
	s << "    <Style>\n";
	s << "      <IconStyle>\n";
	s << "        <scale>0.6</scale>\n";
	s << "        <Icon>\n";
	s << "          <href>http://www.pgweb.cz/img/maps/start.png</href>\n";
	s << "        </Icon>\n";
	s << "      </IconStyle>\n";
	s << "    </Style>\n";
	s << "    <Point>\n";
	s << "      <altitudeMode>absolute</altitudeMode>\n";
	s << "      <coordinates>"
		<< m_departure.wp.longitude() << ","
		<< m_departure.wp.latitude() << ","
		<< m_departure.wp.altitude() << " "
		<< "</coordinates>\n";
	s << "    </Point>\n";
	s << "  </Placemark>\n";
}

void KmlWriter::streamLanding(Q3TextStream& s)
{
	s << "  <Placemark>\n";
	s << "    <name>Landing</name>\n";
	s << "    <Style>\n";
	s << "      <IconStyle>\n";
	s << "        <scale>0.6</scale>\n";
	s << "        <Icon>\n";
	s << "          <href>http://www.pgweb.cz/img/maps/cil.png</href>\n";
	s << "        </Icon>\n";
	s << "      </IconStyle>\n";
	s << "    </Style>\n";
	s << "    <Point>\n";
	s << "      <altitudeMode>absolute</altitudeMode>\n";
	s << "      <coordinates>"
		<< m_finish.wp.longitude() << ","
		<< m_finish.wp.latitude() << ","
		<< m_finish.wp.altitude() << " "
		<< "</coordinates>\n";
	s << "    </Point>\n";
	s << "  </Placemark>\n";
}
