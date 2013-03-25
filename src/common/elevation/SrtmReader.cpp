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

#include <QDebug>

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
  double lat;
  double lon;
  double x;
  double y;
  int value = 0;

  if(m_pFile != NULL)
  {
    lon = (pos.lon() - m_pos.lon());
//    j = (int)(round(lon / m_res));
x = (int)round(lon * m_cols);

    lat = (pos.lat() - m_pos.lat());
//    i = m_rows - (int)round(lat / m_res);
y = m_rows - (int)round(lat * m_rows);

value = read(x, y);

/** bilinearer Filter
double swValue;
double nwValue;
double neValue;
double seValue;
double value1;
double value2;

x = lon * m_cols;
y = (double)m_rows - (lat * m_rows);

swValue = read(x, y);
nwValue = read(x, y + 1);
neValue = read(x + 1, y + 1);
seValue = read(x + 1, y);
value1 = ((int)x + 1 - x) * nwValue + (x - (int)x) * neValue;
value2 = ((int)x + 1 - x) * swValue + (x - (int)x) * seValue;
value = ((int)y + 1 - y) * value1 + (y - (int)y) * value2;

qDebug() << "(" << x << "," << y << ")<" << swValue << nwValue << neValue << seValue << "> =>" << value;
*/

//j = l
//value = read((int)x, (int)y);

/**
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

//qDebug() << "read (" << j << "," << i << ")" << value;
printf("%i %i\n", j, value);
//qDebug() << j  << value;

      if(value == -32768)
      {
        value = 0;
      }
    }
    */
  }

  return value;
}

int SrtmReader::read(int x, int y)
{
  enum {BuffSize = 2};
  char buff[BuffSize];
  qint64 size;
  qint64 offset;
  int value = 0;

  offset = (y * m_rows + x) * m_bytesPerSample;
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
      value = (buff[0] << 8);
      value += (buff[1] & 0xff);
    }
  }

  return value;
}
