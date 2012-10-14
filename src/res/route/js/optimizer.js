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
/*
	Copyright (C) 2002 by Thomas Kuhlmann, Berlin. Contact: coooly@freenet.de
	Thanks to Dr.Dietrich Münchmeyer und Andreas Rieck for valuable inputs!
	adapted, aranged for better understanding and comments in english
	by Alex Graf grafal@sf.net 2005, 2012
	port to javascript by Alex Graf grafal@sf.net 2012
*/

function Optimizer()
{
	this.indexFree = new Array(5);
	this.indexFAI = new Array(5);
	this.indexFlat = new Array(5);
	this.indexStraight = new Array(2);

	this.indexFree = [];
	this.indexFAI = [];
	this.indexFlat = [];
	this.indexStraight = [];
	this.bestFree = 0;
	this.bestFAI = 0;
	this.bestFlat = 0;
	this.bestStraight = 0;
	this.trackDist = 0;

	this.dists = [];
	this.dminindex = [];
	this.maxenddist = [];
	this.maxendpunkt = [];
};

Optimizer.prototype.optimize = function(turnPts)
{
	var nTurnPts = turnPts.length;
	var i4cmp = nTurnPts - 2;
	var i2cmp = nTurnPts - 2;
	var i1;
	var i2;
	var i3;
	var i4;
	var i;
	var a;
	var b;
	var c;
	var d;
	var e;
	var u;
	var w;
	var tmp;
	var aplusb;
	var c25;
	var d5;

	if(turnPts.length < 2)
	{
		return false;
	}

	this.bestFree = 0;
	this.bestFAI = 0;
	this.bestFlat = 0;
	this.bestStraight = 0;
	this.initdmval(turnPts);
	this.initdmin(turnPts);
	this.initmaxend(turnPts);

	for(i2=0; i2<i2cmp; i2++)
	{
		for(i=i1=e=0; i<i2; i++)
		{ // optimize 1st turnpoint for free distance separately
			tmp = this.dists[i + nTurnPts * i2];

			if(tmp > e)
			{
				e = tmp;
				i1 = i;
			}
		} // e, i1 contains for this i2 the best value

		i4cmp = i2 + 2;

		for(i4=nTurnPts; --i4>=i4cmp;)
		{ // optimize 3th turnpoint from tail
			c = this.dists[i2 + nTurnPts * i4];
			c25 = c * 25;
			d = this.dists[i4 + nTurnPts * i2]; // dmin(i2, i4);
			d5 = d * 5;
			aplusb = 0;

			for(i=i3=i2+1; i<i4; i++)
			{ // optimize 2nd turnpoint for free distance separately
				a = this.dists[i2 + nTurnPts * i];
				b = this.dists[i + nTurnPts * i4];
				tmp = a + b;

				if(tmp > aplusb)
				{ // finds greatest a + b (and triangle too)
					aplusb = tmp;
					i3 = i;
				}

				u = tmp + c;

				if(d5 <= u) // 20/100*u = 1/5*u <= d (20 percent rule)
				{ // triangle found 5*d<= a+b+c
					if((c25 >= (tmp = u*7)) && (a * 25 >= tmp) && (b * 25 >= tmp)) // 28/100*u = 7/25*u <= side (28 percent rule)
					{ // FAI Triangle
						w = (u - d);

						if(w > this.bestFAI)
						{ // this FAI Triangle is greater
							this.indexFAI[0] = this.dminindex[i2 + nTurnPts * i4]; // dmini(i2, i4);
							this.indexFAI[1] = i2;
							this.indexFAI[2] = i;
							this.indexFAI[3] = i4;
							this.indexFAI[4] = this.dminindex[i4 + nTurnPts * i2]; // dminj(i2, i4);
							this.bestFAI = w;
						}
					}
					else
					{ // Flat Triangle
						w = (u - d);

						if(w > this.bestFlat)
						{
							this.indexFlat[0] = this.dminindex[i2 + nTurnPts * i4]; // dmini(i2, i4);
							this.indexFlat[1] = i2;
							this.indexFlat[2] = i;
							this.indexFlat[3] = i4;
							this.indexFlat[4] = this.dminindex[i4 + nTurnPts * i2]; // dminj(i2, i4);
							this.bestFlat = w;
						}
					}
				}
			} // aplusb, i3 contains for this i2 and i4 the best value

			tmp = this.maxenddist[i4] + aplusb + e;

			if(tmp > this.bestFree)
			{
				this.indexFree[0] = i1;
				this.indexFree[1] = i2;
				this.indexFree[2] = i3;
				this.indexFree[3] = i4;
				this.indexFree[4] = this.maxendpunkt[i4];
				this.bestFree = tmp;
			}
		}
	}

	this.cleanup();
};

Optimizer.prototype.getFreeDist = function()
{
	return this.bestFree;
};

Optimizer.prototype.getFreeIndex = function()
{
	return this.indexFree;
};

Optimizer.prototype.getStraightDist = function()
{
	return this.bestStraight;
};

Optimizer.prototype.getTrackDist = function()
{
	return this.trackDist;
};

Optimizer.prototype.getStraightIndex = function()
{
	return this.indexStraight;
};

Optimizer.prototype.getFaiDist = function()
{
	return this.bestFAI;
};

Optimizer.prototype.getFaiIndex = function()
{
	return this.indexFAI;
};

Optimizer.prototype.getFlatDist = function()
{
	return this.bestFlat;
};

Optimizer.prototype.getFlatIndex = function()
{
	return this.indexFlat;
};

/*
calc all distances between all points and save in distance matrix.
the calculation is done with doubles, the result is rounded to full meters and
stoerd as integers. Because calculation after is much more faster.
*/
Optimizer.prototype.initdmval = function(turnPts)
{
	var nTurnPts = turnPts.length;
	var sinlat = new Array(nTurnPts);
	var coslat = new Array(nTurnPts);
	var lonrad = new Array(nTurnPts);
	var latrad;
	var sli;
	var cli;
	var lri;
	var i;
	var j;
	var dist;
	var cmp;
	var wpNr;
	var CONST_PI_DIV_180 = (Math.PI / 180.0);
	var first;

	this.dists = new Array(nTurnPts * nTurnPts);
	this.trackDist = 0;
	cmp = nTurnPts + 1;

	for(wpNr=0; wpNr<nTurnPts; wpNr++)
	{ // all deltas to radians and save sin/cos
		lonrad[wpNr] = turnPts[wpNr].getPosition().lng() * CONST_PI_DIV_180;
		latrad = turnPts[wpNr].getPosition().lat() * CONST_PI_DIV_180;
		sinlat[wpNr] = Math.sin(latrad);
		coslat[wpNr] = Math.cos(latrad);
		this.dists[cmp * wpNr] = 0; // fill diagonal of matrix with 0
	}

	this.bestStraight = 0;
	this.indexStraight[0] = 0;
	this.indexStraight[1] = 1;
	cmp = (nTurnPts - 1);

	// this for not backwards!
	for(i=0; i<cmp; i++)
	{
		sli = sinlat[i];
		cli = coslat[i];
		lri = lonrad[i];
		first = true;

		for(j=i+1; j<nTurnPts; j++)
		{
			dist = (6371000.0 * Math.acos(sli * sinlat[j] + cli * coslat[j] * Math.cos(lri-lonrad[j])) + 0.5);
			this.dists[i + nTurnPts * j] = dist;

			if(first)
			{
				this.trackDist += dist;
				first = false;
			}

			if(dist > this.bestStraight)
			{
				this.bestStraight = dist;
				this.indexStraight[0] = i;
				this.indexStraight[1] = j;
			}
		}
	}
};

Optimizer.prototype.initdmin = function(turnPts)
{
	var nTurnPts = turnPts.length;
	var minimum = this.bestStraight;
	var i;
	var j;
	var d;
	var mini;
	var minj = 0;

	this.dminindex = new Array(nTurnPts * nTurnPts);

	// handle 1st row separately
	for(j=(nTurnPts-1); j>0; j--)
	{
		d = this.dists[0 + nTurnPts * j];

		// d<minimum if equal point before should be found
		if(d < minimum)
		{
			minimum = d;
			minj = j;
		}

		this.dists[j] = minimum; // dmin(0, j) = minimum;
		this.dminindex[nTurnPts * j] = 0; // dmini(0, j) = 0;
		this.dminindex[j] = minj; // dminj(0, j) = minj;
	}

	// inherit following rows from there before
	for(i=1; i<(nTurnPts-1); i++)
	{
		j = (nTurnPts - 1); // handle last col for init of minimum separatelly
		minimum = this.dists[j + nTurnPts * (i - 1)]; // dmin(i - 1, j);
		mini = this.dminindex[(i - 1) + nTurnPts * j]; // dmini(i-1, j);
		minj = this.dminindex[j + nTurnPts * (i - 1)]; // dminj(i-1, j);
		d = this.dists[i + nTurnPts * j];

		if(d<minimum)
		{
			minimum = d;
			mini = i;
			minj = j;
		}

		this.dists[j + nTurnPts * i] = minimum; // dmin(i, j) = minimum;
		this.dminindex[i + nTurnPts * j] = mini; // dmini(i, j) = mini;
		this.dminindex[j + nTurnPts * i] = minj; // dminj(i, j) = minj;

		// these cols from tail to front
		for(j=(nTurnPts-2); j>i; j--)
		{
			d = this.dists[i + nTurnPts * j];

			// current point better then minimum before?
			// d<minimum if equal point before should be found
			if(d < minimum)
			{
				minimum = d;
				mini = i;
				minj = j;
			}

			d = this.dists[j + nTurnPts * (i-1)]; // dmin(i-1, j);

			// is minimum before this col is better?
			if(d < minimum)
			{
				minimum = d;
				mini = this.dminindex[(i - 1) + nTurnPts * j]; // dmini(i-1, j);
				minj = this.dminindex[j + nTurnPts * (i - 1)]; // dminj(i-1, j);
			}

			this.dists[j + nTurnPts * i] = minimum; // dmin(i, j) = minimum;
			this.dminindex[i + nTurnPts * j] = mini; // dmini(i, j) = mini;
			this.dminindex[j + nTurnPts * i] = minj; // dminj(i, j) = minj;
		}
	}
};

Optimizer.prototype.initmaxend = function(turnPts)
{
	var nTurnPts = turnPts.length;
	var w3;
	var i;
	var f;
	var maxf;
	var besti;

	this.maxenddist = new Array(nTurnPts);
	this.maxendpunkt = new Array(nTurnPts);

	for(w3=(nTurnPts - 1); w3>1; w3--)
	{
		maxf = 0;
		besti = (nTurnPts - 1);

		for(i=besti; i>=w3; i--)
		{
			f = this.dists[w3 + nTurnPts * i];

			if(f > maxf)
			{
				maxf = f;
				besti = i;
			}
		}

		this.maxenddist[w3]  = maxf;
		this.maxendpunkt[w3] = besti;
	}
};

Optimizer.prototype.cleanup = function()
{
	this.dists = [];
	this.dminindex = [];
	this.maxenddist = [];
	this.maxendpunkt = [];
};
