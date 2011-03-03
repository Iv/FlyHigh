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

#include "Flight.h"
#include "Protocol5020.h"
#include "Tokenizer.h"

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
	m_device.sendTlg(tlg);

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
	m_device.sendTlg(tlg);
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

#if 0
	res = flytec_ll_recieve(ProductResp, &buff[0], &len);
	
	if(res == 0)
	{
		res = memcmp((char*)&buff[0], "PBRTL", 5);
	}
	
	if(res == 0)
	{
		/* nof tracks */
		buff[8] = '\0';
		pTrack->totalNum = atoi((char*)&buff[6]);
		
		/* track number */
		buff[11] = '\0';
		pTrack->trackNum = pTrack->totalNum - atoi((char*)&buff[9]) - 1;
		
		/* date */
		buff[14] = '\0';
		pTrack->date.day= atoi((char*)&buff[12]);
		buff[17] = '\0';
		pTrack->date.month= atoi((char*)&buff[15]);
		buff[20] = '\0';
		pTrack->date.year= 2000 + atoi((char*)&buff[18]);
		
		/* start */
		buff[23] = '\0';
		pTrack->start.hour = atoi((char*)&buff[21]);
		buff[26] = '\0';
		pTrack->start.min = atoi((char*)&buff[24]);
		buff[29] = '\0';
		pTrack->start.sec = atoi((char*)&buff[27]);
		
		/* duration */
		buff[32] = '\0';
		pTrack->duration.hour = atoi((char*)&buff[30]);
		buff[35] = '\0';
		pTrack->duration.min = atoi((char*)&buff[33]);
		buff[38] = '\0';
		pTrack->duration.sec = atoi((char*)&buff[36]);
	}
#endif
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

void Protocol5020::addTail(QString &tlg) const
{
	QString checkSum;

	tlg += "*";
	tlg += getCheckSum(tlg, tlg.size() - 1);
	tlg += "\r\n";
}

QString Protocol5020::getCheckSum(const QString &tlg, uint end) const
{
	enum {CheckSumSize = 2};
	uint charNr;
	char checkSum = 0;
	char strCheckSum[CheckSumSize + 1];

	for(charNr=1; charNr<end; charNr++)
	{
		checkSum ^= tlg.at(charNr).toAscii();
	}

	snprintf(strCheckSum, CheckSumSize + 1, "%02X", checkSum);

	return strCheckSum;
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
