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

#include <QTabWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "DatabaseWidget.h"
#include "DatabaseParameters.h"
#include "DeviceConnectionWidget.h"
#include "DeviceConnectionParameters.h"
#include "FoldersWidgetImpl.h"
#include "FlyHighSettingsDlg.h"

FlyHighSettingsDlg::FlyHighSettingsDlg(QWidget* parent)
	:QDialog(parent)
{
  QWidget *pWidget;

	setWindowTitle(tr("Configure - FlyHigh"));
	setWindowModality(Qt::ApplicationModal);
  resize(450, 300);

	// organized in tabs
	m_pTabWidget = new QTabWidget();

  // a tab for device preferences
  m_pDeviceConfig = new DeviceConnectionWidget(this, tr("Device Connection"));
  m_pTabWidget->addTab(m_pDeviceConfig, tr("Device"));

  // a tab for database preferences
	m_pDBConfig = new DatabaseWidget(this, tr("Database Settings"));
	m_pTabWidget->addTab(m_pDBConfig, tr("Database"));

  // a tab for folders
	pWidget = new FoldersWidgetImpl(this, tr("Folders"));
	m_pTabWidget->addTab(pWidget, tr("Folders"));

  // ok/cancel button
	m_pButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	// wire buttons to dialog slots
	connect(m_pButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

	// arrange the stuff
	m_pLayout = new QVBoxLayout();
	m_pLayout->addWidget(m_pTabWidget);
	m_pLayout->addWidget(m_pButtonBox);
	setLayout(m_pLayout);

	show();
}

FlyHighSettingsDlg::~FlyHighSettingsDlg()
{
	delete m_pDBConfig;
	delete m_pTabWidget;
	delete m_pButtonBox;
	delete m_pLayout;
}

const DatabaseParameters FlyHighSettingsDlg::getDBParameters() const
{
	return m_pDBConfig->getDatabaseParameters();
}

const DeviceConnectionParameters FlyHighSettingsDlg::getDeviceConnectionParameters() const
{
  return m_pDeviceConfig->getParameters();
}
