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

TurnPt.Type = {
	Undefined: 0,
	WayPoint: 1,
	Cross: 2
};

function TurnPt(route, latlng, type)
{
	var turnPt = this;

	this.route = route;
	this.type = TurnPt.Type.Undefined;
	this.altitude = -1;
	this.nextTurnPt = null;
	this.prevLeg = null;
	this.nextLeg = null;
	this.editable = true;
	this.stpos = null;
	this.marker = new google.maps.Marker({
		map: route.getMap(),
		draggable: true,
		raiseOnDrag: false
	});
	this.infoBox = null;
	
	this.setType(type);
	this.setPosition(latlng);

	google.maps.event.addListener(this.marker, 'dragstart', function(event) {tp_dragstart(turnPt);});
	google.maps.event.addListener(this.marker, 'drag', function(event) {tp_drag(turnPt);});
	google.maps.event.addListener(this.marker, 'position_changed', function(event){tp_position_changed(turnPt);});
	google.maps.event.addListener(this.marker, 'dblclick', function(event){tp_dblclick(turnPt);});
	google.maps.event.addListener(this.marker, 'mouseover', function(event){tp_mouseover(turnPt);});
	google.maps.event.addListener(this.marker, 'mouseout', function(event){tp_mouseout(turnPt);});
}

TurnPt.prototype.getRoute = function()
{
	return this.route;
};

TurnPt.prototype.setType = function(type)
{
	if(this.type != type)
	{
		this.type = type;
		this.updateIcon();
	}
};

TurnPt.prototype.getType = function()
{
	return this.type;
};

TurnPt.prototype.remove = function()
{
	this.marker.setMap(null);
};

TurnPt.prototype.setNextTurnPt = function(nextTurnPt)
{
	this.nextTurnPt = nextTurnPt;
};

TurnPt.prototype.getNextTurnPt = function()
{
	return this.nextTurnPt;
};

TurnPt.prototype.setNextLeg = function(leg)
{
	this.nextLeg = leg;
	this.updateIcon();
};

TurnPt.prototype.getNextLeg = function()
{
	return this.nextLeg;
};

TurnPt.prototype.setPrevLeg = function(leg)
{
	this.prevLeg = leg;
	this.updateIcon();
};

TurnPt.prototype.getPrevLeg = function()
{
	return this.prevLeg;
};

TurnPt.prototype.setPosition = function(latlng)
{
	this.marker.setPosition(latlng);
};

TurnPt.prototype.getPosition = function()
{
	return this.marker.getPosition();
};

TurnPt.prototype.setAltitude = function(alt)
{
	this.altitude = alt;
};

TurnPt.prototype.getAltitude = function()
{
	return this.altitude;
};

TurnPt.prototype.setEditable = function(en)
{
	this.editable = en;
};

TurnPt.prototype.getEditable = function()
{
	return this.editable;
}

TurnPt.prototype.getInfoBox = function()
{
	return this.infoBox;
}

/*
	This is an ugly hack, to restore position while drag. Because Qt 4.6 won't display
	markers which are not draggable. In a later version, this should be fixed through
	setting markers draggable=this.editable.
*/
TurnPt.prototype.storePos = function()
{
	this.stpos = new google.maps.LatLng(this.getPosition().lat(), this.getPosition().lng());
};

TurnPt.prototype.restorePos = function()
{
	if(this.storePos != null)
	{
		this.setPosition(this.stpos);
	}
}

TurnPt.prototype.updateIcon = function()
{
	switch(this.getType())
	{
		case TurnPt.Type.WayPoint:
			if(this.getPrevLeg() == null)
			{
				// start
				this.marker.setIcon('http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,00EE00,000000&ext=.png');
			}
			else if(this.getNextLeg() == null)
			{
				// end
				this.marker.setIcon('http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,EE0000,000000&ext=.png');
			}
			else
			{
				// normal
				this.marker.setIcon('http://chart.apis.google.com/chart?cht=mm&chs=20x32&chco=FFFFFF,0000EE,000000&ext=.png');
			}

			this.infoBox = null;
		break;
		case TurnPt.Type.Cross:
			var image = new google.maps.MarkerImage('images/quad.png',
						new google.maps.Size(7, 7), // This marker is 15 pixels wide by 15 pixels tall.
						new google.maps.Point(0, 0), // The origin for this image is 0,0.
						new google.maps.Point(4, 4));// The anchor for this image is the base of the flagpole at 7,7.
			var shape = {
				coord: [0, 0, 7, 0, 7, 7, 0 , 7],
				type: 'poly'
			};

			this.marker.setIcon(image);
			this.infoBox = new InfoBox(this.getRoute().getMap());
		break;
	}
};

function tp_position_changed(turnPt)
{
	if(turnPt.getEditable() && (turnPt.getType() == TurnPt.Type.WayPoint))
	{
		if(turnPt.getPrevLeg() != null)
		{
			turnPt.getPrevLeg().setEndPosition(turnPt.getPosition());
		}

		if(turnPt.getNextLeg() != null)
		{
			turnPt.getNextLeg().setBeginPosition(turnPt.getPosition());
		}

		turnPt.getRoute().update();
	}
}

function tp_dragstart(turnPt)
{
	if(turnPt.getEditable())
	{
		if((turnPt.getType() == TurnPt.Type.Cross) &&
				(turnPt.getPrevLeg() != null))
		{
			turnPt.getInfoBox().hide();
			turnPt.getRoute().spliceLeg(turnPt);
		}
	}
	else
	{
		turnPt.storePos();
	}
}

function tp_drag(turnPt)
{
	if(!turnPt.getEditable())
	{
		turnPt.restorePos();
	}
}

function tp_dblclick(turnPt)
{
	if(turnPt.getEditable() && (turnPt.getType() == TurnPt.Type.WayPoint))
	{
		turnPt.getRoute().removeMarker(turnPt);
	}
}

function tp_mouseover(turnPt)
{
	var leg;
	var dist;
	var duration;
	var latlng;
	var msg;
	var beginPos;
	var endPos;

	if(turnPt.getInfoBox() != null)
	{
		leg = turnPt.getPrevLeg();
		beginPos = leg.getBeginTurnPt().getPosition();
		endPos = leg.getEndTurnPt().getPosition();
		dist = google.maps.geometry.spherical.computeDistanceBetween(beginPos, endPos);
		dist /= 1000.0;
		duration = dist / turnPt.getRoute().getSpeed();
		duration = Math.round(duration * 10) / 10;
		msg = "Distance: " + dist.toFixed(2) + " km ";
		msg += "Duration: " + duration.toFixed(1) + " h";

		latlng = turnPt.getPosition();
		turnPt.getInfoBox().show(latlng, 180, 50, msg);
	}
}

function tp_mouseout(turnPt)
{
	if(turnPt.getInfoBox() != null)
	{
		turnPt.getInfoBox().hide();
	}
}
