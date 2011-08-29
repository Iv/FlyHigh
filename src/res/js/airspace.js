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
var polygons = [];
var labels = [];
var oldSelect = -1;

function XCLoad()
{
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
		for(polyNr=0; polyNr<airspaces.length; polyNr++)
		{
			var poly = new GPolygon(airspaces[polyNr],"#000000", 1, 1, "#ff0000", 0.1, {clickable:false});
			map.addOverlay(poly);
			polygons.push(poly);
		}

		selectAirSpace(0);

		GEvent.addListener(map, "click", function(overlay, point)
		{
			if(!overlay)
			{
				for(var i=0; i<airspaces.length; i++)
				{
					if(isInsidePolygon(airspaces[i], point))
					{
						if(i != oldSelect)
						{
							selectAirSpace(i);
							WebMap.setLine(i);
							break; // Jump out of loop
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

function setAirSpace(label, lat, lon)
{
	var coordNr;
	var airspace = [];
	var latlngs = [];
	var point;

	if(lat.length == lon.length)
	{
		for(coordNr=0; coordNr<lat.length; coordNr++)
		{
			point = new GLatLng(lat[coordNr], lon[coordNr]);
			airspace.push(point);
		}

		airspaces.push(airspace);
		labels.push(label);
	}
}

function selectAirSpace(num)
{
	if(num < polygons.length)
	{
		if(oldSelect >= 0)
		{
			polygons[oldSelect].setFillStyle({weight: 1, color: "#ff0000", opacity: 0.1});
		}

		polygons[num].setFillStyle({weight: 1, color: "#ffff00", opacity: 0.5});
	}

	oldSelect = num;
}

function isInsidePolygon(polygon, point)
{
  var j = 0;
  var oddNodes = false;
  var x = point.lng();
  var y = point.lat();

	for(var i=0; i < polygon.length; i++)
	{
		j++;

		if(j == polygon.length)
		{
			j = 0;
		}

		if(((polygon[i].lat() < y) && (polygon[j].lat() >= y)) ||
			 ((polygon[j].lat() < y) && (polygon[i].lat() >= y)))
		{
			if(polygon[i].lng() + (y - polygon[i].lat()) /
				(polygon[j].lat()-polygon[i].lat()) *
				(polygon[j].lng() - polygon[i].lng()) < x)
			{
				oddNodes = !oddNodes
			}
		}
	}

	return oddNodes;
}
