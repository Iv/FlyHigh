/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                     *
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
#include <byteswap.h>
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

bool Protocol6015::open(const QString &dev, int baud)
{
	bool success;

	success = m_device.openDevice(dev, baud);

//int value = memoryRead(MemPa, DEVICE_NR, UInt32).toUInt();
//int value = memoryRead(MemPa, SW_VERS, UInt16).toUInt();
//int value = memoryRead(MemPa, ALT_DIFF_FLA, UInt32).toUInt();
//memoryWrite(MemFa, AC_TYPE, String, QString("Avax XC 2"));
//QString value = memoryRead(MemFa, AC_TYPE, String).toString();
//int value = memoryRead(MemFa, AUDIO_RISE, UInt16).toUInt();
//memoryWrite(MemFa, PRESS_OFFSET, Int32, -1000);
//int value = memoryRead(MemFa, PRESS_OFFSET, Int32).toUInt();
//QByteArray value = memoryRead(MemPa, BATT_LEVEL_1, Array).toByteArray();


	return success;
}

void Protocol6015::close()
{
	m_device.closeDevice();
}

bool Protocol6015::memoryWrite(MemType memType, int par, DataType dataType, const QVariant &value)
{
	QString tlgValue;
	bool success = false;

	switch(memType)
	{
		case MemFa:
			switch(dataType)
			{
				case UInt8: case Int8:
					writeEnableFa();
					tlgValue.sprintf("%02X", (char)value.toUInt());
					success = writePar(memType, par, tlgValue);
				break;
				case UInt16: case Int16:
					writeEnableFa();
					tlgValue.sprintf("%04X", bswap_16(value.toUInt()));
					success = writePar(memType, par, tlgValue);
				break;
				case UInt32: case Int32:
					writeEnableFa();
					tlgValue.sprintf("%08X", bswap_32(value.toUInt()));
					success = writePar(memType, par, tlgValue);
				break;
				case String:
					writeEnableFa();
					success = writeParString(memType, par, value.toString());
				break;
				case Array:
					writeEnableFa();
					success = writeParArray(memType, par, value.toByteArray());
				break;
			}
		break;
		case MemPa:
		break;
	}

	return success;
}

QVariant Protocol6015::memoryRead(MemType memType, int par, DataType dataType)
{
	QVariant value;

	requestPar(memType, par);

	switch(dataType)
	{
		case UInt8: case Int8: case UInt16: case Int16: case UInt32: case Int32:
			value = readParInt(memType, par, dataType);
		break;
		case String:
			value = readParString(memType, par);
		break;
		case Array:
			value = readParArray(memType, par);
		break;
	}

	return value;
}

bool Protocol6015::recieveDone()
{
	QString tlg;
	bool success = false;

	if(m_device.recieveTlg(100))
	{
		tlg = m_device.getTlg();
		success = (tlg == " Done\r\n");
	}

	return success;
}

bool Protocol6015::trackListReq()
{
	QString tlg = "ACT_20_00\r\n";
	bool success;

	m_device.flush();
	success = m_device.sendTlg(tlg);

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
	QString tlg;
	bool success;

	tlg.sprintf("ACT_21_%02x\r\n", trackNr);
	m_device.flush();
	success = m_device.sendTlg(tlg);

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
	QString tlg;
	bool success;

	tlg.sprintf("ACT_31_00\r\n");
	m_device.flush();
	success = m_device.sendTlg(tlg);

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
	QString tlg;
	bool success = false;

	tlg = "ACT_32_00\r\n";
	m_device.flush();
	m_device.sendTlg(tlg);

	// Waypoint telegram
	getWpSndTlg(wp, tlg);
	m_device.sendTlg(tlg);

	// Recieve Done
	if(m_device.recieveTlg(125))
	{
		tlg = m_device.getTlg();
		success = (tlg == " Done\r\n");
	}

	return success;
}

bool Protocol6015::wpDelAll()
{
	QString tlg = "ACT_30_00\r\n";
	bool success = false;

	m_device.flush();
	success = m_device.sendTlg(tlg);

	return success;
}

bool Protocol6015::routeListReq()
{
	QString tlg;
	bool success;

	tlg.sprintf("ACT_41_00\r\n");
	m_device.flush();
	success = m_device.sendTlg(tlg);

	return success;
}

bool Protocol6015::routeListRec(WayPoint &wp)
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

bool Protocol6015::routeSnd(const WayPoint &wp)
{
	QString tlg;
	bool success = false;

	tlg = "ACT_42_00\r\n";
	m_device.flush();
	m_device.sendTlg(tlg);

	// Waypoint telegram
	getWpSndTlg(wp, tlg);
	m_device.sendTlg(tlg);

	// Recieve Done
	if(m_device.recieveTlg(300))
	{
		tlg = m_device.getTlg();
		success = (tlg == " Done\r\n");
	}

	return success;
}

bool Protocol6015::routeDel()
{
	QString tlg = "ACT_40_00\r\n";
	bool success = false;

	m_device.flush();
	success = m_device.sendTlg(tlg);

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

void Protocol6015::getWpSndTlg(const WayPoint &wp, QString &tlg)
{
	char dir;

	// Name
	tlg = qString2ftString(wp.name(), StringSize);
	tlg += ';';

	// lat
	if(wp.latitude() < 0)
	{
		dir = 'S';
	}
	else
	{
		dir = 'N';
	}

	tlg += deg2ftString(wp.latitude(), 12, dir);
	tlg += ';';

	// lon
	if(wp.longitude() < 0)
	{
		dir = 'W';
	}
	else
	{
		dir = 'E';
	}

	tlg += deg2ftString(wp.longitude(), 12, dir);
	tlg += ';';

	// alt
	tlg += value2ftString(wp.altitude(), 6);
	tlg += ';';

	// cylinder radius default 400 m
	tlg += value2ftString(400, 6);
	tlg += "\r\n";
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

bool Protocol6015::writeEnableFa()
{
	QString tlg = "ACT_82_00\r\n";
	bool success;

	m_device.flush();
	m_device.sendTlg(tlg);

	if(m_device.recieveTlg(100))
	{
		tlg = m_device.getTlg();
		success = (tlg == "ACT_82_00 Done\r\n");
	}

	return success;
}

bool Protocol6015::writePar(MemType memType, int par, const QString &value)
{
	QString tlg;
	QString resp;
	bool success = false;

	switch(memType)
	{
		case MemFa:
			tlg.sprintf("WFA_%02x_", par);
		break;
		case MemPa:
		break;
	}

	tlg += value;
	tlg += "\r\n";

	m_device.flush();
	m_device.sendTlg(tlg);

	if(m_device.recieveTlg(100))
	{
		resp = m_device.getTlg();
		success = (tlg == resp);
	}

	return success;
}

bool Protocol6015::writeParString(MemType memType, int par, const QString &value)
{
	QString ftString;
	QByteArray arrValue;
	bool success;

	ftString = qString2ftString(value, StringSize);
	arrValue.setRawData(ftString.ascii(), ftString.length());
	success = writeParArray(memType, par, arrValue);
	arrValue.resetRawData(ftString.ascii(), ftString.length());

	return success;
}

bool Protocol6015::writeParArray(MemType memType, int par, const QByteArray &value)
{
	QString byte;
	QString tlg;
	uint byteNr;
	bool success;

	for(byteNr=0; byteNr<value.size(); byteNr++)
	{
		byte.setNum(value.at(byteNr), 16);
		tlg += byte.upper();
	}

	success = writePar(memType, par, tlg);

	return success;
}

bool Protocol6015::requestPar(MemType memType, int par)
{
	QString tlg;
	bool success;

	switch(memType)
	{
		case MemFa:
			tlg.sprintf("RFA_%02x\r\n", par);
		break;
		case MemPa:
			tlg.sprintf("RPA_%02x\r\n", par);
		break;
	}

	m_device.flush();
	success = m_device.sendTlg(tlg);

	return success;
}

int Protocol6015::readParInt(MemType memType, int par, DataType dataType)
{
	QString tlg;
	Tokenizer tokenizer;
	QString token;
	int value = 0;
	bool ok;
	bool success;

	if(m_device.recieveTlg(100))
	{
		tlg = m_device.getTlg();
		success = (tlg != "No Par\r\n");

		if(success)
		{
			tokenizer.getFirstToken(tlg, '_', token); // memory

			switch(memType)
			{
				case MemFa:
					success = (token == "RFA");
				break;
				case MemPa:
					success = (token == "RPA");
				break;
			}

			tokenizer.getNextToken(tlg, '_', token); // parameter
		}

		if(success && (token.toInt(&ok, 16) == par))
		{
			tokenizer.getNextToken(tlg, '\r', token);

			switch(dataType)
			{
				case Int8:
					value = (char)token.toInt();
				break;
				case UInt8:
					value = token.toInt();
				break;
				case Int16:
					value = (short)bswap_16(token.toInt(&ok, 16));
				break;
				case UInt16:
					value = bswap_16(token.toInt(&ok, 16));
				break;
				case Int32: case UInt32:
					value = bswap_32(token.toLongLong(&ok, 16));
				break;
				default:
				break;
			}
		}
	}

	return value;
}

QString Protocol6015::readParString(MemType memType, int par)
{
	QString value = "";

	value += readParArray(memType, par);
	value = ftString2qString(value);

	return value;
}

QByteArray Protocol6015::readParArray(MemType memType, int par)
{
	QString tlg;
	Tokenizer tokenizer;
	QString token;
	QByteArray value;
	uint byteNr;
	uint elemNr;
	int byte;
	bool ok;
	bool success;

	if(m_device.recieveTlg(100))
	{
		tlg = m_device.getTlg();
		success = (tlg != "No Par\r\n");

		if(success)
		{
			tokenizer.getFirstToken(tlg, '_', token); // memory

			switch(memType)
			{
				case MemFa:
					success = (token == "RFA");
				break;
				case MemPa:
					success = (token == "RPA");
				break;
			}

			tokenizer.getNextToken(tlg, '_', token); // parameter
		}

		if(success && (token.toInt(&ok, 16) == par))
		{
			tokenizer.getNextToken(tlg, '\r', token);

			if(token.length() >= 2)
			{
				value.resize(token.length() / 2);
				elemNr = 0;

				for(byteNr=0; byteNr<token.length(); byteNr+=2)
				{
					byte = token.mid(byteNr, 2).toInt(&ok, 16);
					value[elemNr] = (char)byte;
					elemNr++;
				}
			}
		}
	}

	return value;
}
