/***************************************************************************
*   Copyright (C) 2005 by Alex Graf                                     *
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

#include <math.h>
#include <qbuffer.h>
#include "GarminFmt.h"

GarminFmt::GarminFmt()
{
}

void GarminFmt::GPS_Fmt_Print_Track(GPS_PTrack *trk, int32 n, Flight::FlightListType &flightList)
{
	int i;
	Flight flight;
	QString bRecord;
	QByteArray igcData;
	QBuffer igcDataBuff;

	switch(gps_trk_transfer)
	{
		case pA300: case pA301:
			igcDataBuff.setBuffer(const_cast<QByteArray*>(&flight.igcData()));
			igcDataBuff.open(QIODevice::WriteOnly);
			
			for(i=0; i<n; ++i)
			{
				if(trk[i]->ishdr)
				{
					// skip track header
					continue;
				}
					
				if(trk[i]->tnew)
				{
					flight.setIgcData(igcData);
					flightList.push_back(flight);
					igcData.resize(0);
					
					// new track
					flight.setNumber(i);
				}
		
				switch(gps_trk_type)
				{
					case pD300:
						trk[i]->alt = 0;
						setBrecord(trk[i], bRecord);
                                                igcDataBuff.write(bRecord.toAscii(), bRecord.length());
					break;
					case pD301:
						setBrecord(trk[i], bRecord);
                                                igcDataBuff.write(bRecord.toAscii(), bRecord.length());
					break;
					default:
					break;
				}
			}
			
			igcDataBuff.close();
			flightList.push_back(flight);
		break;
		default:
			GPS_Error("GPS_Fmt_Print_Track: Unknown protocol");
		return;
	}
}

void GarminFmt::GPS_Fmt_Print_Waypoint(GPS_PWay *way, int32 n, WayPoint::WayPointListType &wpList)
{
	WayPoint wp;
	int i;
	
	for(i=0; i<n; i++)
	{
		setWayPoint(way[i], wp);
		wpList.push_back(wp);
	}
}

void GarminFmt::GPS_Fmt_Print_Route(GPS_PWay *way, int32 n, Route::RouteListType &routeList)
{
	Route route;
	WayPoint wp;
	QString name;
	int32 i;
	bool first = true;

	switch(gps_route_transfer)
	{
		case pA200: case pA201:
			for(i=0; i<n; i++)
			{
				if(way[i]->isrte)
				{
					if(first)
					{
						first = false;
					}
					else
					{
						routeList.push_back(route);
						route.wayPointList().clear();
					}
			
					switch(way[i]->rte_prot)
					{
						case 200:
							name.sprintf("Number: %d", way[i]->rte_num);
							route.setName(name);
						break;
						case 201:
							name.sprintf("Number: %d Comment: %-20.20s", way[i]->rte_num, way[i]->rte_cmnt);
							route.setName(name);
						break;
						case 202:
							name.sprintf("Comment: %s", way[i]->rte_ident);
							route.setName(name);
						break;
						default:
							GPS_Error("Print_Route: Unknown route protocol");
						break;
					}
				}
				else if(way[i]->islink)
				{
					continue; // don't care
				}
				else
				{
					wp.setName(way[i]->ident);
					route.wayPointList().push_back(wp);
				}
			}
			
			routeList.push_back(route);
		default:
			GPS_Error("GPS_Fmt_Print_Route: Unknown protocol");
		break;
	}
}

void GarminFmt::setBrecord(GPS_PTrack trk, QString &line)
{
	QDateTime dateTime;
	QString str;
	
	//	B122548 4708901N 00832137E A00977 00999 000
	
	dateTime.setTime_t(trk->Time);
	
	line = "B";
	line += dateTime.toString("hhmmss");
	
	// Latitude
	if(trk->lat >= 0)
	{
		str.sprintf("%7fN", trk->lat);
	}
	else
	{
		str.sprintf("%7fS", - trk->lat);
	}
	
	line += str;
	
	// Longitude
	if(trk->lon >= 0)
	{
		str.sprintf("%8fE", trk->lon);
	}
	else
	{
		str.sprintf("%8fW", - trk->lon);
	}
	
	line += str;

	// Altitude
	str.sprintf("%5i%5i000\n", 0, (int)roundf(trk->alt));
	line += str;
}

void GarminFmt::setWayPoint(GPS_PWay way, WayPoint &wp)
{
	wp.setName(way->ident);
	wp.setCoordinates(way->lat, way->lon, way->alt);
	wp.setDescription(way->cmnt);
}
