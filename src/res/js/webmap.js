/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
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
var ChartMarginLeft = 29;
var ChartMarginRight = 34;
var LeftWidth;
var ChartWidth;
var PlotHeight;
var chart;

function setTurnPointList(tps)
{
	var tpInput;
	
	tpInput = document.getElementById("defaultTurnpoints");
	tpInput.value = tps;
}

function getTurnPointList()
{
	var tps = [];

	turnpointMarkers.each(function(marker, i) {
		tps[i] = [marker.getLatLng().lat(), marker.getLatLng().lng()] ;
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

init();
}

function getLocation()
{
	var locInput;
	
	locInput = document.getElementById("location");

	return locInput.value;
}

function setFlightType(flightType)
{
	var typeInput;
	
	typeInput = document.getElementById("defaultFlightType");
	typeInput.value = flightType;
}

function setMapSize(w, h)
{
	var div;

	LeftWidth = 280;
	PlotHeight = 140;
	ChartWidth = (w - LeftWidth);
	ChartHeight = (h - PlotHeight);

	// map
	div = document.getElementById("map");
	div.style.width = ChartWidth + "px";
	div.style.height = ChartHeight + "px";
	map.checkResize();

	// plot
	div = document.getElementById("plot");
	div.style.top = ChartHeight + "px";
	div.style.width = ChartWidth + "px";

	// chart
	div = document.getElementById("chart");
	div.style.width = ChartWidth + "px";

	// legend
	div = document.getElementById("legend");
	div.style.width = ChartWidth + "px";

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


function init()
{
	cursorDiv = document.getElementById("cursor").style;
	chartDiv = document.getElementById("chart");
	altDiv = document.getElementById("alt");
	sogDiv = document.getElementById("sog");
	varioDiv = document.getElementById("vario");
	timeDiv = document.getElementById("time");
/*
				latDiv = document.getElementById("lat");
				lonDiv = document.getElementById("lon");
*/

	drawChart();
	setLegend(0);
}

function drawChart()
{
	// trim off points before start / after end
	var j = 0;
	FlightData.points_num = 0;
	FlightData.max_alt = 0;
	FlightData.min_alt = 100000;

	for(i=0; i<flightArray.time.length; i++)
	{
		if(flightArray.time[i]>=StartTime && flightArray.time[i]<= StartTime + Duration)
		{
			FlightData.time[j]=sec2Time(flightArray.time[i]);
			FlightData.elev[j]=flightArray.elev[i];
			FlightData.elevGnd[j]=flightArray.elevGnd[i];
			FlightData.lat[j]=flightArray.lat[i];
			FlightData.lon[j]=flightArray.lon[i];
			FlightData.speed[j]=flightArray.speed[i];
			FlightData.vario[j]=flightArray.vario[i];
			FlightData.distance[j]=flightArray.distance[i];

			// for testing
			// FlightData.elevGnd[j]=800;
			if(j == 0)
			{
				lat = FlightData.lat[j] = flightArray.lat[i];
				lon = FlightData.lon[j] = flightArray.lon[i];
			}
			else
			{
				//P.Wild ground height hack  3.4.08
				if(FlightData.elevGnd[j] == 0)
						FlightData.elevGnd[j] = (FlightData.elevGnd[j-1] * 0.95 ) +
								(FlightData.elevGnd[j-1] * 0.1 * (Math.random()));
			}

			if(FlightData.elev[j] > FlightData.max_alt) FlightData.max_alt = FlightData.elev[j];
			if(FlightData.elevGnd[j] > FlightData.max_alt ) FlightData.max_alt = FlightData.elevGnd[j];
			if(FlightData.elev[j] < FlightData.min_alt) FlightData.min_alt = FlightData.elev[j];
			if(FlightData.elevGnd[j] < FlightData.min_alt) FlightData.min_alt = FlightData.elevGnd[j];
			FlightData.points_num++;
			j++;
		}
	}

	FlightData.label_num = flightArray.label_num;

	var nbPts = FlightData.time.length;
	var label_num = FlightData.label_num;
	var idx = 0;
	var step = Math.floor((nbPts - 1) / (label_num - 1));

	for(i = 0 ; i < label_num; i++)
	{
		FlightData.labels[i] = FlightData.time[idx].substr(0, 5);
		idx += step;
	}

	// add some spaces to the last legend
	FlightData.labels[FlightData.labels.length - 1] += "   ___";

	var min_alt = Math.floor((FlightData.min_alt / 100.0))  * 100;
	var max_alt = Math.ceil((FlightData.max_alt / 100.0)) * 100;
	var ver_label_num = 5;

	chart = new Chart(chartDiv);
	chart.setDefaultType(CHART_LINE);
	chart.setGridDensity(FlightData.label_num, ver_label_num);
	chart.setVerticalRange(min_alt, max_alt);
	chart.setShowLegend(false);
	chart.setLabelPrecision(0);
	chart.setHorizontalLabels(FlightData.labels);
	chart.add("Altitude", "#ff3333", FlightData.elev);
//				chart.add('Ground Elev',  '#C0AF9C', FlightData.elevGnd, CHART_AREA);
	chart.draw();

	addMouseEvents();
}

function sec2Time(secs)
{
	var hour;
	var min;
	var sec;

	sec = secs % 60;
	if(sec < 10) sec = "0" + sec;
	secs = Math.floor(secs / 60);
	min = secs % 60;
	if(min < 10) min = "0" + min;
	hour = Math.floor(secs / 60);
	if(hour < 10) hour = "0" + hour;

	return hour + ":" + min + ":" + sec;
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
		else if(posX > (ChartWidth + ChartMarginLeft - ChartMarginRight))
		{
			posX = (ChartWidth + ChartMarginLeft - ChartMarginRight);
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
/*
alert("setGlider=" + FlightData.speed[0]);
return;
//alert("setGlider=" + posX + ", length=" + FlightData.lat.length);
*/

	idx = (posX - ChartMarginLeft) * (FlightData.lat.length - 1) / (ChartWidth - ChartMarginRight);
	idx = Math.round(idx);

	setLegend(idx);

/*
		var newpos = new GLatLng(lat, lon);
		posMarker.setPoint(newpos);
*/
}

function setLegend(idx)
{
	altDiv.innerHTML = "ALT: " + FlightData.elev[idx] + " m";
	sogDiv.innerHTML = "SOG: " + FlightData.speed[idx] + " km/h";
	varioDiv.innerHTML = "Vario: " + FlightData.vario[idx] + " m/s";
	timeDiv.innerHTML = "Time: " + FlightData.time[idx];
}