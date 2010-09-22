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

#include <qbuffer.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <QTextStream>

class IFlyHighRC
{
	public:
		enum {DevFlytec5020 = 0, DevFlytec6015 = 1};

		static IFlyHighRC* pInstance();
		
		void loadRC();
		void saveRC();
		
		const QString& deviceLine();
		void setDeviceLine(const QString &name);
		uint deviceSpeed();
		void setDeviceSpeed(uint index);

		const QString deviceSpeedString() const;

		uint deviceName();
		void setDeviceName(uint index);
		char utcOffset();
		void setUtcOffset(char offset);
		const QString& lastDir();
		void setLastDir(const QString& name);
		const QString& versionInfo();
		
		const QStringList& deviceNameList();
		const QStringList& deviceSpeedList();
		
		// pilot info
		void setPilotId(int id);
		int pilotId();

	private:
		QFile m_rcFile;
		static IFlyHighRC *m_pInstance;
		uint m_deviceName;
		uint m_deviceSpeed;
		QString m_deviceLine;
		char m_utcOffset;
		QString m_lastDir;
		QString m_versionInfo;
		int m_pilotId;
		
		QStringList m_deviceNameList;
		QStringList m_deviceSpeedList;

		IFlyHighRC();
		
		void parseSerialLine(QBuffer &buff);
		void saveSerialLine(QTextStream &stream);
		void parseDateTime(QBuffer &buff);
		void saveDateTime(QTextStream &stream);
		void parseDirectory(QBuffer &buff);
		void saveDirectory(QTextStream &stream);
		void parsePilot(QBuffer &buff);
		void savePilot(QTextStream &stream);
		void parseValue(char *line, QString &var, QString &val);
};

#endif
