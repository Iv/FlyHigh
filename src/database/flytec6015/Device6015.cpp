/**
	@file Device6015.cpp
	@author Alex Graf
	@date 30-04-2007
	Copyright (C) 2007 by JFS electronic AG, Rotkreuz, Switzerland
*/

#include <sys/ioctl.h>
#include <linux/serial.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "Device6015.h"

Device6015::Device6015()
{
	m_tlg = "";
	m_ttyFd = -1;
	m_tout = 0;
}

bool Device6015::openDevice(const QString &dev, int baud)
{
	struct termios sTermSet;
	int ret;
	uint bBaud;
	bool success;

	m_ttyFd = open(dev.ascii(), O_RDWR | O_NOCTTY | O_NONBLOCK);
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
		sTermSet.c_cc[VTIME] = 0;   /* inter-character timer unused */
		sTermSet.c_cc[VMIN] = 0;   /* 0 not blocking if 1 blocking read until 1 chars received */

		ret = tcsetattr(m_ttyFd, TCSANOW, &sTermSet);
		success = (ret == 0);
	}

	return success;
}

bool Device6015::isOpen()
{
	return isatty(m_ttyFd);
}

void Device6015::closeDevice()
{
	if(m_ttyFd != -1)
	{
		close(m_ttyFd);
	}
}

bool Device6015::recieveTlg(int tout)
{
	int charNr = 0;
	char ch;
	bool validTlg = false;

	m_tlg = "";
	startTimer(tout);

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
	return writeBuffer(tlg.ascii(), tlg.length());
}

bool Device6015::getChar(char &ch)
{
	int nRead;

	nRead = read(m_ttyFd, &ch, 1);

	return (nRead == 1);
}

bool Device6015::writeBuffer(const char *pBuff, int len)
{
	int nWrite;
	bool success;

	nWrite = write(m_ttyFd, pBuff, len);
	success = (nWrite > 0);

	if(success)
	{
		success = (nWrite == len);
	}

	return success;
}

bool Device6015::startTimer(int tout)
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
