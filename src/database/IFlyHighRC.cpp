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

#include <qdir.h>
#include <qstringlist.h>
#include <stdlib.h>
#include "IFlyHighRC.h"

#define MAX_LINE_SIZE 255
#define MAX_VAR_SIZE 20

const QString DeviceTag = "[device]\n";
const QString DeviceNameVar = "name=";
const QString DeviceLineVar = "line=";
const QString DeviceSpeedVar = "speed=";
const QString DateTimeTag = "[datetime]\n";
const QString DateTimeUtcVar = "utc=";
const QString DirectoryTag = "[directory]\n";
const QString PilotTag = "[pilot]\n";
const QString PilotId = "pilotId=";
const QString DirectoryLastVar = "last=";
const QString DatabaseTag = "[database]\n";
const QString DatabaseHostVar = "dbserverhost=";
const QString DatabasePortVar = "dbserverport=";
const QString DatabaseNameVar = "dbname=";
const QString DatabaseUserVar = "dbusername=";
const QString DatabasePassVar = "dbpassword=";
const QString DatabaseTypeVar = "dbtype=";
const QString DatabaseFileVar = "dbfile=";

IFlyHighRC *IFlyHighRC::m_pInstance = NULL;

IFlyHighRC* IFlyHighRC::pInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new IFlyHighRC;
	}

	return m_pInstance;
}

IFlyHighRC::IFlyHighRC()
{
	m_deviceNameList += "5020 / Competino";
	m_deviceNameList += "6015 / IQ Basic";
	m_deviceNameList += "6020 / Competino+";
//	m_deviceNameList += "Garmin";

/*	m_deviceSpeedList = "9600";
	m_deviceSpeedList += "19200";
	m_deviceSpeedList += "38400";*/
	m_deviceSpeedList += "57600";

	m_deviceName = 0;
	m_deviceLine = "/dev/ttyS0";
	m_deviceSpeed = 0;
	m_utcOffset = 0;
	m_lastDir = QDir::homePath();

	m_pilotId = -1;
	m_versionInfo = "FlyHigh Version 0.8.2";
	m_copyInfo =
			"Copyright (c): 2004-2011 by Alex Graf <grafal@sf.net> and\n"
			"Jurg Allemann <ja_kern@sf.net>.\n"
			"Thanks to: Manolis Andreadakis, Victor Berchet, Petr Chromec,\n"
			"Alex Graf, Marcus Kroiss, Jonty Lawson, Tom Payne and\n"
			"Marc Poulhies for some lines of code for Web Map.\n"
			"FlyHigh is distributed under the terms of the General Public\n"
			"License (GPL). Visit www.gnu.org for more information.\n";

	m_rcFile.setFileName(QDir::homePath() + "/.flyhighrc");

	m_dbTypeList += "mysql";
	m_dbTypeList += "sqlite";
	m_dbHost = "localhost";
	m_dbPort = 3306;
	m_dbName = "flyhigh_v2";
	m_dbUser = "flyhigh";
	m_dbPass = "flyhigh";
	m_dbType = m_dbTypeList[0];
	// relative to userhome or absolute:
	m_dbFile = "Flights/flyhigh_v2.sqlite";
}

uint IFlyHighRC::deviceName() const
{
	return m_deviceName;
}

void IFlyHighRC::setDeviceName(uint index)
{
	m_deviceName = index;
}

const QString& IFlyHighRC::deviceLine() const
{
	return m_deviceLine;
}

void IFlyHighRC::setDeviceLine(const QString &line)
{
	m_deviceLine = line;
}

uint IFlyHighRC::deviceSpeed() const
{
	return m_deviceSpeed;
}

void IFlyHighRC::setDeviceSpeed(uint index)
{
	m_deviceSpeed = index;
}

const QString IFlyHighRC::deviceSpeedString() const
{
	QString speed = "";

	if(m_deviceSpeed < m_deviceSpeedList.size())
	{
		speed = m_deviceSpeedList[m_deviceSpeed];
	}

	return speed;
}

char IFlyHighRC::utcOffset() const
{
	return m_utcOffset;
}

void IFlyHighRC::setUtcOffset(char offset)
{
	if(offset > 12)
	{
		offset = 12;
	}
	else if(offset < -12)
	{
		offset = -12;
	}

	m_utcOffset = offset;
}

const QString& IFlyHighRC::lastDir() const
{
	return m_lastDir;
}

void IFlyHighRC::setLastDir(const QString& name)
{
	m_lastDir = name;
}

const QString& IFlyHighRC::versionInfo() const
{
	return m_versionInfo;
}

const QString& IFlyHighRC::copyInfo() const
{
	return m_copyInfo;
}

const QStringList& IFlyHighRC::deviceNameList() const
{
	return m_deviceNameList;
}

const QStringList& IFlyHighRC::deviceSpeedList() const
{
	return m_deviceSpeedList;
}

void IFlyHighRC::setPilotId(int id)
{
	m_pilotId = id;
}

int IFlyHighRC::pilotId() const
{
	return m_pilotId;
}

void IFlyHighRC::setDBHost(const QString& host)
{
	m_dbHost = host;
}

const QString& IFlyHighRC::dBHost() const
{
	return m_dbHost;
}

void IFlyHighRC::setDBName(const QString& name)
{
	m_dbName = name;
}

const QString& IFlyHighRC::dBName() const
{
	return m_dbName;
}

void IFlyHighRC::setDBPort(int port)
{
	m_dbPort = port;
}

int IFlyHighRC::dBPort() const
{
	return m_dbPort;
}

void IFlyHighRC::setDBUser(const QString& user)
{
	m_dbUser = user;
}

const QString& IFlyHighRC::dBUser() const
{
	return m_dbUser;
}

void IFlyHighRC::setDBPass(const QString& pass)
{
	m_dbPass = pass;
}

const QString& IFlyHighRC::dBPass() const
{
	return m_dbPass;
}

void IFlyHighRC::setDBType(const QString& dbtype)
{
	m_dbType = dbtype;
}

const QString& IFlyHighRC::dBType() const
{
	return m_dbType;
}

void IFlyHighRC::setDBFile(const QString& dbfile)
{
	m_dbFile = dbfile;
}

const QString& IFlyHighRC::dBFile() const
{
	return m_dbFile;
}

DatabaseParameters IFlyHighRC::getDBParameters() const
{
	return DatabaseParameters(m_dbType,
														m_dbName,
														m_dbHost,
														m_dbPort,
														m_dbUser,
														m_dbPass,
														m_dbFile);
}

void IFlyHighRC::loadRC()
{
	QByteArray rcFileData;
	QBuffer buff;
	char line[MAX_LINE_SIZE];

	if(m_rcFile.open(QIODevice::ReadOnly))
	{
		rcFileData = m_rcFile.readAll();
		m_rcFile.close();
		buff.setBuffer(&rcFileData);
		buff.open(QIODevice::ReadOnly);

		while(buff.readLine(line, MAX_LINE_SIZE) > 0)
		{
			if(line == DeviceTag)
			{
				parseSerialLine(buff);
			}
			else if(line ==  DateTimeTag)
			{
				parseDateTime(buff);
			}
			else if(line == DirectoryTag)
			{
				parseDirectory(buff);
			}
			else if(line == PilotTag)
			{
				parsePilot(buff);
			}
			else if(line == DatabaseTag)
			{
				parseDBParam(buff);
			}
		}
		buff.close();
	}
}

void IFlyHighRC::saveRC()
{
	if(m_rcFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QTextStream stream(&m_rcFile);

		saveSerialLine(stream);
		saveDateTime(stream);
		saveDirectory(stream);
		savePilot(stream);
		saveDBParam(stream);

		stream.flush();

		m_rcFile.close();
	}
}

void IFlyHighRC::parseSerialLine(QBuffer &buff)
{
	char line[MAX_LINE_SIZE];
	QString var;
	QString val;

	while(buff.readLine(line, MAX_LINE_SIZE) > 0)
	{
		parseValue(line, var, val);

		if(DeviceLineVar.indexOf(var) == 0)
		{
			setDeviceLine(val);
		}
		else if(DeviceSpeedVar.indexOf(var) == 0)
		{
			setDeviceSpeed(m_deviceSpeedList.indexOf(val));
		}
		else if(DeviceNameVar.indexOf(var) == 0)
		{
			setDeviceName(m_deviceNameList.indexOf(val));
		}
		else
		{
			break;
		}
	}
}

void IFlyHighRC::saveSerialLine(QTextStream &stream) const
{
	QString str;

	stream << DeviceTag;
	str = m_deviceNameList[m_deviceName];
	stream << DeviceNameVar << str << "\n";
	stream << DeviceLineVar << m_deviceLine << "\n";
	str = m_deviceSpeedList[m_deviceSpeed];
	stream << DeviceSpeedVar << str << "\n";
	stream << "\n";
}

void IFlyHighRC::parseDateTime(QBuffer &buff)
{
	char line[MAX_LINE_SIZE];
	QString var;
	QString val;

	while(buff.readLine(line, MAX_LINE_SIZE) > 0)
	{
		parseValue(line, var, val);

		if(DateTimeUtcVar.indexOf(var) == 0)
		{
			setUtcOffset(val.toInt());
		}
		else
		{
			break;
		}
	}
}

void IFlyHighRC::saveDateTime(QTextStream &stream) const
{
	QString str;

	stream << DateTimeTag;
	str.sprintf("%i", m_utcOffset);
	stream << DateTimeUtcVar << str << "\n";
	stream << "\n";
}

void IFlyHighRC::parseDirectory(QBuffer &buff)
{
	char line[MAX_LINE_SIZE];
	QString var;
	QString val;

	while(buff.readLine(line, MAX_LINE_SIZE) > 0)
	{
		parseValue(line, var, val);

                if(DirectoryLastVar.indexOf(var) == 0)
		{
			m_lastDir = val;
		}
		else
		{
			break;
		}
	}
}

void IFlyHighRC::saveDirectory(QTextStream &stream) const
{
	stream << DirectoryTag;
	stream << DirectoryLastVar << m_lastDir << "\n";
	stream << "\n";
}

void IFlyHighRC::parsePilot(QBuffer &buff)
{
	char line[MAX_LINE_SIZE];
	QString var;
	QString val;

	while(buff.readLine(line, MAX_LINE_SIZE) > 0)
	{
		parseValue(line, var, val);

		if(PilotId.indexOf(var) == 0)
		{
			setPilotId(val.toInt());
		}
		else
		{
			break;
		}
	}
}

void IFlyHighRC::savePilot(QTextStream &stream) const
{
	stream << PilotTag;
	stream << PilotId << m_pilotId << "\n";
	stream << "\n";
}

void IFlyHighRC::parseDBParam(QBuffer &buff)
{
	char line[MAX_LINE_SIZE];
	QString var;
	QString val;

	while(buff.readLine(line, MAX_LINE_SIZE) > 0)
	{
		parseValue(line, var, val);

		if(DatabaseHostVar.indexOf(var) == 0)
		{
			m_dbHost = val;
		}
		else if (DatabasePortVar.indexOf(var) == 0)
		{
			m_dbPort = val.toInt();
		}
		else if (DatabaseNameVar.indexOf(var) == 0)
		{
			m_dbName = val;
		}
		else if (DatabaseUserVar.indexOf(var) == 0)
		{
			m_dbUser = val;
		}
		else if (DatabasePassVar.indexOf(var) == 0)
		{
			m_dbPass = val;
		}
		else if (DatabaseTypeVar.indexOf(var) == 0)
		{
			// some validation
			if (m_dbTypeList.contains(val))
			{
				m_dbType = val;
			}
			else
			{
				// unknown db type - defaulting to mysql
				m_dbType = m_dbTypeList[0];
			}
		}
		else if (DatabaseFileVar.indexOf(var) == 0)
		{
			m_dbFile = val;
		}
	}
}

void IFlyHighRC::saveDBParam(QTextStream &stream) const
{
	stream << DatabaseTag;
	stream << DatabaseHostVar << m_dbHost << "\n";
	stream << DatabasePortVar << m_dbPort << "\n";
	stream << DatabaseNameVar << m_dbName << "\n";
	stream << DatabaseUserVar << m_dbUser << "\n";
	stream << DatabasePassVar << m_dbPass << "\n";
	stream << DatabaseTypeVar << m_dbType << "\n";
	stream << DatabaseFileVar << m_dbFile << "\n";
	stream << "\n";
}

void IFlyHighRC::parseValue(char *line, QString &var, QString &val) const
{
	QString str = line;
	int valEnd;
	int varEnd;

	varEnd = str.indexOf('=');
	valEnd = str.indexOf('\n');
	var = str.left(varEnd);
	val = str.mid(varEnd + 1, valEnd -  varEnd - 1);
}
