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
#include <math.h>
#include "OLCWebForm.h"

OLCWebForm::OLCWebForm()
{
}

void OLCWebForm::setFlight(Flight &flight)
{
	m_flight = flight;
}

void OLCWebForm::setDeparture(const FlightPointList::FlightPointType &dep)
{
	m_departure = dep;
}

void OLCWebForm::set1stWayPoint(const WayPoint &wp)
{
	m_1stWayPoint = wp;
}

void OLCWebForm::set2ndWayPoint(const WayPoint &wp)
{
	m_2ndWayPoint = wp;
}

void OLCWebForm::set3rdWayPoint(const WayPoint &wp)
{
	m_3rdWayPoint = wp;
}

void OLCWebForm::setFinish(const FlightPointList::FlightPointType &fin)
{
	m_finish = fin;
}

bool OLCWebForm::save(const QString & name)
{
	QFile file;
	QString str;
	bool success;
	
        file.setFileName(name);
	success = file.open(QIODevice::WriteOnly);
	
	if(success)
	{
		Q3TextStream s(&file);
	
		s << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n";
		s << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n";
		s << "<html>\n";
		
		streamHead(s);
		
		s << "	<body>\n";
		s << "		<h1>flight claim for HGPG Online-CONTEST International 2006</h1>\n";
		s << "		<form action=\"http://www.onlinecontest.org/olc-cgi/2006/holc-i/olc\" method=\"get\">\n";
		s << "			<table>\n";
		
		streamName(s);
		streamBirth(s);
		streamTakeoffLoc(s);
		streamCallsign(s);
		streamTrack(s);
		streamFlightDate(s);
		streamModel(s);
		streamTakeOffTime(s);
		streamDepartureTime(s);
		streamWayPoint(s, 0, m_departure.wp);
		streamWayPoint(s, 1, m_1stWayPoint);
		streamWayPoint(s, 2, m_2ndWayPoint);
		streamWayPoint(s, 3, m_3rdWayPoint);
		streamWayPoint(s, 4, m_finish.wp);
		streamFinishTime(s);
		streamComment(s);
		streamSubmit(s);
		
		s << "			</table>\n";
		s << "		</form>\n";
		s << "	</body>\n";
		s << "</html>\n";
		
		file.close();
	}
	
	return success;
}

void OLCWebForm::olcFileName(QString &fileName)
{
	fileName = getOLCchar(m_flight.date().year() % 100);
	fileName += getOLCchar(m_flight.date().month());
	fileName += getOLCchar(m_flight.date().day());
	fileName += m_flight.pilot().lastName().left(4).toLower();
	fileName += "1";
}

void OLCWebForm::streamHead(Q3TextStream& s)
{
	s << "	<head>\n";
	s << "		<title>HGPG Online-CONTEST International flight claim</title>\n";
	s << "		<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\"/>\n";
	s << "	</head>\n";
}

void OLCWebForm::streamName(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>First/Given name</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"60\" size=\"60\" name=\"OLCvnolc\" value=\"" << m_flight.pilot().firstName() << "\"/>\n";
	s << "					</td>\n";
	s << "				</tr>\n";
	s << "				<tr>\n";
	s << "					<td>Surname</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"60\" size=\"60\" name=\"na\" value=\"" << m_flight.pilot().lastName() << "\"/>\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamBirth(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>Date of birth</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"8\" size=\"8\" name=\"geb\" value=\"";
	s << m_flight.pilot().birthDate().toString("dd.MM.yy") << "\"/>&nbsp;&nbsp;(dd.mm.yy)\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamTakeoffLoc(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>Take-off location</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"20\" size=\"20\" name=\"sta\" value=\"";
	s << m_flight.startPt().name() << "\"/>&nbsp;&nbsp;no ICAO-identifiers please!\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamCallsign(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>Callsign</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"20\" size=\"20\" name=\"gid\" value=\"" << m_flight.pilot().callSign() <<"\"/>\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamTrack(Q3TextStream& s)
{
	QString str;
	
	olcFileName(str);
	
	s << "				<tr>\n";
	s << "					<td>IGC-filename</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"8\" size=\"8\" name=\"igcfn\" value=\"";
	s << str <<"\"/>.igc\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamFlightDate(Q3TextStream& s)
{
	QDate refDate(2005, 5, 1);  // <option value=147678 > 1 May 2005 551 &nbsp;OLC\n";
	const int refDays = 147678;
	QString str;
	int daysTo;
	
	daysTo = refDate.daysTo(m_flight.date());
	str.sprintf("%i", refDays + daysTo);

	s << "				<tr>\n";
	s << "					<td>Date of flight</td>\n";
	s << "					<td>\n";
	s << "						<select name=\"ft\" >\n";
	s << "							<option value=\"" << str << "\" selected=\"selected\">" << m_flight.date().toString("d MMM yyyy") << " OLC</option>\n";
	s << "						</select>\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamModel(Q3TextStream& s)
{
	QString model;

	m_flight.glider().olcName(model);

	s << "				<tr>\n";
	s << "					<td>Model of glider</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"50\" size=\"50\" name=\"gty\" value=\"";
	s << model << "\"/>&nbsp;&nbsp;enter [manufacturer glidertype]\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamTakeOffTime(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>Take-off time</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"8\" size=\"8\" name=\"t0\" value=\"";
	s << m_departure.time.toString(Qt::ISODate) << "\"/>&nbsp;&nbsp;(hh:mm:ss) (UTC)\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamDepartureTime(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>Departure time</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"8\" size=\"8\" name=\"s0\" value=\"";
	s << m_departure.time.toString(Qt::ISODate) << "\"/>&nbsp;&nbsp;(hh:mm:ss) (UTC)\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamWayPoint(Q3TextStream& s, uint wpNr, WayPoint &wp)
{
	double lat;
	double lon;
	double deg;
	double min;
	QString strDeg;
	QString strMin;
	QString strDmin;
	QString strWpNr;
	
	strWpNr.sprintf("%i", wpNr);
	
	// begin of table row
	s << "				<tr>\n";
	s << "					<td>";
	
	switch(wpNr)
	{
		case 0:
			s << "Departure";
		break;
		case 1:
			s << "1st waypoint";
		break;
		case 2:
			s << "2nd waypoint";
		break;
		case 3:
			s << "3rd waypoint";
		break;
		case 4:
			s << "Finish";
		break;
	}
	
	s << "</td>\n";
	s << "					<td>\n";
	
	// latitude
	s << "						<select name=\"w" << strWpNr << "bh\">\n";
	
	lat = wp.latitude();
	
	if(lat < 0)
	{
		s << "							<option value=\"N\">N</option>\n";
		s << "							<option value=\"S\" selected=\"selected\">S</option>\n";
		lat *= -1.0;
	}
	else
	{
		s << "							<option value=\"N\" selected=\"selected\">N</option>\n";
		s << "							<option value=\"S\">S</option>\n";
	}
	
	s << "						</select>\n";
	
	// 46.840083
	deg = trunc(lat); // 46
	min = (lat - deg) * 60.0 * 1000.0; // min = 50404.98
	min = round(min); // 50405
	strDeg.sprintf("%02i", (int)deg); // 46
	strMin.sprintf("%02i", (int)min / 1000); // 50
	strDmin.sprintf("%03i", (int)min % 1000); // 405
	
	s << "						<input type=\"text\" value=\"" << strDeg <<  "\" maxlength=\"2\" size=\"2\" name=\"w";
	s << strWpNr << "bg\"/>:<input type=\"text\" value=\"" << strMin << "\" maxlength=\"2\" size=\"2\" name=\"w";
	s << strWpNr << "bm\"/>.<input type=\"text\" value=\"" << strDmin << "\" maxlength=\"3\" size=\"3\" name=\"w";
	s << strWpNr << "bmd\"/>\n";
	
	// longitude
	s << "						<select name=\"w" << strWpNr << "lh\">\n";
	
	lon = wp.longitude();
	
	if(lon < 0)
	{
		s << "							<option value=\"E\">E</option>\n";
		s << "							<option value=\"W\" selected=\"selected\">W</option>\n";
		lon *= -1.0;
	}
	else
	{
		s << "							<option value=\"E\" selected=\"selected\">E</option>\n";
		s << "							<option value=\"W\">W</option>\n";
	}

	s << "						</select>\n";
	
	// 8.422713
	deg = trunc(lon); // 8
	min = (lon - deg) * 60.0 * 1000.0; // min = 25362.78
	min = round(min); // 25363
	strDeg.sprintf("%03i", (int)deg); // 008
	strMin.sprintf("%02i", (int)min / 1000); // 25
	strDmin.sprintf("%03i", (int)min % 1000); // 363
	
	s << "						<input type=\"text\" value=\"" << strDeg <<  "\" maxlength=\"3\" size=\"3\" name=\"w";
	s << strWpNr << "lg\"/>:<input type=\"text\" value=\"" << strMin << "\" maxlength=\"2\" size=\"2\" name=\"w";
	s << strWpNr << "lm\"/>.<input type=\"text\" value=\"" << strDmin << "\" maxlength=\"3\" size=\"3\" name=\"w";
	s << strWpNr << "lmd\"/>\n";
	
	// end of table row
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamFinishTime(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>Finish time</td>\n";
	s << "					<td>\n";
	s << "						<input type=\"text\" maxlength=\"8\" size=\"8\" name=\"s4\" value=\"";
	s << m_finish.time.toString(Qt::ISODate) << "\"/>&nbsp;&nbsp;(hh:mm:ss) (UTC)\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamComment(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td>Comment Pilot</td>\n";
	s << "					<td>\n";
	s << "						<textarea rows=\"5\" cols=\"60\" name=\"cpilo\">\n";
	s <<							 "optimized and claimed with http://flyhigh.sf.net\n" << m_flight.comment();
	s << "						</textarea>\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

void OLCWebForm::streamSubmit(Q3TextStream& s)
{
	s << "				<tr>\n";
	s << "					<td></td>\n";
	s << "					<td>\n";
	s << "						<input type=\"submit\" name=\"test\" value=\"Check entry\"/>\n";
	s << "					</td>\n";
	s << "				</tr>\n";
}

char OLCWebForm::getOLCchar(int value)
{
	char resChar;
	
	if((value >= 0) && (value <=9))
	{
		resChar = '0' + value;
	}
	else if((value >= 10) && (value <=31))
	{
		resChar = 'a' + value - 10;
	}
	else
	{
		resChar = '0';
	}
	
	return resChar;
}
