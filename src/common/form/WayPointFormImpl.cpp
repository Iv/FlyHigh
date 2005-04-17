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
	:WayPointForm(parent, caption, true)
{
	QString str;
	double lat;
	double lon;

	Q_CHECK_PTR(pWayPoint);
	m_pWayPoint = pWayPoint;

	WayPointForm::setCaption(caption);
		
	// name
	lineEditName->setText(pWayPoint->name());
	
	// latitude
	lat = pWayPoint->latitude();
	
	if(lat < 0.0)
	{
		comboBoxNS->setCurrentItem(1);
		lat *= -1.0;
	}
	else
	{
		comboBoxNS->setCurrentItem(0);
	}
	str.sprintf("%f", lat);
	lineEditLat->setText(str);
	
	// longitude
	lon = pWayPoint->longitude();
	
	if(lon < 0.0)
	{
		comboBoxEW->setCurrentItem(1);
		lon *= -1.0;
	}
	else
	{
		comboBoxEW->setCurrentItem(0);
	}
	str.sprintf("%f", lon);
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
	
	// latitude
	lat = lineEditLat->text().toDouble();
	
	if(comboBoxNS->currentText() == "S")
	{
		lat *= -1.0;
	}
	
	// longitude
	lon = lineEditLon->text().toDouble();
	
	if(comboBoxEW->currentText() == "W")
	{
		lon *= -1.0;
	}
	
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
	
	if(lat < 0.0)
	{
		lat = 0.0;
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
	
	if(lon < 0.0)
	{
		lon = 0.0;
	}
	else if(lon > 180.0)
	{
		lon = 180.0;
	}
	
	text.sprintf("%.5f", lon);
	lineEditLon->setText(text);
}

#include "WayPointFormImpl.moc"

