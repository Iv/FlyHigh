#include "Map.h"

Map::Map(const QString &tilePath)
{
	m_zoom = 0;
	m_tilePath = tilePath;
}

Map::~Map()
{
	clean();
}

bool Map::loadMap(double north, double east, double south, double west)
{
	return false;
}

void Map::LLtoPix(double lat, double lon, QPoint &pt)
{
}

void Map::pixToLL(const QPoint &pt, double &lat, double &lon)
{
}

void Map::LLrect(double &north, double &east, double &south, double &west)
{
}

void Map::pixRect(QRect &rect)
{
	int rows;
	int cols = 0;
	int pixX;
	int pixY;

	rows = m_tileMatrix.size();

	if(rows > 0)
	{
		cols = m_tileMatrix[0].size();
		pixTileSize(pixX, pixY);
	}

	rect.setTop(0);
	rect.setLeft(0);
	rect.setRight(cols * pixX);
	rect.setBottom(rows * pixY);
}

void Map::setZoom(int z)
{
	MapTile *pTile;
	int rowNr;
	int rows;
	int colNr;
	int cols;

	rows = m_tileMatrix.size();

	for(rowNr=0; rowNr<rows; rowNr++)
	{
		cols = m_tileMatrix[rowNr].size();

		for(colNr=0; colNr<cols; colNr++)
		{
			pTile = m_tileMatrix[rowNr][colNr];

			if(pTile != NULL)
			{
				pTile->setZoom(z);
			}
		}
	}

	m_zoom = z;
}

int Map::zoom()
{
	return m_zoom;
}

Map::TileMatrix& Map::tileMatrix()
{
	return m_tileMatrix;
}

QString& Map::tilePath()
{
	return m_tilePath;
}

void Map::clean()
{
	MapTile *pTile;
	int rowNr;
	int rows;
	int colNr;
	int cols;

	rows = m_tileMatrix.size();

	for(rowNr=0; rowNr<rows; rowNr++)
	{
		cols = m_tileMatrix[rowNr].size();

		for(colNr=0; colNr<cols; colNr++)
		{
			pTile = m_tileMatrix[rowNr][colNr];

			if(pTile != NULL)
			{
				delete pTile;
			}
		}

		m_tileMatrix[rowNr].clear();
	}

	m_tileMatrix.clear();
}

void Map::pixTileSize(int &pixX, int &pixY)
{
	pixX = 0;
	pixY = 0;
}

void Map::cancel()
{
}
