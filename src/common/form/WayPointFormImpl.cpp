/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <qcombobox.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include "WayPointFormImpl.h"

WayPointFormImpl::WayPointFormImpl(QWidget* parent, const QString &caption, WayPoint *pWayPoint)
	:QDialog(parent)
{
        setupUi(this);
	QString str;

	Q_CHECK_PTR(pWayPoint);
	m_pWayPoint = pWayPoint;

        setWindowTitle(caption);
		
	// name
	lineEditName->setText(pWayPoint->name());

	// spot
	lineEditSpot->setText(pWayPoint->spot());

	// country
	lineEditCountry->setText(pWayPoint->country());

	// latitude
	str.sprintf("%f", pWayPoint->latitude());
	lineEditLat->setText(str);
	
	// longitude
	str.sprintf("%f", pWayPoint->longitude());
	lineEditLon->setText(str);
	
	// altitude
	spinBoxAlt->setValue(pWayPoint->altitude());
}

void WayPointFormImpl::accept()
{
	double lat;
	double lon;
	int alt;
	
	// name
	m_pWayPoint->setName(lineEditName->text());

	// spot
	m_pWayPoint->setSpot(lineEditSpot->text());

	// country
	m_pWayPoint->setCountry(lineEditCountry->text());
	
	// latitude
	lat = lineEditLat->text().toDouble();
	
	// longitude
	lon = lineEditLon->text().toDouble();
	
	// altitude
	alt = spinBoxAlt->value();
	m_pWayPoint->setCoordinates(lat, lon, alt);
	
	QDialog::accept();
}

void WayPointFormImpl::limitLatitude()
{
	QString text;
	double lat;

	lat = lineEditLat->text().toDouble();
	
	if(lat < -90.0)
	{
		lat = -90.0;
	}
	else if(lat > 90.0)
	{
		lat = 90.0;
	}

	text.sprintf("%.5f", lat);
	lineEditLat->setText(text);
}

void WayPointFormImpl::limitLongitude()
{
	QString text;
	double lon;
	
	lon = lineEditLon->text().toDouble();
	
	if(lon < -180.0)
	{
		lon = -180.0;
	}
	else if(lon > 180.0)
	{
		lon = 180.0;
	}
	
	text.sprintf("%.5f", lon);
	lineEditLon->setText(text);
}

#include "moc_WayPointFormImpl.cxx"

