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

#include "AirSpace.h"
#include "AirSpaceFormImpl.h"

AirSpaceFormImpl::AirSpaceFormImpl(QWidget* parent, const QString &caption, AirSpace *pAirSpace)
	:QDialog(parent)
{
	setupUi(this);
	setWindowTitle(caption);
	setAirSpace(pAirSpace);
}

void AirSpaceFormImpl::setAirSpace(AirSpace *pAirSpace)
{
	m_pAirSpace = pAirSpace;

	if(pAirSpace != NULL)
	{
		lineEditName->setText(m_pAirSpace->name());
    spinBoxLow->setValue(m_pAirSpace->low());
    spinBoxHigh->setValue(m_pAirSpace->high());
    lineEditClass->setText(m_pAirSpace->airspaceClass());
    textEditComment->setText(m_pAirSpace->remark());
	}
}

void AirSpaceFormImpl::accept()
{
  if(m_pAirSpace != NULL)
  {
    m_pAirSpace->setName(lineEditName->text());
    m_pAirSpace->setLow(spinBoxLow->value());
    m_pAirSpace->setHigh(spinBoxHigh->value());
    m_pAirSpace->setAirspaceClass(lineEditClass->text());
    m_pAirSpace->setRemark(textEditComment->toPlainText());
  }

  QDialog::accept();
}
