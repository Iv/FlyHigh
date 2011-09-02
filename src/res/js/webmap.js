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

var FlightData={
	time: [], elev: [], elevGnd: [], lat: [], lon: [],
	speed: [], vario: [], distance: [] , labels: []};
var StartTime = 61516;
var Duration = 1856;

var chartDiv;
var cursorDiv;
var altDiv;
var sogDiv;
var varioDiv;
var timeDiv;
/*
			var latDiv;
			var lonDiv;
*/
var ChartMarginLeft = 20;
var ChartMarginRight = 38;
var LeftWidth = 280;
var LegendHeight = 20;
var mapHeight;
var plotWidth;
var chart;
var glider;

function setTurnPointList(tps)
{
	var tpInput;

	tpInput = document.getElementById("defaultTurnpoints");
	tpInput.value = tps;
}

function getTurnPointList()
{
	var tps = [];

	turnpointMarkers.each(function(tp, i) {
		tps[i] = [tp.getLatLng().lat(), tp.getLatLng().lng()] ;
	});

	return tps;
}

function setWayPointList(encTrack, encLevels, weight, color)
{
	var polyline;

	polyline = new GPolyline.fromEncoded({
			color: color,
			weight: weight,
			opacity: 1.0,
			points: encTrack,
			levels: encLevels,
			zoomFactor: 2,
			numLevels: 18
	});

	map.addOverlay(polyline);
}

function setTurnPointsDragable(drag)
{
	TurnPointsDragable = drag;
}

function setLocation(loc)
{
	var locInput;

	locInput = document.getElementById("defaultLocation");
	locInput.value = loc;
}

function getLocation()
{
	var locInput;

	locInput = document.getElementById("location");

	return locInput.value;
}

function setName(name)
{
	var nameInput;

	nameInput = document.getElementById("name");
	nameInput.value = name;
}

function getName()
{
	var nameInput;

	nameInput = document.getElementById("name");

	return nameInput.value;
}

function setFlightType(flightType)
{
	var typeInput;

	typeInput = document.getElementById("defaultFlightType");
	typeInput.value = flightType;
}

function getFlightType()
{
	var typeInput;

	typeInput = document.getElementById("flightType");

	return typeInput.value;
}

function setMapPos(top, left)
{
	var div;

	div = document.getElementById("map");
	div.style.top = top + "px";
	div.style.left = left + "px";
	map.checkResize();
}

function setMapSize(width, height)
{
	var div;

	mapHeight = height;
	div = document.getElementById("map");
	div.style.width = width + "px";
	div.style.height = height + "px";
	map.checkResize();
}

function setPlotSize(width, height)
{
	var div;
	var plotHeight;

	plotWidth = width;
	plotHeight = height;

	// plot
	div = document.getElementById("plot");
	div.style.top = mapHeight + "px";
	div.style.width = plotWidth + "px";

	// chart
	div = document.getElementById("chart");
	div.style.width = plotWidth + "px";
	div.style.height = (height - LegendHeight) + "px";

	// legend
	div = document.getElementById("legend");
	div.style.width = plotWidth + "px";
	div.style.height = LegendHeight + "px";

	// redraw chart
	chart.draw();
}

function zoomIn()
{
	map.zoomIn();
}

function zoomOut()
{
	map.zoomOut();
}

function setFlightTime(time, start, duration)
{
	FlightData.time = time;
	StartTime = start;
	Duration = duration;
}

function setFlightAlt(alt, minAlt, maxAlt)
{
	FlightData.elev = alt;
	FlightData.min_alt = minAlt;
	FlightData.max_alt = maxAlt;

	if(maxAlt > 999)
	{
		ChartMarginLeft = 34;
	}
	else if(maxAlt > 99)
	{
		ChartMarginLeft = 27;
	}
	else if(maxAlt > 9)
	{
		ChartMarginLeft = 20;
	}
}

function setFlightLatLon(lat, lon)
{
	FlightData.lat = lat;
	FlightData.lon = lon;
}

function setSog(sog)
{
	FlightData.speed = sog;
}

function setVario(vario)
{
	FlightData.vario = vario;
}

function showPlot()
{
	var nbPts;
	var horLabelNum = 8;
	var verLabelNum = 5;
	var idx = 0;
	var roundIdx;
	var step;
	var labelNr;

	nbPts = FlightData.time.length;
	step = (nbPts - 1) / (horLabelNum - 1);

	// divs
	cursorDiv = document.getElementById("cursor").style;
	chartDiv = document.getElementById("chart");
	altDiv = document.getElementById("alt");
	sogDiv = document.getElementById("sog");
	varioDiv = document.getElementById("vario");
	timeDiv = document.getElementById("time");

	for(labelNr=0; labelNr<horLabelNum; labelNr++)
	{
		roundIdx = Math.round(idx);
		FlightData.labels[labelNr] = FlightData.time[roundIdx].substr(0, 5);
		idx += step;
	}

	// add some spaces to the last legend
	FlightData.labels[FlightData.labels.length - 1] += "   ___";

	// chart
	chart = new Chart(chartDiv);
	chart.setDefaultType(CHART_LINE);
	chart.setGridDensity(horLabelNum, verLabelNum);
	chart.setVerticalRange(FlightData.min_alt, FlightData.max_alt);
	chart.setShowLegend(false);
	chart.setLabelPrecision(0);
	chart.setHorizontalLabels(FlightData.labels);
	chart.add("Altitude", "#ff3333", FlightData.elev);
//				chart.add('Ground Elev',  '#C0AF9C', FlightData.elevGnd, CHART_AREA);
	chart.draw();

	// glider
	var icon = MapIconMaker.createLabeledMarkerIcon({width: 32, height: 32, label: "G", primaryColor: "#ffffff"});
	glider = new GMarker(new GLatLng(FlightData.lat[0], FlightData.lon[0]), {draggable: false, icon: icon});
	glider.rev = 0;
	glider.ele = -9999;
	map.addOverlay(glider);

	addMouseEvents();
	setLegend(0);

	// the simplest way to put route over track
	XCUpdateRoute();
}

function addMouseEvents()
{
	chartDiv.addEventListener('mousemove', function(e)
	{
		var posX = (e.pageX - LeftWidth);

		if(posX < ChartMarginLeft)
		{
			posX = ChartMarginLeft;
		}
		else if(posX > (plotWidth + ChartMarginLeft - ChartMarginRight))
		{
			posX = (plotWidth + ChartMarginLeft - ChartMarginRight);
		}

		setTimeLine(posX);
		setGlider(posX);
	}, true);
}

function setTimeLine(posX)
{
	cursorDiv.left = posX + "px";
}

function setGlider(posX)
{
	var lat;
	var lon;
	var idx;

	idx = (posX - ChartMarginLeft) * (FlightData.time.length - 1) / (plotWidth - ChartMarginRight);
	idx = Math.round(idx);
	glider.setLatLng(new GLatLng(FlightData.lat[idx], FlightData.lon[idx]));
	setLegend(idx);
}

function setLegend(idx)
{
	altDiv.innerHTML = "ALT: " + FlightData.elev[idx] + " m";
	sogDiv.innerHTML = "SOG: " + FlightData.speed[idx] + " km/h";
	varioDiv.innerHTML = "Vario: " + FlightData.vario[idx] + " m/s";
	timeDiv.innerHTML = "Time: " + FlightData.time[idx];
}

function validateNum(event)
{
	var theEvent = event || window.event;
	var key = theEvent.keyCode || theEvent.which;
	var regex = /[0-9]|\./;

	key = String.fromCharCode(key);

	if(!regex.test(key))
	{
		theEvent.returnValue = false;

		if(theEvent.preventDefault)
		{
			theEvent.preventDefault();
		}
	}
}

// Extended GMarker
Marker.prototype = new GMarker(new GLatLng(0, 0));
 
function Marker(point, opts)
{
	this.id = opts.id;
	this.alt = opts.alt;
	GMarker.call(this, point, opts);
}
