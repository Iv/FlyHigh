/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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
 
#include <qbuffer.h>
#include <qdatetime.h>
#include "FlytecDevice.h"
extern "C"
{
	#include "flytec_al.h"
}

FlytecDevice::FlytecDevice()
{
	setPort("/dev/ttyS0");
}

void FlytecDevice::setPort(const QString &port)
{
	ft_init(port.ascii());
}

// memory
bool FlytecDevice::memoryRead(QByteArray &arr)
{
	u_char pageNr;
	u_int pageAddr;
	u_char nofPages = ft_MemSize / ft_PageSize;
	bool success = false;

	arr.resize(ft_MemSize);
	
	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		pageAddr = pageNr * ft_PageSize;
		success = (ft_memoryRead(pageAddr, (u_char*)(arr.data()+pageAddr)) == 0);
		
		if(!success)
		{
			break;
			// read Error!
		}
	}
	
	// the last few bytes....
	if(success)
	{
		pageAddr = ft_MemSize - ft_PageSize;
		success = (ft_memoryRead(pageAddr, (u_char*)(arr.data()+pageAddr)) == 0);
	}
	
	return success;
}

bool FlytecDevice::memoryWrite(QByteArray &arr)
{
	u_char pageNr;
	u_int pageAddr;
	u_char nofPages = ft_MemSize / ft_PageSize;
	bool success = false;
	
	if(arr.size() != ft_MemSize)
	{
		return false;
	}

	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		pageAddr = pageNr * ft_PageSize;
		success = (ft_memoryWrite(pageAddr, (u_char*)(arr.data()+pageAddr)) == 0);

		if(!success)
		{
			break;
			// write Error!
		}
	}

	// the last few bytes....
	if(success)
	{
		pageAddr = ft_MemSize - ft_PageSize;
		success = (ft_memoryWrite(pageAddr, (uchar*)(arr.data()+pageAddr)) == 0);
	}
	
	if(success)
	{
		ft_updateConfiguration();
	}
	
	return success;
}

// flights
bool FlytecDevice::flightList(Flight::FlightListType &flightList)
{
	bool success;
	TrackType track;
	Flight flight;
	
	success = (ft_trackListReq() == 0);
	
	if(success)
	{
		while(ft_trackListRec(&track) == 0)
		{
			flight.setNumber(track.trackNum);
			flight.setDate(QDate(track.date.year, track.date.month, track.date.day));
			flight.setDuration(track.duration.hour*3600 + track.duration.min*60 + track.duration.sec);
			flightList.push_back(flight);
		}
	}
	
	return success;
}

bool FlytecDevice::igcFile(uint flightNr, QByteArray &arr)
{
	QBuffer buff;
	uchar line[255];
	uchar size;
	bool success;
	
	buff.setBuffer(arr);
	success = (ft_trackReq(flightNr) == 0);
	
	if(success)
	{
		buff.open(IO_WriteOnly);
	
		while(ft_trackRec(&line[0], &size) == 0)
		{
			buff.writeBlock((char*)&line[0], size);
		}
		buff.close();
	}
	
	return success;
}

// waypoints
bool FlytecDevice::add(WayPoint &wp)
{
	ft_WayPointType ftWp;
	
	ft_string2ftstring(wp.name().ascii(), ftWp.name);
	ftWp.latitude = wp.latitude();
	ftWp.longitude = wp.longitude();
	ftWp.altitude = wp.altitude();

	return (ft_wayPointSnd(&ftWp) == 0);
}

bool FlytecDevice::delWayPoint(const QString &name)
{
	return (ft_wayPointDel(name.ascii()) == 0);
}

bool FlytecDevice::wayPointList(WayPoint::WayPointListType &wpList)
{
	WayPoint wp;
	ft_WayPointType ftWp;
	bool success;
	char name[20];

	success = (ft_wayPointListReq() == 0);
	
	if(success)
	{
		while(ft_wayPointListRec(&ftWp) == 0)
		{
			wp.setCoordinates(ftWp.latitude, ftWp.longitude, ftWp.altitude);
			ft_ftstring2string(name, ftWp.name);
			wp.setName(name);
			
			wpList.push_back(wp);
		}
	}
	
	return success;
}

// routes
bool FlytecDevice::add(Route &route)
{
	QString name;
	ft_RouteType ftRoute;
	uint wpNr;
	uint nofWp;
	bool success;
	
	ft_string2ftstring(route.name().ascii(), ftRoute.name);
	ftRoute.routeNum = 1;
	nofWp = route.wayPointList().size();
	ftRoute.totalSent = nofWp + 1;
	ftRoute.actSent = 0;

	// Name
	success = (ft_routeSnd(&ftRoute) == 0);
	
	if(success)
	{
		// WayPoints
		for(wpNr=0; wpNr<nofWp; wpNr++)
		{
			name = *route.wayPointList().at(wpNr);
			ft_string2ftstring(name.ascii(), ftRoute.name);
			ftRoute.actSent = wpNr + 1;
			success = (ft_routeSnd(&ftRoute) == 0);
			
			if(!success)
			{
				break;
			}
		}
	}
	
	return success;
}

bool FlytecDevice::routeList(Route::RouteListType &routeList)
{
	WayPoint wp;
	Route route;
	ft_RouteType ftRoute;
	bool success;
	char name[20];

	success = (ft_routeListReq() == 0);
	
	if(success)
	{
		// route
		while(ft_routeListRec(&ftRoute) == 0)
		{
			// name
			ft_ftstring2string(name, ftRoute.name);
			route.setName(name);
			route.wayPointList().clear();
			
			// waypoints
			while(ftRoute.actSent < (ftRoute.totalSent - 1))
			{
				if(ft_routeListRec(&ftRoute) == 0)
				{
					ft_ftstring2string(name, ftRoute.name);
					route.wayPointList().push_back(name);
				}
				else
				{
					break;
				}
			}
			
			routeList.push_back(route);
		}
	}
	
	return success;
}

bool FlytecDevice::delRoute(const QString &name)
{
	ft_routeDel(name.ascii());
	sleep(1);
	ft_routeDel(name.ascii());
		sleep(1);
	return (ft_routeDel(name.ascii()) == 0);
}
