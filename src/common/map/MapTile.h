#ifndef MapTile_h
#define MapTile_h

#include <QImage>

class MapTile
{
	public:
		MapTile(const QString &name, uint sizeX, uint sizeY);
		~MapTile();

		int width();
		int height();
		void setZoom(int z);
		QImage& image();

	private:
		QImage m_image;
		QImage m_scaledImage;
};

#endif
