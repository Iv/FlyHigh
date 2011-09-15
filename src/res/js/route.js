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
function setLocation(loc)
{
	var locInput;

	locInput = document.getElementById("defaultLocation");
	locInput.value = loc;
}

function getLocation()
{
	var locInput;

	locInput = document.getElementById("location");

	return locInput.value;
}

function setName(name)
{
	var nameInput;

	nameInput = document.getElementById("name");
	nameInput.value = name;
}

function getName()
{
	var nameInput;

	nameInput = document.getElementById("name");

	return nameInput.value;
}

function setTurnPointList(tps)
{
	var tpInput;

	tpInput = document.getElementById("defaultTurnpoints");
	tpInput.value = tps;
}

function getTurnPointList()
{
	var tps = [];

	turnpointMarkers.each(function(tp, i) {
		tps[i] = [tp.getLatLng().lat(), tp.getLatLng().lng()] ;
	});

	return tps;
} 

function setTurnPointsDragable(drag)
{
	TurnPointsDragable = drag;
}
