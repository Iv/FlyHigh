/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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

		virtual bool loadMap(double north, double east, double south, double west);

		virtual void LLrect(double &north, double &east, double &south, double &west);

		virtual void pixRect(QRect &rect);

		virtual void LLtoPix(double lat, double lon, QPoint &pt);

		virtual void pixToLL(const QPoint &pt, double &lat, double &lon);

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
};

#endif
