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
#include <QLabel>
#include <QGroupBox>
#include <QApplication>
#include <QStringList>
#include "IFlyHighRC.h"
#include "DeviceConnectionParameters.h"
#include "DeviceConnectionWidget.h"

DeviceConnectionWidget::DeviceConnectionWidget(QWidget* parent, const QString& title)
	:QWidget(parent)
{
	setAutoFillBackground(false);

  m_pDeviceType = new QComboBox();
  m_pDevicePort = new QComboBox();
  m_pDeviceLineSpeed = new QComboBox();
  m_pDeviceTypeLabel = new QLabel(tr("Device type"));
  m_pDevicePortLabel = new QLabel(tr("Port"));
  m_pDeviceLineSpeedLabel = new QLabel(tr("Line Speed"));

  populateValues();

  QGroupBox* deviceSettings = new QGroupBox(title);
  QVBoxLayout* layout = new QVBoxLayout();
  QVBoxLayout* deviceBoxLayout = new QVBoxLayout(deviceSettings);

  deviceBoxLayout->addWidget(m_pDeviceTypeLabel);
  deviceBoxLayout->addWidget(m_pDeviceType);
  deviceBoxLayout->addWidget(m_pDevicePortLabel);
  deviceBoxLayout->addWidget(m_pDevicePort);
  deviceBoxLayout->addWidget(m_pDeviceLineSpeedLabel);
  deviceBoxLayout->addWidget(m_pDeviceLineSpeed);
  deviceBoxLayout->setSpacing(0);

  layout->setMargin(0);
  layout->addWidget(deviceSettings);
  layout->addStretch();

	setLayout(layout);

	adjustSize();
}

DeviceConnectionWidget::~DeviceConnectionWidget()
{
}

DeviceConnectionParameters DeviceConnectionWidget::getParameters() const {

  DeviceConnectionParameters parameters(m_pDeviceType->currentText(),
                                        m_pDevicePort->itemData(m_pDevicePort->currentIndex()).toString(),
                                        m_pDeviceLineSpeed->currentText());

  return parameters;
}

void DeviceConnectionWidget::populateValues()
{
  // assuming indexes are matching
  m_pDeviceType->addItems(IFlyHighRC::pInstance()->deviceNameList());
  m_pDeviceType->setCurrentIndex(IFlyHighRC::pInstance()->deviceNameIndex());
  m_pDeviceLineSpeed->addItems(IFlyHighRC::pInstance()->deviceSpeedList());
  m_pDeviceLineSpeed->setCurrentIndex(IFlyHighRC::pInstance()->deviceSpeedIndex());

  // we're storing the port name in the item data, this way we may
  // use the text for additional information, such as the currently
  // configured port is not available now (e.g. USB devices not plugged in)
  // we add the currently configured first
  QStringList lines = IFlyHighRC::pInstance()->deviceLineList();
  const QString& curLine = IFlyHighRC::pInstance()->deviceLine();
  int idx = lines.indexOf(curLine);
  if(idx >= 0)
  {
    // the currently configured port is available
    m_pDevicePort->addItem(curLine,curLine);
    // remove from list
    lines.removeAt(idx);
  }
  else
  {
    // configured port is not available
    // we add it to the list and put a note there
    m_pDevicePort->addItem(curLine + tr(" (unavailable)"),curLine);
  }

  // add the remaining stuff
  foreach(QString line,lines)
  {
    m_pDevicePort->addItem(line,line);
  }
}
