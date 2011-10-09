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
#ifndef Protocol_h
#define Protocol_h

#include <qvariant.h>
#include "Defs5020.h"
#include "Defs6015.h"

class AirSpace;
class Flight;
class Route;
class WayPoint;

/**
	@author Alex Graf <grafal@sourceforge.net>
*/
class Protocol
{
	public:
		Protocol();

		virtual ~Protocol();

    virtual bool open(const QString &dev, int baud);

		virtual void close();

    /// 5020 specific
    virtual bool devInfoReq();

    virtual bool devInfoRec(DeviceInfo &devInfo);

    virtual bool memoryRead(uint addr);

    virtual bool memoryWrite(uint addr);

    virtual bool updateConfiguration();

    virtual bool recAck();

    /** Memory */
    virtual bool parWrite(MemType memType, int par, FtDataType dataType, const QVariant &value);

    virtual QVariant parRead(MemType memType, int par, FtDataType dataType);

		/** Track */
		virtual bool trackListReq();

		virtual bool trackListRec(int &total, Flight &flight);

		virtual bool trackReq(int trackNr);

		virtual bool trackRec(QString &line);

		/** Waypoint */
		virtual bool wpListReq();

		virtual bool wpListRec(WayPoint &wp);

		virtual bool wpSnd(const WayPoint &wp);

    /// only 5020
		virtual bool wpDel(const QString &name);

		virtual bool wpDelAll();

		/** Route */
		virtual bool routeListReq();

		virtual bool routeListRec(uint &curSent, uint &totalSent, Route &route);

    virtual bool routeSnd(uint &curSent, uint &totalSent, Route &route);

		virtual bool routeDel(const QString &name);

		/** AirSpaces */
    virtual bool ctrInfoReq();

		virtual bool ctrInfoRec(uint &nofCtr, uint &maxCtr, uint &nofFree);

		virtual bool ctrListReq();

		virtual bool ctrListRec(uint &curSent, uint &totalSent, AirSpace *pAirSpace);

		virtual bool ctrSnd(uint curSent, uint totalSent, AirSpace &airspace);

		virtual bool ctrDel(const QString &name);
};

#endif
