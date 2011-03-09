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

#include <math.h>
#include "Flight.h"
#include "Protocol5020.h"
#include "Tokenizer.h"
#include "WayPoint.h"

#include <QtDebug>

Protocol5020::Protocol5020()
{
}

Protocol5020::~Protocol5020()
{
	close();
}

bool Protocol5020::open(const QString &dev, int baud)
{
	bool success;

	success = m_device.openDevice(dev, baud);

	return success;
}

void Protocol5020::close()
{
	m_device.closeDevice();
}

bool Protocol5020::devInfoReq()
{
	QString tlg;

	tlg = "$PBRSNP,";
	addTail(tlg);

	return m_device.sendTlg(tlg);

qDebug() << "devInfoReq" << tlg;
}

bool Protocol5020::devInfoRec(DeviceInfo &devInfo)
{
	if(m_device.recieveTlg(500))
	{
qDebug() << m_device.getTlg();
//		printf("rec: %s\n", m_device.getTlg());
	}
}

bool Protocol5020::trackListReq()
{
	QString tlg;

	tlg = "$PBRTL,";
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::trackListRec(uint &total, Flight &flight)
{
	Tokenizer tokenizer;
	QString token;
	QString tlg;
	bool valid = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();

		tokenizer.getFirstToken(tlg, ',', token); // $PBRTL
		valid = (token == "$PBRTL");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
			// total
			tokenizer.getNextToken(tlg, ',', token);
			total = token.toUInt();

			// nr
			tokenizer.getNextToken(tlg, ',', token);
			flight.setNumber(token.toUInt());

			// date
			tokenizer.getNextToken(tlg, ',', token);
			flight.setDate(parseDate(token));

			// time
			tokenizer.getNextToken(tlg, ',', token);
			flight.setTime(parseTime(token));

			// duration
			tokenizer.getNextToken(tlg, '*', token);
			flight.setDuration(parseTime(token));
		}
	}

	return valid;
}

bool Protocol5020::trackReq(uint trackNr)
{
	QString tlg;

	tlg = "$PBRTR,";
	tlg += QString::number(trackNr).rightJustified(2, '0');
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::trackRec(QString &line)
{
	bool success;

	success = m_device.recieveTlg(500, false);

	if(success)
	{
		line = m_device.getTlg();
	}

	return success;
}

bool Protocol5020::wpListReq()
{
	QString tlg;

	tlg = "$PBRWPS,";
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::wpListRec(WayPoint &wp)
{
	Tokenizer tokenizer;
	QString token;
	QString degToken;
	QString dirToken;
	QString tlg;
	bool valid = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();

		tokenizer.getFirstToken(tlg, ',', token); // $PBRWPS
		valid = (token == "$PBRWPS");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
			// latitude
			tokenizer.getNextToken(tlg, ',', degToken);
			tokenizer.getNextToken(tlg, ',', dirToken);
			wp.setLatitude(parseDeg(degToken, dirToken));

			// longitude
			tokenizer.getNextToken(tlg, ',', degToken);
			tokenizer.getNextToken(tlg, ',', dirToken);
			wp.setLongitude(parseDeg(degToken, dirToken));

			// short name
			tokenizer.getNextToken(tlg, ',', token); // skip

			// name
			tokenizer.getNextToken(tlg, ',', token);
			wp.setName(ftString2qString(token));

			// altitude
			tokenizer.getNextToken(tlg, '*', token);
			wp.setAltitude(token.toInt());
		}
	}

	return valid;
}

bool Protocol5020::wpSnd(const WayPoint &wp)
{
	QString tlg;

	tlg = "$PBRWPR,";

	// latitude
	tlg += degToString(wp.latitude(), 8);

	if(wp.latitude() < 0)
	{
		tlg += ",S,";
	}
	else
	{
		tlg += ",N,";
	}

	// longitude
	tlg += degToString(wp.longitude(), 9);

	if(wp.longitude() < 0)
	{
		tlg += ",W,";
	}
	else
	{
		tlg += ",E,";
	}

	// separator
	tlg += ",";

	// name
	tlg += qString2ftString(wp.name(), 17);
	tlg += ",";

	// altitude
	tlg += QString::number(wp.altitude()).rightJustified(4, '0');

	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::wpDel(const QString wpName)
{
	QString tlg;

// why this does not work???

	tlg = "$PBRWPX,";
	tlg += qString2ftString(wpName, 17);
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::wpDelAll()
{
}

QDate Protocol5020::parseDate(const QString &token) const
{
	Tokenizer tokenizer;
	QString timeToken;
	int year;
	int month;
	int day;

	tokenizer.getFirstToken(token, '.', timeToken);
	day = timeToken.toInt();
	tokenizer.getNextToken(token, '.', timeToken);
	month = timeToken.toInt();
	tokenizer.getNextToken(token, '\0', timeToken);
	year = timeToken.toInt() + 2000;

	return QDate(year, month, day);
}

QTime Protocol5020::parseTime(const QString &token) const
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

double Protocol5020::parseDeg(const QString &degToken, const QString &dirToken)
{
	Tokenizer subTokenizer;
	QString subToken;
	QChar dir;
	int intValue;
	double dValue;
	double deg;

	dValue = degToken.toDouble() / 100.0;
	intValue = floor(dValue);
	deg = intValue + (dValue - intValue) * 100 / 60;

	if((dirToken == "W") || (dirToken == "S"))
	{
		deg *= (-1); // negate values below equator and west
	}

	return deg;
}

QString Protocol5020::degToString(double deg, int size) const
{
	int intValue;
	double value;

	intValue = floor(deg);
	value = intValue * 100 + (deg - intValue) * 60;

	return QString::number(value, 'f', 3).rightJustified(size, '0');
}

QString Protocol5020::qString2ftString(const QString &qString, uint length)
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

QString Protocol5020::ftString2qString(const QString &ftString)
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

void Protocol5020::addTail(QString &tlg) const
{
	QString checkSum;

	tlg += "*";
	tlg += getCheckSum(tlg, tlg.size() - 1);
	tlg += "\r\n";
}

QString Protocol5020::getCheckSum(const QString &tlg, uint end) const
{
	uint charNr;
	char checkSum = 0;

	for(charNr=1; charNr<end; charNr++)
	{
		checkSum ^= tlg.at(charNr).toAscii();
	}

	return QString::number(checkSum, 16).rightJustified(2, '0').toUpper();
}

bool Protocol5020::validateCheckSum(const QString &tlg) const
{
	Tokenizer tokenizer;
	QString token;
	QString checkSum;

	tokenizer.getFirstToken(tlg, '*', token);
	tokenizer.getNextToken(tlg, '\r', token);
	checkSum = getCheckSum(tlg, tlg.size() - 5);

	return (checkSum == token);
}
