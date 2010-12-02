function setTurnPointList(tps)
{
	var tpInput;
	
	tpInput = document.getElementById("defaultTurnpoints");
	tpInput.value = tps;
}

function setWayPointList(encTrack, encLevels, weight, color)
{
	var polyline;

	polyline = new GPolyline.fromEncoded({
			color: color,
			weight: weight,
			opacity: 1.0,
			points: encTrack,
			levels: encLevels,
			zoomFactor: 2,
			numLevels: 18
	});

	map.addOverlay(polyline);
}

function setTurnPointsDragable(drag)
{
	TurnPointsDragable = drag;
}

function setLocation(loc)
{
	var locInput;
	
	locInput = document.getElementById("defaultLocation");
	locInput.value = loc;
}

function setFlightType(flightType)
{
	var typeInput;
	
	typeInput = document.getElementById("defaultFlightType");
	typeInput.value = flightType;
}

function setMapSize(w, h)
{
	var map_div;

	map_div = document.getElementById("map");
	map_div.style.width = (w - 280) + "px";
	map_div.style.height = h + "px";
	map.checkResize();
}

function zoomIn()
{
	map.zoomIn();
}

function zoomOut()
{
	map.zoomOut();
}