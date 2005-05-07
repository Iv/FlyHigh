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

#include <qpushbutton.h>
#include <qdatetimeedit.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qstringlist.h>
#include <qtextedit.h>
#include "Glider.h"
#include "IGliderForm.h"
#include "ISql.h"
#include "Servicing.h"
#include "ServicingFormImpl.h"

ServicingFormImpl::ServicingFormImpl(QWidget* parent, const QString &caption, Servicing *pServicing)
	:ServicingForm(parent, caption, true)
{
	ServicingForm::setCaption(caption);
	setServicing(pServicing);
	updateGlider();
	selectGlider();
}

void ServicingFormImpl::accept()
{
	m_pServicing->setDate(dateEditDate->date());
	m_pServicing->setGlider(comboBoxModel->currentText());
	m_pServicing->setResponsibility(lineEditRespons->text());
	m_pServicing->setComment(textEditComment->text());

  QDialog::accept();
}

void ServicingFormImpl::newGlider()
{
	Glider glider;
	IGliderForm newGlider(this, "New Glider", &glider);
	
	if(newGlider.exec())
	{
		ISql::pInstance()->add(glider);
		updateGlider();
	}
}

void ServicingFormImpl::updateGlider()
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

void ServicingFormImpl::selectGlider()
{
	QString str;
	int index;
	int maxIndex;
	bool found = false;
	
	str = m_pServicing->glider();
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

void ServicingFormImpl::setServicing(Servicing *pServicing)
{
	m_pServicing = pServicing;
	
	if(pServicing != NULL)
	{
		dateEditDate->setDate(m_pServicing->date());
	}
}

#include "ServicingFormImpl.moc"
