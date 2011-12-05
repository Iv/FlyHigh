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

#include <qbuffer.h>
#include <qpushbutton.h>
#include <qcursor.h>
#include <q3datetimeedit.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qstringlist.h>
#include <qspinbox.h>
#include <q3textedit.h>
#include "FlightFormImpl.h"
#include "ISql.h"
#include "IGliderForm.h"
#include "Glider.h"
#include "IFlyHighRC.h"
#include "IWayPointForm.h"
#include "WayPoint.h"

FlightFormImpl::FlightFormImpl(QWidget* parent, const QString &caption, Flight *pFlight)
: QDialog(parent)
{
        setupUi(this);
        setWindowTitle(caption);
	setFlight(pFlight);
}

void FlightFormImpl::updateWayPoints()
{
	QString name;
	WayPoint::WayPointListType::iterator it;

	m_wpList.clear();
	ISql::pInstance()->wayPointList(WayPoint::TypeStartLand, m_wpList);
	comboBoxStart->clear();
	comboBoxLand->clear();

	for(it=m_wpList.begin(); it!=m_wpList.end(); it++)
	{
		(*it).fullName(name);
                comboBoxStart->addItem(name);
                comboBoxLand->addItem(name);
	}
}

void FlightFormImpl::updateGlider()
{
	QString gliderModel;
	Glider::GliderListType::iterator it;

	m_gliderList.clear();
	ISql::pInstance()->gliderList(m_gliderList);
	comboBoxModel->clear();

	for(it=m_gliderList.begin(); it!=m_gliderList.end(); it++)
	{
		(*it).fullName(gliderModel);
                comboBoxModel->addItem(gliderModel);
	}
}

void FlightFormImpl::enableInput(bool b)
{
	buttonOk->setEnabled(b);
	pushButtonGlider->setEnabled(b);
	pushButtonStart->setEnabled(b);
	pushButtonLand->setEnabled(b);

	if(b)
	{
		QWidget::unsetCursor();
	}
	else
	{
		QWidget::setCursor(QCursor(Qt::WaitCursor));
	}
}

void FlightFormImpl::setFlight(Flight *pFlight)
{
	QString str;
	QTime dur(0, 0, 0);
	m_pFlight = pFlight;
	Pilot pilot;

	if(pFlight != NULL)
	{
		ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);
		m_pFlight->setPilot(pilot);
		spinBoxFlightNr->setValue(m_pFlight->number());
		dateEditStart->setDate(m_pFlight->date());
		timeEditStart->setTime(m_pFlight->time());
		timeEditDuration->setTime(dur.addSecs(m_pFlight->duration()));
		updateWayPoints();
		updateGlider();
		selectStart();
		selectLand();
		selectGlider();
		str.sprintf("%.3f", m_pFlight->distance() / 1000.0);
		lineEditDistance->setText(str);
	}
}

void FlightFormImpl::accept()
{
	m_pFlight->setNumber(spinBoxFlightNr->value());
	m_pFlight->setDate(dateEditStart->date());
	m_pFlight->setTime(timeEditStart->time());
	m_pFlight->setDuration(timeEditDuration->time());

	// glider
	m_pFlight->setGlider(m_gliderList[comboBoxModel->currentIndex()]);

	// start
	m_pFlight->setStartPt(m_wpList[comboBoxStart->currentIndex()]);

	// land
	m_pFlight->setLandPt(m_wpList[comboBoxLand->currentIndex()]);

	// distance
	m_pFlight->setDistance((uint)(lineEditDistance->text().toDouble()*1000.0));

	// comment
	m_pFlight->setComment(textEditComment->text());

  QDialog::accept();
}

void FlightFormImpl::selectStart()
{
	WayPoint wp;
	int index;
	int maxIndex;
	bool found = false;

	maxIndex = comboBoxStart->count();

	for(index=0; index<maxIndex; index++)
	{
		found = (m_wpList[index] == m_pFlight->startPt());

		if(found)
		{
			comboBoxStart->setCurrentIndex(index);
			break;
		}
	}
}

void FlightFormImpl::selectLand()
{
	int index;
	int maxIndex;
	bool found = false;

	maxIndex = comboBoxLand->count();

	for(index=0; index<maxIndex; index++)
	{
		found = (m_wpList[index] == m_pFlight->landPt());

		if(found)
		{
			comboBoxLand->setCurrentIndex(index);
			break;
		}
	}
}

void FlightFormImpl::selectGlider()
{
	int index;
	int maxIndex;
	bool found = false;

	maxIndex = comboBoxModel->count();

	for(index=0; index<maxIndex; index++)
	{
		found = (m_gliderList[index] == m_pFlight->glider());

		if(found)
		{
			comboBoxModel->setCurrentIndex(index);
			break;
		}
	}
}

void FlightFormImpl::newGlider()
{
	Glider glider;
	IGliderForm newGlider(this, "New Glider", &glider);

	if(newGlider.exec())
	{
		ISql::pInstance()->add(glider);
		updateGlider();
	}
}

void FlightFormImpl::newWayPoint()
{
	WayPoint wp;
	IWayPointForm newWayPoint(this, "New WayPoint", &wp);

	if(newWayPoint.exec())
	{
		ISql::pInstance()->add(wp);
		updateWayPoints();
	}
}

void FlightFormImpl::limitDistance()
{
	QString text;
	double dist;

	dist = lineEditDistance->text().toDouble();

	if(dist < 0.0)
	{
		dist = 0.0;
	}
	else if(dist > 500.0)
	{
		dist = 500.0;
	}

	text.sprintf("%.5f", dist);
	lineEditDistance->setText(text);
}

#include "moc_FlightFormImpl.cxx"

