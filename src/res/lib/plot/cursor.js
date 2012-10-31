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
	this.updateClickCb = null;

	base.getParent().addEventListener('mousemove', function(event) {cu_mousemove(event, cursor);}, false);
	base.getParent().addEventListener('mousedown', function(event) {cu_mousedown(event, cursor);}, false);
}

function on_canvas_click(ev)
{
	alert("click");
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
	top = this.getBase().getBorder();
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
	this.cursorDiv.style.left = (this.getBase().getLeft() + pos + this.getBase().getBorder()) + 'px';

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

Cursor.prototype.setUpdateClickCb = function(updateClickCb)
{
	this.updateClickCb = updateClickCb;
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

	pos = getCursorPos(event, cursor);

	if(pos >= 0)
	{
		cursor.setPos(pos);
	}
}

function cu_mousedown(event, cursor)
{
	var pos;

	if(cursor.updateClickCb !== null)
	{
		pos = getCursorPos(event, cursor);

		if(pos >= 0)
		{
			cursor.updateClickCb(pos);
		}
	}
}

function getCursorPos(event, cursor)
{
	var MysteriousOffset = 7;
	var posX;
	var posY;
	var pos = -1;

	posX = (event.pageX - cursor.parentDiv.offsetLeft - cursor.getBase().getLeft()) - MysteriousOffset;
	posY = (event.pageY - cursor.parentDiv.offsetTop - cursor.getBase().getTop() - cursor.base.getBorder());

	if(((posX >= 0) && (posX < cursor.getBase().getWidth())) &&
		((posY >= 0) && (posY < cursor.getBase().getHeight())))
	{
		pos = posX;
	}

	return pos;
}
