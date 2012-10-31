function Context(parentDiv, id, border)
{
	this.parentDiv = parentDiv;
	this.id = id;
	this.left = 0;
	this.top = 0;
	this.width = 0;
	this.height = 0;
	this.border = border;
	this.canvas = this.createCanvas(id, border);
	this.context = this.canvas.getContext('2d');
}

Context.prototype.setGeometry = function(left, top, width, height)
{
	this.left = left;
	this.top = top;
	this.width = width;
	this.height = height;
	this.canvas.style.left = left + 'px';
	this.canvas.style.top = top + 'px';
	this.canvas.width = width;
	this.canvas.height = height;
};

Context.prototype.getLeft = function()
{
	return this.left;
};

Context.prototype.getTop = function()
{
	return this.top;
};

Context.prototype.getWidth = function()
{
	return this.width;
};

Context.prototype.getHeight = function()
{
	return this.height;
};

Context.prototype.getBorder = function()
{
	return this.border;
};

Context.prototype.getCanvas = function()
{
	return this.canvas;
};

Context.prototype.createCanvas = function(id, border)
{
	var canvas;

	canvas = document.createElement('canvas');
	canvas.setAttribute("id", id);
	canvas.style.position = 'absolute';
	canvas.style.border = border + "px solid black";
	canvas.style.backgroundColor = 'white';
	this.parentDiv.appendChild(canvas);

	return canvas;
};

Context.prototype.fillRect = function(left, top, width, height, style)
{
	this.context.fillStyle = style.color;
	this.context.fillRect(left, top, width, height);
};

Context.prototype.strokeRect = function(left, top, width, height, style)
{
	this.context.lineWidth = style.width;
	this.context.strokeStyle = style.color;
	this.context.strokeRect(left, top, width, height);
};

Context.prototype.clearRect = function(left, top, width, height)
{
	this.context.clearRect(left, top, width, height);
};

Context.prototype.strokeLine = function(line, style)
{
	var ptNr;

	this.context.lineWidth = style.width;
	this.context.strokeStyle = style.color;
	this.context.beginPath();
	this.context.moveTo(line[0].x, line[0].y);

	for(ptNr=1; ptNr<line.length; ptNr++)
	{
		this.context.lineTo(line[ptNr].x, line[ptNr].y);
	}

	this.context.stroke();
};

Context.prototype.strokeLines = function(lines, style)
{
	var line;
	var lineNr;
	var ptNr;

	this.context.lineWidth = style.width;
	this.context.strokeStyle = style.color;
	this.context.beginPath();

	for(lineNr=0; lineNr<lines.length; lineNr++)
	{
		line = lines[lineNr];
		this.context.moveTo(line[0].x, line[0].y);

		for(ptNr=1; ptNr<line.length; ptNr++)
		{
			this.context.lineTo(line[ptNr].x, line[ptNr].y);
		}
	}

	this.context.stroke();
};

Context.prototype.save = function()
{
	this.context.save();
};

Context.prototype.translate = function(x, y)
{
	this.context.translate(x, y);
};

Context.prototype.restore = function()
{
	this.context.restore();
};
