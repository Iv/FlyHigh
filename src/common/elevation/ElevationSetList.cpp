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
#include "ElevationSetList.h"

ElevationSetList::ElevationSetList()
{
}

ElevationSetList::~ElevationSetList()
{
  clear();
}

void ElevationSetList::push_back(ElevationSet *pSet)
{
  m_setList.push_back(pSet);
}

void ElevationSetList::clear()
{
  iterator it;

  for(it=begin(); it!=end(); it++)
  {
    delete *it;
  }

  m_setList.clear();
}

ElevationSetList::iterator ElevationSetList::begin()
{
  return m_setList.begin();
}

ElevationSetList::iterator ElevationSetList::end()
{
  return m_setList.end();
}

ElevationSet* ElevationSetList::operator[] (int pos)
{
  return m_setList[pos];
}
