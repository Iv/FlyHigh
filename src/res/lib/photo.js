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

/* WebMapFlight.cpp
	QString code = "fl_pushPhoto({lat: %1, lng: %2, path: '%3'});";
*/

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

	google.maps.event.addListener(this.marker, 'click', function(event) {ph_display(photo);});
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
}

function ph_display(photo)
{
  var contentString =
		'<div id="content">' +
		'<div id="siteNotice">' +
		'</div>' +
		'<div id="bodyContent">' +
		'<img src="file://' + photo.getPath() + '" alt="" width="600" height="480" >'
		'</div>' +
		'</div>';

  var infowindow = new google.maps.InfoWindow({
      content: contentString
  });

	infowindow.open(photo.getMap(), photo.getMarker());
}
