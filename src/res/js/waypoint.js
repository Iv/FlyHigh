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
var markerCluster;

function wp_init()
{
	if(GBrowserIsCompatible()) {
		map = new GMap2(document.getElementById('map'));
		map.setCenter(new GLatLng(47, 8.5), 8);
		map.addControl(new GMapTypeControl());
		map.removeMapType(G_SATELLITE_MAP);
		map.addMapType(G_PHYSICAL_MAP);
		map.setMapType(G_PHYSICAL_MAP);
		map.enableScrollWheelZoom();
		map.disableDoubleClickZoom();

/**
		GEvent.addListener(map, "zoomend", function(oldLevel, newLevel)
		{ 
			if(curMarker)
			{
				selectMarker(curMarker);
			}
		}); 
*/

		// markers are set before
		markerCluster = new MarkerClusterer(map, markers);
	}
}

function wp_pushWayPoint(opts)
{
	var latlng = new GLatLng(opts.lat, opts.lon);
	var marker;

	marker = new Marker(latlng, {id: opts.id, name: opts.name, spot: opts.spot,
											country: opts.country, alt: opts.alt, draggable: true});

	GEvent.addListener(marker, "mousedown", function(latlng)
	{ 
		selectMarker(this);
		WebMap.setLine(this.getId());
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
		updateMarkerAlt(this);
		markerCluster.updateClusters(this);
	});

	markers.push(marker);
}

function wp_selectWayPoint(id)
{
	var wpNr;
	var marker;

	for(wpNr=0; wpNr<markers.length; wpNr++)
	{
		marker = markers[wpNr];

		if(marker.getId() == id)
		{
			zoomMarker(marker);
			selectMarker(marker);
			break;
		}
	}
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
	var numValue;

	if(curMarker)
	{
		numValue = parseFloat(lat);

		if((numValue >= -90) && (numValue <= 90))
		{
			curMarker.setLat(numValue);
			updateMarkerAlt(curMarker);
			markerCluster.updateClusters(curMarker);
			zoomMarker(curMarker);
		}
		else
		{
			wp_setLatLng(curMarker.getLat(), curMarker.getLon());
		}
	}
}

function wp_setMarkerLon(lon)
{
	var numValue;

	if(curMarker)
	{
		numValue = parseFloat(lon);

		if((numValue >= -180) && (numValue <= 180))
		{
			curMarker.setLon(numValue);
			updateMarkerAlt(curMarker);
			markerCluster.updateClusters(curMarker);
			zoomMarker(curMarker);
		}
		else
		{
			wp_setLatLng(curMarker.getLat(), curMarker.getLon());
		}
	}
}

function wp_setLatLng(lat, lon)
{
	var locInput;
	
	lat = Math.round(lat * 100000) / 100000;
	locInput = document.getElementById("lat");
	locInput.value = lat;

	lon = Math.round(lon * 100000) / 100000;
	locInput = document.getElementById("lon");
	locInput.value = lon;
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
	var numValue;

	if(curMarker)
	{
		numValue = parseInt(alt);

		if((numValue >= -500) && (numValue <= 8000))
		{
			curMarker.setAlt(numValue);
		}
		else
		{
			wp_setAlt(curMarker.getAlt());
		}
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

function wp_setOk(ok)
{
	var marker;

	if(ok)
	{
		for(nr=0; nr<markers.length; nr++)
		{
			marker = markers[nr];

			if(marker.getModified())
			{
				WebMapWayPoint.saveWayPoint(marker.getId(),
					marker.getName(), marker.getSpot(), marker.getCountry(),
					marker.getLat(), marker.getLon(), marker.getAlt());
			}
		}
	}

	WebMap.setOk(ok);
}

function selectMarker(marker)
{
	if(curMarker)
	{
		curMarker.setSelect(false);
	}

	marker.setSelect(true);
	wp_setName(marker.getName());
	wp_setSpot(marker.getSpot());
	wp_setCountry(marker.getCountry());
	wp_setLatLng(marker.getLat(), marker.getLon());
	wp_setAlt(marker.getAlt());
	curMarker = marker;
}

function zoomMarker(marker)
{
	var bounds;

	bounds = new GLatLngBounds(marker.getLatLng(), marker.getLatLng());
	map.setCenter(bounds.getCenter(), 15);
}

function updateMarkerAlt(marker)
{
	var req;

	req = "http://maps.googleapis.com/maps/api/elevation/json?locations=";
	req += marker.getLat();
	req += ",";
	req += marker.getLon();
	req += "&sensor=false";

	WebMap.netRequest(marker.getId(), req, "wp_altReply");
}
