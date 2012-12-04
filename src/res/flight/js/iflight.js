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
wm_include('../airspace/js/airspace.js');
wm_include('../route/js/fai.js');
wm_include('../route/js/infobox.js');
wm_include('../route/js/leg.js');
wm_include('../route/js/measure.js');
wm_include('../route/js/optimizer.js');
wm_include('../route/js/route.js');
wm_include('../route/js/turnpt.js');
wm_include('../lib/plot/context.js');
wm_include('../lib/plot/cursor.js');
wm_include('../lib/plot/legend.js');
wm_include('../lib/plot/plot.js');
wm_include('../lib/plot/value.js');

var airspaces = [];
var route = null;
var map = null;
var flight = null;
var plot = null;
var measure;
var oldSelect = -1;
var airspaceNr;

function fl_init()
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
			flight = new Flight(map);

			route = new Route(map);
			route.setChangeCallback(routeChanged);

			plot = new Plot(document.getElementById('plot'));
			plot.setValueLabels(["TIME", "ALT", "SOG", "VARIO"]);
			plot.setUpdateCursorPosCb(updateCursorPos);
			plot.setUpdateCursorClickCb(updateCursorClick);

			measure = new Measure(map);
			measure.setChangeCallback(measureChanged);
			wm_emitAppReady();
		}
	});

	google.maps.event.addListener(map, 'click', function(event)
	{
		var inside = false;
		
		if(!measure.getEnable())
		{
			// select next airspace
			airspaceNr = oldSelect;

			for(var i=0; i<airspaces.length; i++)
			{
				airspaceNr = (airspaceNr + 1) % airspaces.length;
				inside = airspaces[airspaceNr].isInside(event.latLng);

				if(inside)
				{
					if(airspaceNr != oldSelect)
					{
						break; // jump out of loop
					}
				}
			}
			
			if(inside)
			{
				as_selectAirSpaceNr(airspaceNr);
			}
			else
			{
				as_selectAirSpaceNr(-1);
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

	airspace = new AirSpace(map, latlngs, opts);
	airspaces.push(airspace);
}

function as_selectAirSpaceNr(num)
{
	var airspace;

	if(num < airspaces.length)
	{
		if(oldSelect >= 0)
		{
			airspaces[oldSelect].setSelect(false);
		}

		oldSelect = num;

		if(num >= 0)
		{
			airspace = airspaces[num];
			airspace.setSelect(true);
			wm_setDivValue("airspace", airspace.getName(), false);

			if(airspace.getLow() === 0)
			{
				wm_setDivValue("low", "GND", false);
			}
			else
			{
				wm_setDivValue("low", airspace.getLow() + " m", false);
			}

			wm_setDivValue("high", airspace.getHigh() + " m", false);
			wm_setDivValue("class", airspace.getClass(), false);
		}
		else
		{
			wm_setDivValue("airspace", "", false);
			wm_setDivValue("low", "", false);
			wm_setDivValue("high", "", false);
			wm_setDivValue("class", "", false);
		}
	}
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

	locInput = document.getElementById("sstart");
	locInput.innerHTML = name;
}

function fl_setPlotSize(width, height)
{
	var left;
	var top;

	if(plot !== null)
	{
		left = (window.innerWidth - width - 5);
		top = (window.innerHeight - height);
		plot.setGeometry(left, top, width, height);
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

function fl_setFlightLatLon(latlngs)
{
	var latlng;
	var posNr;
	var bounds;
	var path = [];

	bounds = new google.maps.LatLngBounds();

	for(posNr=0; posNr<latlngs.length; posNr++)
	{
		latlng = new google.maps.LatLng(latlngs[posNr][0], latlngs[posNr][1]);
		path.push(latlng);
		bounds.extend(latlng);
	}

	flight.setTrackPts(path);
	map.fitBounds(bounds);
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
	var index;
	var maxIndex;
	var data = [];

	maxIndex = flight.getTimeList().length;

	for(index=0; index<maxIndex; index++)
	{
		data.push({valueX: flight.getTimeAt(index), valueY: flight.getAltAt(index)});
	}

	plot.adjustMinMaxX(data);
	plot.setMinMaxY(0, flight.getMaxAlt());
	plot.plot(data);
	flight.moveGliderTo(0);
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

		if(speed > 1.0)
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

function fl_measure(div)
{
	var show;
	
	show = (div.className == "button_up");
	measure.show(show);

	if(show)
	{
		div.className = "button_down";
		div = document.getElementById("smeasure");
		div.innerHTML = "0.0 km";
	}
	else
	{
		div.className = "button_up";
		div = document.getElementById("smeasure");
		div.innerHTML = "Off";
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

function updateCursorPos(pos)
{
	var index;
	var date;
	var time;
	var alt;
	var sog;
	var vario;
	var timeList;
	var duration;

	// legend-values
	timeList = flight.getTimeList();
	duration = (timeList[timeList.length - 1] - timeList[0]);
	time = timeList[0] + Math.round(pos * duration / plot.getPlotAreaWidth());
	index = getIndex(timeList, time);
	date = new Date(flight.getTimeAt(index) * 1000);
	time = date.getHours() + ":";

	if(date.getMinutes() < 10)
	{
		time += '0';
	}

	time += date.getMinutes() + " UTC";
	alt = flight.getAltAt(index).toFixed(0) + " m";
	sog = flight.getSogAt(index).toFixed(1) + " km/h";
	vario = flight.getVarioAt(index).toFixed(1) + " m/s";
	plot.setValues([time, alt, sog, vario]);

	// glider
	index = Math.round(pos * flight.getTrackPts().length / plot.getPlotAreaWidth());
	flight.moveGliderTo(index);
}

function updateCursorClick(pos)
{
	var index;

	index = Math.round(pos * flight.getTrackPts().length / plot.getPlotAreaWidth());
	map.setCenter(flight.getTrackPtAt(index));
	map.setZoom(14);
}

function measureChanged()
{
	var div;

	div = document.getElementById("smeasure");
	div.innerHTML = measure.getDist().toFixed(1) + " km";
}

function getIndex(arr, value)
{
	var index;
	var deltaLow = 0;
	var deltaHigh;

	index = bSearch(arr, 0, (arr.length - 1), value);

	if(index > 0)
	{
		deltaLow = (value - arr[index - 1]);
	}

	deltaHigh = (arr[index] - value);

	if(((deltaLow > 0) && (deltaHigh > 0)) && (deltaLow < deltaHigh))
	{
		index--;
	}

	return index;
}

function bSearch(arr, low, high, key)
{
	var mid;

	if(low == high)
	{
		mid = low;
	}
	else
	{
		mid = Math.floor((low + high) / 2);

		if(key > arr[mid])
		{
			mid = bSearch(arr, mid + 1, high, key);
		}
		else if(key < arr[mid])
		{
			mid = bSearch(arr, low, mid, key);
		}
	}

	return mid;
}
