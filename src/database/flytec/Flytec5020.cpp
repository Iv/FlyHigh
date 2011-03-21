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

#include "AirSpace.h"
#include "AirSpaceList.h"
#include "Error.h"
#include "Flytec5020.h"
#include "IFlyHighRC.h"
#include "Protocol5020.h"

#include <QDebug>

Flytec5020::Flytec5020()
{
	m_protocol = new Protocol5020();
	m_cancel = false;
}

Flytec5020::~Flytec5020()
{
	delete m_protocol;
}

bool Flytec5020::open()
{
	bool success;

	success = m_protocol->open(IFlyHighRC::pInstance()->deviceLine(),
					IFlyHighRC::pInstance()->deviceSpeedString().toUInt());
	Error::verify(success, Error::FLYTEC_OPEN);

/*
DeviceInfo devInfo;

m_protocol->devInfoReq();
m_protocol->devInfoRec(devInfo);
*/

	return success;
}

void Flytec5020::close()
{
	m_protocol->close();
}

void Flytec5020::cancel()
{
	m_cancel = true;
}

bool Flytec5020::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	Flight flight;
	bool success;
	uint total;

	(void)pilot;

	success = m_protocol->trackListReq();
	m_cancel = false;

	if(success)
	{
		do
		{
			m_protocol->trackListRec(total, flight);
			flightList.push_back(flight);

			if(total > 0)
			{
				emit progress(flight.number() * 100 / total);
			}

			if(m_cancel)
			{
				return false;
			}
		}while(flight.number() < (total - 1));
	}

	Error::verify((flightList.size() > 0), Error::FLYTEC_CMD);

	return success;
}

bool Flytec5020::loadIGCFile(Flight &flight)
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

bool Flytec5020::add(WayPoint &wp)
{
	bool success;

	success = m_protocol->wpSnd(wp);
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);

	return success;
}

bool Flytec5020::delWayPoint(WayPoint &wp)
{
	bool success;

	success = m_protocol->wpDel(wp.name());
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);

	return success;
}

bool Flytec5020::delAllWayPoints()
{
	int prog;
	bool success;

	success = m_protocol->wpDelAll();

/**
	for(prog=0; prog<=100; prog+=10)
	{
		emit progress(prog);

		if(m_protocol->recieveDone())
		{
			break;
		}
	}
*/

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);

	return success;
}

bool Flytec5020::wayPointList(WayPoint::WayPointListType &wpList)
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

bool Flytec5020::add(Route &route)
{
	uint curSent;
	uint totalSent;
	uint wpNr;
	uint nofWp;
	bool success;

	// make sure all waypoints exist on GPS
	nofWp = route.wayPointList().size();

	for(wpNr=0; wpNr<nofWp; wpNr++)
	{
		emit progress(wpNr *100 / nofWp);
		m_protocol->wpSnd(route.wayPointList().at(wpNr));
	}

	IGPSDevice::setLastModified(IGPSDevice::WayPoints);

	// now write the route
	totalSent = 1 + route.wayPointList().size();
	m_cancel = false;

	for(curSent=0; curSent<totalSent; curSent++)
	{
		emit progress(curSent * 100 / totalSent);

		if(m_cancel)
		{
			return false;
		}

		success = m_protocol->routeSnd(curSent, totalSent, route);

		if(!success)
		{
			break;
		}
	}

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::Routes);

	return success;
}

bool Flytec5020::routeList(Route::RouteListType &routeList)
{
	Route route;
	WayPoint wp;
	bool success = false;
	uint curSent;
	uint totalSent;

	m_cancel = false;

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

bool Flytec5020::delRoute(Route &route)
{
	bool success;

	success = m_protocol->routeDel(route.name());

/**
	for(prog=0; prog<=100; prog+=10)
	{
		emit progress(prog);

		if(m_protocol->recieveDone())
		{
			success = true;
			break;
		}
	}
*/

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::Routes);

	return success;
}

bool Flytec5020::add(AirSpace &airspace)
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
		success == m_protocol->recAck();
	}

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::AirSpaces);

	return success;
}

bool Flytec5020::delAirSpace(AirSpace &airspace)
{
	bool success;

	success = m_protocol->ctrDel(airspace.name());
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::AirSpaces);

	return success;
}

bool Flytec5020::airspaceList(AirSpaceList &airspaceList)
{
	AirSpace *pAirspace;
	uint curSent;
	uint totalSent;
  uint nofCtr;
  uint maxCtr;
  uint nofFree;
  uint ctrNr = 0;
	bool success = false;
	bool used = false;
	bool ctrAv = false;

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

bool Flytec5020::memoryRead(QByteArray &arr)
{
	uint pageNr;
	uint pageAddr;
	uint nofPages = Flytec5020MemSize / Flytec5020PageSize;
	bool success = false;

	arr.resize(Flytec5020MemSize);
	m_cancel = false;

	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		emit progress(pageNr * 100 / nofPages);

		if(m_cancel)
		{
			return false;
		}

		pageAddr = pageNr * Flytec5020PageSize;
		success = m_protocol->memoryRead(pageAddr, (u_char*)(arr.data()+pageAddr));

		if(!success)
		{
			break;
			// read Error!
		}
	}

	// the last few bytes....
	if(success)
	{
		pageAddr = Flytec5020MemSize - Flytec5020PageSize;
		success = m_protocol->memoryRead(pageAddr, (u_char*)(arr.data()+pageAddr));
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}

bool Flytec5020::memoryWrite(QByteArray &arr)
{
	uint pageNr;
	uint pageAddr;
	uint nofPages = Flytec5020MemSize / Flytec5020PageSize;
	bool success = false;

	if(arr.size() != Flytec5020MemSize)
	{
		return false;
	}

	m_cancel = false;

	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		emit progress(pageNr * 100 / nofPages);

		if(m_cancel)
		{
			return false;
		}

		pageAddr = pageNr * Flytec5020PageSize;
		success = m_protocol->memoryWrite(pageAddr, (u_char*)(arr.data()+pageAddr));

		if(!success)
		{
			break;
			// write Error!
		}
	}

	// the last few bytes....
	if(success)
	{
		pageAddr = Flytec5020MemSize - Flytec5020PageSize;
		success = m_protocol->memoryWrite(pageAddr, (uchar*)(arr.data()+pageAddr));
	}

	if(success)
	{
		m_protocol->updateConfiguration();
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}
