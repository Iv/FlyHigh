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
 *   It is prohibited to serve or run this code over network p.e. as web   *
 *   service in combination with closed source.                            *
 ***************************************************************************/

wm_include('js/waypoint.js');
wm_include('../lib/markerclusterer.js');

var map = null;
var cluster = null;
var wayPts = [];
var curWayPt = null;
var WpEditable = true;
var nextModified = 0;

function wp_init()
{
	var mapLoaded = false;
	var wayPt;
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
			cluster = new MarkerClusterer(map);
/*
var opts;

opts = {id: 1, name: "wp 1", spot: "spot 1", country: "CH", lat: 47.0, lng: 8.5,
				alt: 0, editable: true, modified: false, selected: false};
wp_pushWayPoint(opts);

opts = {id: 2, name: "wp 2", spot: "spot 2", country: "CH", lat: 47.0, lng: 8.6,
				alt: 0, editable: true, modified: false, selected: false};
wp_pushWayPoint(opts);

wp_selectWayPoint(1);
*/
			wm_emitAppReady();
		}
	});
}

function wp_setEditable(editable)
{
	var div;
	var sdiv;
	var nr;

	WpEditable = editable;
	setDivEditable("name", editable);
	setDivEditable("spot", editable);
	setDivEditable("country", editable);
	setDivEditable("lat", editable);
	setDivEditable("lng", editable);
	setDivEditable("alt", editable);

	for(nr=0; nr<wayPts.length; nr++)
	{
		wayPts[nr].setEditable(editable);
	}
}

function wp_pushWayPoint(opts)
{
	var wayPt;

	wayPt = new WayPoint(map, opts);
	wayPt.setEditable(WpEditable);
	wayPt.setChangeCallback(wayPtChanged);
	wayPts.push(wayPt);
	cluster.addMarker(wayPt.getMarker());
}

function wp_selectWayPoint(id)
{
	var wpNr;
	var wayPt;

	for(wpNr=0; wpNr<wayPts.length; wpNr++)
	{
		wayPt = wayPts[wpNr];

		if(wayPt.getId() == id)
		{
			wayPt.setSelected(true);
			zoomToWayPt(wayPt);
			break;
		}
	}
}

function wp_setWayPtName(name)
{
	if((curWayPt != null) && WpEditable)
	{
		curWayPt.setName(name);
	}
}

function wp_setName(name)
{
	setDivValue("name", name);
}

function wp_getName()
{
	return getDivValue("name");
}

function wp_setWayPtSpot(spot)
{
	if((curWayPt != null) && WpEditable)
	{
		curWayPt.setSpot(spot);
	}
}

function wp_setSpot(spot)
{
	setDivValue("spot", spot);
}

function wp_getSpot()
{
	return getDivValue("spot");
}

function wp_setWayPtCountry(country)
{
	if((curWayPt != null) && WpEditable)
	{
		curWayPt.setCountry(country);
	}
}

function wp_setCountry(country)
{
	setDivValue("country", country);
}

function wp_getCountry()
{
	return getDivValue("country");
}

function wp_setWayPtLat(lat)
{
	var numValue;

	if(curWayPt != null)
	{
		numValue = parseFloat(lat);

		if(WpEditable && ((numValue >= -90) && (numValue <= 90)))
		{
			curWayPt.setLat(numValue);
			updateWayPtAlt(curWayPt);
			gotoWayPt(curWayPt);
		}
		else
		{
			wp_setLatLng(curWayPt.getPosition().lat(), curWayPt.getPosition().lng());
		}
	}
}

function wp_setWayPtLng(lng)
{
	var numValue;

	if(curWayPt != null)
	{
		numValue = parseFloat(lng);

		if(WpEditable && ((numValue >= -180) && (numValue <= 180)))
		{
			curWayPt.setLng(numValue);
			updateWayPtAlt(curWayPt);
			gotoWayPt(curWayPt);
		}
		else
		{
			wp_setLatLng(curWayPt.getPosition().lat(), curWayPt.getPosition().lng());
		}
	}
}

function wp_setLatLng(lat, lng)
{
	wp_setLat(lat);
	wp_setLng(lng);
}

function wp_setLat(lat)
{
	setDivValue("lat", lat.toFixed(5));
}

function wp_setLng(lng)
{
	setDivValue("lng", lng.toFixed(5));
}

function wp_getLatLng()
{
	alert("undefined");
/*
	var wp;

	wp = [markers[0].getLatLng().lat(), markers[0].getLatLng().lng()];

	return wp;
*/
}

function wp_altReply(id, param)
{
	var alt;
	var nr;

	if(param.status == "OK")
	{
		alt = Math.round(param.results[0].elevation);
		wp_setAlt(alt);

		for(nr=0; nr<wayPts.length; nr++)
		{
			if(wayPts[nr].id == id)
			{
				wayPts[nr].alt = alt;
				break;
			}
		}
	}
}

function wp_setWayPtAlt(alt)
{
	var numValue;

	if(curWayPt != null)
	{
		numValue = parseInt(alt);

		if(WpEditable && ((numValue >= -500) && (numValue <= 8000)))
		{
			curWayPt.setAlt(numValue);
		}
		else
		{
			wp_setAlt(curWayPt.getAlt());
		}
	}
}

function wp_setAlt(alt)
{
	setDivValue("alt", alt.toFixed(0));
}

function wp_getAlt()
{
	return getDivValue("alt");
}

function wp_getNextModified()
{
	var opts = null;

	for(nr=nextModified; nr<wayPts.length; nr++)
	{
		wayPt = wayPts[nr];

		if(wayPt.getModified())
		{
			nextModified = (nr + 1);
			opts = {id: wayPt.getId(), name: wayPt.getName(), spot: wayPt.getSpot(),
							country: wayPt.getCountry(), lat: wayPt.getPosition().lat(),
							lng: wayPt.getPosition().lng(), alt: wayPt.getAlt()};
			break;
		}
	}

	return opts;
}

function wp_setOk(ok)
{
	nextModified = 0;
	wm_emitOk(ok);
}

function selectWayPt(marker)
{
	if(curWayPt && (curWayPt != marker))
	{
		curWayPt.setSelect(false);
		updateWayPt(curWayPt);
	}

	marker.setSelect(true);
	wp_setName(marker.getName());
	wp_setSpot(marker.getSpot());
	wp_setCountry(marker.getCountry());
	wp_setLatLng(marker.getLat(), marker.getLon());
	wp_setAlt(marker.getAlt());
}

function wayPtChanged(event, wayPt)
{
	switch(event)
	{
		case WayPoint.CallbackType.Selected:
			if(curWayPt != wayPt)
			{
				if(curWayPt != null)
				{
					curWayPt.setSelected(false);
				}

				curWayPt = wayPt;
				wp_setName(wayPt.getName());
				wp_setSpot(wayPt.getSpot());
				wp_setCountry(wayPt.getCountry());
				wp_setLatLng(wayPt.getPosition().lat(), wayPt.getPosition().lng());
				wp_setAlt(wayPt.getAlt());
				wm_emitLineChanged(wayPt.getId());
			}
		break;
		case WayPoint.CallbackType.Position:
			wp_setLatLng(wayPt.getPosition().lat(), wayPt.getPosition().lng());
		break;
		case WayPoint.CallbackType.DragEnd:
			updateWayPtAlt(wayPt);
			cluster.redraw();
		break;
	}
}

function zoomToWayPt(wayPt)
{
	map.setCenter(wayPt.getPosition());
	map.setZoom(13);
}

function updateWayPtAlt(wayPt)
{
	var req;

	req = "http://maps.googleapis.com/maps/api/elevation/json?locations=";
	req += wayPt.getPosition().lat();
	req += ",";
	req += wayPt.getPosition().lng();
	req += "&sensor=false";
	wm_emitNetRequest(wayPt.getId(), req, "wp_altReply");
}

function setDivEditable(divId, editable)
{
	var div;
	var sdiv;

	div = document.getElementById(divId);
	sdiv = document.getElementById('s' + divId);

	if(editable)
	{
		div.style.display = "";
		sdiv.style.display = "none";
	}
	else
	{
		div.style.display = "none";
		sdiv.style.display = "";
	}
}

function setDivValue(divId, value)
{
	var div;

	if(WpEditable)
	{
		div = document.getElementById(divId);
		div.value = value;
	}
	else
	{
		div = document.getElementById('s' + divId);
		div.innerHTML = value;
	}
}

function getDivValue(divId)
{
	var div;
	var value;

	if(WpEditable)
	{
		div = document.getElementById(divId);
		value = div.value;
	}
	else
	{
		div = document.getElementById('s' + divId);
		value = div.innerHTML;
	}
	
	return value;
}
