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

wm_include('js/flight.js');
wm_include('js/plot.js');
wm_include('../route/js/fai.js');
wm_include('../route/js/leg.js');
wm_include('../route/js/optimizer.js');
wm_include('../route/js/route.js');
wm_include('../route/js/turnpt.js');
wm_include('../route/js/infobox.js');
wm_include('../lib/chartFX/chart.js');
wm_include('../lib/chartFX/excanvas.js');
wm_include('../lib/chartFX/canvaschartpainter.js');

var route = null;
var map = null;
var flight = null;
var plot = null;

function fl_init(width, height)
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
			wm_setMapSize(width, height);

			flight = new Flight(map);

			route = new Route(map);
			route.setChangeCallback(routeChanged);

			plot = new Plot(map);
			plot.setFlight(flight);

			wm_emitAppReady();
		}
	});
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

	map.fitBounds(bounds);
}

function rt_setName(name)
{
	var locInput;

	locInput = document.getElementById("sname");
	locInput.innerHTML = name;
}

function fl_setTrack(encTrack)
{
	var path;

	path = google.maps.geometry.encoding.decodePath(encTrack);
	flight.setTrackPts(path);
}

function fl_setPlotSize(width, height)
{
	if(plot != null)
	{
		plot.setSize(width, height);
	}
}

function fl_setFlightTime(timeList, start, duration)
{
	flight.setTimeList(timeList);
	route.setDuration(duration / 3600);
	updateDurationAndSpeed();
}

function fl_setFlightAlt(altList, minAlt, maxAlt)
{
	flight.setAltList(altList, minAlt, maxAlt);
}

function fl_setSog(sogList)
{
	flight.setSogList(sogList);
}

function fl_setVario(varioList)
{
	flight.setVarioList(varioList);
}

function fl_showPlot()
{
	plot.show();
}

function fl_setOk(ok)
{
	wm_emitOk(ok);
}

function fl_speedUp()
{
	var speed;

	speed = route.getSpeed();

	if(speed < 50.0)
	{
		route.setSpeed(speed + 0.5);
		updateDurationAndSpeed();
	}
}

function fl_speedDown()
{
	var speed;

	if(route.getEditable())
	{
		speed = route.getSpeed();

		if(speed > 0.0)
		{
			route.setSpeed(speed - 0.5);
			updateDurationAndSpeed();
		}
	}
}

function fl_durationUp()
{
	var duration;

	if(route.getEditable())
	{
		duration = route.getDuration();

		if(duration < 24)
		{
			route.setDuration(duration + 0.5);
			updateDurationAndSpeed();
		}
	}
}

function fl_durationDown()
{
	var duration;

	if(route.getEditable())
	{
		duration = route.getDuration();

		if(duration > 1)
		{
			route.setDuration(duration - 0.5);
			updateDurationAndSpeed();
		}
	}
}

function routeChanged()
{
	var div;

	div = document.getElementById("type");
	div.innerHTML = Route.ScoreTypeText[route.getType()];
	div = document.getElementById("distance");
	div.innerHTML = route.getDist().toFixed(2) + " km";
	div = document.getElementById("score");
	div.innerHTML = route.getScore().toFixed(2) + " points";
	div = document.getElementById("track");
	div.innerHTML = route.getTrackDist().toFixed(0) + " km";
	updateDurationAndSpeed();
}

function updateDurationAndSpeed()
{
	var div;

	div = document.getElementById("speed");
	div.value = route.getSpeed().toFixed(1) + " km/h";
	div = document.getElementById("duration");
	div.value = route.getDuration().toFixed(1) + " h";
}
