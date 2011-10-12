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

#include <QString>
#include <sys/timeb.h>
#include "qextserialport.h"
#include "Device.h"

Device::Device(bool flow)
{
	m_tlg = "";
	m_tout = 0;

	m_serialPort = new QextSerialPort(QextSerialPort::Polling);

	if(flow)
	{
    m_serialPort->setFlowControl(FLOW_XONXOFF);
	}
	else
	{
    m_serialPort->setFlowControl(FLOW_OFF);
	}

	m_serialPort->setParity(PAR_NONE);
	m_serialPort->setDataBits(DATA_8);
	m_serialPort->setStopBits(STOP_1);
#ifdef Q_OS_WIN
  // on windoze, set timeout to -1 for
  // non-blocking read/write operations
  m_serialPort->setTimeout(-1);
#else
  m_serialPort->setTimeout(0);
#endif
}

Device::~Device()
{
	delete m_serialPort;
}

bool Device::openDevice(const QString &dev, int baud)
{
	bool success;

	m_serialPort->setPortName(dev);

	if(baud == 57600)
	{
		m_serialPort->setBaudRate(BAUD57600);
	}

	success = m_serialPort->open(QIODevice::ReadWrite);

	return success;
}

bool Device::isOpen()
{
	return m_serialPort->isOpen();
}

void Device::closeDevice()
{
	m_serialPort->close();
}

bool Device::recieveTlg(int tout, bool head)
{
	int charNr = 0;
	bool validTlg = false;
	char ch;
	State state;

	if(head)
	{
		state = SearchHead;
	}
	else
	{
		state = ReadTlg;
	}

	m_tlg = "";
	startTimer(tout);

	do
	{
		if(getChar(ch))
		{
			if(state == SearchHead)
			{
				if(ch == '$')
				{
					state = ReadTlg;
					m_tlg = ch;
				}
			}
			else
			{
				charNr++;
				m_tlg += ch;
				validTlg = (ch == '\n');

				if(charNr > MaxTlgSize)
				{
					m_tlg = "";
				}

				if(validTlg || (charNr > MaxTlgSize))
				{
					break;
				}
			}
		}
	}while(!isElapsed());

	return validTlg;
}

const QString& Device::getTlg()
{
	return m_tlg;
}

bool Device::sendTlg(const QString &tlg)
{
	return writeBuffer(tlg.toAscii().constData(), tlg.length());
}

void Device::flush()
{
	m_tlg = "";
	// Flush buffer
	m_serialPort->readAll();
}

bool Device::getChar(char &ch)
{
	bool success;

	success = m_serialPort->getChar(&ch);

	return success;
}

bool Device::writeBuffer(const char *pBuff, int len)
{
	int nWrite;

	nWrite = m_serialPort->write(pBuff, len);

	return (nWrite == len);
}

void Device::startTimer(int tout)
{
	struct timeb tb;

	ftime(&tb);
	m_tout = (tb.time % 1000) * 1000 + tb.millitm + tout;
}

bool Device::isElapsed()
{
	struct timeb tb;
	int curTime;

	ftime(&tb);
	curTime = (tb.time % 1000) * 1000 + tb.millitm;

	return (curTime >= m_tout);
}
