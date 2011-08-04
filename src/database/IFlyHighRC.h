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

#ifndef IFlyHighRC_h
#define IFlyHighRC_h

#include <QString>
#include <QStringList>
#include "DatabaseParameters.h"

class QSettings;

/**
 * Encapsulates the flyhigh configuration file
 *
 * Settings are stored in ini file format. The file location depends on OS:
 *	Linux: /home/<user>/.config/flyhigh/flyhigh.ini
 *  Windoze: c:\Documents and Settings\<user>\Application Data\flyhigh\flyhigh.ini
 *
 * Until version 0.8.2, the configuration file was /home/<user>/.flyhighrc
 * This file will be move to the new location upon the first start of flyhigh
 *
 * If no config file can be found at neither old nor new location, a default
 * will be created.
 */
class IFlyHighRC
{
	public:
		typedef enum DeviceId{DevFlytec5020 = 0, DevFlytec6015 = 1, DevFlytec6020 = 2}DeviceId;

		static IFlyHighRC* pInstance();

		~IFlyHighRC();

		void loadRC();

		void saveRC();

		const QString& deviceLine() const;

		void setDeviceLine(const QString &name);

		uint deviceSpeed() const;

		void setDeviceSpeed(uint index);

		const QString deviceSpeedString() const;

		uint deviceName() const;

		void setDeviceName(uint index);

		char utcOffset() const;

		void setUtcOffset(char offset);

		const QString& lastDir() const;

		void setLastDir(const QString& name);

		const QString& versionInfo() const;

		const QString& copyInfo() const;

		const QStringList& deviceNameList() const;

		const QStringList& deviceSpeedList() const;

		// pilot info
		void setPilotId(int id);
		int pilotId() const;

		// mysql db sever host
		void setDBHost(const QString& host);
		const QString& dBHost() const;

		// mysql db sever port
		void setDBPort(int port);
		int dBPort() const;

		// mysql db name (schema)
		void setDBName(const QString& user);
		const QString& dBName() const;

		// mysql db user (non-priviledged)
		void setDBUser(const QString& user);
		const QString& dBUser() const;

		// mysql db password
		void setDBPass(const QString& pass);
		const QString& dBPass() const;

		// db backend type
		void setDBType(const QString& dbtype);
		const QString& dBType() const;

		// sqlite db file path and name
		void setDBFile(const QString& dbfile);
		const QString& dBFile() const;

		DatabaseParameters getDBParameters() const;

		// gnuplot exe location
		void setGnuplotPath(const QString& path);
		const QString& gnuplotPath() const;

	private:
		static IFlyHighRC *m_pInstance;
		QSettings* m_pSettings;
		uint m_deviceName;
		int m_deviceSpeed;
		QString m_deviceLine;
		char m_utcOffset;
		QString m_lastDir;
		QString m_versionInfo;
		QString m_copyInfo;
		int m_pilotId;
		QString m_dbHost;
		int m_dbPort;
		QString m_dbName;
		QString m_dbUser;
		QString m_dbPass;
		QString m_dbType;
		QString m_dbFile;
		QString m_gnuplot;

		QStringList m_deviceNameList;
		QStringList m_deviceSpeedList;
		QStringList m_dbTypeList;

		IFlyHighRC();
};

#endif
