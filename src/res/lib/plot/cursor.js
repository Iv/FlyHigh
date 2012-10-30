function Cursor(parentDiv, base, id, width)
{
	var cursor = this;

	this.parentDiv = parentDiv;
	this.base = base;
	this.id = id;
	this.cursorDiv = this.createCursor(id, 0, base.getBorder(), width, base.getHeight());
	this.labelH = 15;
	this.base = base;
	this.pos = 0;
	this.width = width;
	this.updatePosCb = null;

	base.getCanvas().addEventListener('mousemove', function(event) {cu_mousemove(event, cursor);}, false);
}

Cursor.prototype.resize = function()
{
	var top;
	var left;
	var width;
	var height;
	var div;
	
	div = this.cursorDiv;
	left = this.getBase().getBorder();
	top = 0;
	width = this.width;
	height = this.getBase().getHeight();
	div.style.left = (left + this.getBase().getLeft() + this.getBase().getBorder()) + 'px';
	div.style.top = (top + this.getBase().getTop()) + 'px';
	div.style.width = width + 'px';
	div.style.height = height + 'px';
};

Cursor.prototype.setPos = function(pos)
{
	this.pos = pos;
	this.cursorDiv.style.left = (this.getBase().getLeft() + this.getBase().getBorder() + pos) + 'px';

	if(this.updatePosCb !== null)
	{
		this.updatePosCb(pos);
	}
};

Cursor.prototype.getPos = function()
{
	return this.pos;
};

Cursor.prototype.setUpdatePosCb = function(updatePosCb)
{
	this.updatePosCb = updatePosCb;
};

Cursor.prototype.getBase = function()
{
	return this.base;
};

Cursor.prototype.createCursor = function(id, left, top, width, height)
{
	var div;

	div = document.createElement('div');
	div.setAttribute("id", id);
	div.style.position = 'absolute';
	div.style.top = (top + this.getBase().getTop());
	div.style.left = (left + this.getBase().getLeft() + this.getBase().getBorder());
	div.style.width = width;
	div.style.height = height;
	div.style.backgroundColor = "#00ff00";
	this.parentDiv.appendChild(div);

	return div;
};

function cu_mousemove(event, cursor)
{
	var pos;

	pos = (event.pageX - cursor.parentDiv.offsetLeft - cursor.getBase().getLeft());

	if(pos < cursor.getBase().getWidth())
	{
		cursor.setPos(pos);
	}
}
