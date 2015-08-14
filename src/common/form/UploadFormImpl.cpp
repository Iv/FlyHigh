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

#include <QInputDialog>
#include <QLineEdit>
#include "ISql.h"
#include "IFlyHighRC.h"
#include "IAccountForm.h"
#include "IOLCUploader.h"
#include "OLCUploaderFactory.h"
#include "Account.h"
#include "UploadFormImpl.h"


UploadFormImpl::UploadFormImpl(QWidget* parent, const QString &caption, Flight *pFlight)
  :QDialog(parent)
{
  setupUi(this);
  // change text on "apply" button
  buttonBox->button(QDialogButtonBox::Apply)->setText(tr("Upload"));
  progressBar->setMinimum(0);
  progressBar->setMaximum(100);
  progressBar->setValue(0);
  setWindowTitle(caption);

  connect(buttonBox->button(QDialogButtonBox::Apply),SIGNAL(clicked()),this,SLOT(upload()));
  connect(buttonBox->button(QDialogButtonBox::Close),SIGNAL(clicked()),this,SLOT(close()));

	setFlight(pFlight);
  updateAccount();
  validate();
}

void UploadFormImpl::setFlight(Flight *pFlight)
{
  m_pFlight = pFlight;
}

void UploadFormImpl::close()
{
  QDialog::accept();
}

void UploadFormImpl::upload()
{
  bool ok;
  QString password;
  Account* pAccount = &m_accountList[comboBoxModel->currentIndex()];
  IOLCUploader* pUploader = OLCUploaderFactory::getOLCUploader(pAccount);

  connect(pUploader, SIGNAL(error(QString)), this, SLOT(errorOnUpload(QString)));
  connect(pUploader, SIGNAL(finished()), this, SLOT(uploadSuccessful()));
  connect(pUploader, SIGNAL(step(QString,int)), this, SLOT(updateProgress(QString,int)));

  // get password if none is provided
  if(pAccount->password().isEmpty())
  {
    password = QInputDialog::getText(this,
                                     tr("Password required"),
                                     tr("Password for user %1:").arg(pAccount->username()),
                                     QLineEdit::Password,
                                     "",
                                     &ok);
    if(!ok)
    {
      // cancel pressed
      return;
    }
    pAccount->setPassword(password);
  }

  if(pUploader)
  {
    m_upload = true;
    validate(); // disable button
    pUploader->uploadFlight(m_pFlight);
  }
}

void UploadFormImpl::newAccount()
{
  Account::AccountListType accounts;
  Account account;

  ISql::pInstance()->accountList(accounts);

  IAccountForm newAccount(this, "New Online Contest Account", &account, accounts);

  if(newAccount.exec())
  {
    ISql::pInstance()->add(account);
    updateAccount();
    validate();
  }
}

void UploadFormImpl::validate()
{
  bool valid;
  QPushButton* pUploadButton;

  valid = (comboBoxModel->currentIndex() >= 0);
  pUploadButton = buttonBox->button(QDialogButtonBox::Apply);
  pUploadButton->setEnabled(valid && !m_upload);
}

void UploadFormImpl::updateAccount()
{
  Account::AccountListType::iterator it;
  QString entry;
  m_accountList.clear();
  ISql::pInstance()->accountList(m_accountList);
  comboBoxModel->clear();

  for(it=m_accountList.begin(); it!=m_accountList.end(); it++)
  {
    entry = it->username() + " (" + Account::typeAsString(it->type()) + ")";
    comboBoxModel->addItem(entry);
  }
}

void UploadFormImpl::uploadSuccessful()
{
  // set progress bar to 100%
  progressBar->setValue(progressBar->maximum());
  // set result label
  textLabelResult->setText("Success!");
  textLabelResult->setStyleSheet("QLabel { color : green; }");
  // write to text area
  messages->appendPlainText(tr("Flight successfuly uploaded"));
}

void UploadFormImpl::updateProgress(const QString& msg, const int& percent)
{
  int percentage = percent;
  if(percent < progressBar->minimum())
  {
    percentage = progressBar->minimum();
  } else if (percent > progressBar->maximum())
  {
    percentage = progressBar->maximum();
  }
  progressBar->setValue(percentage);
  messages->appendPlainText(msg);
}

void UploadFormImpl::errorOnUpload(const QString& msg)
{
  // set result label
  textLabelResult->setText("Error!");
  textLabelResult->setStyleSheet("QLabel { color : red; }");
  // write to text area
  messages->appendPlainText(msg);
  messages->appendPlainText(tr("Error while uploading flight"));
}

#include "moc_UploadFormImpl.cxx"
