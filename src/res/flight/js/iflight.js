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

var FlightData={
	time: [], elev: [], elevGnd: [], lat: [], lon: [],
	speed: [], vario: [], distance: [] , labels: []};
/* unused
	var StartTime = 61516;
	var Duration = 1856;
*/
var chartDiv;
var cursorDiv;
var altDiv;
var sogDiv;
var varioDiv;
var timeDiv;
/*
var ChartMarginLeft = 20;
var ChartMarginRight = 38;
var LeftWidth = 280;
var LegendHeight = 20;
*/
var mapHeight;
var plotWidth;
var chart;
var glider;

var route = null;
var map = null;
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

			route = new Route(map);
			route.setSpeed(22.0);

			plot = new Plot(map);

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

//	updateDurationAndSpeed();
	map.fitBounds(bounds);
}

function fl_setTrack(encTrack, encLevels, weight, color)
{
	var line;
	var path;

	line = new google.maps.Polyline({
		strokeColor: color,
		strokeOpacity: 1.0,
		strokeWeight: weight,
		map: null,
		zIndex: 1,
		map: map
	});

	path = google.maps.geometry.encoding.decodePath(encTrack);
	line.setPath(path);
}

function fl_setPlotSize(width, height)
{
	if(plot != null)
	{
		plot.setSize(width, height);
	}

/*
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
*/
}

function fl_setFlightTime(timeList, start, duration)
{
	plot.setTimeList(timeList);
/*
	FlightData.time = time;
*/
/** unused
	StartTime = start;
	Duration = duration;
*/
}

function fl_setFlightAlt(alt, minAlt, maxAlt)
{
	plot.setAltList(alt);
	plot.setMinMaxAlt(minAlt, maxAlt);

/*
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
*/
}

function fl_setFlightLatLon(lat, lon)
{
	FlightData.lat = lat;
	FlightData.lon = lon;
}

function fl_setSog(sog)
{
	FlightData.speed = sog;
}

function fl_setVario(vario)
{
	FlightData.vario = vario;
}

function fl_showPlot()
{
	plot.show();

	/*
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
*/
	// glider
/*
	var icon = MapIconMaker.createLabeledMarkerIcon({width: 32, height: 32, label: "G", primaryColor: "#ffffff"});
	glider = new GMarker(new GLatLng(FlightData.lat[0], FlightData.lon[0]), {draggable: false, icon: icon});
	glider.rev = 0;
	glider.ele = -9999;
	map.addOverlay(glider);
*/
/*
	addMouseEvents();
	setLegend(0);
*/
}

function fl_setOk(ok)
{
	wm_emitOk(ok);
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
///	glider.setLatLng(new GLatLng(FlightData.lat[idx], FlightData.lon[idx]));
	setLegend(idx);
}

function setLegend(idx)
{
	altDiv.innerHTML = "ALT: " + FlightData.elev[idx] + " m";
	sogDiv.innerHTML = "SOG: " + FlightData.speed[idx] + " km/h";
	varioDiv.innerHTML = "Vario: " + FlightData.vario[idx] + " m/s";
	timeDiv.innerHTML = "Time: " + FlightData.time[idx];
}
