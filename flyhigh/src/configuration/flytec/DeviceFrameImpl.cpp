/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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

#include <qlineedit.h>

#include "DeviceFrameImpl.h"
extern "C"
{
	#include "flytec_al.h"
}

DeviceFrameImpl::DeviceFrameImpl(QWidget* parent, const char* name, WFlags fl)
: DeviceFrame(parent,name,fl)
{
}

DeviceFrameImpl::~DeviceFrameImpl()
{
}

void DeviceFrameImpl::update(QByteArray &arr)
{
	QString qstr;
	DeviceInfoType devInfo;
	char str[FT_STRING_SIZE];
    
	if(ft_deviceInfoRead(&devInfo) == 0)
	{
		lineEdit_SerialNr->setText(QString("%1").arg(devInfo.serialNr));
		lineEdit_SwVersion->setText(QString(devInfo.swVersion));
		lineEdit_DeviceIdent->setText(QString(devInfo.deviceIdent));
	}

	ft_ftstring2string(str, (char*)&arr[PILOT_NAME_POS]);
	qstr = str;
	lineEdit_PilotName->setText(qstr);
	
	ft_ftstring2string(str, (char*)&arr[GLYDER_TYPE_POS]);
	qstr = str;
	lineEdit_GliderType->setText(qstr);

	ft_ftstring2string(str, (char*)&arr[GLYDER_ID_POS]);
	qstr = str;
	lineEdit_GliderID->setText(qstr);
}

void DeviceFrameImpl::store(QByteArray &arr)
{
	QString qstr;

	qstr = lineEdit_PilotName->text();
	ft_string2ftstring(qstr.ascii(), (char*)&arr[PILOT_NAME_POS]);
	
	qstr = lineEdit_GliderType->text();
	ft_string2ftstring(qstr.ascii(), (char*)&arr[GLYDER_TYPE_POS]);

	qstr = lineEdit_GliderID->text();
	ft_string2ftstring(qstr.ascii(), (char*)&arr[GLYDER_ID_POS]);
}


#include "DeviceFrameImpl.moc"

