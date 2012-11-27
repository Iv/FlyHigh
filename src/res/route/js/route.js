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

Route.ScoreType =
{
	Undefined: 0,
	Free: 1,
	Straight: 2,
	Flat: 3,
	Fai: 4
};

Route.ScoreTypeText = ["Undefined", "Free distance", "Straight distance", "Flat triangle", "FAI triangle"];

function Route(map)
{
	this.map = map;
	this.changeCallback = null;
	this.firstTurnPt = null;
	this.turnPtCount = 0;
	this.type = 0;
	this.dist = 0;
	this.score = 0;
	this.fai = new Fai(this);
	this.trackDist = 0;
	this.editable = true;
	this.speed = 0;
	this.duration = 0;

	this.line = new google.maps.Polyline({
		strokeColor: '#FFFFFF',
		strokeOpacity: 1.0,
		strokeWeight: 2,
		map: null,
		zIndex: 2
	});
}

Route.prototype.getMap = function()
{
	return this.map;
};

Route.prototype.setChangeCallback = function(callback)
{
	this.changeCallback = callback;
};

Route.prototype.getType = function()
{
	return this.type;
};

Route.prototype.getScore = function()
{
	return this.score;
};

Route.prototype.getDist = function()
{
	return this.dist;
};

Route.prototype.getTrackDist = function()
{
	return this.trackDist;
};

Route.prototype.setSpeed = function(speed)
{
	this.speed = speed;
	this.calcDuration();
};

Route.prototype.getSpeed = function()
{
	return this.speed;
};

Route.prototype.setDuration = function(duration)
{
	this.duration = duration;
	this.calcSpeed();
};

Route.prototype.getDuration = function()
{
	return this.duration;
};

Route.prototype.addTurnPt = function(turnPt)
{
	var beginTurnPt;
	var leg;
	
	turnPt.setEditable(this.getEditable());

	if(this.firstTurnPt === null)
	{
		this.firstTurnPt = turnPt;
	}
	else
	{
		beginTurnPt = this.firstTurnPt;

		while(beginTurnPt.getNextTurnPt() !== null)
		{
			beginTurnPt = beginTurnPt.getNextTurnPt();
		}

		beginTurnPt.setNextTurnPt(turnPt);
		leg = new Leg(this);
		leg.setEditable(this.getEditable());
		leg.setTurnPts(beginTurnPt, turnPt);
		beginTurnPt.setNextLeg(leg);
		turnPt.setPrevLeg(leg);
		this.update();
	}

	this.turnPtCount++;
};

Route.prototype.getTurnPts = function()
{
	var turnPts = [];
	var turnPt;

	turnPt = this.firstTurnPt;

	while(turnPt !== null)
	{
		turnPts.push(turnPt);
		turnPt = turnPt.getNextTurnPt();
	}

	return turnPts;
};

Route.prototype.setEditable = function(en)
{
	var turnPt;
	var leg;

	this.editable = en;
	turnPt = this.firstTurnPt;

	while(turnPt !== null)
	{
		turnPt.setEditable(en);
		leg = turnPt.getNextLeg();

		if(leg !== null)
		{
			leg.setEditable(en);
		}

		turnPt = turnPt.getNextTurnPt();
	}
};

Route.prototype.getEditable = function(en)
{
	return this.editable;
};

Route.prototype.calcSpeed = function()
{
	this.speed = Math.round((this.getTrackDist() / this.getDuration()) * 2) / 2; // * 10 / 5
};

Route.prototype.calcDuration = function()
{
	this.duration = Math.round((this.getTrackDist() / this.getSpeed()) * 2) / 2; // * 10 / 5
};

Route.prototype.spliceLeg = function(turnPt)
{
	var leg;
	var beginTurnPt;
	var endTurnPt;

	// turnPt is cross on the middle of the line between two turn points
	// after: WayPoint-------Cross--------WayPoint
	// this is not the neatest solution, but this is the simplest

	// get begin and end
	leg = turnPt.getPrevLeg();
	beginTurnPt = leg.getBeginTurnPt();
	endTurnPt = leg.getEndTurnPt();

	// reconnect leg to turnPt
	leg.setTurnPts(beginTurnPt, turnPt);
	turnPt.setPrevLeg(leg);
	beginTurnPt.setNextTurnPt(turnPt);

	// create new leg between turnPt and endTurnPt
	leg = new Leg(this);
	leg.setTurnPts(turnPt, endTurnPt);
	turnPt.setNextLeg(leg);
	endTurnPt.setPrevLeg(leg);
	turnPt.setNextTurnPt(endTurnPt);
	
	turnPt.setType(TurnPt.Type.WayPoint);
	this.turnPtCount++;
	this.update();
};

Route.prototype.removeMarker = function(turnPt)
{
	var leg;
	var beginTurnPt;
	var endTurnPt;

	if(this.turnPtCount > 2)
	{
		prevLeg = turnPt.getPrevLeg();
		nextLeg = turnPt.getNextLeg();

		if((prevLeg !== null) && (nextLeg !== null))
		{
			// turnPt is between two legs
			beginTurnPt = prevLeg.getBeginTurnPt();
			endTurnPt = turnPt.getNextTurnPt();
			prevLeg.setEndTurnPt(endTurnPt);
			endTurnPt.setPrevLeg(prevLeg);
			beginTurnPt.setNextTurnPt(endTurnPt);
			nextLeg.remove();
		}
		else if(prevLeg === null)
		{ 
			// turnPt is first turn point
			endTurnPt = turnPt.getNextTurnPt();
			endTurnPt.setPrevLeg(null);
			nextLeg.remove();
			this.firstTurnPt = endTurnPt;
		}
		else if(nextLeg === null)
		{
			// turnPt is last turn point
			beginTurnPt = prevLeg.getBeginTurnPt();
			beginTurnPt.setNextLeg(null);
			beginTurnPt.setNextTurnPt(null);
			prevLeg.remove();
		}

		turnPt.remove();
		this.turnPtCount--;
		this.update();
	}
};

Route.prototype.update = function()
{
	var locInput;
	var turnPt;
	var turnPts = [];
	var faiPts = [];
	var path = [];
	var scoreType;
	var bestScore;
	var score;
	var bestDist;
	var dist;
	var nr;
	var indexs = null;
	var optimizer;

	turnPt = this.firstTurnPt;

	while(turnPt !== null)
	{
		turnPts.push(turnPt);
		turnPt = turnPt.getNextTurnPt();
	}

	optimizer = new Optimizer();
	optimizer.optimize(turnPts);

	scoreType = Route.ScoreType.Free;
	dist = optimizer.getFreeDist() / 1000.0;
	indexs = optimizer.getFreeIndex();
	bestDist = dist;
	bestScore = dist * 1.0;

	dist = optimizer.getStraightDist() / 1000.0;
	score = dist * 1.2;

	if(score > bestScore)
	{
		scoreType = Route.ScoreType.Straight;
		bestDist = dist;
		bestScore = score;
		indexs = optimizer.getStraightIndex();
	}

	dist = optimizer.getFlatDist() / 1000.0;
	score = dist * 1.2;

	if(score > bestScore)
	{
		scoreType = Route.ScoreType.Flat;
		bestDist = dist;
		bestScore = score;
		indexs = optimizer.getFlatIndex();
	}

	dist = optimizer.getFaiDist() / 1000.0;
	score = dist * 1.3;

	if(score > bestScore)
	{
		scoreType = Route.ScoreType.Fai;
		bestDist = dist;
		bestScore = score;
		indexs = optimizer.getFaiIndex();
	}

	switch(scoreType)
	{
		case Route.ScoreType.Free:
			path.push(turnPts[indexs[0]].getPosition());
			path.push(turnPts[indexs[1]].getPosition());
			path.push(turnPts[indexs[2]].getPosition());
			path.push(turnPts[indexs[3]].getPosition());
			path.push(turnPts[indexs[4]].getPosition());
			this.fai.show(null);
		break;
		case Route.ScoreType.Straight:
			path.push(turnPts[indexs[0]].getPosition());
			path.push(turnPts[indexs[1]].getPosition());
			this.fai.show(null);
		break;
		case Route.ScoreType.Flat:
		case Route.ScoreType.Fai:
			path.push(turnPts[indexs[0]].getPosition());
			path.push(turnPts[indexs[1]].getPosition());
			path.push(turnPts[indexs[2]].getPosition());
			path.push(turnPts[indexs[3]].getPosition());
			path.push(turnPts[indexs[4]].getPosition());
			this.fai.show(path);
		break;
	}

	this.line.setMap(this.getMap());
	this.line.setPath(path);
	this.type = scoreType;
	this.dist = bestDist;
	this.score = bestScore;
	this.trackDist = optimizer.getTrackDist() / 1000.0;
	this.calcDuration();

	if(this.changeCallback !== null)
	{
		this.changeCallback();
	}
};
