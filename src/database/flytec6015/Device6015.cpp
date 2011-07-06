/**
	@file Device6015.cpp
	@author Alex Graf
	@date 30-04-2007
	Copyright (C) 2007 by JFS electronic AG, Rotkreuz, Switzerland
*/

#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

#include "qextserialport.h"
#include "Device6015.h"

Device6015::Device6015()
{
	m_tlg = "";
	m_ttyFd = -1;
	m_tout = 0;
	
	m_serialPort = new QextSerialPort(QextSerialPort::Polling);
	m_serialPort->setFlowControl(FLOW_OFF);
	m_serialPort->setParity(PAR_NONE);
	m_serialPort->setDataBits(DATA_8);
	m_serialPort->setStopBits(STOP_1);
	m_serialPort->setTimeout(0);
}

Device6015::~Device6015()
{
	delete m_serialPort;
}

bool Device6015::openDevice(const QString &dev, int baud)
{
	bool success;

	m_serialPort->setPortName(dev);

	if(baud == 57600)
	{
		m_serialPort->setBaudRate(BAUD57600);
	}

	success = m_serialPort->open(QIODevice::ReadWrite);

/**
struct termios sTermSet;
	int ret;
	uint bBaud;

	m_ttyFd = open(dev.toAscii().constData(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	success = (m_ttyFd > 0);

	if(isatty(m_ttyFd))
	{
		tcgetattr(m_ttyFd, &sTermSet);

		switch(baud)
		{
			case 4800:
				bBaud = B4800;
			break;
			case 9600:
				bBaud = B9600;
			break;
			case 38400:
				bBaud = B38400;
			break;
			case 57600:
				bBaud = B57600;
			break;
			case 115200:
				bBaud = B115200;
			break;
			default:
				bBaud = B9600;
			break;
		}

		sTermSet.c_cflag = bBaud | CS8 | CLOCAL | CREAD;
		sTermSet.c_iflag = IGNPAR;
		sTermSet.c_oflag = 0;

		// input mode non-canonical, no echo
		sTermSet.c_lflag = 0;
		sTermSet.c_cc[VTIME] = 0;   // inter-character timer unused
		sTermSet.c_cc[VMIN] = 0;   // not blocking if 1 blocking read until 1 chars received

		ret = tcsetattr(m_ttyFd, TCSANOW, &sTermSet);
		success = (ret == 0);

		flush();
	}
*/

	return success;
}

bool Device6015::isOpen()
{
	return m_serialPort->isOpen();

/// return isatty(m_ttyFd);
}

void Device6015::closeDevice()
{
	m_serialPort->close();

/**
	if(m_ttyFd != -1)
	{
		close(m_ttyFd);
	}
*/
}

bool Device6015::recieveTlg(int tout)
{
	int charNr = 0;
	bool validTlg = false;
	char ch;

	m_tlg = "";
	startTimer(tout);

//	m_serialPort->setTimeout(tout);

	do
	{
		if(getChar(ch))
		{
			charNr++;
			m_tlg += ch;
			validTlg = (ch == '\n');

			if(validTlg || (charNr >= MaxTlgSize))
			{
				break;
			}

			if(m_tlg.length() >= MaxTlgSize)
			{
				m_tlg = "";
			}
		}
	}while(!isElapsed());

	return validTlg;
}

const QString& Device6015::getTlg()
{
	return m_tlg;
}

bool Device6015::sendTlg(const QString &tlg)
{
	return writeBuffer(tlg.toAscii().constData(), tlg.length());
}

void Device6015::flush()
{
	char ch;

	m_tlg = "";
	while(getChar(ch)){}; // Flush buffer
}

bool Device6015::getChar(char &ch)
{
	bool success;

	success = m_serialPort->getChar(&ch);

	return success;

/*
	int nRead;

	nRead = read(m_ttyFd, &ch, 1);

	return (nRead == 1);
*/
}

bool Device6015::writeBuffer(const char *pBuff, int len)
{
	int nWrite;
	
	nWrite = m_serialPort->write(pBuff, len);

	return (nWrite == len);

/**
	int nWrite;
	bool success;

	nWrite = write(m_ttyFd, pBuff, len);
	success = (nWrite > 0);

	if(success)
	{
		success = (nWrite == len);
	}

	return success;
*/
}

void Device6015::startTimer(int tout)
{
	struct timeb tb;

	ftime(&tb);
	m_tout = (tb.time % 1000) * 1000 + tb.millitm + tout;
}

bool Device6015::isElapsed()
{
	struct timeb tb;
	int curTime;

	ftime(&tb);
	curTime = (tb.time % 1000) * 1000 + tb.millitm;

	return (curTime >= m_tout);
}
