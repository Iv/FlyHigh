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

function wm_include(file)
{
	var script = document.createElement('script');
	script.src = file;
	script.type = 'text/javascript';
	script.defer = true;

	document.getElementsByTagName('head').item(0).appendChild(script);
}

function wm_setMapPos(top, left)
{
	var div;

	div = document.getElementById("map");
	div.style.top = top + "px";
	div.style.left = left + "px";
	google.maps.event.trigger(map, 'resize');
}

function wm_setMapSize(width, height)
{
	var div;

	div = document.getElementById("map");
	div.style.width = width + "px";
	div.style.height = height + "px";
	google.maps.event.trigger(map, 'resize');
}

function wm_emitAppReady()
{
	WebMap.appReady();
}

function wm_emitSetOk(ok)
{
	WebMap.setOk(ok);
}
