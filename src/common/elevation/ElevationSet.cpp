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
#include "ElevationSet.h"
#include "ElevationTile.h"

ElevationSet::ElevationSet()
{
}

ElevationSet::ElevationSet(const QString &target, const QString &url, const BoundBox &bbox)
{
  m_target = target;
  m_url = url;
  m_bbox = bbox;
}

QString ElevationSet::target(const QString &path) const
{
  return path + "/" + m_target;
}

QString ElevationSet::path(const QString &path) const
{
  int index;

  index = m_target.lastIndexOf('/');

  return path + m_target.left(index + 1);
}

const QUrl& ElevationSet::url() const
{
  return m_url;
}

const BoundBox& ElevationSet::boundBox() const
{
  return m_bbox;
}
