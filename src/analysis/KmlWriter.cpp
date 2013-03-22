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
#include <qfile.h>
#include "Flight.h"
#include "KmlWriter.h"

KmlWriter::KmlWriter()
{
	m_triangle = false;
  m_departure = 0;
  m_1stTurnPoint = 0;
  m_2ndTurnPoint = 0;
  m_3rdTurnPoint = 0;
  m_finish = 0;
}

KmlWriter::~KmlWriter()
{
}

void KmlWriter::setFlight(Flight &flight)
{
	m_flight = flight;
}

void KmlWriter::setDeparture(int index)
{
	m_departure = index;
}

void KmlWriter::set1stTurnPoint(int index)
{
	m_1stTurnPoint = index;
}

void KmlWriter::set2ndTurnPoint(int index)
{
	m_2ndTurnPoint = index;
}

void KmlWriter::set3rdTurnPoint(int index)
{
	m_3rdTurnPoint = index;
}

void KmlWriter::setFinish(int index)
{
	m_finish = index;
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
		QTextStream s(&file);

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

		s.flush();

		file.close();
	}

	return success;
}

void KmlWriter::streamCoordinates(QTextStream& s)
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
		s << m_flightPointList[index]->pos().lon() << ","
			<< m_flightPointList[index]->pos().lat() << ","
			<< m_flightPointList[index]->alt() << " ";

		if((index % 5) == 0)
		{
			s << "\n";
		}
	}

	s << "     </coordinates>\n";
	s << "   </LineString>\n";
	s << " </Placemark>\n";
}

void KmlWriter::streamOLC(QTextStream& s)
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
		s << m_flightPointList[m_departure]->pos().lon() << ","
			<< m_flightPointList[m_departure]->pos().lat() << ","
			<< m_flightPointList[m_departure]->alt() << " ";
	}

	s << m_flightPointList[m_1stTurnPoint]->pos().lon() << ","
		<< m_flightPointList[m_1stTurnPoint]->pos().lat() << ","
		<< m_flightPointList[m_1stTurnPoint]->alt() << " ";

	s << m_flightPointList[m_2ndTurnPoint]->pos().lon() << ","
		<< m_flightPointList[m_2ndTurnPoint]->pos().lat() << ","
		<< m_flightPointList[m_2ndTurnPoint]->alt() << " ";

	s << m_flightPointList[m_3rdTurnPoint]->pos().lon() << ","
		<< m_flightPointList[m_3rdTurnPoint]->pos().lat() << ","
		<< m_flightPointList[m_3rdTurnPoint]->alt() << " ";

	if(!m_triangle)
	{
		s << m_flightPointList[m_finish]->pos().lon() << ","
			<< m_flightPointList[m_finish]->pos().lat() << ","
			<< m_flightPointList[m_finish]->alt() << "\n";
	}
	else
	{
    s << m_flightPointList[m_1stTurnPoint]->pos().lon() << ","
      << m_flightPointList[m_1stTurnPoint]->pos().lat() << ","
      << m_flightPointList[m_1stTurnPoint]->alt() << " ";
	}

	s << "     </coordinates>\n";
	s << "   </LineString>\n";
	s << " </Placemark>\n";
}

void KmlWriter::streamStart(QTextStream& s)
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
		<< m_flightPointList[m_departure]->pos().lon() << ","
		<< m_flightPointList[m_departure]->pos().lat() << ","
		<< m_flightPointList[m_departure]->alt() << " "
		<< "</coordinates>\n";
	s << "    </Point>\n";
	s << "  </Placemark>\n";
}

void KmlWriter::streamLanding(QTextStream& s)
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
		<< m_flightPointList[m_finish]->pos().lon() << ","
		<< m_flightPointList[m_finish]->pos().lat() << ","
		<< m_flightPointList[m_finish]->alt() << " "
		<< "</coordinates>\n";
	s << "    </Point>\n";
	s << "  </Placemark>\n";
}
