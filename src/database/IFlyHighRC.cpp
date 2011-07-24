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

#include <QDir>
#include <QStringList>
#include <QSettings>
#include "IFlyHighRC.h"

// definition of ini entries in the form <section>/<key>
const QString DeviceNameKey    = "device/name";
const QString DeviceLineKey    = "device/line";
const QString DeviceSpeedKey   = "device/speed";
const QString DateTimeUtcKey   = "datetime/utc";
const QString PilotIdKey       = "pilot/pilotId";
const QString DirectoryLastKey = "directory/last";
const QString DatabaseHostKey  = "database/dbserverhost";
const QString DatabasePortKey  = "database/dbserverport";
const QString DatabaseNameKey  = "database/dbname";
const QString DatabaseUserKey  = "database/dbusername";
const QString DatabasePassKey  = "database/dbpassword";
const QString DatabaseTypeKey  = "database/dbtype";
const QString DatabaseFileKey  = "database/dbfile";


IFlyHighRC *IFlyHighRC::m_pInstance = NULL;

IFlyHighRC* IFlyHighRC::pInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new IFlyHighRC();
	}

	return m_pInstance;
}

IFlyHighRC::IFlyHighRC()
{
	// settings stored at
	// Linux: /home/<user>/.config/flyhigh/flyhigh.ini
	// windoze: c:\Documents and Settings\<user>\Application Data\flyhigh\flyhigh.ini
	m_pSettings = new QSettings(QSettings::IniFormat,
															QSettings::UserScope,
															"flyhigh",
															"flyhigh");

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

	m_dbTypeList += "sqlite";
	m_dbTypeList += "mysql";
	m_dbHost = "localhost";
	m_dbPort = 3306;
	m_dbName = "flyhigh_v2";
	m_dbUser = "flyhigh";
	m_dbPass = "flyhigh";
	m_dbType = m_dbTypeList[0];
	// relative to userhome or absolute:
	m_dbFile = "Flights/flyhigh_v2.sqlite";
}

IFlyHighRC::~IFlyHighRC()
{
	m_pSettings->sync();
	delete m_pSettings;
	m_pSettings = NULL;
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
	QSettings* pSettings=NULL;
	QString legacyfile;

	// check if file exists at default location
	if (!QFile::exists(m_pSettings->fileName()))
	{
		// no. check the former location ~/.flyhighrc
		legacyfile = QDir::homePath() + "/.flyhighrc";
		if (QFile::exists(legacyfile))
		{
			// yes, exists.
			// we'll try to read the old settings, but store
			// them at the new location
			// create new QSettings object, but keep old pointer
			pSettings = m_pSettings;
			m_pSettings = new QSettings(legacyfile,QSettings::IniFormat);
		}
	}

	// database settings
	m_dbType = m_pSettings->value(DatabaseTypeKey,m_dbType).toString();
	m_dbHost = m_pSettings->value(DatabaseHostKey,m_dbHost).toString();
	m_dbPort = m_pSettings->value(DatabasePortKey,m_dbPort).toInt();
	m_dbName = m_pSettings->value(DatabaseNameKey,m_dbName).toString();
	m_dbUser = m_pSettings->value(DatabaseUserKey,m_dbUser).toString();
	m_dbPass = m_pSettings->value(DatabasePassKey,m_dbPass).toString();
	m_dbFile = m_pSettings->value(DatabaseFileKey,m_dbFile).toString();

	// device settings
	m_deviceLine = m_pSettings->value(DeviceLineKey,m_deviceLine).toString();
	QString currDevName = m_pSettings->value(DeviceNameKey,m_deviceNameList.at(0)).toString();
	m_deviceName = m_deviceNameList.indexOf(currDevName);
	QString currDevSpeed = m_pSettings->value(DeviceSpeedKey,m_deviceSpeedList.at(0)).toString();
	m_deviceSpeed = m_deviceSpeedList.indexOf(currDevSpeed);

	// directory settings
	m_lastDir = m_pSettings->value(DirectoryLastKey,QDir::homePath()).toString();

	// date/time settings
	m_utcOffset = m_pSettings->value(DateTimeUtcKey,m_utcOffset).toChar().toAscii();

	// pilot settings
	m_pilotId = m_pSettings->value(PilotIdKey,m_pilotId).toInt();

	// a bit of validation
	if (!m_dbTypeList.contains(m_dbType))
	{
		// invalid db type. revert to default
		m_dbType = m_dbTypeList.at(0);
	}

	// check if we've been reading from legacy rc file
	if (pSettings)
	{
		// the QSettings object pointing to legacy file is not needed anymore
		delete m_pSettings;
		// revert to original QSettings object
		m_pSettings = pSettings;
		pSettings = NULL;
		// store settings at new location
		saveRC();
		// check for errors
		if (m_pSettings->status()==QSettings::NoError)
		{
			// success. now remove legacy file
			QFile::remove(legacyfile);
			// does not matter much if removing fails. the file would just
			// remain in place, but is ignored.
		}
	}
}

void IFlyHighRC::saveRC()
{
	if (m_pSettings && m_pSettings->isWritable())
	{
		// database settings
		m_pSettings->setValue(DatabaseTypeKey,m_dbType);
		m_pSettings->setValue(DatabaseHostKey,m_dbHost);
		m_pSettings->setValue(DatabasePortKey,m_dbPort);
		m_pSettings->setValue(DatabaseNameKey,m_dbName);
		m_pSettings->setValue(DatabaseUserKey,m_dbUser);
		m_pSettings->setValue(DatabasePassKey,m_dbPass);
		m_pSettings->setValue(DatabaseFileKey,m_dbFile);

		// device settings
		m_pSettings->setValue(DeviceLineKey,m_deviceLine);
		m_pSettings->setValue(DeviceNameKey,m_deviceNameList.at(m_deviceName));
		m_pSettings->setValue(DeviceSpeedKey,m_deviceSpeedList.at(m_deviceSpeed));

		// directory settings
		m_pSettings->setValue(DirectoryLastKey,m_lastDir);

		// date/time settings
		m_pSettings->setValue(DateTimeUtcKey,m_utcOffset);

		// pilot settings
		m_pSettings->setValue(PilotIdKey,m_pilotId);

		// flush
		m_pSettings->sync();
	}
}
