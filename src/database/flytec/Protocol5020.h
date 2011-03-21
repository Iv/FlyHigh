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

#include "Defs5020.h"
#include "Device5020.h"

class QString;
class AirSpace;
class Flight;
class Route;
class WayPoint;

class Protocol5020
{
	public:
		Protocol5020();

		~Protocol5020();

		bool open(const QString &dev, int baud);

		void close();

		bool devInfoReq();

		bool devInfoRec(DeviceInfo &devInfo);

/**
int memoryRead(uint addr, uchar *pPage);

int memoryWrite(uint addr, uchar *pPage);

int ft_updateConfiguration();
*/

		/** Track */
		bool trackListReq();

		bool trackListRec(uint &total, Flight &flight);

		bool trackReq(uint trackNr);

		bool trackRec(QString &line);

		/** Waypoint */
		bool wpListReq();

		bool wpListRec(WayPoint &wp);

		bool wpSnd(const WayPoint &wp);

		bool wpDel(const QString &name);

		bool wpDelAll();

		/** Route */
		bool routeListReq();

		bool routeListRec(uint &curSent, uint &totalSent, Route &route);

		bool routeSnd(uint curSent, uint totalSent, Route &route);

		bool routeDel(const QString &name);

		/** CTR */
		bool ctrInfoReq();

		bool ctrInfoRec(uint &nofCtr, uint &maxCtr, uint &nofFree);

		bool ctrListReq();

		bool ctrListRec(uint &curSent, uint &totalSent, AirSpace *pAirSpace);

		bool ctrSnd(uint curSent, uint totalSent, AirSpace &airspace);

		bool ctrDel(const QString &name);

		bool recAck();

		/** Memory */
		bool memoryRead(uint addr, uchar *pPage);

    bool memoryWrite(uint addr, uchar *pPage);

    bool updateConfiguration();

	private:
		Device5020 m_device;

		QDate parseDate(const QString &token) const;

		QTime parseTime(const QString &token) const;

		double parseDeg(const QString &degToken, const QString &dirToken);

		QString latToString(double deg, int size) const;

		QString lonToString(double deg, int size) const;

		QString degToString(double deg, int size) const;

		QString qString2ftString(const QString &qString, uint length);

		QString ftString2qString(const QString &ftString);

		void addTail(QString &tlg) const;

		QString getCheckSum(const QString &tlg, uint end) const;

		bool validateCheckSum(const QString &tlg) const;
};

#endif
