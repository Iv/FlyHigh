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

#include <QDateTimeEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QStringList>
#include <QTextEdit>
#include "Glider.h"
#include "IGliderForm.h"
#include "ISql.h"
#include "Servicing.h"
#include "ServicingFormImpl.h"

ServicingFormImpl::ServicingFormImpl(QWidget* parent, const QString &caption, Servicing *pServicing)
	:QDialog(parent)
{
  setupUi(this);
  setWindowTitle(caption);
  Q_CHECK_PTR(pServicing);
	updateGlider();
	setServicing(pServicing);
}

void ServicingFormImpl::accept()
{
	m_pServicing->setDate(dateEditDate->date());
	m_pServicing->setGlider(m_gliderList[comboBoxModel->currentIndex()]);
	m_pServicing->setResponsibility(lineEditRespons->text());
	m_pServicing->setComment(textEditComment->toPlainText());

  QDialog::accept();
}

void ServicingFormImpl::newGlider()
{
  Glider::GliderListType gliders;
	Glider glider;

  ISql::pInstance()->gliderList(gliders);
	IGliderForm newGlider(this, "New Glider", &glider, gliders);

	if(newGlider.exec())
	{
		ISql::pInstance()->add(glider);
		updateGlider();
	}
}

void ServicingFormImpl::updateGlider()
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

void ServicingFormImpl::setServicing(Servicing *pServicing)
{
  QString str;

	m_pServicing = pServicing;

	if(pServicing != NULL)
	{
		dateEditDate->setDate(m_pServicing->date());
		m_pServicing->glider().fullName(str);
    select(comboBoxModel, str);
    lineEditRespons->setText(m_pServicing->responsibility());
    textEditComment->setText(m_pServicing->comment());
	}
}

void ServicingFormImpl::select(QComboBox *pCombo, const QString &text)
{
	int index;
	int maxIndex;
	bool found = false;

	maxIndex = pCombo->count();

	for(index=0; index<maxIndex; index++)
	{
		found = (pCombo->itemText(index) == text);

		if(found)
		{
			pCombo->setCurrentIndex(index);
			break;
		}
	}
}
