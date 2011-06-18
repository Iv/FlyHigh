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

#include <QFileInfo>
#include <QDir>
#include <QHostInfo>
#include "IFlyHighRC.h"
#include "DatabaseParameters.h"

DatabaseParameters::DatabaseParameters()
{
}

DatabaseParameters::DatabaseParameters(const QString& type,
                                       const QString& databaseName,
                                       const QString& hostName,
                                       int port,
                                       const QString& userName,
																			 const QString& password,
																			 const QString& file)
	: m_dBType(type)
	, m_dBName(databaseName)
	, m_hostName(hostName)
	, m_port(port)
	, m_userName(userName)
	, m_password(password)
	, m_dBFile(file)
{
	validateDBType();
	validateDBFile();
}

bool DatabaseParameters::operator==(const DatabaseParameters& other) const
{
	return m_dBType   == other.m_dBType &&
			m_dBName   == other.m_dBName &&
			m_hostName == other.m_hostName &&
			m_port     == other.m_port &&
			m_userName == other.m_userName &&
			m_password == other.m_password &&
			m_dBFile   == other.m_dBFile;
}

bool DatabaseParameters::operator!=(const DatabaseParameters& other) const
{
	return !operator==(other);
}

bool DatabaseParameters::isSQLite() const
{
	return m_dBType == DatabaseParameters::SQLiteDatabaseType();
}

bool DatabaseParameters::isMySQL() const
{
	return m_dBType == DatabaseParameters::MySQLDatabaseType();
}

QString DatabaseParameters::SQLiteDatabaseType()
{
	return "QSQLITE";
}

QString DatabaseParameters::MySQLDatabaseType()
{
	return "QMYSQL";
}

void DatabaseParameters::apply(QSqlDatabase db) const
{
	if (isMySQL())
	{
		db.setDatabaseName(m_dBName);
		db.setUserName(m_userName);
		db.setPassword(m_password);
		db.setHostName(m_hostName);
		db.setPort(m_port);
	}
	else if (isSQLite())
	{
		db.setDatabaseName(m_dBFile);
	}
}

bool DatabaseParameters::match(const DatabaseParameters& other) const
{
	bool res = (m_dBType == other.m_dBType);
	if (res && isMySQL())
	{
		res &= (m_dBName == other.m_dBName);
		// compare port numbers as well. it's improbable that
		// different ports lead to the same db
		res &= (m_port == other.m_port);
		// hostname lookup can take some time, so just do it if necessary
		if (res)
		{
			// hostnames are not unique and so are ip's. but ip's may
			// be more reliable
			QHostInfo here = QHostInfo::fromName(m_hostName);
			QHostInfo there = QHostInfo::fromName(other.m_hostName);
			// just comparing one address
			if (!here.addresses().empty()&&
					!there.addresses().empty())
			{
				res &= (here.addresses().first() == there.addresses().first());
			}
		}
	}
	else
	{
		// check a bit better than just filenames
		QFileInfo here(m_dBFile);
		QFileInfo there(other.m_dBFile);
		res &= (here == there);
	}
	return res;
}

void DatabaseParameters::readFromConfig()
{
	m_dBType = IFlyHighRC::pInstance()->dBType();
	m_dBName = IFlyHighRC::pInstance()->dBName();
	m_hostName = IFlyHighRC::pInstance()->dBHost();
	m_port = IFlyHighRC::pInstance()->dBPort();
	m_userName = IFlyHighRC::pInstance()->dBUser();
	m_password = IFlyHighRC::pInstance()->dBPass();
	m_dBFile = IFlyHighRC::pInstance()->dBFile();

	validateDBType();
	validateDBFile();
}

void DatabaseParameters::writeToConfig() const
{
	IFlyHighRC::pInstance()->loadRC();
	IFlyHighRC::pInstance()->setDBName(m_dBName);
	IFlyHighRC::pInstance()->setDBHost(m_hostName);
	IFlyHighRC::pInstance()->setDBPort(m_port);
	IFlyHighRC::pInstance()->setDBUser(m_userName);
	IFlyHighRC::pInstance()->setDBPass(m_password);
	IFlyHighRC::pInstance()->setDBFile(m_dBFile);

	// db type in config is not the driver name, but
	// a simple human-readable string
	if (m_dBType == SQLiteDatabaseType() )
	{
		IFlyHighRC::pInstance()->setDBType("sqlite");
	}
	else
	{
		IFlyHighRC::pInstance()->setDBType("mysql");
	}
	IFlyHighRC::pInstance()->saveRC();
}

DatabaseParameters DatabaseParameters::defaultParameters()
{
	DatabaseParameters parameters;

	parameters.m_dBType = "QSQLITE";
	parameters.m_dBName = "flyhigh_v2";
	parameters.m_hostName = "localhost";
	parameters.m_userName = "flyhigh";
	parameters.m_password = "flyhigh";
	parameters.m_port = 3306;
	parameters.m_dBFile = "Flights/flyhigh_v2.sqlite";

	parameters.validateDBFile();

	return parameters;
}

void DatabaseParameters::validateDBType()
{
	if (m_dBType.contains("sqlite",Qt::CaseInsensitive))
	{
		m_dBType = DatabaseParameters::SQLiteDatabaseType();
	}
	else
	{
		m_dBType = DatabaseParameters::MySQLDatabaseType();
	}
}

void DatabaseParameters::validateDBFile()
{
	if (QFileInfo(m_dBFile).isRelative())
	{
		// prepend userhome
		m_dBFile.prepend(QDir::separator()).prepend(QDir::homePath());
	}
}

const QString& DatabaseParameters::dBType() const
{
	return m_dBType;
}

const QString& DatabaseParameters::dBName() const
{
	return m_dBName;
}

const QString& DatabaseParameters::dBHostName() const
{
	return m_hostName;
}

const QString& DatabaseParameters::dBUserName() const
{
	return m_userName;
}

const QString& DatabaseParameters::dBPassword() const
{
	return m_password;
}

const QString& DatabaseParameters::dBFile() const
{
	return m_dBFile;
}

int DatabaseParameters::dBPort() const
{
	return m_port;
}
