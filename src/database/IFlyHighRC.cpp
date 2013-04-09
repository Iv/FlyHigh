/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#include <QDir>
#include <QStringList>
#include <QSettings>
#include <QRegExp>
#include "qextserialenumerator.h"
#include "IFlyHighRC.h"

#include <QDebug>

// definition of ini entries in the form <section>/<key>
const QString DeviceNameKey    = "device/name";
const QString DeviceLineKey    = "device/line";
const QString DeviceSpeedKey   = "device/speed";
const QString DateTimeUtcKey   = "datetime/utc";
const QString PilotIdKey       = "pilot/pilotId";
const QString DirLastKey       = "directory/last";
const QString DirFlyHighKey    = "directory/flyhigh";
const QString DirElevationKey  = "directory/elevation";
const QString DirSwissTopoKey  = "directory/swisstopo";
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

  // must match the order in IFlyHighRC::DeviceId:
	m_deviceNameList += "5020 / Competino";
	m_deviceNameList += "6015 / IQ Basic";
	m_deviceNameList += "6020 / Competino+";
//	m_deviceNameList += "Garmin";

/*	m_deviceSpeedList = "9600";
	m_deviceSpeedList += "19200";
	m_deviceSpeedList += "38400";*/
	m_deviceSpeedList += "57600";

	m_versionInfo = "FlyHigh Version 0.9.6";
	m_copyInfo =
			"Copyright (c): 2004-2013 by Alex Graf <grafal@sf.net>, \n"
			"Jurg Allemann <ja_kern@sf.net> and Markus Franke.\n"
			"Thank you Thomas Kuhlmann for OLC optimizer and Luke Mahe\n"
			"for MarkerClusterer.\n"
			"FlyHigh is distributed under the terms of the General Public\n"
			"License (GPL Version 2). Visit www.gnu.org for more information.\n";

	m_dbTypeList += "sqlite";
	m_dbTypeList += "mysql";
//	m_dbHost = "localhost";
//	m_dbPort = 3306;
//	m_dbName = "flyhigh_v2";
//	m_dbUser = "flyhigh";
//	m_dbPass = "flyhigh";
//	m_dbType = m_dbTypeList[0];
	// relative to userhome or absolute:
///	m_dbFile = "Flights/flyhigh_v2.sqlite";
}

IFlyHighRC::~IFlyHighRC()
{
	delete m_pSettings;
	m_pSettings = NULL;
}

uint IFlyHighRC::deviceNameIndex() const
{
  QString name;
  uint index;

  name = deviceName();
  index = m_deviceNameList.indexOf(name);

  if(index < 0)
  {
    index = 0;
  }

	return index;
}

void IFlyHighRC::setDeviceNameIndex(uint index)
{
  if(index < (uint)m_deviceNameList.size())
  {
    m_pSettings->setValue(DeviceNameKey, m_deviceNameList[index]);
  }
}

void IFlyHighRC::setDeviceName(const QString &name)
{
  int index;

  index = m_deviceNameList.indexOf(name);

  if(index >= 0)
  {
    m_pSettings->setValue(DeviceNameKey, name);
  }
}

QString IFlyHighRC::deviceName() const
{
  return m_pSettings->value(DeviceNameKey, m_deviceNameList[0]).toString();
}

QString IFlyHighRC::deviceLine() const
{
  return m_pSettings->value(DeviceLineKey, "/dev/ttyS0").toString();
}

void IFlyHighRC::setDeviceLine(const QString &line)
{
  m_pSettings->setValue(DeviceLineKey, line);
}

uint IFlyHighRC::deviceSpeedIndex() const
{
  QString speed;
  int index;

  speed = deviceSpeed();
  index = m_deviceSpeedList.indexOf(speed);

  if(index < 0)
  {
    index = 0;
  }

  return index;
}

void IFlyHighRC::setDeviceSpeedIndex(uint index)
{
  if(index < (uint)m_deviceSpeedList.size())
  {
    setDeviceSpeed(m_deviceSpeedList[index]);
  }
}

void IFlyHighRC::setDeviceSpeed(const QString &speed)
{
  int index;

  index = m_deviceSpeedList.indexOf(speed);

  if(index < 0)
  {
    index = 0;
  }

  m_pSettings->setValue(DeviceSpeedKey, m_deviceSpeedList[index]);
}

QString IFlyHighRC::deviceSpeed() const
{
  return m_pSettings->value(DateTimeUtcKey, m_deviceSpeedList[0]).toString();
}

float IFlyHighRC::utcOffset() const
{
	return m_pSettings->value(DateTimeUtcKey, 0).toFloat();
}

void IFlyHighRC::setUtcOffset(float offset)
{
	if(offset > 12)
	{
		offset = 12;
	}
	else if(offset < -12)
	{
		offset = -12;
	}

	m_pSettings->setValue(DateTimeUtcKey, offset);
}

QString IFlyHighRC::lastDir() const
{
	return m_pSettings->value(DirLastKey, QDir::homePath()).toString();
}

void IFlyHighRC::setLastDir(const QString &path)
{
	m_pSettings->setValue(DirLastKey, path);
}

QString IFlyHighRC::flyHighDir() const
{
  return m_pSettings->value(DirFlyHighKey, QDir::homePath() + "/flyhigh").toString();
}

void IFlyHighRC::setFlyHighDir(const QString &path)
{
  m_pSettings->setValue(DirFlyHighKey, path);
}

QString IFlyHighRC::elevationDir() const
{
  return m_pSettings->value(DirElevationKey, QDir::homePath() + "/flyhigh/elevation").toString();
}

void IFlyHighRC::setElevationDir(const QString &path)
{
  m_pSettings->setValue(DirElevationKey, path);
}

QString IFlyHighRC::swissTopoDir() const
{
  return m_pSettings->value(DirSwissTopoKey, QDir::homePath() + "/flyhigh/swisstopo").toString();
}

void IFlyHighRC::setSwissTopoDir(const QString &path)
{
  m_pSettings->setValue(DirSwissTopoKey, path);
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

const QStringList& IFlyHighRC::deviceLineList() const
{
  // rebuild list each call. Maybe the user connects his device
  // at runtime, so he'll get up-to-date list each time he opens
  // the settings menu.
  // connect/disconnect notification does not yet work in qextserialport
  m_deviceLineList.clear();

  foreach(QextPortInfo port, QextSerialEnumerator::getPorts())
  {
    // filter for /dev/ttyS* or /dev/ttyUSB* on unix-like platforms
    // else we get all the tty's and console devices, too
#ifdef Q_OS_UNIX
    QRegExp rx = QRegExp(".*tty(S|USB).*");

    if(rx.indexIn(port.portName) != -1)
#endif
    {
      m_deviceLineList += port.portName;
    }
  }

  return m_deviceLineList;
}

void IFlyHighRC::setPilotId(int id)
{
  m_pSettings->setValue(PilotIdKey, id);
}

int IFlyHighRC::pilotId() const
{
	return m_pSettings->value(PilotIdKey, -1).toInt();
}

void IFlyHighRC::setDBHost(const QString &host)
{
  m_pSettings->setValue(DatabaseHostKey, host);
}

QString IFlyHighRC::dBHost() const
{
  return m_pSettings->value(DatabaseHostKey, "localhost").toString();
}

void IFlyHighRC::setDBName(const QString &name)
{
	m_pSettings->setValue(DatabaseNameKey, name);
}

QString IFlyHighRC::dBName() const
{
	return m_pSettings->value(DatabaseNameKey, "flyhigh_v2").toString();
}

void IFlyHighRC::setDBPort(int port)
{
	m_pSettings->setValue(DatabasePortKey, port);
}

int IFlyHighRC::dBPort() const
{
	return m_pSettings->value(DatabasePortKey, 3306).toInt();
}

void IFlyHighRC::setDBUser(const QString &user)
{
	m_pSettings->setValue(DatabaseUserKey, user);
}

QString IFlyHighRC::dBUser() const
{
	return m_pSettings->value(DatabaseUserKey, "flyhigh").toString();
}

void IFlyHighRC::setDBPass(const QString &pass)
{
  m_pSettings->setValue(DatabasePassKey, pass);
}

QString IFlyHighRC::dBPass() const
{
	return m_pSettings->value(DatabasePassKey, "flyhigh").toString();
}

void IFlyHighRC::setDBType(const QString &dbtype)
{
  m_pSettings->setValue(DatabaseTypeKey, dbtype);
}

QString IFlyHighRC::dBType() const
{
  return m_pSettings->value(DatabaseTypeKey, m_dbTypeList[0]).toString();
}

void IFlyHighRC::setDBFile(const QString &dbfile)
{
  m_pSettings->setValue(DatabaseFileKey, dbfile);
}

QString IFlyHighRC::dBFile() const
{
  return m_pSettings->value(DatabaseFileKey, "Flights/flyhigh_v2.sqlite").toString();
}

DatabaseParameters IFlyHighRC::getDBParameters() const
{
	return DatabaseParameters(dBType(), // m_dbType,
														dBName(), // m_dbName,
														dBHost(), // m_dbHost,
														dBPort(), // m_dbPort,
														dBUser(), // m_dbUser,
														dBPass(), // m_dbPass,
														dBFile()); //m_dbFile);
}

void IFlyHighRC::loadRC()
{
	QSettings *pSettings = NULL;
	QString legacyfile;

	// check if file exists at default location
	if(!QFile::exists(m_pSettings->fileName()))
	{
		// check the former location ~/.flyhighrc
		legacyfile = QDir::homePath() + "/.flyhighrc";

		if(QFile::exists(legacyfile))
		{
			// yes, exists.
			// we'll try to read the old settings, but store
			// them at the new location
			// create new QSettings object, but keep old pointer
			pSettings = m_pSettings;
			m_pSettings = new QSettings(legacyfile, QSettings::IniFormat);
		}
	}

	// check if we've been reading from legacy rc file
	if(pSettings)
	{
		// the QSettings object pointing to legacy file is not needed anymore
		delete m_pSettings;

		// revert to original QSettings object
		m_pSettings = pSettings;
		pSettings = NULL;

		// store settings at new location
		m_pSettings->sync();

		// check for errors
		if(m_pSettings->status() == QSettings::NoError)
		{
			// success. now remove legacy file
			QFile::remove(legacyfile);
			// does not matter much if removing fails. the file would just
			// remain in place, but is ignored.
		}
	}

/**
	QSettings* pSettings=NULL;
	QString legacyfile;

	// check if file exists at default location
	if(!QFile::exists(m_pSettings->fileName()))
	{
		// no. check the former location ~/.flyhighrc
		legacyfile = QDir::homePath() + "/.flyhighrc";

		if(QFile::exists(legacyfile))
		{
			// yes, exists.
			// we'll try to read the old settings, but store
			// them at the new location
			// create new QSettings object, but keep old pointer
			pSettings = m_pSettings;
			m_pSettings = new QSettings(legacyfile, QSettings::IniFormat);
		}
	}

	// database settings
	m_dbType = m_pSettings->value(DatabaseTypeKey, m_dbType).toString();
	m_dbHost = m_pSettings->value(DatabaseHostKey, m_dbHost).toString();
	m_dbPort = m_pSettings->value(DatabasePortKey, m_dbPort).toInt();
	m_dbName = m_pSettings->value(DatabaseNameKey, m_dbName).toString();
	m_dbUser = m_pSettings->value(DatabaseUserKey, m_dbUser).toString();
	m_dbPass = m_pSettings->value(DatabasePassKey, m_dbPass).toString();
	m_dbFile = m_pSettings->value(DatabaseFileKey, m_dbFile).toString();

	// device settings
	m_deviceLine = m_pSettings->value(DeviceLineKey, m_deviceLine).toString();
	QString currDevName = m_pSettings->value(DeviceNameKey, m_deviceNameList.at(0)).toString();
	m_deviceName = m_deviceNameList.indexOf(currDevName);
	QString currDevSpeed = m_pSettings->value(DeviceSpeedKey, m_deviceSpeedList.at(0)).toString();
	m_deviceSpeed = m_deviceSpeedList.indexOf(currDevSpeed);

	// directory settings
	m_dirLast = m_pSettings->value(DirLastKey, QDir::homePath()).toString();
  m_dirFlyHigh = m_pSettings->value(DirFlyHighKey, QDir::homePath() + "/flyhigh").toString();
	m_dirElevation = m_pSettings->value(DirElevationKey, QDir::homePath() + "/flyhigh/elevation").toString();
	m_dirSwissTopo = m_pSettings->value(DirSwissTopoKey, QDir::homePath() + "/flyhigh/swisstopo").toString();

	// date/time settings
	m_utcOffset = m_pSettings->value(DateTimeUtcKey, m_utcOffset).toChar().toAscii();

	// pilot settings
	m_pilotId = m_pSettings->value(PilotIdKey, m_pilotId).toInt();

	// a bit of validation
	if(!m_dbTypeList.contains(m_dbType))
	{
		// invalid db type. revert to default
		m_dbType = m_dbTypeList.at(0);
	}

	// check if we've been reading from legacy rc file
	if(pSettings)
	{
		// the QSettings object pointing to legacy file is not needed anymore
		delete m_pSettings;

		// revert to original QSettings object
		m_pSettings = pSettings;
		pSettings = NULL;

		// store settings at new location
		saveRC();

		// check for errors
		if(m_pSettings->status() == QSettings::NoError)
		{
			// success. now remove legacy file
			QFile::remove(legacyfile);
			// does not matter much if removing fails. the file would just
			// remain in place, but is ignored.
		}
	}
*/
}
