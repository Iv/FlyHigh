/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <QCursor>
#include <QFileDialog>
#include "UploadFormImpl.h"
#include "ISql.h"
#include "IFlyHighRC.h"
#include "IAccountForm.h"
#include "Account.h"

#include <QDebug>

UploadFormImpl::UploadFormImpl(QWidget* parent, const QString &caption, Flight *pFlight)
  :QDialog(parent)
{
  setupUi(this);
  setWindowTitle(caption);
	setFlight(pFlight);
}

void UploadFormImpl::enableInput(bool b)
{
	buttonOk->setEnabled(b);
  pushButtonAccount->setEnabled(b);
  comboBoxModel->setEnabled(b);
  checkBoxFlightActive->setEnabled(b);

	if(b)
	{
		QWidget::unsetCursor();
	}
	else
	{
		QWidget::setCursor(QCursor(Qt::WaitCursor));
	}
}

void UploadFormImpl::setFlight(Flight *pFlight)
{
  m_pFlight = pFlight;
}

void UploadFormImpl::accept()
{
  QDialog::accept();
}


void UploadFormImpl::selectAccount()
{
	int index;
	int maxIndex;
	bool found = false;

	maxIndex = comboBoxModel->count();

	for(index=0; index<maxIndex; index++)
	{
    //found = (m_gliderList[index] == m_pFlight->glider());
    found = true;

		if(found)
		{
			comboBoxModel->setCurrentIndex(index);
			break;
		}
	}
}

void UploadFormImpl::newAccount()
{
  IAccountForm newAccount(this, "New Online Contest Account", NULL);

  if(newAccount.exec())
  {
    qDebug() << "New Account created";
  }
}

#include "moc_UploadFormImpl.cxx"
