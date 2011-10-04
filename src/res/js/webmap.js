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
 *   It is prohibited to serve or run this code over network p.e. as web   *
 *   service in combination with closed source.                            *
 ***************************************************************************/

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

function zoomIn()
{
	map.zoomIn();
}

function zoomOut()
{
	map.zoomOut();
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
	this.modified = opts.modified;
	this.select = opts.select;
	opts.icon = new GIcon(G_DEFAULT_ICON);
	opts.icon.image = this.getImage();
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
}

Marker.prototype.getName = function()
{
	return this.name;
}

Marker.prototype.setSpot = function(spot)
{
	this.spot = spot;
	this.modified = true;
}

Marker.prototype.getSpot = function()
{
	return this.spot;
}

Marker.prototype.setCountry = function(country)
{
	this.country = country;
	this.modified = true;
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
}

Marker.prototype.getLon = function()
{
	return this.getLatLng().lng();
}

Marker.prototype.setAlt = function(alt)
{
	this.alt = alt;
	this.modified = true;
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

Marker.prototype.getSelect = function()
{
	return this.select;
}

Marker.prototype.setModified = function(modified)
{
	this.modified = modified;
	this.update();
}

Marker.prototype.getModified = function()
{
	return this.modified;
}

Marker.prototype.update = function()
{
	this.setImage(this.getImage());
}

Marker.prototype.getImage = function()
{
	var image;

	if(this.select)
	{
		if(this.modified)
		{
			image = "http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=EE0000,FFFF00,000000&ext=.png";
		}
		else
		{
			image = "http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=00EE00,FFFF00,000000&ext=.png";
		}
	}
	else
	{
		if(this.modified)
		{
			image = "http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,EE0000,000000&ext=.png";
		}
		else
		{
			image = "http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,00EE00,000000&ext=.png";
		}
	}
	
	return image;
}
