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
#include <qdatetimeedit.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qstringlist.h>
#include <qspinbox.h>
#include <qtextedit.h>
#include "FlightFormImpl.h"
#include "ISql.h"
#include "IGliderForm.h"
#include "Glider.h"
#include "IWayPointForm.h"
#include "WayPoint.h"

FlightFormImpl::FlightFormImpl(QWidget* parent, const QString &caption, Flight *pFlight)
 :FlightForm(parent, caption, true)
{
	FlightForm::setCaption(caption);
	setFlight(pFlight);
}

void FlightFormImpl::updateWayPoints()
{
	QStringList list;
	WayPoint::WayPointListType wpList;
	WayPoint::WayPointListType::iterator it;
	
	ISql::pInstance()->wayPointList(wpList);
	comboBoxStart->clear();
	comboBoxLand->clear();
	
	for(it=wpList.begin(); it!=wpList.end(); it++)
	{
		comboBoxStart->insertItem((*it).name());
		comboBoxLand->insertItem((*it).name());
	}
}

void FlightFormImpl::updateGlider()
{
	QStringList list;
	QString gliderModel;
	Glider::GliderListType gliderList;
	Glider::GliderListType::iterator it;
	
	ISql::pInstance()->gliderList(gliderList);
	comboBoxModel->clear();
	
	for(it=gliderList.begin(); it!=gliderList.end(); it++)
	{
		(*it).modelOfGlider(gliderModel);
		comboBoxModel->insertItem(gliderModel);
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
	
	if(pFlight != NULL)
	{
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
	m_pFlight->setGlider(comboBoxModel->currentText());

	// start
	m_pFlight->setStartPt(comboBoxStart->currentText());
	
	// land
	m_pFlight->setLandPt(comboBoxLand->currentText());
	
	// distance
	m_pFlight->setDistance((uint)(lineEditDistance->text().toDouble()*1000.0));

	// comment
	m_pFlight->setComment(textEditComment->text());

  QDialog::accept();
}

void FlightFormImpl::selectStart()
{
	QString str;
	int index;
	int maxIndex;
	bool found = false;
	
	maxIndex = comboBoxStart->count();
	str =  m_pFlight->startPt();
	
	for(index=0; index<maxIndex; index++)
	{
		found = (comboBoxStart->text(index) == str);
		
		if(found)
		{
			comboBoxStart->setCurrentItem(index);
			break;
		}
	}
}

void FlightFormImpl::selectLand()
{
	QString str;
	int index;
	int maxIndex;
	bool found = false;
	
	maxIndex = comboBoxLand->count();
	str = m_pFlight->landPt();
	
	for(index=0; index<maxIndex; index++)
	{
		found = (comboBoxLand->text(index) == str);
		
		if(found)
		{
			comboBoxLand->setCurrentItem(index);
			break;
		}
	}
}

void FlightFormImpl::selectGlider()
{
	QString str;
	int index;
	int maxIndex;
	bool found = false;
	
	str = m_pFlight->glider();
	maxIndex = comboBoxModel->count();
	
	for(index=0; index<maxIndex; index++)
	{
		found = (comboBoxModel->text(index) == str);
		
		if(found)
		{
			comboBoxModel->setCurrentItem(index);
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

#include "FlightFormImpl.moc"

