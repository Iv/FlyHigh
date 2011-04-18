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

#include <QDialogButtonBox>
#include <QPushButton>
#include <QGroupBox>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include "DatabaseWidget.h"
#include "DatabaseParameters.h"
#include "MigrationDlg.h"

MigrationDlg::MigrationDlg(QWidget* parent)
	:QDialog(parent)
{
	setWindowTitle(tr("FlyHigh Database Migration"));
	setWindowModality(Qt::ApplicationModal);

	//copyThread = new DatabaseCopyThread(this);

	// db configuration widgets
	m_pFromDBConfig = new DatabaseWidget(this, tr("From:"));
	m_pToDBConfig = new DatabaseWidget(this, tr("To:"));
	m_pMigrateButton = new QPushButton(tr("Migrate ->"), this);
	m_pCancelButton = new QPushButton(tr("Cancel"), this);
	m_pCancelButton->setEnabled(false);

	// area for progress information
	QGroupBox* progressBox = new QGroupBox(tr("Progress Information"), this);
	QVBoxLayout* vlay = new QVBoxLayout(progressBox);
	m_pProgressBar = new QProgressBar(progressBox);
	m_pProgressBar->setTextVisible(true);
	m_pProgressBar->setRange(0,13);
	m_pProgressBarSmallStep = new QProgressBar(progressBox);
	m_pProgressBarSmallStep->setTextVisible(true);
	m_pOverallStepTitle = new QLabel(tr("Step Progress"), progressBox);
	vlay->addWidget(new QLabel(tr("Overall Progress"), progressBox));
	vlay->addWidget(m_pProgressBar);
	vlay->addWidget(m_pOverallStepTitle);
	vlay->addWidget(m_pProgressBarSmallStep);

	// assemble elements
	QWidget* mainWidget = new QWidget();
	QGridLayout* layout = new QGridLayout();
	mainWidget->setLayout(layout);
	layout->addWidget(m_pFromDBConfig,  0, 0, 4, 1);
	layout->addWidget(m_pMigrateButton,       1, 1);
	layout->addWidget(m_pCancelButton,        2, 1);
	layout->addWidget(m_pToDBConfig,    0, 2, 4, 1);
	layout->addWidget(progressBox,      4, 0, 1, 3);

	// add 'close' button at the bottom
	m_pButtonBox = new QDialogButtonBox(QDialogButtonBox::Close);
	// wire 'close' to reject(), but we don't really care
	connect(m_pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addWidget(mainWidget);
	vbox->addWidget(m_pButtonBox);

	// insert into dialog
	setLayout(vbox);
}

MigrationDlg::~MigrationDlg()
{
}
