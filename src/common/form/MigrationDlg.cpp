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
#include <QMessageBox>
#include "DatabaseWidget.h"
#include "DatabaseParameters.h"
#include "Migrator.h"
#include "MigratorThread.h"
#include "MigrationDlg.h"

MigrationDlg::MigrationDlg(QWidget* parent)
	:QDialog(parent)
{
	m_pMigratorThread = new MigratorThread(this);

	setWindowTitle(tr("FlyHigh Database Migration"));
	setWindowModality(Qt::ApplicationModal);

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
	m_pProgressBar->setRange(0,10); // 10 steps
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

	connect(m_pMigrateButton, SIGNAL(clicked()), this, SLOT(performCopy()));

	connect(m_pMigratorThread->migrator(),
					SIGNAL(finished(int, QString)),
					this,
					SLOT(handleFinish(int, QString)));

	connect(m_pMigratorThread->migrator(),
					SIGNAL(stepStarted(QString)),
					this,
					SLOT(handleStepStarted(QString)));

	connect(m_pMigratorThread->migrator(),
					SIGNAL(smallStepStarted(int, int)),
					this,
					SLOT(handleSmallStepStarted(int, int)));

	connect(m_pButtonBox,
					SIGNAL(rejected()),
					m_pMigratorThread->migrator(),
					SLOT(stopProcessing()));

	connect(m_pCancelButton,
					SIGNAL(clicked()),
					m_pMigratorThread->migrator(),
					SLOT(stopProcessing()));

}

MigrationDlg::~MigrationDlg()
{
	m_pMigratorThread->terminate();
	delete m_pMigratorThread;
}

void MigrationDlg::performCopy()
{
	m_pMigratorThread->init(m_pFromDBConfig->getDatabaseParameters(),
													m_pToDBConfig->getDatabaseParameters());

	lockInputFields();
	m_pMigratorThread->start();
}

void MigrationDlg::unlockInputFields()
{
	m_pFromDBConfig->setEnabled(true);
	m_pToDBConfig->setEnabled(true);
	m_pMigrateButton->setEnabled(true);
	m_pProgressBar->setValue(0);
	m_pProgressBarSmallStep->setValue(0);
	m_pCancelButton->setEnabled(false);
}

void MigrationDlg::lockInputFields()
{
	m_pFromDBConfig->setEnabled(false);
	m_pToDBConfig->setEnabled(false);
	m_pMigrateButton->setEnabled(false);
	m_pCancelButton->setEnabled(true);
}

void MigrationDlg::handleFinish(int finishState, QString errorMsg)
{
	switch (finishState)
	{
	case Migrator::failed:
		QMessageBox::critical(this, tr("Database migrator"), errorMsg );
		unlockInputFields();
		break;
	case Migrator::success:
		QMessageBox::information(this, tr("Database migrator"), tr("Database copied successfully.") );
		unlockInputFields();
		break;
	case Migrator::canceled:
		QMessageBox::information(this, tr("Database migrator"), tr("Database conversion canceled.") );
		unlockInputFields();
		break;
	}
}

void MigrationDlg::handleStepStarted(const QString& stepName)
{
	int progressBarValue = m_pProgressBar->value();
	QString txt = tr("Step Progress (%1)");
	m_pOverallStepTitle->setText(txt.arg(stepName));
	m_pProgressBar->setValue(++progressBarValue);
}

void MigrationDlg::handleSmallStepStarted(int currentValue, int maxValue)
{
	m_pProgressBarSmallStep->setMaximum(maxValue);
	m_pProgressBarSmallStep->setValue(currentValue);
}
