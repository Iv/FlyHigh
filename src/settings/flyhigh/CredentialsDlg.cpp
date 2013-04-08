/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDialogButtonBox>
#include "CredentialsDlg.h"

#include "QDebug"


CredentialsDlg::CredentialsDlg(QWidget* parent, const QString& text)
	: QDialog(parent,Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setAutoFillBackground(false);
	setWindowTitle(tr("Authorization required"));

	QGridLayout* pLayout = new QGridLayout();
	setLayout(pLayout);

	QLabel* pTextLbl = new QLabel(text);
	QLabel* pUserLbl = new QLabel(tr("Username:"));
	m_pUsername = new QLineEdit(this);
	QLabel* pPassLbl = new QLabel(tr("Password:"));
	m_pPassword = new QLineEdit(this);
	m_pPassword->setEchoMode(QLineEdit::Password);

	QDialogButtonBox* pBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(pBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(pBox, SIGNAL(rejected()), this, SLOT(reject()));

	pLayout->addWidget(pTextLbl,1,1,1,2);
	pLayout->addWidget(pUserLbl,2,1);
	pLayout->addWidget(m_pUsername,2,2);
	pLayout->addWidget(pPassLbl,3,1);
	pLayout->addWidget(m_pPassword,3,2);
	pLayout->addWidget(pBox,4,1,1,2);

	m_pPassword->setFocus();

	adjustSize();
}

CredentialsDlg::~CredentialsDlg()
{
}

void CredentialsDlg::setUsername(const QString& user)
{
	m_pUsername->setText(user);
}

QString CredentialsDlg::getUsername() const
{
	return m_pUsername->text();
}

QString CredentialsDlg::getPassword() const
{
	return m_pPassword->text();
}
