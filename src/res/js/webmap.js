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

// Extended GMarker
Marker.prototype = new GMarker(new GLatLng(0, 0));
 
function Marker(point, opts)
{
	this.id = opts.id;
	this.name = opts.name;
	this.spot = opts.spot;
	this.country = opts.country;
	this.alt = opts.alt;
	this.modified = false;
	this.select = false;
	opts.icon = new GIcon(G_DEFAULT_ICON);
	opts.icon.image = "http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,00EE00,000000&ext=.png";
	GMarker.call(this, point, opts);
}

Marker.prototype.getId = function()
{
	return this.id;
}

Marker.prototype.setName = function(name)
{
	this.name = name;
	this.modified = true;
	this.update();
}

Marker.prototype.getName = function()
{
	return this.name;
}

Marker.prototype.setSpot = function(spot)
{
	this.spot = spot;
	this.modified = true;
	this.update();
}

Marker.prototype.getSpot = function()
{
	return this.spot;
}

Marker.prototype.setCountry = function(country)
{
	this.country = country;
	this.modified = true;
	this.update();
}

Marker.prototype.getCountry = function()
{
	return this.country;
}

Marker.prototype.setLat = function(lat)
{
	var latlng;

	latlng = new GLatLng(lat, this.getLatLng().lng());
	this.setLatLng(latlng);
	this.modified = true;
	this.update();
}

Marker.prototype.getLat = function()
{
	return this.getLatLng().lat();
}

Marker.prototype.setLon = function(lon)
{
	var latlng;

	latlng = new GLatLng(this.getLatLng().lat(), lon);
	this.setLatLng(latlng);
	this.modified = true;
	this.update();
}

Marker.prototype.getLon = function()
{
	return this.getLatLng().lng();
}

Marker.prototype.setAlt = function(alt)
{
	this.alt = alt;
	this.modified = true;
	this.update();
}

Marker.prototype.getAlt = function()
{
	return this.alt;
}

Marker.prototype.setSelect = function(select)
{
	this.select = select;
	this.update();
}

Marker.prototype.getModified = function()
{
	return this.modified;
}

Marker.prototype.update = function()
{
	if(this.select)
	{
		if(this.modified)
		{
			this.setImage("http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=EE0000,FFFF00,000000&ext=.png");
		}
		else
		{
			this.setImage("http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=00EE00,FFFF00,000000&ext=.png");
		}
	}
	else
	{
		if(this.modified)
		{
			this.setImage("http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,EE0000,000000&ext=.png");
		}
		else
		{
			this.setImage("http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,00EE00,000000&ext=.png");
		}
	}
}
