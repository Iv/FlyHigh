#ifndef SwissMap100_h
#define SwissMap100_h

#include <Map.h>

class SwissMap100: public Map
{
	public:
		SwissMap100(const QString &tilePath);

		bool loadMap(double n, double e, double s, double w);
		void LLrect(double &n, double &e, double &s, double &w);
		void pixTileSize(int &pixX, int &pixY);

		void cancel();
		
	private:
		static const uint max_swiss_n;
		static const uint max_swiss_w;
		QRect m_tilesRect;
		bool m_cancel;

		bool loadSwissMap(uint swiss_n, uint swiss_e, uint swiss_s, uint swiss_w);
		bool loadTiles(QRect &tilesRect);
		bool tileName(uint x, uint y, QString &name);
};

#endif
