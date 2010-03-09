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

#include <math.h>
#include "Protocol6015.h"
#include "Tokenizer.h"
#include "WayPoint.h"

Protocol6015::Protocol6015()
{
	m_pos = 0;
}

Protocol6015::~Protocol6015()
{
	close();
}

bool Protocol6015::open(const std::string &dev, int baud)
{
	bool success;

	success = m_device.openDevice(dev, baud);

	return success;
}

void Protocol6015::close()
{
	m_device.closeDevice();
}

bool Protocol6015::recieveDone()
{
	QString response;
	bool success = false;

	if(m_device.recieveTlg(100))
	{
		response = m_device.getTlg();
		success = (response == " Done\r\n");
	}

	return success;
}

bool Protocol6015::trackListReq()
{
	QString request = "ACT_20_00\r\n";
	bool success;

	success = m_device.sendTlg(request);

	return success;
}

bool Protocol6015::trackListRec(Flight &flight)
{
	QString tlg;
	bool success = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();

		if(tlg != " Done\r\n")
		{
			success = parseTrack(tlg, flight);
		}
	}

	return success;
}

bool Protocol6015::trackReq(int trackNr)
{
	QString request;
	bool success;

	request.sprintf("ACT_21_%02x\r\n", trackNr);
	success = m_device.sendTlg(request);

	return success;
}

bool Protocol6015::trackRec(QString &line)
{
	bool success = false;

	if(m_device.recieveTlg(100))
	{
		line = m_device.getTlg();
		success = (line.length() > 0);
	}

	return success;
}

bool Protocol6015::wpListReq()
{
	QString request;
	bool success;

	request.sprintf("ACT_31_00\r\n");
	success = m_device.sendTlg(request);

	return success;
}

bool Protocol6015::wpListRec(WayPoint &wp)
{
	QString tlg;
	bool success = false;

	if(m_device.recieveTlg(200))
	{
		tlg = m_device.getTlg();

		if((tlg != " Done\r\n") && (tlg != "No Data\r\n"))
		{
			success = parseWp(tlg, wp);
		}
	}

	return success;
}

bool Protocol6015::wpSnd(const WayPoint &wp)
{
	QString request;
	QString response;
	char dir;
	bool success = false;

	request = "ACT_32_00\r\n";
	m_device.sendTlg(request);

	// Name
	request = qString2ftString(wp.name(), 16);
	request += ';';

	// lat
	if(wp.latitude() < 0)
	{
		dir = 'S';
	}
	else
	{
		dir = 'N';
	}

	request += deg2ftString(wp.latitude(), 12, dir);
	request += ';';

	// lon
	if(wp.longitude() < 0)
	{
		dir = 'W';
	}
	else
	{
		dir = 'E';
	}

	request += deg2ftString(wp.longitude(), 12, dir);
	request += ';';

	// alt
	request += value2ftString(wp.altitude(), 6);
	request += ';';

	// cylinder radius default 400 m
	request += value2ftString(400, 6);
	request += "\r\n";

	m_device.sendTlg(request);

	// Recieve Done
	if(m_device.recieveTlg(125))
	{
		response = m_device.getTlg();
		success = (response == " Done\r\n");
	}

	return success;
}

bool Protocol6015::wpDelAll()
{
	QString request = "ACT_30_00\r\n";
	QString response;
	bool success = false;

	success = m_device.sendTlg(request);

	return success;
}

bool Protocol6015::parseTrack(const QString &tlg, Flight &flight)
{
	Tokenizer tokenizer;
	QString token;
	QTime time;

	// number
	tokenizer.getFirstToken(tlg, ';', token);
	flight.setNumber(token.toInt());

	// date
	tokenizer.getNextToken(tlg, ';', token);
	flight.setDate(parseDate(token));

	// time
	tokenizer.getNextToken(tlg, ';', token);
	flight.setTime(parseTime(token));

	// utc offset
	tokenizer.getNextToken(tlg, ';', token); // skip

	// duration
	tokenizer.getNextToken(tlg, ';', token);
	time = parseTime(token);
	flight.setDuration(time.hour() * 3600 + time.minute () * 60 + time.second());

	return true;
}

bool Protocol6015::parseWp(const QString &tlg, WayPoint &wp)
{
	Tokenizer tokenizer;
	QString token;

	// name
	tokenizer.getFirstToken(tlg, ';', token);
	wp.setName(ftString2qString(token));

	// latitude
	tokenizer.getNextToken(tlg, ';', token);
	wp.setLatitude(ftString2Deg(token));

	// longitude
	tokenizer.getNextToken(tlg, ';', token);
	wp.setLongitude(ftString2Deg(token));

	// altitude
	tokenizer.getNextToken(tlg, ';', token);
	wp.setAltitude(token.toInt());

	return true;
}

QDate Protocol6015::parseDate(const QString &token)
{
	Tokenizer tokenizer;
	QString timeToken;
	int year;
	int month;
	int day;

	tokenizer.getFirstToken(token, '.', timeToken);
	year = timeToken.toInt() + 2000;
	tokenizer.getNextToken(token, '.', timeToken);
	month = timeToken.toInt();
	tokenizer.getNextToken(token, '\0', timeToken);
	day = timeToken.toInt();

	return QDate(year, month, day);
}

QTime Protocol6015::parseTime(const QString &token)
{
	Tokenizer tokenizer;
	QString timeToken;
	int hour;
	int min;
	int sec;

	tokenizer.getFirstToken(token, ':', timeToken);
	hour = timeToken.toInt();
	tokenizer.getNextToken(token, ':', timeToken);
	min = timeToken.toInt();
	tokenizer.getNextToken(token, '\0', timeToken);
	sec = timeToken.toInt();

	return QTime(hour, min, sec);
}

QString Protocol6015::value2ftString(int value, int length)
{
	QString ftStr;
	QString pad;
	QString strValue;

	strValue.sprintf("%i", value);
	strValue.truncate(length);
	pad.fill(' ', length - strValue.length());
	ftStr = pad;
	ftStr += strValue;

	return ftStr;
}

QString Protocol6015::deg2ftString(double value, int length, char dir)
{
	QString ftStr;
	QString degStr;
	QString pad;
	double degValue;
	double minValue;

	if(value < 0)
	{
		value *= (-1);
	}

	degValue = floor(value);
	minValue = (value - degValue) * 60;

	degStr.sprintf("%.0f'%06.3f", degValue, minValue);
	pad.fill(' ', length - degStr.length() - 1);

	ftStr = dir;
	ftStr += pad;
	ftStr += degStr;

	return ftStr;
}

/* format: "[N|S|E|W] dddd[d]'mm.mmm" */
double Protocol6015::ftString2Deg(const QString &token)
{
	Tokenizer subTokenizer;
	QString subToken;
	char dir;
	double deg;

	subTokenizer.getFirstToken(token, ' ', subToken);
	dir = token.at(0);

	subTokenizer.getNextToken(token, '\'', subToken);
	deg = subToken.toDouble();

	subTokenizer.getNextToken(token, ' ', subToken);
	deg += (subToken.toDouble() / 60);

	if((dir == 'W') || (dir == 'S'))
	{
		deg *= (-1); // negate values below equator and west
	}

	return deg;
}

QString Protocol6015::qString2ftString(const QString &qString, uint length)
{
	QString pad;
	QString ftString;

	if(qString.length() < length)
	{
		pad.fill(' ', length - qString.length());
		ftString = qString;
		ftString += pad;
	}
	else
	{
		ftString = qString;
		ftString.truncate(length);
	}

	return ftString;
}

QString Protocol6015::ftString2qString(const QString &ftString)
{
	int cpyLength;
	
	for(cpyLength=ftString.length(); cpyLength>0; cpyLength--)
	{
		if(ftString[cpyLength-1] != ' ')
		{
			break;
		}
	}

	return ftString.left(cpyLength);
}
