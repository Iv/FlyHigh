function Value(parentDiv, id, left, top, width, height)
{
	this.parentDiv = parentDiv;
	this.id = id;
	this.left = left;
	this.top = top;
	this.width = width;
	this.height = height;
	this.horizontal = (width > height);
	this.labels = [];
	this.legendDiv = this.createEmptyLegend(id, left, top, width, height);
	this.labelH = 15;
	this.divs = [];
	this.labels = null;
}

Value.prototype.setGeometry = function(left, top, width, height)
{
	var labelW;
	var pos = 0;

	this.left = left;
	this.top = top;
	this.width = width;
	this.height = height;
	this.legendDiv.style.left = left + 'px';
	this.legendDiv.style.top = top + 'px';
	this.legendDiv.style.width = width + 'px';
	this.legendDiv.style.height = height + 'px';

	// resize labels
	if(this.divs.length > 0)
	{
		labelW = (width / this.divs.length);

		for(nr=0; nr<this.divs.length; nr++)
		{
			div = this.divs[nr];
			div.style.top = 0 + 'px';
			div.style.left = pos.toFixed(0) + 'px';
			div.style.width = labelW.toFixed(0) + 'px';
			div.style.height = height + 'px';
			pos += labelW;
		}
	}
};

Value.prototype.getWidth = function()
{
	return this.width;
};

Value.prototype.getHeight = function()
{
	return this.height;
};

Value.prototype.setLabels = function(labels)
{
	var labelW;
	var labelH;
	var label;
	var div;
	var nr;
	var pos = 0;

	// clean labels
	for(nr=0; nr<this.divs.length; nr++)
	{
		div = this.divs[nr];
		this.legendDiv.removeChild(div);
	}

	this.divs = [];
	this.labels = labels;
	labelW = (this.getWidth() / labels.length);
	labelH = this.getHeight();

	for(nr=0; nr<labels.length; nr++)
	{
		label = labels[nr];
		div = document.createElement('div');
		div.style.position = 'absolute';
		div.style.top = 0;
		div.style.left = pos.toFixed(0);
		div.style.width = labelW.toFixed(0);
		div.style.height = labelH;
		div.style.textAlign = 'left';
		this.legendDiv.appendChild(div);
		this.divs.push(div);
		pos += labelW;
	}
};

Value.prototype.setValues = function(values)
{
	var div;
	var nr;
	var length;

	length = this.divs.length;

	if(length > values.length)
	{
		length = values.length;
	}

	for(nr=0; nr<length; nr++)
	{
		div = this.divs[nr];
		div.innerHTML = this.labels[nr] + ": " + values[nr];
	}
};

Value.prototype.createEmptyLegend = function(id, left, top, width, height)
{
	var div;

	div = document.createElement('div');
	div.setAttribute("id", id);
	div.style.position = 'absolute';
	div.style.top = top;
	div.style.left = left;
	div.style.width = width;
	div.style.height = height;
	this.parentDiv.appendChild(div);

	return div;
};
