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
 *                                                                         *
 *   It is prohibited to server or run this code over network p.e. as web  *
 *   service in combination with closed source.                            *
 ***************************************************************************/

var FlightType = ["undef", "xc2", "xc3", "xc4", "xc5", "xc3c"];
var routeId = -1;
var routeName = "";

function rt_init()
{
	XCLoad();
}

function rt_setLocation(loc)
{
	var locInput;

	locInput = document.getElementById("defaultLocation");
	locInput.value = loc;
}

function rt_getLocation()
{
	var locInput;

	locInput = document.getElementById("location");

	return locInput.value;
}

function rt_setRouteName(name)
{
	if(RouteEditable)
	{
		routeName = name;
	}
	else
	{
		rt_setName(routeName);
	}
}

function rt_setName(name)
{
	var nameInput;

	routeName = name;
	nameInput = document.getElementById("name");
	nameInput.value = name;
}

function rt_getName()
{
	var nameInput;

	nameInput = document.getElementById("name");

	return nameInput.value;
}

function rt_setTurnPointList(tps)
{
	var tpInput;

	tpInput = document.getElementById("defaultTurnpoints");
	tpInput.value = tps;
}

function rt_getTurnPointList()
{
	var tps = [];

	turnpointMarkers.each(function(tp, i) {
		tps[i] = [tp.getLatLng().lat(), tp.getLatLng().lng()] ;
	});

	return tps;
} 

function rt_setEditable(en)
{
	RouteEditable = en;
}

function rt_setFlightType(type)
{
	var typeInput;

	typeInput = document.getElementById("defaultFlightType");
	typeInput.value = FlightType[type];
}

function rt_getFlightType()
{
	var typeInput;
	var typeNr = 0;

	typeInput = document.getElementById("flightType");

	for(typeNr=0; typeNr<FlightType.length; typeNr++)
	{
		if(FlightType[typeNr] == typeInput.value)
		{
			break;
		}
	}

	return typeNr;
}

function rt_setOk(ok)
{
	var wpName;
	var type;

	if(ok && RouteEditable)
	{
		type = rt_getFlightType();
		WebMapRoute.beginSaveRoute();
		WebMapRoute.saveRoute(routeId, routeName, type);

		turnpointMarkers.each(function(marker, i)
		{
			wpName = routeName + "_" + i;
			WebMapRoute.saveWayPoint(wpName, marker.getLatLng().lat(), marker.getLatLng().lng(), marker.ele);
		});
		
		WebMapRoute.endSaveRoute();
	}

	WebMap.setOk(ok);
}
