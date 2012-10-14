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

wm_include('js/fai.js');
wm_include('js/leg.js');
wm_include('js/optimizer.js');
wm_include('js/route.js');
wm_include('js/turnpt.js');

var map;
var route;
var m_speed = 20.0;
var m_duration;

function rt_init(width, height)
{
	var turnPt;
	var nextTurnPt;
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
			wm_setMapSize(width, height);

			route = new Route(map);
			route.setChangeCallback(rt_change);

			wm_emitAppReady();
		}
	});
}

function rt_setName(name)
{
	locInput = document.getElementById("name");
	locInput.value = name;
	// route.setName(name);
}

function rt_getName()
{
	locInput = document.getElementById("name");

	return locInput.value;
}

function rt_setTurnPts(turnPts)
{
	var turnPt;
	var tpNr;
	var bounds;
	var latlng;

	bounds = new google.maps.LatLngBounds();
	
	for(tpNr=0; tpNr<turnPts.length; tpNr++)
	{
		latlng = new google.maps.LatLng(turnPts[tpNr][0], turnPts[tpNr][1]);
		bounds.extend(latlng);
		turnPt = new TurnPt(route, latlng, TurnPt.Type.WayPoint);
		route.addTurnPt(turnPt);
	}

	updateSpeed();
	calcDuration();
	map.fitBounds(bounds);
}

function rt_getTurnPts()
{
	var turnPts;
	var turnPt;
	var turnPtArray = [];
	var tpNr;

	turnPts = route.getTurnPts();

	for(tpNr=0; tpNr<turnPts.length; tpNr++)
	{
		turnPt = turnPts[tpNr];
		turnPtArray.push(new Array(turnPt.getPosition().lat(), turnPt.getPosition().lng(), turnPt.getAltitude()));
	}

	return turnPtArray;
}

function rt_getType()
{
	return route.getType();
}

function rt_getDist()
{
	return route.getDist();
}

function rt_setEditable(en)
{
	route.setEditable(en);
}

function rt_change()
{
	var RouteType = ["Undefined", "Free distance", "Straight distance", "Flat triangle", "FAI triangle"];
	var locInput;

	locInput = document.getElementById("type");
	locInput.innerHTML = RouteType[route.getType()];

	locInput = document.getElementById("track");
	locInput.innerHTML = route.getTrackDist().toFixed(0) + " km";

	locInput = document.getElementById("distance");
	locInput.innerHTML = route.getDist().toFixed(2) + " km";

	locInput = document.getElementById("score");
	locInput.innerHTML = route.getScore().toFixed(2) + " points";

	calcDuration();
}

function rt_speedUp()
{
	if(m_speed < 50.0)
	{
		m_speed += 0.5;
		updateSpeed();
		calcDuration();
	}
}

function rt_speedDown()
{
	if(m_speed > 0.0)
	{
		m_speed -= 0.5;
		updateSpeed();
		calcDuration();
	}
}

function rt_durationUp()
{
	if(m_duration < 24)
	{
		m_duration += 0.5;
		updateDuration();
		calcSpeed();
	}
}

function rt_durationDown()
{
	if(m_duration > 1)
	{
		m_duration -= 0.5;
		updateDuration();
		calcSpeed();
	}
}

function rt_setOk(ok)
{
	wm_emitOk(ok);
}

function calcSpeed()
{
	m_speed = Math.round((route.getTrackDist() / m_duration) * 2) / 2
	updateSpeed();
}

function updateSpeed()
{
	locInput = document.getElementById("speed");
	locInput.value = m_speed.toFixed(1) + " km/h";
}

function calcDuration()
{
	m_duration = Math.round((route.getTrackDist() / m_speed) * 2) / 2; // * 10 / 5
	updateDuration();
}

function updateDuration()
{
	locInput = document.getElementById("duration");
	locInput.value = m_duration.toFixed(1) + " h";
}
