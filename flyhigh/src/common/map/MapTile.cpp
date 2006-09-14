#include "MapTile.h"

MapTile::MapTile(const QString &name, uint sizeX, uint sizeY)
{
	if(!m_image.load(name, 0))
	{
		m_image.create(sizeX, sizeY, 32);
		m_image.fill(QColor(Qt::lightGray).pixel());
	}

	m_scaledImage = m_image;
}

MapTile::~MapTile()
{
}

int MapTile::width()
{
	return m_scaledImage.width();
}

int MapTile::height()
{
	return m_scaledImage.height();
}

void MapTile::setZoom(int z)
{
	int scale;

	if(z < 0)
	{
		scale = -z * 2;
		m_scaledImage = m_image.smoothScale(m_image.width() * scale, m_image.height() * scale);
	}
	else if(z > 0)
	{
		scale = z * 2;
		m_scaledImage = m_image.smoothScale(m_image.width() / scale, m_image.height() / scale);
	}
	else
	{
		m_scaledImage = m_image;
	}
}

QImage& MapTile::image()
{
	return m_scaledImage;
}
