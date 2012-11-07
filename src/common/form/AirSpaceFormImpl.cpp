/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
 *   grafal@sourceforge.net                                                         *
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


#include <QBrush>
#include <QPainter>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QStringList>
#include <QTableWidget>
#include "AirSpace.h"
#include "AirSpaceFormImpl.h"

AirSpaceFormImpl::AirSpaceFormImpl(QWidget* parent, const QString &caption, AirSpace *pAirSpace)
	:QDialog(parent),
	m_drawRect(290, 10, 350, 350)
{
  QStringList nameList;

	setupUi(this);
	setWindowTitle(caption);

	tableEdgePoints->setColumnWidth(Use, 20);
	tableEdgePoints->setColumnWidth(Longitude, 95);
	tableEdgePoints->setColumnWidth(Latitude, 95);

	nameList += tr("");
	nameList += tr("Longitude\n[Deg,min]");
	nameList += tr("Latitude\n[Deg,min]");
	tableEdgePoints->setHorizontalHeaderLabels(nameList);

	setAirSpace(pAirSpace);
}

void AirSpaceFormImpl::setAirSpace(AirSpace *pAirSpace)
{
  QTableWidgetItem *pItem;
	QString str;
	uint ptNr;
	uint maxPts;

	m_pAirSpace = pAirSpace;
	tableEdgePoints->setRowCount(0); // clear table

	if(pAirSpace != NULL)
	{
		setWindowTitle(m_pAirSpace->name());
		maxPts = m_pAirSpace->pointList().size();
		tableEdgePoints->setRowCount(maxPts);

		for(ptNr=0; ptNr<maxPts; ptNr++)
		{
      pItem = new QTableWidgetItem();
      pItem->setFlags(Qt::ItemIsUserCheckable);
      pItem->setCheckState(Qt::Checked);
      tableEdgePoints->setItem(ptNr, Use, pItem);

      pItem = new QTableWidgetItem();
      str.sprintf("%.5f", m_pAirSpace->pointList().at(ptNr).lon());
      pItem->setText(str);
      tableEdgePoints->setItem(ptNr, Longitude, pItem);

      pItem = new QTableWidgetItem();
      str.sprintf("%.5f", m_pAirSpace->pointList().at(ptNr).lat());
      pItem->setText(str);
      tableEdgePoints->setItem(ptNr, Latitude, pItem);
		}

		repaint();
	}
}

void AirSpaceFormImpl::tableChanged(int row, int col)
{
	repaint();
}

void AirSpaceFormImpl::closeEvent(QCloseEvent * e)
{
	// prevent from closing
}

void AirSpaceFormImpl::paintEvent(QPaintEvent *pEvent)
{
	QPainter paint(this);
	PointArray edgePts;
	QRect boundRect;
	QTableWidgetItem *pTabItem;
	uint ptNr = 0;
	uint maxPts;
	uint nPts = 0;
	int lat;
	int lon;
	double sx;
	double sy;
	int minLat;
	int minLon;
	int maxLat;
	int maxLon;

	if(m_pAirSpace != NULL)
	{
		// fill points to point array
		maxPts = m_pAirSpace->pointList().size();
		edgePts.resize(maxPts);

		if(maxPts > 0)
		{
			minLat = m_pAirSpace->pointList().at(ptNr).lat() * 1000;
			minLon = m_pAirSpace->pointList().at(ptNr).lon() * 1000;
			maxLat = minLat;
			maxLon = minLon;

			for(ptNr=0; ptNr<maxPts; ptNr++)
			{
				pTabItem = tableEdgePoints->item(ptNr, Use);

				if(pTabItem->checkState() == Qt::Checked)
				{
					lat = (int)(m_pAirSpace->pointList().at(ptNr).lon() * 1000);
					lon = -(int)(m_pAirSpace->pointList().at(ptNr).lat() * 1000); // y axis is inverse
					edgePts[nPts] = QPoint(lat, lon);
					nPts++;

					minLat = qMin(lat, minLat);
					maxLat = qMax(lat, minLat);
					minLon = qMin(lon, minLon);
					maxLon = qMax(lon, minLon);
				}
			}

			edgePts.resize(nPts);

			// calc translation and scale
			boundRect = QRect(QPoint(maxLat, minLon), QPoint(minLat, maxLon)); // edgePts.boundingRect();
			sx = (double)(boundRect.right() - boundRect.left()) / (double)(m_drawRect.right() - m_drawRect.left());
			sy=  (double)(boundRect.bottom() - boundRect.top()) / (double)(m_drawRect.bottom() - m_drawRect.top());

			// translate and scale to draw rect
			translateEdgePts(edgePts, -boundRect.left(), -boundRect.top());
			scaleEdgePts(edgePts, sx, sy);
			translateEdgePts(edgePts, m_drawRect.left(), m_drawRect.top());

			// draw
			paint.setPen(Qt::black);
			paint.drawPolyline(edgePts);
		}
	}

	QDialog::paintEvent(pEvent);
}

void AirSpaceFormImpl::translateEdgePts(PointArray &edgePts, int dx, int dy)
{
	QPoint delta = QPoint(dx, dy);
	int ptNr;
	int maxPts;

	maxPts = edgePts.size();

	for(ptNr=0; ptNr<maxPts; ptNr++)
	{
		edgePts[ptNr] += delta;
	}
}

void AirSpaceFormImpl::scaleEdgePts(PointArray &edgePts, double sx, double sy)
{
	uint ptNr;
	uint maxPts;
	double scale;

	if(sx > sy)
	{
		scale = sx;
	}
	else
	{
		scale = sy;
	}

	maxPts = edgePts.size();

	for(ptNr=0; ptNr<maxPts; ptNr++)
	{
		edgePts[ptNr] /= scale;
	}
}
