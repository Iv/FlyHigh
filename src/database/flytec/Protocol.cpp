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

#include "Protocol.h"

Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

bool Protocol::open(const QString &dev, int baud)
{
	return false;
}

void Protocol::close()
{
}

bool Protocol::devInfoReq()
{
	return false;
}

bool Protocol::devInfoRec(DeviceInfo &devInfo)
{
	return false;
}

bool Protocol::memoryRead(uint addr)
{
	return false;
}

bool Protocol::memoryWrite(uint addr)
{
	return false;
}

bool Protocol::updateConfiguration()
{
	return false;
}

bool Protocol::recAck()
{
  return false;
}

bool Protocol::parWrite(MemType memType, int par, FtDataType dataType, const QVariant &value)
{
	return false;
}

QVariant Protocol::parRead(MemType memType, int par, FtDataType dataType)
{
	return QVariant();
}

bool Protocol::trackListReq()
{
	return false;
}

bool Protocol::trackListRec(int &total, Flight &flight)
{
	return false;
}

bool Protocol::trackReq(int trackNr)
{
	return false;
}

bool Protocol::trackRec(QString &line)
{
  return false;
}

bool Protocol::wpListReq()
{
  return false;
}

bool Protocol::wpListRec(WayPoint &wp)
{
  return false;
}

bool Protocol::wpSnd(const WayPoint &wp)
{
  return false;
}

bool Protocol::wpDel(const QString &name)
{
  return false;
}

bool Protocol::wpDelAll()
{
  return false;
}

bool Protocol::routeListReq()
{
  return false;
}

bool Protocol::routeListRec(uint &curSent, uint &totalSent, Route &route)
{
  return false;
}

bool Protocol::routeSnd(uint &curSent, uint &totalSent, Route &route)
{
  return false;
}

bool Protocol::routeDel(const QString &name)
{
  return false;
}

bool Protocol::ctrInfoReq()
{
  return false;
}

bool Protocol::ctrInfoRec(uint &nofCtr, uint &maxCtr, uint &nofFree)
{
  return false;
}

bool Protocol::ctrListReq()
{
  return false;
}

bool Protocol::ctrListRec(uint &curSent, uint &totalSent, AirSpace *pAirSpace)
{
  return false;
}

bool Protocol::ctrSnd(uint curSent, uint totalSent, AirSpace &airspace)
{
  return false;
}

bool Protocol::ctrDel(const QString &name)
{
  return false;
}
