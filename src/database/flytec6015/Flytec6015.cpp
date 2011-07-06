/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                     *
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
#include "Flytec6015.h"
#include "Error.h"
#include "IFlyHighRC.h"
#include "Protocol6015.h"

Flytec6015::Flytec6015()
{
	m_protocol = new Protocol6015();
	m_cancel = false;
}

Flytec6015::~Flytec6015()
{
	delete m_protocol;
}

bool Flytec6015::open()
{
	bool success;

	success = m_protocol->open(IFlyHighRC::pInstance()->deviceLine(),
					IFlyHighRC::pInstance()->deviceSpeedString().toUInt());
	Error::verify(success, Error::FLYTEC_OPEN);

	return success;
}

void Flytec6015::close()
{
	m_protocol->close();
}

void Flytec6015::cancel()
{
	m_cancel = true;
}

bool Flytec6015::memoryWrite(MemType memType, int par, DataType dataType, const QVariant &value)
{
	return m_protocol->memoryWrite(memType, par, dataType, value);
}

QVariant Flytec6015::memoryRead(MemType memType, int par, DataType dataType)
{
	return m_protocol->memoryRead(memType, par, dataType);
}

bool Flytec6015::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	Flight flight;
	bool success;
	uint trackNr = 0;

	(void)pilot;

	success = m_protocol->trackListReq();
	m_cancel = false;

	if(success)
	{
		while(m_protocol->trackListRec(flight))
		{
			emit progress(trackNr * 100 / 5);

			if(m_cancel)
			{
				return false;
			}

			flightList.push_back(flight);
			trackNr++;
		}
	}

	Error::verify((flightList.size() > 0), Error::FLYTEC_CMD);

	return success;
}

bool Flytec6015::loadIGCFile(Flight &flight)
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

bool Flytec6015::add(WayPoint &wp)
{
	bool success;

	success = m_protocol->wpSnd(wp);
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);

	return success;
}

bool Flytec6015::delWayPoint(WayPoint &wp)
{
	(void)wp;

	return false;
}

bool Flytec6015::delAllWayPoints()
{
	int prog;
	bool success;

	success = m_protocol->wpDelAll();

	for(prog=0; prog<=100; prog+=10)
	{
		emit progress(prog);

		if(m_protocol->recieveDone())
		{
			break;
		}
	}

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);

	return success;
}

bool Flytec6015::wayPointList(WayPoint::WayPointListType &wpList)
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

bool Flytec6015::add(Route &route)
{
	uint wpNr;
	uint nofWp;
	bool success = false;

	nofWp = route.wayPointList().size();

	m_cancel = false;

	// WayPoints
	for(wpNr=0; wpNr<nofWp; wpNr++)
	{
		emit progress(wpNr * 100 / nofWp);

		if(m_cancel)
		{
			return false;
		}

		success = m_protocol->routeSnd(route.wayPointList().at(wpNr));

		if(!success)
		{
			break;
		}
	}

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::Routes);

	return success;
}

bool Flytec6015::routeList(Route::RouteListType &routeList)
{
	Route route;
	WayPoint wp;
	bool success = false;
	int prog = 0;

	m_cancel = false;

	success = m_protocol->routeListReq();
	routeList.clear();

	if(success)
	{
		route.wayPointList().clear();

		while(m_protocol->routeListRec(wp))
		{
			prog = (prog + 10) % 100;
			emit progress(prog);

			if(m_cancel)
			{
				return false;
			}

			route.wayPointList().push_back(wp);
		}

		if(route.wayPointList().size() > 0)
		{
			route.setName("Route 1");
			routeList.push_back(route);
		}
	}

	Error::verify(success, Error::FLYTEC_CMD);

	return success;
}

bool Flytec6015::delRoute(Route &route)
{
	int prog;
	bool success;

	(void)route;

	success = m_protocol->routeDel();

	for(prog=0; prog<=100; prog+=10)
	{
		emit progress(prog);

		if(m_protocol->recieveDone())
		{
			success = true;
			break;
		}
	}

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::Routes);

	return success;
}
