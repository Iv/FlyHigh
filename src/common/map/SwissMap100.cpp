#include <math.h>
#include "SwissMap100.h"
#include "SwissGrid.h"

#define TILES_MAX_X 165
#define TILES_MAX_Y 94
#define TILE_PIX_X 256
#define TILE_PIX_Y 256
#define PIX_X_M 10
#define PIX_Y_M 10

/*
	The tiles in Swiss Map are arranged in following
	order. # means an image exists, a * represents a
	blank tile. Origin (0; 0) is the blank tile top
	left.

	***###########***** Block 0 (110 pics on 18 lines)
	##############***** Block 1 (137 pics on 19 lines)
	################### Block 2 (165 pics on 38 lines)
	###################
	###########******** Block 3 (110 pics on 19 lines)
*/

const uint SwissMap100::max_swiss_n = 302000;
const uint SwissMap100::max_swiss_w = 480000;

SwissMap100::SwissMap100(const QString &tilePath)
	:Map(tilePath)
{
}

bool SwissMap100::loadMap(double n, double e, double s, double w)
{
	double northing;
	double easting;
	uint swiss_n;
	uint swiss_e;
	uint swiss_s;
	uint swiss_w;

	LLtoSwissGrid(n, e, northing, easting);
	swiss_n = (uint)round(northing);
	swiss_e = (uint)round(easting);
	LLtoSwissGrid(s, w, northing, easting);
	swiss_s = (uint)round(northing);
	swiss_w = (uint)round(easting);

	return loadSwissMap(swiss_n, swiss_e, swiss_s, swiss_w);
}

void SwissMap100::LLrect(double &n, double &e, double &s, double &w)
{
	uint swiss_n;
	uint swiss_e;

	swiss_n = max_swiss_n - m_tilesRect.top() * (TILE_PIX_Y * PIX_Y_M);
	swiss_e = max_swiss_w + m_tilesRect.left() * (TILE_PIX_X * PIX_X_M);
	SwissGridtoLL(swiss_n, swiss_e, n, w);

	swiss_n = max_swiss_n - (m_tilesRect.bottom() + 1) * (TILE_PIX_Y * PIX_Y_M);
	swiss_e = max_swiss_w + (m_tilesRect.right() + 1) * (TILE_PIX_X * PIX_X_M);
	SwissGridtoLL(swiss_n, swiss_e, s, e);
}

void SwissMap100::pixTileSize(int &pixX, int &pixY)
{
	int z = zoom();
	int scale;

	if(z < 0)
	{
		scale = -z * 2;
		pixX = TILE_PIX_X * scale;
		pixY = TILE_PIX_Y * scale;
	}
	else if(z > 0)
	{
		scale = z * 2;
		pixX = TILE_PIX_X / scale;
		pixY = TILE_PIX_Y / scale;
	}
	else
	{
		pixX = TILE_PIX_X;
		pixY = TILE_PIX_Y;
	}
}

void SwissMap100::cancel()
{
	m_cancel = true;
}

bool SwissMap100::loadSwissMap(uint swiss_n, uint swiss_e, uint swiss_s, uint swiss_w)
{
	QRect tilesRect;

	tilesRect.setTop((max_swiss_n - swiss_n) / (TILE_PIX_Y * PIX_Y_M) - 1);
	tilesRect.setRight((swiss_e - max_swiss_w) / (TILE_PIX_X * PIX_X_M) + 1);
	tilesRect.setBottom((max_swiss_n - swiss_s) / (TILE_PIX_Y * PIX_Y_M) + 1);
	tilesRect.setLeft((swiss_w - max_swiss_w) / (TILE_PIX_Y * PIX_Y_M) - 1);

	return loadTiles(tilesRect);
}

bool SwissMap100::loadTiles(QRect &tilesRect)
{
	QString name;
	TileRow tileRow;
	MapTile *pTile;
	int rowNr;
	int colNr;
	int tilesLoaded = 0;

	if(tilesRect.top() < 0) tilesRect.setTop(0);
	if(tilesRect.bottom() > TILES_MAX_Y) tilesRect.setBottom(TILES_MAX_Y);
	if(tilesRect.left() < 0) tilesRect.setLeft(0);
	if(tilesRect.right() > TILES_MAX_X) tilesRect.setRight(TILES_MAX_X);

	clean();
	m_cancel = false;

	for(rowNr=tilesRect.top(); rowNr<=tilesRect.bottom(); rowNr++)
	{
		for(colNr=tilesRect.left(); colNr<=tilesRect.right(); colNr++)
		{
			if(m_cancel)
			{
				clean();
				return false;
			}

			if(tileName(colNr, rowNr, name))
			{
				pTile = new MapTile(Map::tilePath() + name, TILE_PIX_X, TILE_PIX_Y);
			}
			else
			{
				pTile = NULL;
			}
			
			emit progress(tilesLoaded * 100 / (tilesRect.width() * tilesRect.height()));
			tileRow.push_back(pTile);
			tilesLoaded++;
		}
		
		tileMatrix().push_back(tileRow);
		tileRow.clear();
	}

	m_tilesRect = tilesRect;
	
	return true;
}

bool SwissMap100::tileName(uint x, uint y, QString &name)
{
	const uint picsBlock0 = 110;
	const uint startPadBlock0 = 27;
	const uint picsBlock1 = 137;
	const uint picsBlock2 = 165;
	const uint picsBlock3 = 110;

	const uint linesBlock0 = 18;
	const uint linesBlock1 = 19;
	const uint linesBlock2 = 38;
	const uint linesBlock3 = 19;

	const uint baseBlock0 = 0;
	const uint baseBlock1 = baseBlock0 + picsBlock0 * linesBlock0;
	const uint baseBlock2 = baseBlock1 + picsBlock1 * linesBlock1;
	const uint baseBlock3 = baseBlock2 + picsBlock2 * linesBlock2;

	uint lineOffset;
	int imageNr = -1;
	bool exist;

	if((x < TILES_MAX_X) && y < TILES_MAX_Y)
	{
		if(y < linesBlock0)
		{//Block 0
			if((x > startPadBlock0) && (x < startPadBlock0 + picsBlock0))
			{
				lineOffset = y;
				imageNr = baseBlock0 + x - startPadBlock0 + lineOffset * picsBlock0;
			}
		}
		else if(y < linesBlock0 + linesBlock1)
		{// Block 1
			if(x < picsBlock1)
			{
				lineOffset = y - linesBlock0;
				imageNr = baseBlock1 + x + lineOffset * picsBlock1;
			}
		}
		else if(y < linesBlock0 + linesBlock1 + linesBlock2)
		{// Block 2
			if(x < picsBlock2)
			{
				lineOffset = y - linesBlock0 - linesBlock1;
				imageNr = baseBlock2 + x + lineOffset * picsBlock2;
			}
		}
		else if(y < linesBlock0 + linesBlock1 + linesBlock2 + linesBlock3)
		{// Block 3
			if(x < picsBlock3)
			{
				lineOffset = y - linesBlock0 - linesBlock1 - linesBlock2;
				imageNr = baseBlock3 + x + lineOffset * picsBlock3;
			}
		}
	}

	exist = (imageNr >= 0);

	if(exist)
	{
		name.sprintf("/%05d.ppm", imageNr);
	}

	return exist;
}
