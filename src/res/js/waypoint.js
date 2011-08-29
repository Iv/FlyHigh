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

var geocoder = null;
var map = null;
var icon;
var markers = [];

function XCLoad()
{
	if(GBrowserIsCompatible()) {
		map = new GMap2(document.getElementById('map'));
		map.setCenter(new GLatLng(47, 8.5), 8);
		map.addControl(new GMapTypeControl())
		map.removeMapType(G_SATELLITE_MAP);
		map.addMapType(G_PHYSICAL_MAP);
		map.setMapType(G_PHYSICAL_MAP);
		map.enableScrollWheelZoom();

		icon = new GIcon(G_DEFAULT_ICON);
		icon.image = "http://chart.apis.google.com/chart?cht=mm&chs=24x32&chco=FFFFFF,008CFF,000000&ext=.png";

		// markers are set before
		var markerCluster = new MarkerClusterer(map, markers);
	}
}

function XCUnload()
{
	geocoder = null;
	map = null;
	GUnload();
}

function setWayPoint(name, lat, lon, alt)
{
	var latlng = new GLatLng(lat, lon);
	var marker = new GMarker(latlng, {title: name, icon: icon});
	markers.push(marker);
}
