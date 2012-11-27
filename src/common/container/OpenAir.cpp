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

#include <math.h>
#include "OpenAir.h"

#include <QDebug>

OpenAir::OpenAir()
{
  m_id = -1;
	m_name = "";
	m_airspaceClass = "";
  m_high = 0;
	m_low = 0;
	m_warnDist = 0;
	m_remark = "";
}

OpenAir::~OpenAir()
{
  clearItemList();
}

int OpenAir::id() const
{
  return m_id;
}

void OpenAir::setId(int id)
{
  m_id = id;
}

const QString& OpenAir::name() const
{
	return m_name;
}

void OpenAir::setName(const QString &name)
{
	QString locName = name;

	locName.remove('#');
	m_name = locName.trimmed();
}

int OpenAir::high() const
{
	return m_high;
}

void OpenAir::setHigh(int high)
{
	m_high = high;
}

int OpenAir::low() const
{
	return m_low;
}

void OpenAir::setLow(int low)
{
	m_low = low;
}

const QString& OpenAir::airspaceClass() const
{
	return m_airspaceClass;
}

void OpenAir::setAirspaceClass(const QString &airspaceClass)
{
	m_airspaceClass = airspaceClass;
}

void OpenAir::setWarnDist(uint meters)
{
	m_warnDist = meters;
}

uint OpenAir::warnDist() const
{
	return m_warnDist;
}

void OpenAir::setRemark(const QString &remark)
{
	m_remark = remark;
}

const QString& OpenAir::remark() const
{
	return m_remark;
}

void OpenAir::push_back(OpenAirItem *pItem)
{
  m_itemList.push_back(pItem);
}

void OpenAir::clearItemList()
{
  OpenAirItemList::iterator it;

  for(it=m_itemList.begin(); it!=m_itemList.end(); it++)
  {
    delete (*it);
  }

  m_itemList.clear();
}

const OpenAirItemList& OpenAir::itemList() const
{
	return m_itemList;
}

void OpenAir::createPointList(LatLngList &itemList)
{
	OpenAirItemList::iterator it;
	OpenAirItemPoint *pSegCenter = NULL;
	OpenAirItemSeg *pSegBegin = NULL;
	OpenAirItemSeg *pSegEnd;
	OpenAirItemCircle *pCircle = NULL;
	OpenAirItemPoint *pPoint;
	LatLng center;
  LatLng curPt;
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

	itemList.clear();

	for(it=m_itemList.begin(); it!=m_itemList.end(); it++)
	{
		switch((*it)->type())
		{
			case OpenAirItem::Point:
				pPoint = dynamic_cast<OpenAirItemPoint*>(*it);

				if(pPoint != NULL)
				{
					itemList.push_back(pPoint->pos());
					m_boundBox.setMinMax(pPoint->pos());
				}
			break;
			case OpenAirItem::Circle:
				pCircle = dynamic_cast<OpenAirItemCircle*>(*it);

				if(pCircle != NULL)
				{
					center = pCircle->pos();
					centerLat = (center.lat() * M_PI) / 180;
					centerLon = (center.lon() * M_PI) / 180;
					angDist = pCircle->radius() / LatLng::EarthRadius;

					for(stepNr=0; stepNr<=360; stepNr+=10)
					{
						bear = stepNr * M_PI / 180;
						curLat = asin(sin(centerLat) * cos(angDist) + cos(centerLat) * sin(angDist) * cos(bear));
						curLon = ((centerLon + atan2(sin(bear) * sin(angDist) * cos(centerLat), cos(angDist) - sin(centerLat) * sin(centerLat))) *
									180) / M_PI;
						curLat = (curLat * 180) / M_PI;
						curPt.setPos(curLat, curLon);
						itemList.push_back(curPt);
						m_boundBox.setMinMax(curPt);
					}
				}
			break;
			case OpenAirItem::Center:
				pSegCenter = dynamic_cast<OpenAirItemPoint*>(*it);
			break;
			case OpenAirItem::StartSegment:
				pSegBegin = dynamic_cast<OpenAirItemSeg*>(*it);
			break;
			case OpenAirItem::StopSegment:
				pSegEnd = dynamic_cast<OpenAirItemSeg*>(*it);

				if((pSegCenter != NULL) && (pSegBegin != NULL) && (pSegEnd != NULL))
				{
					center = pSegCenter->pos();
					center.distBear(pSegBegin->pos(), dist1, beginArc);
					center.distBear(pSegEnd->pos(), dist2, endArc);
					itemList.push_back(pSegBegin->pos());
					m_boundBox.setMinMax(pSegBegin->pos());
					centerLat = (center.lat() * M_PI) / 180;
					centerLon = (center.lon() * M_PI) / 180;
					angDist = ((dist1 + dist2) / 2) / LatLng::EarthRadius;

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
            curPt.setPos(curLat, curLon);
            itemList.push_back(curPt);
            prev = bear;
            over = getNextBear(pSegBegin->dir(), endArc, prev, bear);
          }

					itemList.push_back(pSegEnd->pos());
					m_boundBox.setMinMax(pSegEnd->pos());
				}
			break;
		}
	}
}

const BoundBox& OpenAir::boundBox() const
{
	return m_boundBox;
}

bool OpenAir::getNextBear(bool dir, double endBear, double prevBear, double &bear)
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
