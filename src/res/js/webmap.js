var map;

function initialize(lat, lon, zoom)
{
	if(GBrowserIsCompatible())
	{
		map = new GMap2(document.getElementById("map"));
		map.setCenter(new GLatLng(lat, lon), zoom);
		map.setMapType(G_PHYSICAL_MAP);
	}
}

function getPointFromLatLon(lat, lon)
{
	var proj;
	var west;
	var north;
	var nwPoint;
	var point;

	proj = map.getCurrentMapType().getProjection();
	west = map.getBounds().getSouthWest().lng();
	north = map.getBounds().getNorthEast().lat();
	nwPoint = proj.fromLatLngToPixel(new GLatLng(north, west), map.getZoom());
	point = proj.fromLatLngToPixel(new GLatLng(lat, lon), map.getZoom());

	return new Array(point.x - nwPoint.x, point.y - nwPoint.y);
}

function getLatLonFromPoint(x, y)
{
	var proj;
	var west;
	var north;
	var nwPoint;
	var gLatLng;

	west = map.getBounds().getSouthWest().lng();
	north = map.getBounds().getNorthEast().lat();
	proj = map.getCurrentMapType().getProjection();
	nwpoint = proj.fromLatLngToPixel(new GLatLng(north, west), map.getZoom());
	gLatLng = proj.fromPixelToLatLng(new GPoint(nwpoint.x + x,
						nwpoint.y + y), map.getZoom());

	return new Array(gLatLng.lat(), gLatLng.lng());
}

function zoomIn()
{
	map.zoomIn();
}

function zoomOut()
{
	map.zoomOut();
}

function zoomTo(north, east, south, west)
{
	var bounds;
	var sw;
	var ne;

	sw = new GLatLng(south, west);
	ne = new GLatLng(north, east);
	bounds = new GLatLngBounds(sw, ne);
	map.setCenter(new GLatLng(bounds.getCenter().lat(), bounds.getCenter().lng()),
				map.getBoundsZoomLevel(bounds));
}

function setPolyLine(encPoints, encLevels, weight, color)
{
	var encPolyLine;

	encPolyLine = new GPolyline.fromEncoded({
			color: color,
			weight: weight,
			opacity: 1.0,
			points: encPoints,
			levels: encLevels,
			zoomFactor: 2,
			numLevels: 18
	});

	map.addOverlay(encPolyLine);
}

function setMarker(lat, lon, image)
{
	var latLng;
	var marker;
	var icon;
	var markerOpts;

	icon = new GIcon(G_DEFAULT_ICON);
	icon.image = image;
	latLng = new GLatLng(lat, lon);
	markerOpts = {icon: icon};
	marker = new GMarker(latLng, markerOpts);
	map.addOverlay(marker);
}

function setSize(w, h)
{
	var map_div;

	map_div = document.getElementById("map");
	map_div.style.width = w + "px";
	map_div.style.height = h + "px";
	map.checkResize();
}

function setCenter(lat, lon)
{
	map.setCenter(new GLatLng(lat, lon));
}