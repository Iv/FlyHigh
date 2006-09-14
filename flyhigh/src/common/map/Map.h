#ifndef Map_h
#define Map_h

#include <vector>
#include <qobject.h>
#include <qrect.h>
#include <qstring.h>
#include "MapTile.h"

class Map: public QObject
{
	Q_OBJECT
	public:
		typedef std::vector<MapTile*> TileRow;
		typedef std::vector<TileRow> TileMatrix;

		Map(const QString &tilePath);
		virtual ~Map();

		virtual bool loadMap(double n, double e, double s, double w);
		virtual void LLrect(double &n, double &e, double &s, double &w);
		virtual void pixRect(QRect &rect);
		void LLtoPix(double lat, double lon, QPoint &pt);
		void pixToLL(const QPoint &pt, double &lat, double &lon);
		virtual void setZoom(int z); // < 0 shrink; 0 nothing; > 0 enlarge
		int zoom();
		TileMatrix& tileMatrix();
		QString& tilePath();
		void clean();
		virtual void pixTileSize(int &pixX, int &pixY);

	signals:
		void progress(int percent);
		
	public slots:
		virtual void cancel();

	private:
		int m_zoom;
		TileMatrix m_tileMatrix;
		QString m_tilePath;

		void pixRatio(double &n, double &w, double &rX, double &rY);
};

#endif
