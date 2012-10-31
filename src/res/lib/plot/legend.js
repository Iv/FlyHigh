function Legend(parentDiv, id)
{
	this.parentDiv = parentDiv;
	this.id = id;
	this.left = 0;
	this.top = 0;
	this.width = 0;
	this.height = 0;
	this.horizontal = true;
	this.legendDiv = this.createEmptyLegend(id);
	this.divs = [];
	this.labelH = 15;
}

Legend.prototype.setGeometry = function(left, top, width, height)
{
	this.horizontal = (width > height);
	this.left = left;
	this.top = top;
	this.width = width;
	this.height = height;
	this.legendDiv.style.left = left + 'px';
	this.legendDiv.style.top = top + 'px';
	this.legendDiv.style.width = width + 'px';
	this.legendDiv.style.height = height + 'px';
};

Legend.prototype.getWidth = function()
{
	return this.width;
};

Legend.prototype.getHeight = function()
{
	return this.height;
};

Legend.prototype.setLabels = function(labels)
{
	var labelW;
	var labelH;
	var labelNr;
	var label;
	var div;
	var pos;

	// clean labels
	for(labelNr=0; labelNr<this.divs.length; labelNr++)
	{
		div = this.divs[labelNr];
		this.legendDiv.removeChild(div);
	}

	this.divs = [];

	// create labels
	if(this.horizontal)
	{
		labelW = (this.getWidth() / labels.length);
		labelH = this.getHeight();

		for(labelNr=0; labelNr<labels.length; labelNr++)
		{
			label = labels[labelNr];
			div = document.createElement('div');
			div.style.position = 'absolute';

			// overlapping labels
			if((label.pos + labelW / 2) > this.getWidth())
			{
				div.style.textAlign = 'right';
				pos = this.getWidth() - labelW;
			}
			else
			{
				div.style.textAlign = 'center';
				pos = (label.pos - labelW / 2);
			}

			div.style.top = 0 + 'px';
			div.style.left = pos.toFixed(0) + 'px';
			div.style.width = labelW.toFixed(0) + 'px';
			div.style.height = labelH + 'px';
			div.innerHTML = label.value;
			this.legendDiv.appendChild(div);
			this.divs.push(div);
		}
	}
	else
	{
		labelW = this.getWidth();
		labelH = this.labelH;

		for(labelNr=0; labelNr<labels.length; labelNr++)
		{
			label = labels[labelNr];
			div = document.createElement('div');
			div.style.position = 'absolute';
			div.style.top = (label.pos - labelH / 2).toFixed(0) + 'px';
			div.style.left = 0  + 'px';
			div.style.width = labelW + 'px';
			div.style.height = labelH + 'px';
			div.style.textAlign = 'right';
			div.innerHTML = label.value;
			this.legendDiv.appendChild(div);
			this.divs.push(div);
		}
	}
};

Legend.prototype.createEmptyLegend = function(id)
{
	var div;

	div = document.createElement('div');
	div.setAttribute("id", id);
	div.style.position = 'absolute';
	this.parentDiv.appendChild(div);

	return div;
};
