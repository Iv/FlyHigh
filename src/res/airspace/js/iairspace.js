/***************************************************************************
 *   Copyright (C) 2012 by Alex Graf                                       *
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

wm_include('js/airspace.js');

var map = null;
var airspaces = [];
var oldSelect = -1;
var airspaceNr;

function as_init()
{
	var mapLoaded = false;
	var mapOptions =
	{
		zoom: 9,
		center: new google.maps.LatLng(47.0, 8.5),
		mapTypeId: google.maps.MapTypeId.TERRAIN,
		disableDefaultUI: false,
		mapTypeControl: true,
		panControl: false,
		zoomControl: false,
		streetViewControl: false
	};

	map = new google.maps.Map(document.getElementById('map'), mapOptions);

	google.maps.event.addListener(map, 'idle', function()
	{
		if(!mapLoaded)
		{
			mapLoaded = true;
			wm_emitAppReady();
		}
	});

	google.maps.event.addListener(map, 'click', function(event)
	{
		// select next airspace
		airspaceNr = oldSelect;

		for(var i=0; i<airspaces.length; i++)
		{
			airspaceNr = (airspaceNr + 1) % airspaces.length;

			if(airspaces[airspaceNr].isInside(event.latLng))
			{
				if(airspaceNr != oldSelect)
				{
					as_selectAirSpace(airspaceNr);
					wm_emitLineChanged(airspaceNr);
					break; // jump out of loop
				}
			}
		}
	});
}

function as_pushAirSpace(coords, opts)
{
	var latlngs = [];
	var airspace;
	var nr;

	for(nr=0; nr<coords.length; nr++)
	{
		latlngs.push(new google.maps.LatLng(coords[nr][0], coords[nr][1]));
	}

	airspace = new AirSpace(map, {id: opts.id, path: latlngs});
	airspaces.push(airspace);
}

function as_selectAirSpace(num)
{
	var airspace;

	if(num < airspaces.length)
	{
		if(oldSelect >= 0)
		{
			airspaces[oldSelect].setSelect(false);
		}

		oldSelect = num;
		airspace = airspaces[num];
		airspace.setSelect(true);

/**
		as_setName(airspace.getName());
		as_setLow(airspace.getLow());
		as_setHigh(airspace.getHigh());
		as_setClass(airspace.getClass());
*/
	}
}

function as_setOk(ok)
{
	wm_emitOk(ok);
}

