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

AirSpaceItemList& AirSpace::airSpaceItemList()
{
	return m_airSpaceItemList;
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

void AirSpace::createPointList()
{
	AirSpaceItemList::iterator it;
	AirSpaceItemPoint *pSegCenter = NULL;
	AirSpaceItemSeg *pSegBegin = NULL;
	AirSpaceItemSeg *pSegEnd;
	AirSpaceItemCircle *pCircle = NULL;
	AirSpaceItemPoint *pPoint;
	WayPoint center;
  WayPoint curPt;
	double centerLat;
	double centerLon;
	double curLat;
	double curLon;
	double dist1;
	double dist2;
	double bear;
  double prev;
	double beginArc;
	double endArc;
	double angDist;
	int stepNr;
  bool over;

	m_pointList.clear();

	for(it=m_airSpaceItemList.begin(); it!=m_airSpaceItemList.end(); it++)
	{
		switch((*it)->type())
		{
			case AirSpaceItem::Point:
				pPoint = dynamic_cast<AirSpaceItemPoint*>(*it);

				if(pPoint != NULL)
				{
					m_pointList.push_back(pPoint->pos());
					m_boundBox.setMinMax(pPoint->pos());
				}
			break;
			case AirSpaceItem::Circle:
				pCircle = dynamic_cast<AirSpaceItemCircle*>(*it);

				if(pCircle != NULL)
				{
					center = pCircle->pos();
					centerLat = (center.latitude() * M_PI) / 180;
					centerLon = (center.longitude() * M_PI) / 180;
					angDist = pCircle->radius() / WayPoint::earthRadius;

					for(stepNr=0; stepNr<=360; stepNr+=10)
					{
						bear = stepNr * M_PI / 180;
						curLat = asin(sin(centerLat) * cos(angDist) + cos(centerLat) * sin(angDist) * cos(bear));
						curLon = ((centerLon + atan2(sin(bear) * sin(angDist) * cos(centerLat), cos(angDist) - sin(centerLat) * sin(centerLat))) *
									180) / M_PI;
						curLat = (curLat * 180) / M_PI;
						curPt.setCoordinates(curLat, curLon);
						m_pointList.push_back(curPt);
						m_boundBox.setMinMax(curPt);
					}
				}
			break;
			case AirSpaceItem::Center:
				pSegCenter = dynamic_cast<AirSpaceItemPoint*>(*it);
			break;
			case AirSpaceItem::StartSegment:
				pSegBegin = dynamic_cast<AirSpaceItemSeg*>(*it);
			break;
			case AirSpaceItem::StopSegment:
				pSegEnd = dynamic_cast<AirSpaceItemSeg*>(*it);

				if((pSegCenter != NULL) && (pSegBegin != NULL) && (pSegEnd != NULL))
				{
					center = pSegCenter->pos();
					center.distBear(pSegBegin->pos(), dist1, beginArc);
					center.distBear(pSegEnd->pos(), dist2, endArc);
					m_pointList.push_back(pSegBegin->pos());
					m_boundBox.setMinMax(pSegBegin->pos());
					centerLat = (center.latitude() * M_PI) / 180;
					centerLon = (center.longitude() * M_PI) / 180;
					angDist = ((dist1 + dist2) / 2) / WayPoint::earthRadius;

					// interpolate arc
          over = false;
          prev = beginArc;
          bear = ((int)(beginArc / 10)) * 10; // quantize to 10
          over = getNextBear(pSegBegin->dir(), endArc, prev, bear);

          while(!over)
          {
            curLat = asin(sin(centerLat) * cos(angDist) + cos(centerLat) * sin(angDist) * cos(bear * M_PI / 180));
            curLon = ((centerLon + atan2(sin(bear * M_PI / 180) * sin(angDist) * cos(centerLat),
                       cos(angDist) - sin(centerLat) * sin(centerLat))) * 180) / M_PI;
            curLat = (curLat * 180) / M_PI;
            curPt.setCoordinates(curLat, curLon);
            m_pointList.push_back(curPt);
            prev = bear;
            over = getNextBear(pSegBegin->dir(), endArc, prev, bear);
          }

					m_pointList.push_back(pSegEnd->pos());
					m_boundBox.setMinMax(pSegEnd->pos());
				}
			break;
		}
	}
}

const WayPoint::WayPointListType& AirSpace::pointList() const
{
	return m_pointList;
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
    if(((m_pointList[i].latitude() > wp.latitude()) != (m_pointList[j].latitude() > wp.latitude())) &&
				(wp.longitude() < (m_pointList[j].longitude() - m_pointList[i].longitude()) *
				(wp.latitude() - m_pointList[i].latitude()) / (m_pointList[j].latitude() - m_pointList[i].latitude()) +
				m_pointList[i].longitude()))
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
