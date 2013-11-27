/***************************************************************************
 *   Copyright (C) 2013 by Alex Graf                                       *
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

var ph_oms = null;

function ph_initPhotoList(map)
{
	var infowin;

	infowin = new google.maps.InfoWindow();
	ph_oms = new OverlappingMarkerSpiderfier(map, {markersWontMove: true, markersWontHide: true});

	ph_oms.addListener('click', function(marker)
	{
		var content;

		content = '<img src="file://' + marker.path +
							'" alt="" style="max-width:600px;height:480px;width:auto;">';
		infowin.setContent(content);
		infowin.open(map, marker);
	});

	ph_oms.addListener('spiderfy', function(markers)
	{
		infowin.close();
	});
}

function ph_addToPhotoList(photo)
{
	var marker;

	marker = photo.getMarker();
	marker.path = photo.getPath();
	ph_oms.addMarker(marker);
}

function Photo(map, opts)
{
	var photo = this;

	this.map = map;
	this.path = opts.path;
	this.marker = new google.maps.Marker({
		map: map,
		draggable: false,
		raiseOnDrag: false,
		position: new google.maps.LatLng(opts.lat, opts.lng),
		zIndex: 12
	});

	this.updateIcon();
}

Photo.prototype.getMap = function()
{
	return this.map;
};

Photo.prototype.getMarker = function()
{
	return this.marker;
};

Photo.prototype.getPath = function()
{
	return this.path;
};

Photo.prototype.updateIcon = function()
{
	var image = new google.maps.MarkerImage('qrc:/camera.png',
				new google.maps.Size(24, 24),
				new google.maps.Point(0, 0),
				new google.maps.Point(12, 12));

	this.marker.setIcon(image);
};
