/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
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
 ***************************************************************************/

#include <stack>
#include <math.h>
#include "PolyLineEncoder.h"

const double PolyLineEncoder::undefined = 1000000;

PolyLineEncoder::PolyLineEncoder(const WayPoint::WayPointListType &wpList)
{
	double zoomLevelBreak;
	uint levelNr;

	m_wpList = wpList;

	m_numLevels = 18;
	m_zoomFactor = 2;
	m_verySmall = 0.00001;
	m_forceEndpoints = true;

	for(levelNr=0; levelNr<m_numLevels; levelNr++)
	{
		zoomLevelBreak = m_verySmall * pow(m_zoomFactor, m_numLevels - levelNr - 1);
		m_zoomLevelBreaks.push_back(zoomLevelBreak);
	}
}

PolyLineEncoder::~PolyLineEncoder()
{
}

void PolyLineEncoder::dpEncode(const WayPoint::WayPointListType &points)
{
	std::stack<StackItem> stack;
	StackItem current;
	StackItem tmpStackItem;
	double absMaxDist;
	DistsType dists;
	double maxDist;
	double temp;
	double segmentLength;
	int maxLoc;
	int i;
	std::string encodedPoints;
	std::string encodedLevels;

	if(points.size() > 2)
	{
		absMaxDist = 0;
		dists.resize(points.size(), undefined);
		current.i = 0;
		current.j = points.size() - 1;
		stack.push(current);

		while(stack.size() > 0)
		{
			current = stack.top();
			stack.pop();
			maxDist = 0;
			segmentLength = pow(points[current.j].latitude() - points[current.i].latitude(), 2) +
							pow(points[current.j].longitude() - points[current.i].longitude(), 2);

			for(i=current.i + 1; i < current.j; i++)
			{
				temp = distance(points[i], points[current.i], points[current.j], segmentLength);

				if(temp > maxDist)
				{
					maxDist = temp;
					maxLoc = i;

					if(maxDist > absMaxDist)
					{
						absMaxDist = maxDist;
					}
				}
			}

			if(maxDist > m_verySmall)
			{
				dists[maxLoc] = maxDist;
				tmpStackItem.i = current.i;
				tmpStackItem.j = maxLoc;
				stack.push(tmpStackItem); // stack.push([current.i, maxLoc]);
				tmpStackItem.i = maxLoc;
				tmpStackItem.j = current.j;
				stack.push(tmpStackItem); // stack.push([maxLoc, current.j]);
			}
		}
	}

	encodedPoints = createEncodings(points, dists);
	encodedLevels = encodeLevels(points, dists, absMaxDist);

/*
	return {
		encodedPoints: encodedPoints,
		encodedLevels: encodedLevels,
		encodedPointsLiteral: encodedPoints.replace(/\\/g,"\\\\")
	}
*/
}

std::string PolyLineEncoder::createEncodings(const WayPoint::WayPointListType &points, DistsType &dists)
{
	int i;
	double lat;
	double lng;
	int dlat;
	int dlng;
	int late5;
	int lnge5;
	int plat = 0;
	int plng = 0;
	std::string encoded_points = "";
	
	for(i = 0; i < points.size(); i++)
	{
		if(dists[i] != undefined || i == 0 || i == points.size() - 1)
		{
			lat = points[i].latitude();
			lng = points[i].longitude();
			late5 = (int)floor(lat * 1e5);
			lnge5 = (int)floor(lng * 1e5);
			dlat = late5 - plat;
			dlng = lnge5 - plng;
			plat = late5;
			plng = lnge5;
			encoded_points += encodeSignedNumber(dlat) + encodeSignedNumber(dlng);
		}
	}

	return encoded_points;
}

std::string PolyLineEncoder::encodeLevels(const WayPoint::WayPointListType &points, DistsType &dists, double absMaxDist)
{
	std::string encoded_levels = "";
	int i;

	if(m_forceEndpoints)
	{
		encoded_levels += encodeNumber(m_numLevels - 1);
	}
	else
	{
		encoded_levels += encodeNumber(m_numLevels - computeLevel(absMaxDist) - 1);
	}

	for(i=1; i<(points.size() - 1); i++)
	{
		if(dists[i] != undefined)
		{
			encoded_levels += encodeNumber(m_numLevels - computeLevel(dists[i]) - 1);
		}
	}

	if(m_forceEndpoints)
	{
		encoded_levels += encodeNumber(m_numLevels - 1);
	}
	else
	{
		encoded_levels += encodeNumber(m_numLevels - computeLevel(absMaxDist) - 1);
	}

	return encoded_levels;
}

uint PolyLineEncoder::computeLevel(double dd)
{
	uint lev = 0;
	
	if(dd > m_verySmall)
	{
		while(dd < m_zoomLevelBreaks[lev])
		{
			lev++;
		}
	}

	return lev;
}

std::string PolyLineEncoder::encodeSignedNumber(int num)
{
	uint sgn_num = (num << 1);
	
	if(num < 0)
	{
		sgn_num = ~(sgn_num);
	}

	return encodeNumber(sgn_num);
}

std::string PolyLineEncoder::encodeNumber(uint num)
{
	std::string encodeString = "";
	uint nextValue;
	uint finalValue;

	while(num >= 0x20)
	{
		nextValue = (0x20 | (num & 0x1f)) + 63;
//     if (nextValue == 92) {
//       encodeString += (String.fromCharCode(nextValue));
//     }
//		encodeString += (String.fromCharCode(nextValue));
		num >>= 5;
	}

	finalValue = num + 63;
//   if (finalValue == 92) {
//     encodeString += (String.fromCharCode(finalValue));
//   }
//	encodeString += (String.fromCharCode(finalValue));

	return encodeString;
}

double PolyLineEncoder::distance(const WayPoint &p0, const WayPoint &p1, const WayPoint &p2, double segLength)
{
  double u;
	double out;

	if(p1.latitude() == p2.latitude() && p1.longitude() == p2.longitude())
	{
		out = sqrt(pow(p2.latitude() - p0.latitude(), 2) + pow(p2.longitude() - p0.longitude() , 2));
	}
	else
	{
		u = ((p0.latitude() - p1.latitude()) * (p2.latitude() - p1.latitude()) +
				 (p0.longitude() - p1.longitude()) * (p2.longitude() - p1.longitude())) /
					segLength;
	
		if(u <= 0)
		{
			out = sqrt(pow(p0.latitude() - p1.latitude(), 2) + pow(p0.longitude() - p1.longitude(), 2));
		}
		else if(u >= 1)
		{
			out = sqrt(pow(p0.latitude() - p2.latitude(), 2) + pow(p0.longitude() - p2.longitude(), 2));
		}
		else
		{
			out = sqrt(pow(p0.latitude() - p1.latitude() - u * (p2.latitude() - p1.latitude()), 2) +
				pow(p0.longitude() - p1.longitude() -u * (p2.longitude() - p1.longitude()), 2));
		}
	}
	
	return out;
}

#if 0

// The main function.  Essentially the Douglas-Peucker
// algorithm, adapted for encoding. Rather than simply
// eliminating points, we record their from the
// segment which occurs at that recursive step.  These
// distances are then easily converted to zoom levels.
PolylineEncoder.prototype.dpEncode = function(points) {
  var absMaxDist = 0;
  var stack = [];
  var dists = new Array(points.length);
  var maxDist, maxLoc, temp, first, last, current;
  var i, encodedPoints, encodedLevels;
  var segmentLength;
  
  if(points.length > 2) {
    stack.push([0, points.length-1]);
    while(stack.length > 0) {
      current = stack.pop();
      maxDist = 0;
      segmentLength = Math.pow(points[current[1]].lat()-points[current[0]].lat(),2) + 
        Math.pow(points[current[1]].lng()-points[current[0]].lng(),2);
      for(i = current[0]+1; i < current[1]; i++) {
        temp = this.distance(points[i], 
          points[current[0]], points[current[1]],
          segmentLength);
        if(temp > maxDist) {
          maxDist = temp;
          maxLoc = i;
          if(maxDist > absMaxDist) {
            absMaxDist = maxDist;
          }
        }
      }
      if(maxDist > this.verySmall) {
        dists[maxLoc] = maxDist;
        stack.push([current[0], maxLoc]);
        stack.push([maxLoc, current[1]]);
      }
    }
  }
  
  encodedPoints = this.createEncodings(points, dists);
  encodedLevels = this.encodeLevels(points, dists, absMaxDist);
  return {
    encodedPoints: encodedPoints,
    encodedLevels: encodedLevels,
    encodedPointsLiteral: encodedPoints.replace(/\\/g,"\\\\")
  }
}

PolylineEncoder.prototype.dpEncodeToJSON = function(points,
  color, weight, opacity) {
  var result;
  
  if(!opacity) {
    opacity = 0.9;
  }
  if(!weight) {
    weight = 3;
  }
  if(!color) {
    color = "#0000ff";
  }
  result = this.dpEncode(points);
  return {
    color: color,
    weight: weight,
    opacity: opacity,
    points: result.encodedPoints,
    levels: result.encodedLevels,
    numLevels: this.numLevels,
    zoomFactor: this.zoomFactor
  }
}

PolylineEncoder.prototype.dpEncodeToGPolyline = function(points,
  color, weight, opacity) {
  if(!opacity) {
    opacity = 0.9;
  }
  if(!weight) {
    weight = 3;
  }
  if(!color) {
    color = "#0000ff";
  }
  return new GPolyline.fromEncoded(
    this.dpEncodeToJSON(points, color, weight, opacity));
}

PolylineEncoder.prototype.dpEncodeToGPolygon = function(pointsArray,
  boundaryColor, boundaryWeight, boundaryOpacity,
  fillColor, fillOpacity, fill, outline) {
  var i, boundaries;
  if(!boundaryColor) {
    boundaryColor = "#0000ff";
  }
  if(!boundaryWeight) {
    boundaryWeight = 3;
  }
  if(!boundaryOpacity) {
    boundaryOpacity = 0.9;
  }
  if(!fillColor) {
    fillColor = boundaryColor;
  }
  if(!fillOpacity) {
    fillOpacity = boundaryOpacity/3;
  }
  if(fill==undefined) {
    fill = true;
  }
  if(outline==undefined) {
    outline = true;
  }
  
  boundaries = new Array(0);
  for(i=0; i<pointsArray.length; i++) {
    boundaries.push(this.dpEncodeToJSON(pointsArray[i],
      boundaryColor, boundaryWeight, boundaryOpacity));
  }
  return new GPolygon.fromEncoded({
    polylines: boundaries,
    color: fillColor,
    opacity: fillOpacity,
    fill: fill,
    outline: outline
  });
}

// distance(p0, p1, p2) computes the distance between the point p0
// and the segment [p1,p2].  This could probably be replaced with
// something that is a bit more numerically stable.
PolylineEncoder.prototype.distance = function(p0, p1, p2, segLength) {
  var u, out;
  
  if(p1.lat() === p2.lat() && p1.lng() === p2.lng()) {
    out = Math.sqrt(Math.pow(p2.lat()-p0.lat(),2) + Math.pow(p2.lng()-p0.lng(),2));
  }
  else {
    u = ((p0.lat()-p1.lat())*(p2.lat()-p1.lat())+(p0.lng()-p1.lng())*(p2.lng()-p1.lng()))/
      segLength;
  
    if(u <= 0) {
      out = Math.sqrt(Math.pow(p0.lat() - p1.lat(),2) + Math.pow(p0.lng() - p1.lng(),2));
    }
    if(u >= 1) {
      out = Math.sqrt(Math.pow(p0.lat() - p2.lat(),2) + Math.pow(p0.lng() - p2.lng(),2));
    }
    if(0 < u && u < 1) {
      out = Math.sqrt(Math.pow(p0.lat()-p1.lat()-u*(p2.lat()-p1.lat()),2) +
        Math.pow(p0.lng()-p1.lng()-u*(p2.lng()-p1.lng()),2));
    }
  }
  return out;
}

// The createEncodings function is very similar to Google's
// http://www.google.com/apis/maps/documentation/polyline.js
// The key difference is that not all points are encoded, 
// since some were eliminated by Douglas-Peucker.
PolylineEncoder.prototype.createEncodings = function(points, dists) {
  var i, dlat, dlng;
  var plat = 0;
  var plng = 0;
  var encoded_points = "";

  for(i = 0; i < points.length; i++) {
    if(dists[i] != undefined || i == 0 || i == points.length-1) {
      var point = points[i];
      var lat = point.lat();
      var lng = point.lng();
      var late5 = Math.floor(lat * 1e5);
      var lnge5 = Math.floor(lng * 1e5);
      dlat = late5 - plat;
      dlng = lnge5 - plng;
      plat = late5;
      plng = lnge5;
      encoded_points += this.encodeSignedNumber(dlat) + 
        this.encodeSignedNumber(dlng);
    }
  }
  return encoded_points;
}

// This computes the appropriate zoom level of a point in terms of it's 
// distance from the relevant segment in the DP algorithm.  Could be done
// in terms of a logarithm, but this approach makes it a bit easier to
// ensure that the level is not too large.
PolylineEncoder.prototype.computeLevel = function(dd) {
  var lev;
  if(dd > this.verySmall) {
    lev=0;
    while(dd < this.zoomLevelBreaks[lev]) {
      lev++;
    }
    return lev;
  }
}

// Now we can use the previous function to march down the list
// of points and encode the levels.  Like createEncodings, we
// ignore points whose distance (in dists) is undefined.
PolylineEncoder.prototype.encodeLevels = function(points, dists, absMaxDist) {
  var i;
  var encoded_levels = "";
  if(this.forceEndpoints) {
    encoded_levels += this.encodeNumber(this.numLevels-1)
  } else {
    encoded_levels += this.encodeNumber(
      this.numLevels-this.computeLevel(absMaxDist)-1)
  }
  for(i=1; i < points.length-1; i++) {
    if(dists[i] != undefined) {
      encoded_levels += this.encodeNumber(
        this.numLevels-this.computeLevel(dists[i])-1);
    }
  }
  if(this.forceEndpoints) {
    encoded_levels += this.encodeNumber(this.numLevels-1)
  } else {
    encoded_levels += this.encodeNumber(
      this.numLevels-this.computeLevel(absMaxDist)-1)
  }
  return encoded_levels;
}

// This function is very similar to Google's, but I added
// some stuff to deal with the double slash issue.
PolylineEncoder.prototype.encodeNumber = function(num) {
  var encodeString = "";
  var nextValue, finalValue;
  while (num >= 0x20) {
    nextValue = (0x20 | (num & 0x1f)) + 63;
//     if (nextValue == 92) {
//       encodeString += (String.fromCharCode(nextValue));
//     }
    encodeString += (String.fromCharCode(nextValue));
    num >>= 5;
  }
  finalValue = num + 63;
//   if (finalValue == 92) {
//     encodeString += (String.fromCharCode(finalValue));
//   }
  encodeString += (String.fromCharCode(finalValue));
  return encodeString;
}

// This one is Google's verbatim.
PolylineEncoder.prototype.encodeSignedNumber = function(num) {
  var sgn_num = num << 1;
  if (num < 0) {
    sgn_num = ~(sgn_num);
  }
  return(this.encodeNumber(sgn_num));
}


// The remaining code defines a few convenience utilities.
// PolylineEncoder.latLng
PolylineEncoder.latLng = function(y, x) {
	this.y = y;
	this.x = x;
}
PolylineEncoder.latLng.prototype.lat = function() {
	return this.y;
}
PolylineEncoder.latLng.prototype.lng = function() {
	return this.x;
}

// PolylineEncoder.pointsToLatLngs
PolylineEncoder.pointsToLatLngs = function(points) {
	var i, latLngs;
	latLngs = new Array(0);
	for(i=0; i<points.length; i++) {
		latLngs.push(new PolylineEncoder.latLng(points[i][0], points[i][1]));
	}
	return latLngs;
}

// PolylineEncoder.pointsToGLatLngs
PolylineEncoder.pointsToGLatLngs = function(points) {
	var i, gLatLngs;
	gLatLngs = new Array(0);
	for(i=0; i<points.length; i++) {
		gLatLngs.push(new GLatLng(points[i][0], points[i][1]));
	}
	return gLatLngs;
}
#endif