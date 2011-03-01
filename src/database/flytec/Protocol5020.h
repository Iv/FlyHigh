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
#ifndef Protocol5020_h
#define Protocol5020_h

#include "Device5020.h"

class QString;

typedef struct DeviceInfo
{
	QString deviceIdent;
	QString pilotName;
	uint serialNr;
	QString swVersion;
}DeviceInfoType;

class Protocol5020
{
	public:
		Protocol5020();

		~Protocol5020();

		bool open(const QString &dev, int baud);

		void close();

		bool devInfoReq();

		bool devInfoRec(DeviceInfo &devInfo);

	private:
		Device5020 m_device;

		void addTail(QString &tlg) const;

		QString getCheckSum(const QString &tlg, uint end) const;
};

#endif
