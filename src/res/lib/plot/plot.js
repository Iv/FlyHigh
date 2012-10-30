function Plot(plotDiv)
{
	var plot = this;
	var canvas;
	var base;
	var line = [];
	var lines = [];
	var x;
	var y;
	var width;
	var height;

	this.plotDiv = plotDiv;
	this.top = 0;
	this.left = 0;
	this.width = this.plotDiv.getAttribute('width');
	this.height = this.plotDiv.getAttribute('height');
	this.labelHeightX = 18;
	this.labelWidthY = 100;
	this.minValueX = 0;
	this.maxValueX = 0;
	this.minValueY = 0;
	this.maxValueY = 0;
	this.border = 2;
	this.data = null;

	// base
	x = 0; //this.labelWidthY;
	y = 0; //this.labelHeightX;
	width = 0; //(this.width - this.labelWidthY);
	height = 0; // (this.height - 2 * this.labelHeightX);
	this.base = new Context(plotDiv, 'base', x, y, width, height, this.border);

	// x-axis
	x = this.labelWidthY + this.border;
	y = this.height - this.labelHeightX + this.border;
	width = (this.width - this.labelWidthY);
	height = this.labelHeightX;
	this.horLegend = new Legend(plotDiv, 'horleg', x, y, width, height);

	// y-axis
	x = 0;
	y = this.border + this.labelHeightX;
	width = this.labelWidthY - this.border;
	height = (this.getHeight() - 2 * this.labelHeightX);
	this.verLegend = new Legend(plotDiv, 'verleg', x, y, width, height);

	// cursor
	width = 1;
	this.cursor = new Cursor(plotDiv, this.base, 'cursor', width);

	// value
	x = this.labelWidthY + this.border;
	y = 0;
	width = (this.width - this.labelWidthY);
	height = this.labelHeightX;
	this.value = new Value(plotDiv, 'value', x, y, width, height);
}

Plot.prototype.setGeometry = function(left, top, width, height)
{
	var x;
	var y;
	var w;
	var h;

	this.top = left;
	this.left = top;
	this.width = width;
	this.height = height;
	this.plotDiv.style.left = left + 'px';
	this.plotDiv.style.top = top + 'px';
	this.plotDiv.style.width = width + 'px';
	this.plotDiv.style.height = height + 'px';
this.plotDiv.style.backgroundColor = 'red';

	// base
	x = this.labelWidthY;
	y = this.labelHeightX;
	w = (width - this.labelWidthY);
	h = (height - 2 * this.labelHeightX);
	this.base.setGeometry(x, y, w, h);

	// cursor
	this.cursor.resize();

	// x-axis
	x = this.labelWidthY + this.border;
	y = (height - this.labelHeightX + this.border);
	w = (width - this.labelWidthY);
	h = this.labelHeightX;
	this.horLegend.setGeometry(x, y, w, h);

	// y-axis
	x = 0;
	y = this.border + this.labelHeightX;
	w = this.labelWidthY - this.border;
	h = (height - 2 * this.labelHeightX);
	this.verLegend.setGeometry(x, y, w, h);
	
	// value
	x = this.labelWidthY + this.border;
	y = 0;
	w = (width - this.labelWidthY);
	h = this.labelHeightX;
	this.value.setGeometry(x, y, w, h);
};

Plot.prototype.getWidth = function()
{
	return this.plotDiv.style.width;
};

Plot.prototype.getHeight = function()
{
	return this.plotDiv.style.height;
};

Plot.prototype.getPlotAreaWidth = function()
{
	return this.base.getWidth();
};

Plot.prototype.adjustMinMaxX = function(data)
{
	var nData;

	nData = data.length;
	this.minValueX = data[0].valueX;
	this.maxValueX = data[nData - 1].valueX;
};

Plot.prototype.setMinMaxX = function(minX, maxX)
{
	this.minValueX = minX;
	this.maxValueX = maxX;
};

Plot.prototype.setMinMaxY = function(minY, maxY)
{
	this.minValueY = minY;
	this.maxValueY = maxY;
};

Plot.prototype.setUpdateCursorPosCb = function(updatePosCb)
{
	this.cursor.setUpdatePosCb(updatePosCb);
};

Plot.prototype.setValueLabels = function(labels)
{
	this.value.setLabels(labels);
};

Plot.prototype.setValues = function(values)
{
	this.value.setValues(values);
};

Plot.prototype.plot = function(data)
{
	this.data = data;
	this.drawGrid(data);
	this.drawData(data);
	this.cursor.setPos(0);
};

Plot.prototype.drawData = function(data)
{
	var delta;
	var nData;
	var stepX;
	var stepY;
	var x;
	var y;
	var line = [];

	nData = data.length;
	delta = (this.maxValueX - this.minValueX);
	stepX = this.base.getWidth() / delta;
	delta = (this.maxValueY - this.minValueY);
	stepY = this.base.getHeight() / delta;

	for(nr=0; nr<nData; nr++)
	{
		x = (data[nr].valueX - this.minValueX) * stepX;
		y = this.base.getHeight() - (data[nr].valueY - this.minValueY) * stepY;
		line.push({x: x, y: y});
	}

	this.base.strokeLine(line, {color: "#ff0000", width: 1});
};

Plot.prototype.drawGrid = function(data)
{
	var lines = [];
	var line = [];
	var labels = [];
	var date;
	var delta;
	var incValue;
	var minutes;
	var convFactor;
	var value;
	var valueStr;

	// x-grid
	nData = data.length;
	delta = (this.maxValueX - this.minValueX);
	value = this.minValueX;
	date = new Date(value * 1000);

	if(delta <= 3600)
	{
		incValue = 600;
		minutes = Math.floor(date.getUTCMinutes() / 10) * 10;
	}
	else
	{
		incValue = 3600;
		minutes = 0;
	}

	value = (Date.UTC(date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate(), date.getUTCHours(), minutes, 0) / 1000) + incValue;
	convFactor = this.base.getWidth() / delta;

	while(value < this.maxValueX)
	{
		// line
		line = [];
		x = (value - this.minValueX) * convFactor;
		line.push({x: x + 0.5, y: 0 + 0.5});
		line.push({x: x + 0.5, y: this.getHeight() + 0.5});
		lines.push(line);

		// label
		date = new Date(value * 1000);
		valueStr = date.getHours() + ":" + date.getMinutes();

		if(date.getMinutes() < 10)
		{
			valueStr += '0';
		}

		labels.push({pos: x, value: valueStr});
		value += incValue;
	}

	this.base.strokeLines(lines, {color: "#545454", width: 0.2});
	this.horLegend.setLabels(labels);

	// y-grid
	delta = (this.maxValueY - this.minValueY);

	if(delta < 1000)
	{
		incValue = 100;
	}
	else if(delta < 2000)
	{
		incValue = 500;
	}
	else if(delta < 4000)
	{
		incValue = 1000;
	}

	value = (this.minValueY + incValue);
	convFactor = this.base.getHeight() / delta;
	lines = [];
	labels = [];

	while(value < this.maxValueY)
	{
		line = [];
		y = this.base.getHeight() - (value - this.minValueY) * convFactor;
		line.push({x: 0 + 0.5, y: y + 0.5});
		line.push({x: this.base.getWidth() + 0.5, y: y + 0.5});
		lines.push(line);
		labels.push({pos: y, value: value});
		value += incValue;
	}

	this.base.strokeLines(lines, {color: "#545454", width: 0.2});
	this.verLegend.setLabels(labels);
};
