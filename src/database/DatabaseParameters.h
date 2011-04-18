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

#ifndef DATABASEPARAMETERS_H
#define DATABASEPARAMETERS_H

#include <QString>

/**
 * Encapsulates database parameters.
 */
class DatabaseParameters
{

public:

	DatabaseParameters(const QString& type,
										 const QString& name,
										 const QString& hostName,
										 int   port,
										 const QString& user,
										 const QString& password,
										 const QString& file);

	DatabaseParameters();

	bool operator==(const DatabaseParameters& other) const;
	bool operator!=(const DatabaseParameters& other) const;

	bool isSQLite() const;
	bool isMySQL() const;

	void readFromConfig();
	void writeToConfig() const;

	const QString& dBType() const;
	const QString& dBName() const;
	const QString& dBHostName() const;
	const QString& dBUserName() const;
	const QString& dBPassword() const;
	const QString& dBFile() const;
	int dBPort() const;

	static QString SQLiteDatabaseType();
	static QString MySQLDatabaseType();
	static DatabaseParameters defaultParameters();
	static DatabaseParameters parametersFromConfig();

private:
	void validateDBType();
	void validateDBFile();

private:

	QString m_dBType;
	QString m_dBName;
	QString m_hostName;
	int     m_port;
	QString m_userName;
	QString m_password;
	QString m_dBFile;

};

#endif
