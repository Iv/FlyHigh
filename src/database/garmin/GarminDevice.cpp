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
#include "Error.h"
#include "GarminDevice.h"
#include "GarminFmt.h"
#include "IFlyHighRC.h"

#include "gpsapp.h"
#include "gpsmem.h"

GarminDevice::GarminDevice()
{
}

bool GarminDevice::open()
{
	return GPS_Init(IFlyHighRC::pInstance()->deviceLine().toAscii().constData());
}

void GarminDevice::close()
{
	GPS_Command_Off(IFlyHighRC::pInstance()->deviceLine().toAscii().constData());
}

// flights
bool GarminDevice::flightList(Flight::FlightListType &flightList)
{
	GarminFmt fmt;
	GPS_PTrack *trkList;
	int nTracks;
	int trackNr;

	m_flightList.clear();
        nTracks = GPS_Command_Get_Track(IFlyHighRC::pInstance()->deviceLine().toAscii().constData(), &trkList);

	// store tracks and release memory
	if(nTracks > 0)
	{
		fmt.GPS_Fmt_Print_Track(trkList, nTracks, m_flightList);
		flightList = m_flightList; // copy flight list to user

		// release allocated memory
		for(trackNr=0; trackNr<nTracks; trackNr++)
		{
			GPS_Track_Del(&trkList[trackNr]);
		}

		free(trkList);
	}

	return (nTracks > 0);
}

bool GarminDevice::igcFile(int flightNr, QByteArray &arr)
{
	bool success;

	success = m_flightList.size() > flightNr;

	if(success)
	{
		arr = m_flightList[flightNr].igcData();
	}

	return success;
}

// waypoints
bool GarminDevice::add(WayPoint &wp)
{
	GPS_PWay *wayList;
	int ret;
	int nWps = 1;

	wayList = (GPS_PWay*)malloc(sizeof(GPS_PWay*));

	wayList[0] = GPS_Way_New();
	wayList[0]->prot = gps_waypt_transfer;
        memcpy(wayList[0]->ident, wp.name().toAscii().constData(), wp.name().length());
        memcpy(wayList[0]->cmnt, wp.description().toAscii().constData(), wp.description().length());
	wayList[0]->lat = wp.lat();
	wayList[0]->lon = wp.lon();
	wayList[0]->alt = wp.alt();

	// send wp
        ret = GPS_Command_Send_Waypoint(IFlyHighRC::pInstance()->deviceLine().toAscii().constData(), wayList, nWps);

	// release allocated memory
	GPS_Way_Del(&wayList[0]);
	free(wayList);

	return !(ret < 0);
}

bool GarminDevice::delWayPoint(const QString &name)
{
	(void)name;
	return false;
}

bool GarminDevice::wayPointList(WayPoint::WayPointListType &wpList)
{
	GarminFmt fmt;
	GPS_PWay *wayList;
	int nWps;
	int wpNr;

        nWps = GPS_Command_Get_Waypoint(IFlyHighRC::pInstance()->deviceLine().toAscii().constData(), &wayList);

	if(nWps > 0)
	{
		fmt.GPS_Fmt_Print_Waypoint(wayList, nWps, wpList);

		// release allocated memory
		for(wpNr=0; wpNr<nWps; wpNr++)
		{
			GPS_Way_Del(&wayList[wpNr]);
		}

		free(wayList);
	}

	return (nWps > 0);
}

// routes
bool GarminDevice::add(Route &route)
{
	(void)route;
	return false;
}

bool GarminDevice::routeList(Route::RouteListType &routeList)
{
	GarminFmt fmt;
	GPS_PWay *wayList;
	int nWps;
	int wpNr;

        nWps = GPS_Command_Get_Route(IFlyHighRC::pInstance()->deviceLine().toAscii().constData(), &wayList);

	if(nWps > 0)
	{
		fmt.GPS_Fmt_Print_Route(wayList, nWps, routeList);

		// release allocated memory
		for(wpNr=0; wpNr<nWps; wpNr++)
		{
			GPS_Way_Del(&wayList[wpNr]);
		}

		free(wayList);
	}

	return (nWps > 0);
}

bool GarminDevice::delRoute(const QString &name)
{
	(void)name;

	return false;
}

void GarminDevice::cancel()
{
	m_cancel = true;
}
