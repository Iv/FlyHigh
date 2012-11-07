/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#include <math.h>
#include "AirSpace.h"

#include <QDebug>

AirSpace::AirSpace()
{
  m_id = -1;
	m_name = "";
	m_airspaceClass = "";
  m_high = 0;
	m_low = 0;
	m_warnDist = 0;
	m_remark = "";
}

AirSpace::~AirSpace()
{
}

int AirSpace::id() const
{
  return m_id;
}

void AirSpace::setId(int id)
{
  m_id = id;
}

const QString& AirSpace::name() const
{
	return m_name;
}

void AirSpace::setName(const QString &name)
{
	QString locName = name;

	locName.remove('#');
	m_name = locName.trimmed();
}

int AirSpace::high() const
{
	return m_high;
}

void AirSpace::setHigh(int high)
{
	m_high = high;
}

int AirSpace::low() const
{
	return m_low;
}

void AirSpace::setLow(int low)
{
	m_low = low;
}

const QString& AirSpace::airspaceClass() const
{
	return m_airspaceClass;
}

void AirSpace::setAirspaceClass(const QString &airspaceClass)
{
	m_airspaceClass = airspaceClass;
}

void AirSpace::setWarnDist(uint meters)
{
	m_warnDist = meters;
}

uint AirSpace::warnDist() const
{
	return m_warnDist;
}

void AirSpace::setRemark(const QString &remark)
{
	m_remark = remark;
}

const QString& AirSpace::remark() const
{
	return m_remark;
}

LatLngList& AirSpace::pointList()
{
	return m_pointList;
}

void AirSpace::setBoundBox(const BoundBox &bbox)
{
  m_boundBox = bbox;
}

const BoundBox& AirSpace::boundBox() const
{
	return m_boundBox;
}

bool AirSpace::isInside(const WayPoint &wp) const
{
	// cn_PnPoly(): crossing number test for a point in a polygon
	//      Input:   P = a point,
	//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
	//      Return:  0 = outside, 1 = inside
	// This code is patterned after [Franklin, 2000]

  int i;
	int j;
	bool cross = 0;
	int nvert = m_pointList.size();

  for(i = 0, j = nvert-1; i < nvert; j = i++)
	{
    if(((m_pointList[i].lat() > wp.latitude()) != (m_pointList[j].lat() > wp.latitude())) &&
				(wp.longitude() < (m_pointList[j].lon() - m_pointList[i].lon()) *
				(wp.latitude() - m_pointList[i].lat()) / (m_pointList[j].lat() - m_pointList[i].lat()) +
				m_pointList[i].lon()))
		{
       cross = !cross;
		}
  }

	return (cross) && (wp.altitude() >= m_low) && (wp.altitude() <= m_high);
}

bool AirSpace::getNextBear(bool dir, double endBear, double prevBear, double &bear)
{
  bool over;

  if(dir)
  {
    // clock wise
    bear += 10;

    if(bear > 360)
    {
      bear -= 360;
    }

    if(prevBear < bear)
    {
      over = ((prevBear <= endBear) && (bear >= endBear));
    }
    else
    {
      over = (bear >= endBear);
    }
  }
  else
  {
    // counter clock wise
    bear -= 10;

    if(bear < 0)
    {
      bear += 360;
    }

    if(prevBear > bear)
    {
      over = ((prevBear >= endBear) && (bear <= endBear));
    }
    else
    {
      over = (bear <= endBear);
    }
  }

  return over;
}
