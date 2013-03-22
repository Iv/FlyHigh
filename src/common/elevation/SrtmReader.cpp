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
#include <QFile>
#include <math.h>
#include "SrtmReader.h"

SrtmReader::SrtmReader()
{
  m_pFile = NULL;

  m_rows = 0;
  m_cols = 0;
  m_res = 1;
  m_nodata = -32768;
  m_bytesPerSample = 2;
}

SrtmReader::~SrtmReader()
{
  close();
}

void SrtmReader::setCount(int rows, int cols)
{
  m_rows = rows;
  m_cols = cols;
}

int SrtmReader::rows() const
{
  return m_rows;
}

int SrtmReader::cols() const
{
  return m_cols;
}

void SrtmReader::setResolution(int res)
{
  if(res > 0)
  {
    m_res = res;
  }
}

int SrtmReader::resolution() const
{
  return m_res;
}

void SrtmReader::setPos(const LatLng &pos)
{
  m_pos = pos;
}

const LatLng& SrtmReader::pos() const
{
  return m_pos;
}

bool SrtmReader::open(const QString &name)
{
  bool success;

  m_pFile = new QFile(name);
  success = m_pFile->open(QIODevice::ReadOnly);

  return success;
}

void SrtmReader::close()
{
  if(m_pFile != NULL)
  {
    m_pFile->close();
    delete m_pFile;
    m_pFile = NULL;
  }
}

int SrtmReader::elevation(const LatLng &pos)
{
  enum {BuffSize = 2};
  qint64 size;
  qint64 offset;
  double lat;
  double lon;
  int i;
  int j;
  int value = 0;
  char buff[BuffSize];

  if(m_pFile != NULL)
  {
    lat = (pos.lat() - m_pos.lat());
//    i = m_rows - (int)round(lat / m_res);
i = m_rows - (int)round(lat * m_rows);
    lon = (pos.lon() - m_pos.lon());
//    j = (int)(round(lon / m_res));
j = (int)round(lon * m_cols);
    offset = ((i - 1) * m_rows + (j - 1)) * m_bytesPerSample;
    m_pFile->seek(offset);
    size = m_pFile->read(buff, BuffSize);

    if(size == BuffSize)
    {
      if(QSysInfo::ByteOrder == QSysInfo::BigEndian)
      {
        value = ((buff[1] << 8) + buff[0]);
      }
      else
      {
        value = ((buff[0] << 8) + buff[1]);
      }

      if(value == -32768)
      {
        value = 0;
      }
    }
  }

  return value;
}
