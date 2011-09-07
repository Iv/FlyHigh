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

var geocoder = null;
var map = null;
var markers = [];
var curMarker = null;

function wp_load()
{
	if(GBrowserIsCompatible()) {
		map = new GMap2(document.getElementById('map'));
		map.setCenter(new GLatLng(47, 8.5), 8);
		map.addControl(new GMapTypeControl())
		map.removeMapType(G_SATELLITE_MAP);
		map.addMapType(G_PHYSICAL_MAP);
		map.setMapType(G_PHYSICAL_MAP);
		map.enableScrollWheelZoom();

		// markers are set before
		var markerCluster = new MarkerClusterer(map, markers);
	}
}

function wp_unload()
{
	geocoder = null;
	map = null;
	GUnload();
}

function wp_pushWayPoint(opts)
{
	var latlng = new GLatLng(opts.lat, opts.lon);
	var marker;

	marker = new Marker(latlng, {id: opts.id, title: opts.name, spot: opts.spot,
											country: opts.country, alt: opts.alt, draggable: true});

	GEvent.addListener(marker, "mousedown", function(latlng)
	{ 
		wp_setName(this.getTitle());
		wp_setSpot(this.spot);
		wp_setCountry(this.country);
		wp_setLatLng(latlng.lat(), latlng.lng());
		wp_setAlt(this.alt);
		
		if(curMarker)
		{
			curMarker.setSelect(false);
		}

		this.setSelect(true);
		curMarker = this;
	}); 

	GEvent.addListener(marker, "dragstart", function(latlng)
	{
		this.modified = true;
		this.setSelect(true);
	});

	GEvent.addListener(marker, "drag", function(latlng)
	{
		wp_setLatLng(latlng.lat(), latlng.lng());
	});
	
	GEvent.addListener(marker, "dragend", function(latlng)
	{
		var req;

		req = "http://maps.googleapis.com/maps/api/elevation/json?locations="
		req += latlng.lat();
		req += ",";
		req += latlng.lng();
		req += "&sensor=false";

		WebMap.netRequest(this.id, req, "wp_altReply");
	});

	markers.push(marker);
}

function wp_setMarkerName(name)
{
	if(curMarker)
	{
		curMarker.setName(name);
	}
}

function wp_setName(name)
{
	var locInput;

	locInput = document.getElementById("name");

	locInput.value = name;
}

function wp_getName()
{
	var locInput;

	locInput = document.getElementById("name");

	return locInput.value;
}

function wp_setMarkerSpot(spot)
{
	if(curMarker)
	{
		curMarker.setSpot(spot);
	}
}

function wp_setSpot(spot)
{
	var locInput;

	locInput = document.getElementById("spot");

	locInput.value = spot;
}

function wp_getSpot()
{
	var locInput;

	locInput = document.getElementById("spot");

	return locInput.value;
}

function wp_setMarkerCountry(country)
{
	if(curMarker)
	{
		curMarker.setCountry(country);
	}
}

function wp_setCountry(country)
{
	var locInput;

	locInput = document.getElementById("country");

	locInput.value = country;
}

function wp_getCountry()
{
	var locInput;

	locInput = document.getElementById("country");

	return locInput.value;
}

function wp_setMarkerLat(lat)
{
	if(curMarker)
	{
		curMarker.setLat(lat);
	}
}

function wp_setMarkerLon(lon)
{
	if(curMarker)
	{
		curMarker.setLon(lon);
	}
}

function wp_setLatLng(lat, lng)
{
	var locInput;

	locInput = document.getElementById("lat");
	locInput.value = lat;
	
	locInput = document.getElementById("lon");
	locInput.value = lng;
}

function wp_getLatLng()
{
	var wp;

	wp = [markers[0].getLatLng().lat(), markers[0].getLatLng().lng()];

	return wp;
}

function wp_altReply(id, param)
{
	var alt;
	var nr;

	if(param.status == "OK")
	{
		alt = Math.round(param.results[0].elevation);
		wp_setAlt(alt);
		
		for(nr=0; nr<markers.length; nr++)
		{
			if(markers[nr].id == id)
			{
				markers[nr].alt = alt;
				break;
			}
		}
	}
}

function wp_setMarkerAlt(alt)
{
	if(curMarker)
	{
		curMarker.setAlt(alt);
	}
}

function wp_setAlt(alt)
{
	var locInput;

	locInput = document.getElementById("alt");

	locInput.value = alt;
}

function wp_getAlt()
{
	var locInput;

	locInput = document.getElementById("alt");

	return locInput.value;
}
