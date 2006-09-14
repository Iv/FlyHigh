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

bool Map::loadMap(double n, double e, double s, double w)
{
	(void)n;
	(void)e;
	(void)s;
	(void)w;

	return false;
}

void Map::LLtoPix(double lat, double lon, QPoint &pt)
{
	double rX;
	double rY;
	double x;
	double y;
	double n;
	double w;

	pixRatio(n, w, rX, rY);
	x = (lon - w) * rX;
	y = (n - lat) * rY;

	pt.setX((int)x);
	pt.setY((int)y);
}

void Map::pixToLL(const QPoint &pt, double &lat, double &lon)
{
	double rX;
	double rY;
	double x;
	double y;
	double n;
	double w;

	pixRatio(n, w, rX, rY);
	x = (double)(pt.x()) / rX;
	y = (double)(pt.y()) / rY;

	lon = w + x;
	lat = n - y;
}


void Map::LLrect(double &n, double &e, double &s, double &w)
{
	(void)n;
	(void)e;
	(void)s;
	(void)w;
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

void Map::pixRatio(double &n, double &w, double &rX, double &rY)
{
	double e;
	double s;
	QRect rect;

	pixRect(rect);
	LLrect(n, e, s, w);

	rX = (double)(rect.width()) / (e - w);
	rY = (double)(rect.height()) / (n - s);
}
