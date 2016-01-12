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

#include "AccountFormImpl.h"

AccountFormImpl::AccountFormImpl(QWidget* parent, const QString &caption, Account *pAccount)
  :QDialog(parent)
{
  QStringList contests;

  setupUi(this);
  setWindowTitle(caption);

  contests.push_back(Account::typeAsString(Account::XCONTEST));
  comboBox->addItems(contests);

  setAccount(pAccount);

  validate();
}

void AccountFormImpl::setAccount(Account *pAccount)
{
  m_pAccount = pAccount;

  if(m_pAccount != NULL)
  {
    select(comboBox, Account::typeAsString(m_pAccount->type()));
    lineEditUsername->setText(m_pAccount->username());
    lineEditPassword->setText(m_pAccount->password());
    textEditNotes->setText(m_pAccount->description());
  }
}

void AccountFormImpl::accept()
{
  if(m_pAccount != NULL)
  {
    m_pAccount->setUsername(lineEditUsername->text());
    m_pAccount->setPassword(lineEditPassword->text());
    m_pAccount->setDescription(textEditNotes->toPlainText());
    m_pAccount->setType((Account::OLCType)comboBox->currentIndex());
  }
  QDialog::accept();
}

void AccountFormImpl::validate()
{
  bool valid;
  valid = !lineEditUsername->text().isEmpty();
  buttonOk->setEnabled(valid);
}

void AccountFormImpl::select(QComboBox *pCombo, const QString &text)
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
