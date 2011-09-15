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

var map = null;
var airspaces = [];
var oldSelect = -1;

function XCLoad()
{
	var airspaceNr;

	if(GBrowserIsCompatible())
	{
		map = new GMap2(document.getElementById('map'));
		map.setCenter(new GLatLng(47, 8.5), 8);
		map.addControl(new GMapTypeControl())
		map.removeMapType(G_SATELLITE_MAP);
		map.addMapType(G_PHYSICAL_MAP);
		map.setMapType(G_PHYSICAL_MAP);
		map.enableScrollWheelZoom();

		// add polygons
		for(airspaceNr=0; airspaceNr<airspaces.length; airspaceNr++)
		{
			map.addOverlay(airspaces[airspaceNr].getPolygon());
		}

		as_selectAirSpace(0);

		GEvent.addListener(map, "click", function(overlay, point)
		{
			if(!overlay)
			{
				// select next airspace
				airspaceNr = oldSelect;

				for(var i=0; i<airspaces.length; i++)
				{
					airspaceNr = (airspaceNr + 1) % airspaces.length;

					if(airspaces[airspaceNr].isInside(point))
					{
						if(airspaceNr != oldSelect)
						{
							as_selectAirSpace(airspaceNr);
							WebMap.setLine(airspaceNr);
							break; // jump out of loop
						}
					}
				}
			}
		});
	}
}

function XCUnload()
{
	map = null;
	GUnload();
}

function as_pushAirSpace(lat, lon, opts)
{
	var airspace;

	airspace = new AirSpace(lat, lon, {id: opts.id});
	airspaces.push(airspace);
}

function as_selectAirSpace(num)
{
	if(num < airspaces.length)
	{
		if(oldSelect >= 0)
		{
			airspaces[oldSelect].setSelect(false);
		}

		airspaces[num].setSelect(true);
		oldSelect = num;
	}
}

// class AirSpace
function AirSpace(lat, lon, opts)
{
	var vertexNr;
	var vertex;

	this.id = opts.id;
	this.vertexes = [];
	this.polygon = null;

	if(lat.length == lon.length)
	{
		for(vertexNr=0; vertexNr<lat.length; vertexNr++)
		{
			vertex = new GLatLng(lat[vertexNr], lon[vertexNr]);
			this.vertexes.push(vertex);
		}
	}
}

AirSpace.prototype.getId = function()
{
	return this.id;
}

AirSpace.prototype.getPolygon = function()
{
	this.polygon = new GPolygon(this.vertexes, "#000000", 1, 1, "#ff0000", 0.1, {clickable:false});

	return this.polygon;
}

AirSpace.prototype.isInside = function(point)
{
	var j = 0;
	var oddNodes = false;
	var x = point.lng();
	var y = point.lat();

	for(var i=0; i < this.vertexes.length; i++)
	{
		j++;

		if(j == this.vertexes.length)
		{
			j = 0;
		}

		if(((this.vertexes[i].lat() < y) && (this.vertexes[j].lat() >= y)) ||
			 ((this.vertexes[j].lat() < y) && (this.vertexes[i].lat() >= y)))
		{
			if(this.vertexes[i].lng() + (y - this.vertexes[i].lat()) /
				(this.vertexes[j].lat() - this.vertexes[i].lat()) *
				(this.vertexes[j].lng() - this.vertexes[i].lng()) < x)
			{
				oddNodes = !oddNodes
			}
		}
	}

	return oddNodes;
}

AirSpace.prototype.setSelect = function(select)
{
	if(select)
	{
		this.polygon.setFillStyle({weight: 1, color: "#ffff00", opacity: 0.5});
	}
	else
	{
		this.polygon.setFillStyle({weight: 1, color: "#ff0000", opacity: 0.1});
	}
}
