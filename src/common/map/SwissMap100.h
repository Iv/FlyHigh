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

#ifndef SwissMap100_h
#define SwissMap100_h

#include <Map.h>

class SwissMap100: public Map
{
	public:
		SwissMap100(const QString &tilePath);

    /**
      Loads tiles on the basis of the WGS84 coordinates. The tiles are loaded, that
      the requested boundaries are inside.
      @param north top coordinate
      @param east right coordinate
      @param south bottom coordinate
      @param west left coordinate
    */
		bool loadMap(double north, double east, double south, double west);

    /**
      Returns the boundaries of the loaded tiles in WGS84 coordinates.
      @param north top coordinate
      @param east right coordinate
      @param south bottom coordinate
      @param west left coordinate
    */
		void LLrect(double &north, double &east, double &south, double &west);

    /**
      Returns the pixel size of one tile. The size is calculated with conjunction
      of zoom.
      @param pixX count of pixels in x
      @param pixY count of pixels in y
    */
		void pixTileSize(int &pixX, int &pixY);

    /**
      Interrupts a loading progress.
    */
		void cancel();

    /**
      Converts WGS84 lat, lon to pixel coordinate.
      @param lat latitude
      @param lon longitude
      @param pt Pixel coordinate
    */
		void LLtoPix(double lat, double lon, QPoint &pt);

    /**
      Converts pixel coordinate to WGS84 lat, lon.
      @param pt Pixel coordinate
      @param lat latitude
      @param lon longitude
    */
		void pixToLL(const QPoint &pt, double &lat, double &lon);

	private:
		static const uint MaxSwissN;
		static const uint MaxSwissW;
		QRect m_tilesRect;
		bool m_cancel;

		bool loadSwissMap(uint swiss_n, uint swiss_e, uint swiss_s, uint swiss_w);

		bool loadTiles(QRect &tilesRect);

		bool tileName(uint x, uint y, QString &name);
};

#endif
