/***************************************************************************
 *   Copyright (C) 2012 by Alex Graf                                       *
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

#include "AirSpaceItem.h"
#include "AirSpaceItemList.h"

AirSpaceItemList::AirSpaceItemList()
{
}

AirSpaceItemList::~AirSpaceItemList()
{
  clear();
}

void AirSpaceItemList::push_back(AirSpaceItem *pItem)
{
  m_itemList.push_back(pItem);
}

void AirSpaceItemList::clear()
{
  iterator it;

  for(it=m_itemList.begin(); it!=m_itemList.end(); it++)
  {
    delete *it;
  }

  m_itemList.clear();
}

int AirSpaceItemList::size() const
{
  return m_itemList.size();
}

AirSpaceItem* AirSpaceItemList::at(int pos)
{
  return m_itemList.at(pos);
}

AirSpaceItemList::iterator AirSpaceItemList::begin()
{
  return m_itemList.begin();
}

AirSpaceItemList::const_iterator AirSpaceItemList::begin() const
{
  return m_itemList.begin();
}

AirSpaceItemList::iterator AirSpaceItemList::end()
{
  return m_itemList.end();
}

AirSpaceItemList::const_iterator AirSpaceItemList::end() const
{
  return m_itemList.end();
}
