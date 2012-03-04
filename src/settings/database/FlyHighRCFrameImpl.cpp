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

#include <qcombobox.h>
#include <qlineedit.h>
#include "FlyHighRCFrameImpl.h"
#include "IFlyHighRC.h"
#include "IGliderForm.h"
#include "ISql.h"

FlyHighRCFrameImpl::FlyHighRCFrameImpl(QWidget* parent)
	:QDialog(parent)
{
	setupUi(this);

	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), m_curPilot);
	lineEdit_FirstName->setText(m_curPilot.firstName());
	lineEdit_LastName->setText(m_curPilot.lastName());
	dateEditDateOfBirth->setDate(m_curPilot.birthDate());
	lineEdit_GliderID->setText(m_curPilot.callSign());
	updateGlider();
	selectGlider(m_curPilot.glider());
}

void FlyHighRCFrameImpl::newGlider()
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

void FlyHighRCFrameImpl::selectGlider(Glider &glider)
{
	QString str;
	int index;
	int maxIndex;
	bool found = false;

	maxIndex = comboBoxModel->count();

	for(index=0; index<maxIndex; index++)
	{
		found = (m_gliderList[index] == glider);

		if(found)
		{
			comboBoxModel->setCurrentIndex(index);
			break;
		}
	}
}

void FlyHighRCFrameImpl::updateGlider()
{
	QStringList list;
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

void FlyHighRCFrameImpl::ok()
{
	m_curPilot.setFirstName(lineEdit_FirstName->text());
	m_curPilot.setLastName(lineEdit_LastName->text());
	m_curPilot.setBirthDate(dateEditDateOfBirth->date());
	m_curPilot.setCallSign(lineEdit_GliderID->text());

	if(m_gliderList.size() > 0)
	{
                m_curPilot.setGlider(m_gliderList.at(comboBoxModel->currentIndex()));
	}

	if(m_curPilot.id() < 0)
	{
		if(ISql::pInstance()->setId(m_curPilot))
		{
			ISql::pInstance()->update(m_curPilot);
		}
		else
		{
			ISql::pInstance()->add(m_curPilot);
		}

		IFlyHighRC::pInstance()->setPilotId(m_curPilot.id());
	}
	else
	{
		ISql::pInstance()->update(m_curPilot);
	}
}

#include "moc_FlyHighRCFrameImpl.cxx"
