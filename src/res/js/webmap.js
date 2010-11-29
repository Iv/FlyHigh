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

function zoomIn()
{
	map.zoomIn();
}

function zoomOut()
{
	map.zoomOut();
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

function setMapSize(w, h)
{
	var map_div;

	map_div = document.getElementById("map");
	map_div.style.width = (w - 280) + "px";
	map_div.style.height = h + "px";
	map.checkResize();
}
