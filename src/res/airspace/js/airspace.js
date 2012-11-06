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
 *   It is prohibited to server or run this code over network p.e. as web  *
 *   service in combination with closed source.                            *
 ***************************************************************************/

// class AirSpace
function AirSpace(map, path, opts)
{
	var vertexNr;
	var vertex;

	this.id = opts.id;
	this.name = opts.name;
	this.low = opts.low;
	this.high = opts.high;
	this.airclass = opts.airclass;
	this.polygon = new google.maps.Polygon({
		map: map,
		paths: path,
		strokeColor: "#000000",
		strokeOpacity: 0.8,
		strokeWeight: 1,
		fillColor: "#ff0000",
		fillOpacity: 0.1,
		zIndex: 10,
		clickable: false
	});
}

AirSpace.prototype.getMap = function()
{
	return this.map;
};

AirSpace.prototype.getId = function()
{
	return this.id;
};

AirSpace.prototype.getName = function()
{
	return this.name;
};

AirSpace.prototype.getLow = function()
{
	return this.low;
};

AirSpace.prototype.getHigh = function()
{
	return this.high;
};

AirSpace.prototype.getClass = function()
{
	return this.airclass;
};

AirSpace.prototype.isInside = function(latlng)
{
	var i;
	var j = 0;
	var oddNodes = false;
	var x = latlng.lng();
	var y = latlng.lat();
	var path;

	path = this.polygon.getPath();

	for(i=0; i<path.length; i++)
	{
		j++;

		if(j == path.length)
		{
			j = 0;
		}

		if(((path.getAt(i).lat() < y) && (path.getAt(j).lat() >= y)) ||
			((path.getAt(j).lat() < y) && (path.getAt(i).lat() >= y)))
		{
			if(path.getAt(i).lng() + (y - path.getAt(i).lat()) /
				(path.getAt(j).lat() - path.getAt(i).lat()) *
				(path.getAt(j).lng() - path.getAt(i).lng()) < x)
			{
				oddNodes = !oddNodes;
			}
		}
	}

	return oddNodes;
};

AirSpace.prototype.setSelect = function(select)
{
	if(select)
	{
		this.polygon.setOptions({fillColor: "#ffff00", fillOpacity: 0.5});
	}
	else
	{
		this.polygon.setOptions({fillColor: "#ff0000", fillOpacity: 0.1});
	}
};
