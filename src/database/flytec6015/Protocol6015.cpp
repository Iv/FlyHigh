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

#include "Protocol6015.h"
#include "Tokenizer.h"

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

	if(m_device.recieveTlg(100))
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
