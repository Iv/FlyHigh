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

Fai.EarthRadius = 6371000.0;

function Fai(route)
{
	var sector;
	this.route = route;

	this.sectors = [];

	sector = new google.maps.Polyline({
		strokeColor: '#FF0000',
		strokeOpacity: 1.0,
		strokeWeight: 1,
		map: null,
		zIndex: 2
	});

	this.sectors.push(sector);

	sector = new google.maps.Polyline({
		strokeColor: '#00FF00',
		strokeOpacity: 1.0,
		strokeWeight: 1,
		map: null,
		zIndex: 2
	});

	this.sectors.push(sector);

	sector = new google.maps.Polyline({
		strokeColor: '#0000FF',
		strokeOpacity: 1.0,
		strokeWeight: 1,
		map: null,
		zIndex: 2
	});

	this.sectors.push(sector);

	sector = new google.maps.Polyline({
		strokeColor: '#00FFFF',
		strokeOpacity: 1.0,
		strokeWeight: 1,
		map: null,
		zIndex: 2
	});

	this.sectors.push(sector);
}

Fai.prototype.getMap = function()
{
	return this.route.getMap();
};

Fai.prototype.show = function(latlngs)
{
	var pixels;
	var path;

	if(latlngs != null)
	{
		pixels = this.toPixels(latlngs);

		path = this.faiSector([pixels[2], pixels[3], pixels[1]]);
		this.sectors[0].setPath(path);
		this.sectors[0].setMap(this.getMap());

		path = this.faiSector([pixels[3], pixels[1], pixels[2]]);
		this.sectors[1].setPath(path);
		this.sectors[1].setMap(this.getMap());

		path = this.faiSector([pixels[1], pixels[2], pixels[3]]);
		this.sectors[2].setPath(path);
		this.sectors[2].setMap(this.getMap());

		path = this.endSector(latlngs);
		this.sectors[3].setPath(path);
		this.sectors[3].setMap(this.getMap());
	}
	else
	{
		this.sectors[0].setMap(null);
		this.sectors[1].setMap(null);
		this.sectors[2].setMap(null);
		this.sectors[3].setMap(null);
	}
}

Fai.prototype.toPixels = function(latlngs)
{
	var pixels = [];
	var pixel;
	var proj;
	var nr;

	proj = this.getMap().getProjection();
	
	for(nr=0; nr<latlngs.length; nr++)
	{
		pixel = proj.fromLatLngToPoint(latlngs[nr]);
		pixels.push(pixel);
	}

	return pixels;
}

Fai.prototype.faiSector = function(pixels)
{
	var flip = isClockwise(pixels) ? 1 : -1;
	var deltaX;
	var deltaY;
	var theta;
	var cos_theta;
	var sin_theta;
	var proj;
	var pixel;
	var pixelX;
	var pixelY;
	var a;
	var b;
	var c;
	var x;
	var y;
	var result = [];

	deltaX = (pixels[1].x - pixels[0].x);
	deltaY = (pixels[1].y - pixels[0].y);
	theta = flip * Math.atan2(deltaY, deltaX);
	cos_theta = Math.cos(theta);
	sin_theta = Math.sin(theta);
	c = Math.sqrt(deltaX * deltaX + deltaY * deltaY);
	pixelX = pixels[0].x;
	pixelY = pixels[0].y;
	proj = this.getMap().getProjection();

	for(ap = 28; ap < 44; ++ap)
	{
		a = c * ap / 28.0;
		b = c * (72.0 - ap) / 28.0;
		x = (b * b + c * c - a * a) / (2 * c);
		y = Math.sqrt(b * b - x * x);
		pixel = new google.maps.Point(pixelX + x * cos_theta - y * sin_theta, pixelY + flip * (x * sin_theta + y * cos_theta));
		result.push(proj.fromPointToLatLng(pixel));
	}

	for(cp = 28; cp < 44; ++cp)
	{
		a = c * (72.0 - cp) / cp;
		b = c * 28.0 / cp;
		x = (b * b + c * c - a * a) / (2 * c);
		y = Math.sqrt(b * b - x * x);
		pixel = new google.maps.Point(pixelX + x * cos_theta - y * sin_theta, pixelY + flip * (x * sin_theta + y * cos_theta));
		result.push(proj.fromPointToLatLng(pixel));
	}

	for(cp = 44; cp >= 28; --cp)
	{
		a = c * 28.0 / cp;
		b = c * (72.0 - cp) / cp;
		x = (b * b + c * c - a * a) / (2 * c);
		y = Math.sqrt(b * b - x * x);
		pixel = new google.maps.Point(pixelX + x * cos_theta - y * sin_theta, pixelY + flip * (x * sin_theta + y * cos_theta));
		result.push(proj.fromPointToLatLng(pixel));
	}

	return result;
}

Fai.prototype.endSector = function(latlngs)
{
	var SegCount = 32;
	var Phi = Math.PI / 2.0;
	var latLngStart;
	var theta;
	var segNr;
	var dist;
	var result = [];

	latLngStart = latlngs[0];
	theta = initialBearingTo(latLngStart, latlngs[4]);
	dist = distHaversine(latlngs[1], latlngs[2]);
	dist += distHaversine(latlngs[2], latlngs[3]);
	dist += distHaversine(latlngs[3], latlngs[1]);
	dist = ((dist * 0.20) / Fai.EarthRadius); // 20 % rule

	result.push(latLngStart);

	for(segNr=0; segNr<SegCount; segNr++)
	{
		result.push(latLngAt(latLngStart, theta + (segNr / SegCount - 0.5) * Phi, dist));
	}

	result.push(latLngStart);

	return result;
}

function isClockwise(pixels)
{
	return (((pixels[1].y - pixels[0].y) * (pixels[2].x - pixels[0].x) - (pixels[2].y - pixels[0].y) * (pixels[1].x - pixels[0].x)) < 0);
}

function initialBearingTo(latLng1, latLng2)
{
	var lat1;
	var lng1;
	var lat2;
	var lng2;
	var y;
	var x;

	lat1 = latLng1.lat() * Math.PI / 180;
	lng1 = latLng1.lng() * Math.PI / 180;
	lat2 = latLng2.lat() * Math.PI / 180;
	lng2 = latLng2.lng() * Math.PI / 180;

	y = Math.sin(lng1 - lng2) * Math.cos(lat2);
	x = Math.cos(lat1) * Math.sin(lat2) - Math.sin(lat1) * Math.cos(lat2) * Math.cos(lng1 - lng2);

	return -Math.atan2(y, x);
}

function latLngAt(latLng, bearing, distance)
{
	var lat1;
	var lng1;
	var latAt;
	var lngAt;

	lat = latLng.lat() * Math.PI / 180;
	lng = latLng.lng() * Math.PI / 180;
	latAt = Math.asin(Math.sin(lat) * Math.cos(distance) + Math.cos(lat) * Math.sin(distance) * Math.cos(bearing));
	lngAt = lng + Math.atan2(Math.sin(bearing) * Math.sin(distance) * Math.cos(lat), Math.cos(distance) - Math.sin(lat) * Math.sin(latAt));

	return new google.maps.LatLng(latAt * 180 / Math.PI, lngAt * 180 / Math.PI);
}

function distHaversine(latLng1, latLng2)
{
	var lat1;
	var lng1;
	var lat2;
	var lng2;
	var dLat;
	var dLong;
	var a;
	var c;
	var dist;

	lat1 = latLng1.lat() * Math.PI / 180;
	lng1 = latLng1.lng() * Math.PI / 180;
	lat2 = latLng2.lat() * Math.PI / 180;
	lng2 = latLng2.lng() * Math.PI / 180;
	dLat  = (lat2 - lat1);
	dLong = (lng2 - lng1);

	a = Math.sin(dLat / 2) * Math.sin(dLat / 2) + Math.cos(lat1) * Math.cos(lat2) * Math.sin(dLong / 2) * Math.sin(dLong / 2);
	c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
	dist = Fai.EarthRadius * c;

	return dist;
}
