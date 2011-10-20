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

#include <QVariant>
#include <QBuffer>
#include "AirSpace.h"
#include "AirSpaceList.h"
#include "Error.h"
#include "Flytec.h"
#include "IFlyHighRC.h"
#include "Protocol5020.h"
#include "Protocol6015.h"

#include <QDebug>

Flytec::Flytec(IFlyHighRC::DeviceId id)
{
  switch(id)
  {
    case IFlyHighRC::DevFlytec5020:
    case IFlyHighRC::DevFlytec6020:
    	m_protocol = new Protocol5020(id);
    break;
    case IFlyHighRC::DevFlytec6015:
    	m_protocol = new Protocol6015(id);
    break;
  }

  IGPSDevice::setDeviceId(id);
	m_cancel = false;
}

Flytec::~Flytec()
{
	delete m_protocol;
}

bool Flytec::open()
{
	bool success;

	success = m_protocol->open(IFlyHighRC::pInstance()->deviceLine(),
	IFlyHighRC::pInstance()->deviceSpeedString().toUInt());
	Error::verify(success, Error::FLYTEC_OPEN);

	return success;
}

void Flytec::close()
{
	m_protocol->close();
}

bool Flytec::deviceInfo(DeviceInfo &info)
{
  bool success;

  success = m_protocol->devInfoReq();

  if(success)
  {
    success = m_protocol->devInfoRec(info);
  }

  return success;
}

bool Flytec::memoryRead()
{
	uint pageNr;
  uint memSize;
	uint pageSize;
	uint pageAddr;
	uint nofPages;
	bool success = false;

  memSize = Flytec5020MemSize;
  pageSize = Flytec5020PageSize;
  nofPages = memSize / pageSize;
	m_cancel = false;

	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		emit progress(pageNr * 100 / nofPages);

		if(m_cancel)
		{
			return false;
		}

		pageAddr = pageNr * pageSize;
		success = m_protocol->memoryRead(pageAddr);

		if(!success)
		{
			break;
			// read Error!
		}
	}

	// the last few bytes....
	if(success)
	{
		pageAddr = memSize - pageSize;
		success = m_protocol->memoryRead(pageAddr);
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}

bool Flytec::memoryWrite()
{
	uint pageNr;
	uint memSize;
	uint pageSize;
	uint pageAddr;
	uint nofPages = Flytec5020MemSize / Flytec5020PageSize;
	bool success = false;

	memSize = Flytec5020MemSize;
	pageSize = Flytec5020PageSize;
	nofPages = memSize / pageSize;
	m_cancel = false;

	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		emit progress(pageNr * 100 / nofPages);

		if(m_cancel)
		{
			return false;
		}

		pageAddr = pageNr * pageSize;
		success = m_protocol->memoryWrite(pageAddr);

		if(!success)
		{
			break;
			// write Error!
		}
	}

	// the last few bytes....
	if(success)
	{
		pageAddr = memSize - pageSize;
		success = m_protocol->memoryWrite(pageAddr);
	}

	if(success)
	{
		m_protocol->updateConfiguration();
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}

bool Flytec::parWrite(MemType memType, int par, FtDataType dataType, const QVariant &value)
{
  return m_protocol->parWrite(memType, par, dataType, value);
}

QVariant Flytec::parRead(MemType memType, int par, FtDataType dataType)
{
  return m_protocol->parRead(memType, par, dataType);
}

void Flytec::cancel()
{
	m_cancel = true;
}

bool Flytec::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	Flight flight;
	bool success;
	int total=0;
	int prog=0;

	(void)pilot;

	success = m_protocol->trackListReq();
	m_cancel = false;

	if(success)
	{
		while(m_protocol->trackListRec(total, flight))
		{
		  flightList.push_back(flight);

			if(total > 0)
			{
				prog = flight.number() * 100 / total;
			}
			else
			{
				prog = (prog + 10) % 100;
			}

			emit progress(prog);

			if(m_cancel)
			{
				return false;
			}
		}
	}

	Error::verify((flightList.size() > 0), Error::FLYTEC_CMD);

	return success;
}

bool Flytec::loadIGCFile(Flight &flight)
{
	QBuffer buff;
	QByteArray track;
	QString line;
	bool success = false;
	int prog = 0;

	buff.setBuffer(&track);
	m_cancel = false;

	if(m_protocol->trackReq(flight.number()))
	{
		buff.open(QIODevice::WriteOnly);

		while(m_protocol->trackRec(line))
		{
			prog = (prog + 1) % 100;
			emit progress(prog);

			if(m_cancel)
			{
				return false;
			}

			buff.write(line.toAscii(), line.length());
			success = true;
		}

		buff.close();
		flight.setIgcData(track);
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}

bool Flytec::add(WayPoint &wp)
{
	bool success;

	success = m_protocol->wpSnd(wp);
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	emit wayPointsChanged();

	return success;
}

bool Flytec::add(WayPoint::WayPointListType &wpList)
{
  int nofWp;
  int wpNr;
  bool success = true;

  nofWp = wpList.size();

  for(wpNr=0; wpNr<nofWp; wpNr++)
  {
    success &= m_protocol->wpSnd(wpList[wpNr]);
    emit progress(wpNr * 100 / nofWp);
  }

  Error::verify(success, Error::FLYTEC_CMD);
  IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	emit wayPointsChanged();

	return success;
}

bool Flytec::delWayPoint(WayPoint &wp)
{
	bool success;

	success = m_protocol->wpDel(wp.name());
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	emit wayPointsChanged();

	return success;
}

bool Flytec::delWayPoints(WayPoint::WayPointListType &wpList)
{
  int nofWp;
  int wpNr;
  bool success = true;

  nofWp = wpList.size();

  for(wpNr=0; wpNr<nofWp; wpNr++)
  {
    success &= m_protocol->wpDel(wpList[wpNr].name());
    emit progress(wpNr * 100 / nofWp);
  }

  Error::verify(success, Error::FLYTEC_CMD);
  IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	emit wayPointsChanged();

	return success;
}

bool Flytec::delAllWayPoints()
{
	bool success;

	success = m_protocol->wpDelAll();
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	emit wayPointsChanged();

	return success;
}

bool Flytec::wayPointList(WayPoint::WayPointListType &wpList)
{
	WayPoint wp;
	bool success = false;
	int prog = 0;

	m_cancel = false;
	success = m_protocol->wpListReq();

	if(success)
	{
		while(m_protocol->wpListRec(wp))
		{
			prog = (prog + 10) % 100;
			emit progress(prog);

			if(m_cancel)
			{
				return false;
			}

			wpList.push_back(wp);
		}
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}

bool Flytec::add(Route &route)
{
	uint curSent;
	uint totalSent;
	uint wpNr;
	uint nofWp;
	bool success = false;

  m_cancel = false;

	// make sure all waypoints exist on GPS
	if(dynamic_cast<Protocol5020*>(m_protocol) != NULL)
	{
    nofWp = route.wayPointList().size();

    for(wpNr=0; wpNr<nofWp; wpNr++)
    {
      emit progress(wpNr *100 / nofWp);
      m_protocol->wpSnd(route.wayPointList().at(wpNr));

      if(m_cancel)
      {
        return false;
      }
    }

    IGPSDevice::setLastModified(IGPSDevice::WayPoints);
    emit wayPointsChanged();
	}

	// now write the route
	curSent = 0;

	while(m_protocol->routeSnd(curSent, totalSent, route))
	{
		emit progress(curSent * 100 / totalSent);

    if(m_cancel)
		{
			return false;
		}
	}

	success = (curSent == totalSent);
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::Routes);
	emit routesChanged();

	return success;
}

bool Flytec::routeList(Route::RouteListType &routeList)
{
	Route route;
	WayPoint wp;
	bool success = false;
	uint curSent;
	uint totalSent;

	m_cancel = false;
  curSent = 0;
	success = m_protocol->routeListReq();
	routeList.clear();

	if(success)
	{
		while(m_protocol->routeListRec(curSent, totalSent, route))
		{
			emit progress(curSent * 100 / totalSent);

			if(m_cancel)
			{
				return false;
			}

			if((curSent + 1) == totalSent)
			{
				routeList.push_back(route);
				route.wayPointList().clear();
			}
		}
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}

bool Flytec::delRoute(Route &route)
{
	bool success;

	success = m_protocol->routeDel(route.name());
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::Routes);
	emit routesChanged();

	return success;
}

bool Flytec::add(AirSpace &airspace)
{
	bool success = false;
	uint curSent;
	uint totalSent;

	totalSent = 2 + airspace.airSpaceItemList().size();
	m_cancel = false;

	// send airspace
	for(curSent=0; curSent<totalSent; curSent++)
	{
		emit progress(curSent * 100 / totalSent);

		if(m_cancel)
		{
			return false;
		}

		success = m_protocol->ctrSnd(curSent, totalSent, airspace);

		if(!success)
		{
			break;
		}
	}

	// recieve ack
	if(success)
	{
		success = m_protocol->recAck();
	}

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::AirSpaces);
	emit airSpacesChanged();

	return success;
}

bool Flytec::delAirSpace(AirSpace &airspace)
{
	bool success;

	success = m_protocol->ctrDel(airspace.name());
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::AirSpaces);
	emit airSpacesChanged();

	return success;
}

bool Flytec::airspaceList(AirSpaceList &airspaceList)
{
	AirSpace *pAirspace;
	uint curSent;
	uint totalSent;
  uint nofCtr;
  uint maxCtr;
  uint nofFree;
  uint ctrNr = 0;
	bool success = false;
	bool ctrAv = false;

  curSent = 0;
	m_cancel = false;
  airspaceList.clear();
	success = m_protocol->ctrInfoReq();

	if(success)
	{
    m_protocol->ctrInfoRec(nofCtr, maxCtr, nofFree);
    ctrAv = (nofCtr > 0);
    success = m_protocol->ctrListReq();
	}

  if(success && ctrAv)
	{
		pAirspace = new AirSpace();

		while(m_protocol->ctrListRec(curSent, totalSent, pAirspace))
		{
			if(m_cancel)
			{
				break;
			}

			if((curSent + 1) == totalSent)
			{
				airspaceList.push_back(pAirspace);
				pAirspace = new AirSpace();
        emit progress(ctrNr * 100 / nofCtr);
        ctrNr++;
			}
		}

		// last one is never used
		delete pAirspace;
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return (success || !ctrAv);
}
