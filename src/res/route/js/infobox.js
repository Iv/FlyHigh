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

function InfoBox(map)
{
	this.map = map;
	this.infoDiv = null;
}

InfoBox.prototype.show = function(latlng, width, height, msg)
{
	var mapDiv;
	var infoDiv;
	var mapWidth;
	var mapHeight;
	var bounds;
	var boundsSpan;
	var longSpan;
	var latSpan;
	var ptX;
	var ptY;

	mapDiv = this.map.getDiv();
	mapWidth = mapDiv.offsetWidth;
	mapHeight = mapDiv.offsetHeight;
	bounds = map.getBounds();
	boundsSpan = bounds.toSpan();
	ptX = (latlng.lng() - bounds.getSouthWest().lng()) * mapWidth / boundsSpan.lng();
	ptX += mapDiv.offsetLeft;
	ptY = (bounds.getNorthEast().lat() - latlng.lat()) * mapHeight / boundsSpan.lat();
	ptY += mapDiv.offsetTop;
	ptY += 20;

	if(this.infoDiv == null)
	{
		this.createInfoDiv();
	}

	infoDiv = this.infoDiv;
	infoDiv.style.left = ptX.toFixed(0) + "px";
	infoDiv.style.top = ptY.toFixed(0) + "px";
	infoDiv.style.width = width + "px";
	infoDiv.style.height = height + "px";
	infoDiv.innerHTML = msg;
	infoDiv.style.display = "block";
}

InfoBox.prototype.hide = function()
{
	if(this.infoDiv != null)
	{
		this.infoDiv.style.display = "none";
	}
}

InfoBox.prototype.createInfoDiv = function()
{
	var infoDiv;

	infoDiv = document.createElement('div');
	infoDiv.style.position = "absolute";
	infoDiv.style.backgroundColor = "#eeeeee";
	infoDiv.style.border = "1px solid";
	document.body.appendChild(infoDiv);
	this.infoDiv = infoDiv;
}
