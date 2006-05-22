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
#include <qlineedit.h>
#include <qmessagebox.h>
#include "IFlyHighRC.h"
#include "Glider.h"
#include "IGliderForm.h"
#include "ISql.h"

#include "DeviceFrameImpl.h"
extern "C"
{
	#include "flytec_al.h"
}

DeviceFrameImpl::DeviceFrameImpl(QWidget* parent, const char* name, WFlags fl)
: DeviceFrame(parent,name,fl)
{
	updateGlider();
}

DeviceFrameImpl::~DeviceFrameImpl()
{
}

void DeviceFrameImpl::newGlider()
{
	Glider glider;
	IGliderForm newGlider(this, "New Glider", &glider);
	
	if(newGlider.exec())
	{
		ISql::pInstance()->add(glider);
		updateGlider();
	}
}

void DeviceFrameImpl::update(QByteArray &arr)
{
	QString pilot;
	QString rcPilot;
	QString callsign;
	QString rcCallsign;
	QString glider;
	QString rcGlider;
	DeviceInfoType devInfo;
	char str[FT_STRING_SIZE];
	int syncRes;
	
	if(ft_deviceInfoRead(&devInfo) == 0)
	{
		lineEdit_SerialNr->setText(QString("%1").arg(devInfo.serialNr));
		lineEdit_SwVersion->setText(QString(devInfo.swVersion));
		lineEdit_DeviceIdent->setText(QString(devInfo.deviceIdent));
	}

	// pilot name
	ft_ftstring2string(str, (char*)&arr[PILOT_NAME_POS]);
	pilot = str;
	rcPilot = IFlyHighRC::pInstance()->pilotName();
	
	if(pilot != rcPilot)
	{
		syncRes = QMessageBox::question(this, "Different pilots", "Pilot on GPS differ from pilot in database. Sync pilots?",
			 	"GPS to DB", "DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From GPS
				IFlyHighRC::pInstance()->setPilotName(pilot);
			break;
			case 1: // From database
				pilot = rcPilot;
			break;
			default:
			break;
		}
	}
	
	lineEdit_PilotName->setText(pilot);
	
	// glider
	ft_ftstring2string(str, (char*)&arr[GLYDER_TYPE_POS]);
	glider = str;
	rcGlider = IFlyHighRC::pInstance()->glider();
	
	if(glider != rcGlider)
	{
		syncRes = QMessageBox::question(this, "Different gliders", "Glider on GPS differ from glider in database. Sync gliders?",
			 	"GPS to DB", "DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From GPS
				IFlyHighRC::pInstance()->setGlider(glider);
			break;
			case 1: // From database
				glider = rcGlider;
			break;
			default:
			break;
		}
	}
	
	selectGlider(glider);

	// callsign
	ft_ftstring2string(str, (char*)&arr[GLYDER_ID_POS]);
	callsign = str;
	rcCallsign = IFlyHighRC::pInstance()->callsign();
	
	if(callsign != rcCallsign)
	{
		syncRes = QMessageBox::question(this, "Different callsigns", "Callsign on GPS differ from callsign in database. Sync callsigns?",
			 	"GPS to DB", "DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From GPS
				IFlyHighRC::pInstance()->setCallsign(callsign);
			break;
			case 1: // From database
				callsign = rcCallsign;
			break;
			default:
			break;
		}
	}
	
	// glider id
	lineEdit_GliderID->setText(callsign);

	// battery type
	comboBox_BattType->setCurrentItem(arr[BATT_TYPE_POS]);
}

void DeviceFrameImpl::store(QByteArray &arr)
{
	QString pilot;
	QString rcPilot;
	QString callsign;
	QString rcCallsign;
	QString glider;
	QString rcGlider;
	int syncRes;
	
	// pilot
	pilot = lineEdit_PilotName->text();
	rcPilot = IFlyHighRC::pInstance()->pilotName();
	
	if(pilot != rcPilot)
	{
		syncRes = QMessageBox::question(this, "Different pilots", "Pilot in database differ from pilot in dialog. Sync pilots?",
				"Dialog to DB", "DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From lineEdit
				IFlyHighRC::pInstance()->setPilotName(pilot);
			break;
			case 1: // From database
				pilot = rcPilot;
				lineEdit_PilotName->setText(rcPilot);
			break;
			default:
			break;
		}
	}
	
	ft_string2ftstring(pilot.ascii(), (char*)&arr[PILOT_NAME_POS]);
	
	// glider
	glider = comboBoxModel->currentText();
	rcGlider = IFlyHighRC::pInstance()->glider();
	
	if(glider != rcGlider)
	{
		syncRes = QMessageBox::question(this, "Different gliders", "Glider in database differ from glider in dialog. Sync gliders?",
				"Dialog to DB", "DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From lineEdit
				IFlyHighRC::pInstance()->setGlider(glider);
			break;
			case 1: // From database
				glider = rcGlider;
				selectGlider(glider);
			break;
			default:
			break;
		}
	}

	ft_string2ftstring(glider.ascii(), (char*)&arr[GLYDER_TYPE_POS]);

	// callsign
	callsign = lineEdit_GliderID->text();
	rcCallsign= IFlyHighRC::pInstance()->callsign();
	
	if(callsign != rcCallsign)
	{
		syncRes = QMessageBox::question(this, "Different callsigns", "Callsign in database differ from callsign in dialog. Sync callsigns?",
				"Dialog to DB", "DB to GPS", "Ignore");
		
		switch(syncRes)
		{
			case 0: // From lineEdit
				IFlyHighRC::pInstance()->setCallsign(callsign);
			break;
			case 1: // From database
				callsign = rcCallsign;
				lineEdit_PilotName->setText(rcCallsign);
			break;
			default:
			break;
		}
	}
	
	// glider id
	ft_string2ftstring(callsign.ascii(), (char*)&arr[GLYDER_ID_POS]);

	// battery type
	arr[BATT_TYPE_POS] = comboBox_BattType->currentItem();
}

void DeviceFrameImpl::updateGlider()
{
	QStringList list;
	QString gliderModel;
	Glider::GliderListType gliderList;
	Glider::GliderListType::iterator it;
	
	ISql::pInstance()->gliderList(gliderList);
	comboBoxModel->clear();
	
	for(it=gliderList.begin(); it!=gliderList.end(); it++)
	{
		(*it).modelOfGlider(gliderModel);
		comboBoxModel->insertItem(gliderModel);
	}
}

void DeviceFrameImpl::selectGlider(const QString &name)
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

#include "DeviceFrameImpl.moc"

