/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                         *
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

#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qspinbox.h>
#include "Flytec6015.h"
#include "Glider.h"
#include "IFlyHighRC.h"
#include "IGliderForm.h"
#include "ISql.h"
#include "DeviceFrame6015Impl.h"

DeviceFrame6015Impl::DeviceFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: DeviceFrame6015(parent,name,fl)
{
	updateGlider();
}

DeviceFrame6015Impl::~DeviceFrame6015Impl()
{
}

void DeviceFrame6015Impl::newGlider()
{
	Glider glider;
	IGliderForm newGlider(this, "New Glider", &glider);
	
	if(newGlider.exec())
	{
		ISql::pInstance()->add(glider);
		updateGlider();
	}
}

void DeviceFrame6015Impl::update()
{
	Flytec6015 *pDev;
	Pilot dbPilot;
	QString dbPilotName;
	QString dbGlider;
	QString pilotName;
	QString callsign;
	QString glider;
	int syncRes;
	uint uiValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// pilot name
	pilotName = pDev->memoryRead(MemFa, OWNER, String).toString();
	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), dbPilot);
	dbPilot.fullName(dbPilotName);

	if(pilotName != dbPilotName.left(pilotName.length()))
	{
		syncRes = QMessageBox::question(this, "Different pilots", "Pilot on GPS differ from pilot in database. Set pilot?",
			 	"DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From database
				pilotName = dbPilotName;
			break;
			default:
			break;
		}
	}
	
	lineEdit_PilotName->setText(pilotName);

	// glider
	glider = pDev->memoryRead(MemFa, AC_TYPE, String).toString();
	dbGlider = m_gliderList.at(comboBoxModel->currentItem()).model();
	
	if(glider != dbGlider)
	{
		syncRes = QMessageBox::question(this, "Different gliders",
				"Glider on GPS differ from glider in database. Set glider?",
				"DB to Dialog", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From Pilot
				dbPilot.glider().fullName(dbGlider);
				selectGlider(dbGlider);
			break;
			default:
			break;
		}
	}

	// callsign
	callsign = pDev->memoryRead(MemFa, AC_ID, String).toString();
	
	if(callsign != dbPilot.callSign().left(callsign.length()))
	{
		syncRes = QMessageBox::question(this, "Different callsigns", "Callsign on GPS differ from callsign in database. Set callsign?",
				"DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From database
				callsign = dbPilot.callSign();
			break;
			default:
			break;
		}
	}

	lineEdit_GliderID->setText(callsign);

	// serial nr
	uiValue = pDev->memoryRead(MemPa, DEVICE_NR, UInt32).toUInt();
	lineEdit_SerialNr->setText(QString("%1").arg(uiValue));

	// device identifier
	uiValue = pDev->memoryRead(MemPa, DEVICE_TYPE, UInt8).toUInt();

	switch(uiValue)
	{
		case 0:
			lineEdit_DeviceIdent->setText("Flytec 6015");
		break;
		case 1:
			lineEdit_DeviceIdent->setText("IQ Basic GPS");
		break;
	}

	// sw version
	uiValue = pDev->memoryRead(MemPa, SW_VERS, UInt16).toUInt();
	lineEdit_SwVersion->setText(QString("%1").arg(uiValue));

	// auto off
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();
	checkBox_AutoOff->setChecked(uiValue & MASK_AUTO_POWER_OFF);

	// power off time
	uiValue = pDev->memoryRead(MemFa, POWER_OFF_TIME, UInt8).toUInt();
	spinBox_PowerOffTime->setValue(uiValue);
}

void DeviceFrame6015Impl::store()
{
	Flytec6015 *pDev;
	QString pilotName;
	Pilot dbPilot;
	QString dbPilotName;
	QString callsign;
	QString rcCallsign;
	QString dbGlider;
	QString glider;
	uint uiValue;
	int syncRes;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// pilot name
	pilotName = lineEdit_PilotName->text();
	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), dbPilot);
	dbPilot.fullName(dbPilotName);

	if(pilotName != dbPilotName.left(pilotName.length()))
	{
		syncRes = QMessageBox::question(this, "Different pilots", "Pilot in database differ from pilot in dialog. Set pilot?",
				"DB to GPS", "Ignore");

		switch(syncRes)
		{
			case 0: // From database
				pilotName = dbPilotName;
				lineEdit_PilotName->setText(dbPilotName);
			break;
			default:
			break;
		}
	}
	
	pDev->memoryWrite(MemFa, OWNER, String, pilotName);

	// glider
	m_gliderList.at(comboBoxModel->currentItem()).fullName(glider);
	dbPilot.glider().fullName(dbGlider);

	if(glider != dbGlider)
	{
		syncRes = QMessageBox::question(this, "Different gliders",
				"Glider in database differ from glider in dialog. Set glider?",
				"DB to GPS", "Ignore");

		switch(syncRes)
		{
			case 0: // From Pilot
				selectGlider(glider);
				glider = dbPilot.glider().model();
			break;
			default:
				glider = m_gliderList.at(comboBoxModel->currentItem()).model();
			break;
		}
	}
	else
	{
		glider = m_gliderList.at(comboBoxModel->currentItem()).model();
	}

	pDev->memoryWrite(MemFa, AC_TYPE, String, glider);

	// callsign
	callsign = lineEdit_GliderID->text();

	if(callsign != dbPilot.callSign().left(callsign.length()))
	{
		syncRes = QMessageBox::question(this, "Different callsigns", "Callsign in database differ from callsign in dialog. Set callsign?",
				"DB to GPS", "Ignore");

		switch(syncRes)
		{
			case 0: // From database
				callsign = dbPilot.callSign();
				lineEdit_GliderID->setText(callsign);
			break;
			default:
			break;
		}
	}

	pDev->memoryWrite(MemFa, AC_ID, String, callsign);

	// auto off
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();
	uiValue = (uiValue & ~MASK_AUTO_POWER_OFF) | (-checkBox_AutoOff->isChecked() & MASK_AUTO_POWER_OFF);
	pDev->memoryWrite(MemFa, DIV_FLAGS, UInt16, uiValue);

	// power off time
	uiValue = spinBox_PowerOffTime->value();
	uiValue = pDev->memoryWrite(MemFa, POWER_OFF_TIME, UInt8, uiValue);
}

void DeviceFrame6015Impl::updateGlider()
{
	QStringList list;
	QString gliderName;
	Glider::GliderListType::iterator it;
	Pilot dbPilot;
	
	comboBoxModel->clear();
	m_gliderList.clear();
	ISql::pInstance()->gliderList(m_gliderList);
	
	for(it=m_gliderList.begin(); it!=m_gliderList.end(); it++)
	{
		(*it).fullName(gliderName);
		comboBoxModel->insertItem(gliderName);
	}

	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), dbPilot);
	dbPilot.glider().fullName(gliderName);
	selectGlider(gliderName);
}

void DeviceFrame6015Impl::selectGlider(const QString &name)
{
	QString str;
	int index;
	int maxIndex;
	bool found = false;
	
	maxIndex = comboBoxModel->count();
	
	for(index=0; index<maxIndex; index++)
	{
		found = (comboBoxModel->text(index) == name);
		
		if(found)
		{
			comboBoxModel->setCurrentItem(index);
			break;
		}
	}
}

#include "DeviceFrame6015Impl.moc"

