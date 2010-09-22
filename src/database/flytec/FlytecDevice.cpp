/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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
 
#include <qbuffer.h>
#include <qdatetime.h>
#include <qstring.h>
#include "Error.h"
#include "FlytecDevice.h"
#include "IFlyHighRC.h"

extern "C"
{
	#include "flytec_al.h"
}

FlytecDevice::FlytecDevice()
{
}

void FlytecDevice::open()
{
	bool success;

        success = (ft_init(IFlyHighRC::pInstance()->deviceLine().toAscii().constData()) == 0);
	Error::verify(success, Error::FLYTEC_OPEN);
}

void FlytecDevice::close()
{
	ft_exit();
}

// memory
bool FlytecDevice::memoryRead(QByteArray &arr)
{
	u_char pageNr;
	u_int pageAddr;
	u_char nofPages = ft_MemSize / ft_PageSize;
	bool success = false;

	arr.resize(ft_MemSize);
	m_cancel = false;
	
	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		emit progress(pageNr*100/nofPages);
		
		if(m_cancel)
		{
			return false;
		}
		
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
	
	Error::verify(success, Error::FLYTEC_CMD);
	
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
	
	m_cancel = false;
	
	for(pageNr=0; pageNr<nofPages; pageNr++)
	{
		emit progress(pageNr*100/nofPages);
		
		if(m_cancel)
		{
			return false;
		}
		
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
	
	Error::verify(success, Error::FLYTEC_CMD);
	
	return success;
}

// flights
bool FlytecDevice::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	bool success;
	TrackType track;
	Flight flight;
	uint trackNr = 0;

	(void)pilot;
	
	success = (ft_trackListReq() == 0);
	m_cancel = false;
	
	if(success)
	{
		while(ft_trackListRec(&track) == 0)
		{
			emit progress(trackNr*100/track.totalNum);
			
			if(m_cancel)
			{
				return false;
			}
			
			flight.setNumber(trackNr);
			flight.setDate(QDate(track.date.year, track.date.month, track.date.day));
			flight.setTime(QTime(track.start.hour, track.start.min, track.start.sec));
			flight.setDuration(track.duration.hour*3600 + track.duration.min*60 + track.duration.sec);
			flightList.push_back(flight);
			trackNr++;
		}
	}
	
	Error::verify((flightList.size() > 0), Error::FLYTEC_CMD);
	
	return success;
}

bool FlytecDevice::loadIGCFile(Flight &flight)
{
	QBuffer buff;
	QByteArray track;
	uchar line[255];
	uchar size;
	bool success = false;
	int prog = 0;

	buff.setBuffer(&track);
	m_cancel = false;

	if(ft_trackReq(flight.number()) == 0)
	{
		buff.open(QIODevice::WriteOnly);
	
		while(ft_trackRec(&line[0], &size) == 0)
		{
			prog = (prog + 1) % 100;
			emit progress(prog);
			
			if(m_cancel)
			{
				return false;
			}

			buff.write((char*)&line[0], size);
			success = true;
		}

		buff.close();
		flight.setIgcData(track);
	}

	Error::verify(success, Error::FLYTEC_CMD);
	
	return success;
}

// waypoints
bool FlytecDevice::add(WayPoint &wp)
{
	bool success;
	ft_WayPointType ftWp;
	
        ft_string2ftstring(toFtString(wp.name()).toAscii().constData(), ftWp.name);
	ftWp.latitude = wp.latitude();
	ftWp.longitude = wp.longitude();
	ftWp.altitude = wp.altitude();

	success = (ft_wayPointSnd(&ftWp) == 0);
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	
	return success;
}

bool FlytecDevice::delWayPoint(WayPoint &wp)
{
	bool success;
	
        success = (ft_wayPointDel(wp.name().toAscii().constData()) == 0);
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	
	return success;
}

bool FlytecDevice::delAllWayPoints()
{
	bool success;
	
	success = (ft_wayPointDel(NULL) == 0);
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::WayPoints);
	
	return success;
}

bool FlytecDevice::wayPointList(WayPoint::WayPointListType &wpList)
{
	WayPoint wp;
	ft_WayPointType ftWp;
	bool success;
	char name[20];
	int prog = 0;

	success = (ft_wayPointListReq() == 0);
	m_cancel = false;
	
	if(success)
	{
		while(ft_wayPointListRec(&ftWp) == 0)
		{
			prog = (prog + 10) % 100;
			emit progress(prog);
			
			if(m_cancel)
			{
				return false;
			}
			
			wp.setCoordinates(ftWp.latitude, ftWp.longitude, ftWp.altitude);
			ft_ftstring2string(name, ftWp.name);
			wp.setName(name);
			wpList.push_back(wp);
		}
	}
	
	Error::verify(success, Error::FLYTEC_CMD);
	
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

	// make sure all waypoints exist on GPS
	nofWp = route.wayPointList().size();
	
	for(wpNr=0; wpNr<nofWp; wpNr++)
	{
		emit progress(wpNr*100/nofWp);
		add(route.wayPointList()[wpNr]);
	}

	// now write the route
        ft_string2ftstring(toFtString(route.name()).toAscii().constData(), ftRoute.name);
	ftRoute.routeNum = 1;
	nofWp = route.wayPointList().size();
	ftRoute.totalSent = nofWp + 1;
	ftRoute.actSent = 0;

	// Name
	success = (ft_routeSnd(&ftRoute) == 0);
	
	if(success)
	{
		m_cancel = false;
	
		// WayPoints
		for(wpNr=0; wpNr<nofWp; wpNr++)
		{
			emit progress(wpNr*100/nofWp);
			
			if(m_cancel)
			{
				return false;
			}
			
			name = route.wayPointList().at(wpNr).name();
                        ft_string2ftstring(toFtString(name).toAscii().constData(), ftRoute.name);
			ftRoute.actSent = wpNr + 1;
			success = (ft_routeSnd(&ftRoute) == 0);
			
			if(!success)
			{
				break;
			}
		}
	}
	
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::Routes);
	
	return success;
}

bool FlytecDevice::routeList(Route::RouteListType &routeList)
{
	WayPoint wp;
	Route route;
	ft_RouteType ftRoute;
	bool success;
	char name[20];
	int prog = 0;

	success = (ft_routeListReq() == 0);
	
	if(success)
	{
		m_cancel = false;
		
		// route
		while(ft_routeListRec(&ftRoute) == 0)
		{
			prog = (prog + 10) % 100;
			emit progress(prog);
			
			if(m_cancel)
			{
				return false;
			}
		
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
					wp.setName(name);
					route.wayPointList().push_back(wp);
				}
				else
				{
					break;
				}
			}
			
			routeList.push_back(route);
		}
	}
	
	Error::verify((routeList.size() > 0), Error::FLYTEC_CMD);
	
	return success;
}

bool FlytecDevice::delRoute(Route &route)
{
	bool success;
	
        success = (ft_routeDel(route.name().toAscii().constData()) == 0);
	IGPSDevice::setLastModified(IGPSDevice::Routes);
	
	return success;
}

// AirSpace
bool FlytecDevice::add(AirSpace &airspace)
{
	AirSpaceItem *pItem;
	AirSpaceItemSeg *pSegment;
	AirSpaceItemCircle *pCircle;
	QString name;
	ft_CTRType ftCTR;
	uint nofMember;
	uint memberNr;
	bool success;

	nofMember = airspace.airSpaceItemList().size();
	
	// first sentence
	ftCTR.totalSent = nofMember + 2; // inclusive first and second sentence
	ftCTR.actSent = 0;
        ft_string2ftstring(toFtString(airspace.name()).toAscii().constData(), ftCTR.sent.first.name);
	ftCTR.sent.first.warnDist = airspace.warnDist();
	success = (ft_ctrSnd(&ftCTR) == 0);

	// second sentence
	ftCTR.actSent = 1;
        ft_string2ftstring(toFtString(airspace.remark()).toAscii().constData(), ftCTR.sent.second.remark);
	success &= (ft_ctrSnd(&ftCTR) == 0);

	if(success)
	{
		m_cancel = false;
		
		// members
		for(memberNr=0; memberNr<nofMember; memberNr++)
		{
			emit progress(memberNr*100/nofMember);
			
			if(m_cancel)
			{
				return false;
			}
			
			ftCTR.actSent = memberNr + 2;
			pItem = airspace.airSpaceItemList().at(memberNr);

			ftCTR.sent.member.latitude = pItem->lat();
			ftCTR.sent.member.longitude = pItem->lon();

			switch(pItem->type())
			{
				case AirSpaceItem::Point:
				case AirSpaceItem::Center:
					if(pItem->type() == AirSpaceItem::Point)
					{
						ftCTR.sent.member.type = 'P';
					}
					else
					{
						ftCTR.sent.member.type = 'X';
					}
				break;
				case AirSpaceItem::StartSegment:
				case AirSpaceItem::StopSegment:
					if(pItem->type() == AirSpaceItem::StartSegment)
					{
						ftCTR.sent.member.type = 'T';
					}
					else
					{
						ftCTR.sent.member.type = 'Z';
					}

					pSegment = (AirSpaceItemSeg*)pItem;

					if(pSegment->dir())
					{
						ftCTR.sent.member.direction = '+';
					}
					else
					{
						ftCTR.sent.member.direction = '-';
					}
				break;
				case AirSpaceItem::Circle:
					ftCTR.sent.member.type = 'C';
					pCircle = (AirSpaceItemCircle*)pItem;
					ftCTR.sent.member.radius = pCircle->radius();
				break;
			}
			
			success = (ft_ctrSnd(&ftCTR) == 0);
			
			if(!success)
			{
				break;
			}
		}
		
		if(success)
		{
			success = (ft_ctrRecAck() == 0);
		}
	}
	
	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::AirSpaces);
	
	return success;
}

bool FlytecDevice::airspaceList(AirSpace::AirSpaceListType &airspaceList)
{
	AirSpaceItem *pItem = NULL;
	AirSpaceItemSeg *pSegment;
	AirSpaceItemCircle *pCircle;
	AirSpace *pAirspace;
	ft_CTRType ftCTR;
	bool success;
	char string[20];
	int prog = 0;

	success = (ft_ctrListReq() == 0);
	
	if(success)
	{
		m_cancel = false;
		
		while(ft_ctrListRec(&ftCTR) == 0)
		{
			prog = (prog + 10) % 100;
			emit progress(prog);

			if(m_cancel)
			{
				return false;
			}

			pAirspace = new AirSpace;

			// first sentence
			ft_ftstring2string(string, ftCTR.sent.first.name);
			pAirspace->setName(string);
			pAirspace->setWarnDist(ftCTR.sent.first.warnDist);
			
			// second sentence
			success &= (ft_ctrListRec(&ftCTR) == 0);
			ft_ftstring2string(string, ftCTR.sent.second.remark);
			pAirspace->setRemark(string);

			// members
			while(ftCTR.actSent < (ftCTR.totalSent - 1))
			{
				if(ft_ctrListRec(&ftCTR) == 0)
				{
					if(ftCTR.sent.member.type == 'P')
					{
						pItem = new AirSpaceItemPoint(AirSpaceItem::Point);
					}
					else if(ftCTR.sent.member.type == 'X')
					{
						pItem = new AirSpaceItemPoint(AirSpaceItem::Center);
					}
					else if(ftCTR.sent.member.type == 'T')
					{
						pSegment = new AirSpaceItemSeg(AirSpaceItem::StartSegment);
						pSegment->setDir(ftCTR.sent.member.direction == '+');
						pItem = pSegment;
					}
					else if(ftCTR.sent.member.type == 'Z')
					{
						pSegment = new AirSpaceItemSeg(AirSpaceItem::StopSegment);
						pSegment->setDir(ftCTR.sent.member.direction == '+');
						pItem = pSegment;
					}
					else if(ftCTR.sent.member.type == 'C')
					{
						pCircle = new AirSpaceItemCircle;
						pCircle->setRadius(ftCTR.sent.member.radius);
						pItem = pCircle;
					}
					
					pItem->setPoint(ftCTR.sent.member.latitude, ftCTR.sent.member.longitude);
					pAirspace->airSpaceItemList().push_back(pItem);
				}
				else
				{
					break;
				}
			}

			airspaceList.append(pAirspace);
		}
	}

	Error::verify((airspaceList.count() > 0), Error::FLYTEC_CMD);
	
	return success;
}

bool FlytecDevice::delAirSpace(AirSpace &airspace)
{
	bool success;
	
        success = (ft_ctrDel(airspace.name().toAscii().constData()) == 0);

	if(success)
	{
		success = (ft_ctrRecAck() == 0);
	}

	Error::verify(success, Error::FLYTEC_CMD);
	IGPSDevice::setLastModified(IGPSDevice::AirSpaces);
	
	return success;
}

void FlytecDevice::cancel()
{
	m_cancel = true;
}

QString FlytecDevice::toFtString(const QString &inStr)
{
	QString locStr;

	locStr = inStr.toUpper();

	if(locStr.length() > FT_STRING_SIZE)
	{
		locStr = locStr.remove(' ');

		if(locStr.length() > FT_STRING_SIZE)
		{
			locStr = locStr.left(12) + "-" + locStr.right(4);
		}
	}

	return locStr;
}
