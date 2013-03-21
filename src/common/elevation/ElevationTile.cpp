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
#include "ElevationTile.h"
#include "SrtmReader.h"

ElevationTile::ElevationTile()
{
  m_type = Undef;
}

ElevationTile::ElevationTile(const QString &path)
{
  m_path = path;
  m_type = Undef;
}

void ElevationTile::load()
{
}

bool ElevationTile::isLocal()
{
  return false;
}

int ElevationTile::elevation()
{
  return 0;
}

void ElevationTile::setBoundBox(const BoundBox &bbox)
{
  m_bbox = bbox;
}

const BoundBox& ElevationTile::boundBox() const
{
  return m_bbox;
}

QString ElevationTile::path(const QString &path) const
{
  return path + m_path;
}

void ElevationTile::setType(Type type)
{
  m_type = type;
}

ElevationTile::Type ElevationTile::type() const
{
  return m_type;
}
