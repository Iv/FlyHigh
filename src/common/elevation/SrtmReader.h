/***************************************************************************
 *   Copyright (C) 2013 by Alex Graf                                       *
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
#ifndef _SRTMREADER_H
#define _SRTMREADER_H

#include "BoundBox.h"

class QFile;

class SrtmReader
{
  public:
    SrtmReader();

    ~SrtmReader();

    void setCount(int rows, int cols);

    int rows() const;

    int cols() const;

    void setResolution(int res);

    int resolution() const;

    void setPos(const LatLng &pos);

    const LatLng& pos() const;

    bool open(const QString &name);

    void close();

    int elevation(const LatLng &pos);

  private:
    LatLng m_pos;
    QFile *m_pFile;
    int m_rows;
    int m_cols;
    int m_res;
    int m_nodata;
    int m_bytesPerSample;
};
#endif
