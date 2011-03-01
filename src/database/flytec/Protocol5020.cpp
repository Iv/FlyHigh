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
 
#include "Protocol5020.h"

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
