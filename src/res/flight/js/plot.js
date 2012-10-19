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

function pl_sayHello()
{
	alert("hello");
}

var ChartMarginLeft = 20;
var ChartMarginRight = 38;
var LeftWidth = 280;
var LegendHeight = 20;

function Plot(map)
{
	var plot = this;

	this.map = map;
	this.changeCallback = null;

	this.chartDiv = document.getElementById("chart");
	this.plotDiv = document.getElementById("plot");
	this.legendDiv = document.getElementById("legend");
	this.cursorDiv = document.getElementById("cursor").style;
	this.altDiv = document.getElementById("alt");
	this.sogDiv = document.getElementById("sog");
	this.varioDiv = document.getElementById("vario");
	this.timeDiv = document.getElementById("time");

	this.flight = null;
	this.width = 0;
	this.height = 0;
	this.chart = new Chart(this.chartDiv);

	this.chartDiv.addEventListener('mousemove', function(event) {pl_mousemove(event, plot);}, true);
}

Plot.prototype.getMap = function()
{
	return this.map;
}

Plot.prototype.setSize = function(width, height)
{
	var div;
	var top;

	this.width = width;
	this.height = height;
	top = this.getMap().getDiv().style.height;

	// plot
	div = this.plotDiv;
	div.style.top = top + "px";
	div.style.width = width + "px";

	// chart
	div = this.chartDiv;
	div.style.width = width + "px";
	div.style.height = (height - LegendHeight) + "px";

	// legend
	div = this.legendDiv;
	div.style.width = width + "px";
	div.style.height = LegendHeight + "px";

	// redraw chart
	this.chart.draw();
}

Plot.prototype.getWidth = function()
{
	return this.width;
}

Plot.prototype.getHeigth = function()
{
	return this.height;
}

Plot.prototype.setChangeCallback = function(callback)
{
	this.changeCallback = callback;
}

Plot.prototype.setFlight= function(flight)
{
	this.flight = flight;
}

Plot.prototype.show = function()
{
	HorLabelNum = 8;
	VerLabelNum = 5;

	var nofPts;
	var index = 0;
	var roundIdx;
	var step;
	var labelNr;
	var labels = [];
	var timeList;

	if(this.flight.getMaxAlt() > 999)
	{
		ChartMarginLeft = 34;
	}
	else if(this.flight.getMaxAlt() > 99)
	{
		ChartMarginLeft = 27;
	}
	else if(this.flight.getMaxAlt() > 9)
	{
		ChartMarginLeft = 20;
	}

	timeList = this.flight.getTimeList();
	nofPts = timeList.length;
	step = (nofPts - 1) / (HorLabelNum - 1);

	for(labelNr=0; labelNr<HorLabelNum; labelNr++)
	{
		roundIdx = Math.round(index);
		labels.push(timeList[roundIdx].substr(0, 5));
		index += step;
	}

	// add some spaces to the last legend
	labels[labels.length - 1] += "   ___";

	// chart
	this.chart.setDefaultType(CHART_LINE);
	this.chart.setGridDensity(HorLabelNum, VerLabelNum);
	this.chart.setVerticalRange(this.flight.getMinAlt(), this.flight.getMaxAlt());
	this.chart.setShowLegend(false);
	this.chart.setLabelPrecision(0);
	this.chart.setHorizontalLabels(labels);
	this.chart.add("Altitude", "#ff3333", this.flight.getAltList());
//				chart.add('Ground Elev',  '#C0AF9C', FlightData.elevGnd, CHART_AREA);
	this.chart.draw();

	// glider
	this.flight.moveGliderTo(0);
	this.setLegend(0);
}

Plot.prototype.setTimeLine = function(posX)
{
	this.cursorDiv.left = posX + "px";
}

Plot.prototype.setGlider = function(posX)
{
	var index;

	// legend
	index = (posX - ChartMarginLeft) * (this.flight.getTimeList().length - 1) / (this.getWidth() - ChartMarginRight);
	index = Math.round(index);
	this.setLegend(index);

	// glider
	index = (posX - ChartMarginLeft) * (this.flight.getTrackPts().length - 1) / (this.getWidth() - ChartMarginRight);
	index = Math.round(index);
	this.flight.moveGliderTo(index);
}

Plot.prototype.setLegend = function(index)
{
	this.altDiv.innerHTML = "ALT: " + flight.getAltAt(index) + " m";
	this.sogDiv.innerHTML = "SOG: " + flight.getSogAt(index) + " km/h";
	this.varioDiv.innerHTML = "Vario: " + flight.getVarioAt(index) + " m/s";
	this.timeDiv.innerHTML = "Time: " + flight.getTimeAt(index);
}

function pl_mousemove(event, plot)
{
	var posX = (event.pageX - LeftWidth);

	if(posX < ChartMarginLeft)
	{
		posX = ChartMarginLeft;
	}
	else if(posX > (plot.getWidth() + ChartMarginLeft - ChartMarginRight))
	{
		posX = (plot.getWidth() + ChartMarginLeft - ChartMarginRight);
	}

	plot.setTimeLine(posX);
	plot.setGlider(posX);
}
