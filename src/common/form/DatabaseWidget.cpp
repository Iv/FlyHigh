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

#include <QComboBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSqlDatabase>
#include <QSqlError>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QMessageBox>
#include <QApplication>
#include "IFlyHighRC.h"
#include "DatabaseWidget.h"

#include "QDebug"


DatabaseWidget::DatabaseWidget(QWidget* parent)
	: QWidget(parent)
{
	setAutoFillBackground(false);

	QVBoxLayout* layout = new QVBoxLayout();
	setLayout(layout);

	QGroupBox* sqliteSettings = new QGroupBox();
	QVBoxLayout* vlay    = new QVBoxLayout(sqliteSettings);
	m_pDBPathLabel = new QLabel(tr("<p>The location where the database file will be stored on your system.<br/>"
																 "Write access is required.</p>"),
															sqliteSettings);
	m_pDBPathLabel->setWordWrap(true);

	QHBoxLayout* dBFileLayout = new QHBoxLayout();
	m_pDBPath = new QLineEdit();
	m_pFileButton = new QPushButton(QIcon::fromTheme("document-open"),"");

	dBFileLayout->addWidget(m_pDBPath);
	dBFileLayout->addWidget(m_pFileButton);

	QLabel* dbTypeLabel = new QLabel(tr("Type"));
	m_pDBType = new QComboBox();
	QLabel* dbNameLabel = new QLabel(tr("Database Name"));
	m_pDBName = new QLineEdit();
	QLabel* hostNameLabel = new QLabel(tr("Host Name"));
	m_pDBHostName = new QLineEdit();
	QLabel* hostPortLabel = new QLabel(tr("Port"));
	m_pDBPort = new QSpinBox();
	m_pDBPort->setMaximum(65536);
	QLabel* userNameLabel = new QLabel(tr("User"));
	m_pDBUserName = new QLineEdit();
	QLabel* passwordLabel = new QLabel(tr("Password"));
	m_pDBPassword = new QLineEdit();
	m_pDBPassword->setEchoMode(QLineEdit::Password);

	QPushButton* checkDatabaseConnectionButton = new QPushButton(tr("Check DB Connection"));

	m_pMySQLSettings = new QGroupBox();
	m_pMySQLSettings->setFlat(true);
	QFormLayout* mySQLLayout    = new QFormLayout();
	m_pMySQLSettings->setLayout(mySQLLayout);

	mySQLLayout->addRow(hostNameLabel, m_pDBHostName);
	mySQLLayout->addRow(hostPortLabel, m_pDBPort);
	mySQLLayout->addRow(dbNameLabel, m_pDBName);
	mySQLLayout->addRow(userNameLabel, m_pDBUserName);
	mySQLLayout->addRow(passwordLabel, m_pDBPassword);

	mySQLLayout->addWidget(checkDatabaseConnectionButton);

	vlay->addWidget(dbTypeLabel);
	vlay->addWidget(m_pDBType);
	vlay->addWidget(m_pDBPathLabel);
	vlay->addLayout(dBFileLayout);
	vlay->addWidget(m_pMySQLSettings);
	vlay->setSpacing(0);


	layout->setMargin(0);
	layout->addWidget(sqliteSettings);
	layout->addStretch();

	m_pDBType->addItem(tr("SQLite"), "QSQLITE");
	m_pDBType->addItem(tr("MySQL"), "QMYSQL");

	connect(m_pFileButton,SIGNAL(clicked()), this, SLOT(slotDBFileSelected()));

	connect(m_pDBType,
					SIGNAL(currentIndexChanged(int)),
					this,
					SLOT(slotHandleDBTypeIndexChanged(int)));

	connect(checkDatabaseConnectionButton,
					SIGNAL(clicked()),
					this,
					SLOT(checkDatabaseConnection()));

	// preset values from config
	DatabaseParameters paras;
	paras.readFromConfig();
	m_pDBPath->setText(paras.dBFile());
	m_pDBName->setText(paras.dBName());
	m_pDBHostName->setText(paras.dBHostName());
	m_pDBPort->setValue(paras.dBPort());
	m_pDBUserName->setText(paras.dBUserName());
	m_pDBPassword->setText(paras.dBPassword());

	for (int i=0; i<m_pDBType->count(); i++)
	{
		if (m_pDBType->itemData(i).toString() == paras.dBType())
		{
			m_pDBType->setCurrentIndex(i);
			break;
		}
	}

	setDatabaseInputFields(paras.dBType());

	adjustSize();
}

DatabaseWidget::~DatabaseWidget()
{
}

void DatabaseWidget::slotDBFileSelected()
{
	QString file = QFileDialog::getOpenFileName(this,
																							tr("Select file"),
																							IFlyHighRC::pInstance()->lastDir(),
																							tr("DB files (*.sqlite *.db)"),
																							0,
																							QFileDialog::DontUseNativeDialog);
	// ok or cancel?
	if (file != "")
	{
		// update entry field
		m_pDBPath->setText(file);
	}
}


void DatabaseWidget::slotHandleDBTypeIndexChanged(int index)
{
	const QString& dbType = m_pDBType->itemData(index).toString();
	setDatabaseInputFields(dbType);
}

void DatabaseWidget::setDatabaseInputFields(const QString& currentIndexStr)
{
	if (currentIndexStr == QString(DatabaseParameters::SQLiteDatabaseType()))
	{
		m_pDBPathLabel->setVisible(true);
		m_pDBPath->setVisible(true);
		m_pFileButton->setVisible(true);
		m_pMySQLSettings->setVisible(false);
	}
	else
	{
		m_pDBPathLabel->setVisible(false);
		m_pDBPath->setVisible(false);
		m_pFileButton->setVisible(false);
		m_pMySQLSettings->setVisible(true);
	}

	adjustSize();
}


void DatabaseWidget::checkDatabaseConnection()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);

	QString databaseID("ConnectionTest");
	DatabaseParameters paras = getDatabaseParameters();
	{
		QSqlDatabase testDatabase = QSqlDatabase::addDatabase(paras.dBType(), databaseID);
		testDatabase.setHostName(paras.dBHostName());
		testDatabase.setPort(paras.dBPort());
		testDatabase.setUserName(paras.dBUserName());
		testDatabase.setPassword(paras.dBPassword());

		bool result = testDatabase.open();

		QApplication::restoreOverrideCursor();

		if (result)
		{
			QMessageBox::information(this,
															 tr("Database connection test"),
															 tr("Database connection test successful."));
		}
		else
		{
			QMessageBox::critical(this,
														tr("Database connection test"),
														tr("Database connection test was not successful.\n"
															 "Error was: %1").arg(testDatabase.lastError().text()));
		}

		testDatabase.close();
	}
	QSqlDatabase::removeDatabase(databaseID);
}

DatabaseParameters DatabaseWidget::getDatabaseParameters()
{
	DatabaseParameters parameters(m_pDBType->itemData(m_pDBType->currentIndex()).toString(),
																m_pDBName->text(),
																m_pDBHostName->text(),
																m_pDBPort->text().toInt(),
																m_pDBUserName->text(),
																m_pDBPassword->text(),
																m_pDBPath->text());
	return parameters;
}
