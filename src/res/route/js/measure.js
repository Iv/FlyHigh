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

function Measure(map)
{
	this.map = map;
	this.changeCallback = null;
	this.clickListener = null;
	this.moveListener = null;
	this.markers = [];
	this.enable = false;

	this.line = new google.maps.Polyline({
		strokeColor: '#5500ff',
		strokeOpacity: 1.0,
		strokeWeight: 1,
		map: null,
		zIndex: 3,
		clickable: false
	});
}

Measure.prototype.getMap = function()
{
	return this.map;
};

Measure.prototype.setChangeCallback = function(callback)
{
	this.changeCallback = callback;
};

Measure.prototype.show = function(enable)
{
	var nr;
	var measure = this;

	this.enable = enable;

	if(enable)
	{
		this.getMap().setOptions({draggableCursor: 'crosshair'});
		this.line.setMap(this.getMap());
		this.clickListener = google.maps.event.addListener(map, 'click', function(event){ms_click(measure, event);});
		this.moveListener = google.maps.event.addListener(map, 'mousemove', function(event){ms_mousemove(measure, event);});
	}
	else
	{
		this.getMap().setOptions({draggableCursor: 'default'});
		google.maps.event.removeListener(this.clickListener);
		google.maps.event.removeListener(this.moveListener);
		this.line.getPath().clear();
		this.line.setMap(null);
		this.clearMarkers();
	}
};

Measure.prototype.getEnable = function()
{
	return this.enable;
};

Measure.prototype.getDist = function()
{
	var dist = 0;

	path = this.line.getPath();

	if(path.getLength() > 1)
	{
		dist = google.maps.geometry.spherical.computeDistanceBetween(path.getAt(0), path.getAt(1));
		dist /= 1000.0;
	}

	return dist;
};

Measure.prototype.click = function(latlng)
{
	var path;
	var length;
	var marker;

	path = this.line.getPath();

	if(this.markers.length === 0)
	{
		// only on first click
		path.push(latlng);
		path.push(latlng);
	}
	else if(this.markers.length == 2)
	{
		path.setAt(0, latlng);
		path.setAt(1, latlng);
		this.clearMarkers();
	}

	marker = new MeasureMarker(this.getMap(), latlng);
	this.markers.push(marker);
	this.changeCallback();
};

Measure.prototype.mousemove = function(latlng)
{
	var path;
	var length;

	if(this.markers.length == 1)
	{
		path = this.line.getPath();
		length = path.getLength();
		path.setAt(length - 1, latlng);
		this.changeCallback();
	}
};

Measure.prototype.clearMarkers = function()
{
	var nr;
	var marker;

	for(nr=0; nr<this.markers.length; nr++)
	{
		marker = this.markers[nr];
		marker.setMap(null);
	}

	this.markers = [];
};

function ms_click(measure, event)
{
	measure.click(event.latLng);
}

function ms_mousemove(measure, event)
{
	measure.mousemove(event.latLng);
}

function MeasureMarker(map, latlng)
{
	var marker = this;

	this.stpos = null;
	this.marker = new google.maps.Marker({
		position: latlng,
		map: map,
		draggable: true,
		raiseOnDrag: false,
		icon: 'http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,5500ff,000000&ext=.png'
	});

	google.maps.event.addListener(this.marker, 'dragstart', function(event) {mm_dragstart(marker);});
	google.maps.event.addListener(this.marker, 'drag', function(event) {mm_drag(marker);});
}

MeasureMarker.prototype.setMap = function(map)
{
	this.marker.setMap(map);
};

MeasureMarker.prototype.setPosition = function(latlng)
{
	this.marker.setPosition(latlng);
};

MeasureMarker.prototype.getPosition = function()
{
	return this.marker.getPosition();
};

/*
	This is an ugly hack, to restore position while drag. Because Qt 4.6 won't display
	markers which are not draggable. In a later version, this should be fixed through
	setting markers draggable=this.editable.
*/
MeasureMarker.prototype.storePos = function()
{
	this.stpos = new google.maps.LatLng(this.getPosition().lat(), this.getPosition().lng());
};

MeasureMarker.prototype.restorePos = function()
{
	if(this.stpos !== null)
	{
		this.setPosition(this.stpos);
	}
};

function mm_dragstart(marker)
{
	marker.storePos();
}

function mm_drag(marker)
{
	marker.restorePos();
}
