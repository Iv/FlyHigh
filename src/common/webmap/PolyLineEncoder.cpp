/***************************************************************************
 *   Copyright Mark McClure  April/May 2007                                *
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

#include <stack>
#include <math.h>
#include "PolyLineEncoder.h"

const double PolyLineEncoder::undefined = 1000000;

PolyLineEncoder::PolyLineEncoder()
{
	double zoomLevelBreak;
	uint levelNr;

	m_numLevels = 18;
	m_zoomFactor = 2;
//	m_verySmall = 0.00001;
	m_verySmall = 0.00003;
	m_forceEndpoints = true;

	for(levelNr=0; levelNr<m_numLevels; levelNr++)
	{
		zoomLevelBreak = m_verySmall * pow(m_zoomFactor, m_numLevels - levelNr - 1);
		m_zoomLevelBreaks.push_back(zoomLevelBreak);
	}
}

PolyLineEncoder::~PolyLineEncoder()
{
}

void PolyLineEncoder::dpEncode(const WayPoint::WayPointListType &points, QString &encPoints, QString &encLevels)
{
	std::stack<StackItem> stack;
	StackItem current;
	StackItem tmpStackItem;
	double absMaxDist;
	DistsType dists;
	double maxDist;
	double temp;
	double segmentLength;
	int maxLoc;
	int i;

	if(points.size() > 2)
	{
		absMaxDist = 0;
		dists.resize(points.size(), undefined);
		current.i = 0;
		current.j = points.size() - 1;
		stack.push(current);

		while(stack.size() > 0)
		{
			current = stack.top();
			stack.pop();
			maxDist = 0;
			segmentLength = pow(points[current.j].latitude() - points[current.i].latitude(), 2) +
							pow(points[current.j].longitude() - points[current.i].longitude(), 2);

			for(i=current.i + 1; i < current.j; i++)
			{
				temp = distance(points[i], points[current.i], points[current.j], segmentLength);

				if(temp > maxDist)
				{
					maxDist = temp;
					maxLoc = i;

					if(maxDist > absMaxDist)
					{
						absMaxDist = maxDist;
					}
				}
			}

			if(maxDist > m_verySmall)
			{
				dists[maxLoc] = maxDist;
				tmpStackItem.i = current.i;
				tmpStackItem.j = maxLoc;
				stack.push(tmpStackItem); // stack.push([current.i, maxLoc]);
				tmpStackItem.i = maxLoc;
				tmpStackItem.j = current.j;
				stack.push(tmpStackItem); // stack.push([maxLoc, current.j]);
			}
		}
	}
	else
	{
		dists.resize(points.size(), undefined);
	}

	encPoints = createEncodings(points, dists);
	encPoints.replace(QString("\\"), QString("\\\\"));
	encLevels = encodeLevels(points, dists, absMaxDist);

/*
	return {
		encodedPoints: encodedPoints,
		encodedLevels: encodedLevels,
		encodedPointsLiteral: encodedPoints.replace(/\\/g,"\\\\")
	}
*/
}

QString PolyLineEncoder::createEncodings(const WayPoint::WayPointListType &points, DistsType &dists)
{
	int i;
	double lat;
	double lng;
	int dlat;
	int dlng;
	int late5;
	int lnge5;
	int plat = 0;
	int plng = 0;
	QString encoded_points = "";
	
	for(i = 0; i < points.size(); i++)
	{
		if(dists[i] != undefined || i == 0 || i == points.size() - 1)
		{
			lat = points[i].latitude();
			lng = points[i].longitude();
			late5 = (int)floor(lat * 1e5);
			lnge5 = (int)floor(lng * 1e5);
			dlat = late5 - plat;
			dlng = lnge5 - plng;
			plat = late5;
			plng = lnge5;
			encoded_points += encodeSignedNumber(dlat) + encodeSignedNumber(dlng);
		}
	}

	return encoded_points;
}

QString PolyLineEncoder::encodeLevels(const WayPoint::WayPointListType &points, DistsType &dists, double absMaxDist)
{
	QString encoded_levels = "";
	int i;

	if(m_forceEndpoints)
	{
		encoded_levels += encodeNumber(m_numLevels - 1);
	}
	else
	{
		encoded_levels += encodeNumber(m_numLevels - computeLevel(absMaxDist) - 1);
	}

	for(i=1; i<(points.size() - 1); i++)
	{
		if(dists[i] != undefined)
		{
			encoded_levels += encodeNumber(m_numLevels - computeLevel(dists[i]) - 1);
		}
	}

	if(m_forceEndpoints)
	{
		encoded_levels += encodeNumber(m_numLevels - 1);
	}
	else
	{
		encoded_levels += encodeNumber(m_numLevels - computeLevel(absMaxDist) - 1);
	}

	return encoded_levels;
}

uint PolyLineEncoder::computeLevel(double dd)
{
	uint lev = 0;
	
	if(dd > m_verySmall)
	{
		while(dd < m_zoomLevelBreaks[lev])
		{
			lev++;
		}
	}

	return lev;
}

QString PolyLineEncoder::encodeSignedNumber(int num)
{
	uint sgn_num = (num << 1);
	
	if(num < 0)
	{
		sgn_num = ~(sgn_num);
	}

	return encodeNumber(sgn_num);
}

QString PolyLineEncoder::encodeNumber(uint num)
{
	QString encodeString = "";
	uint nextValue;
	uint finalValue;

	while(num >= 0x20)
	{
		nextValue = (0x20 | (num & 0x1f)) + 63;
//     if (nextValue == 92) {
//       encodeString += (String.fromCharCode(nextValue));
//     }
		encodeString += (char)nextValue;
		num >>= 5;
	}

	finalValue = num + 63;
//   if (finalValue == 92) {
//     encodeString += (String.fromCharCode(finalValue));
//   }
	encodeString += (char)finalValue;

	return encodeString;
}

double PolyLineEncoder::distance(const WayPoint &p0, const WayPoint &p1, const WayPoint &p2, double segLength)
{
  double u;
	double out;

	if(p1.latitude() == p2.latitude() && p1.longitude() == p2.longitude())
	{
		out = sqrt(pow(p2.latitude() - p0.latitude(), 2) + pow(p2.longitude() - p0.longitude() , 2));
	}
	else
	{
		u = ((p0.latitude() - p1.latitude()) * (p2.latitude() - p1.latitude()) +
				 (p0.longitude() - p1.longitude()) * (p2.longitude() - p1.longitude())) /
					segLength;
	
		if(u <= 0)
		{
			out = sqrt(pow(p0.latitude() - p1.latitude(), 2) + pow(p0.longitude() - p1.longitude(), 2));
		}
		else if(u >= 1)
		{
			out = sqrt(pow(p0.latitude() - p2.latitude(), 2) + pow(p0.longitude() - p2.longitude(), 2));
		}
		else
		{
			out = sqrt(pow(p0.latitude() - p1.latitude() - u * (p2.latitude() - p1.latitude()), 2) +
				pow(p0.longitude() - p1.longitude() -u * (p2.longitude() - p1.longitude()), 2));
		}
	}
	
	return out;
}
