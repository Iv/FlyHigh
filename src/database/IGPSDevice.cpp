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

#include <qdatetime.h>
#include <stdlib.h>
#include "IFlyHighRC.h"
#include "IGPSDevice.h"
#include "Flytec6015.h"
#include "FlytecDevice.h"
#include "GarminDevice.h"

IGPSDevice *IGPSDevice::m_pGPSDevice = NULL;

IGPSDevice::IGPSDevice()
{
	uint fieldNr;
	
	for(fieldNr=0; fieldNr<NofFields; fieldNr++)
	{
		setLastModified((FieldType)fieldNr);
	}
}

IGPSDevice::~IGPSDevice()
{
}

IGPSDevice* IGPSDevice::pInstance()
{
	uint curDevice;
	bool newDevice = false;
	
	curDevice = IFlyHighRC::pInstance()->deviceName();
	newDevice = (m_pGPSDevice == NULL);
	
	if(!newDevice)
	{
		newDevice = (curDevice != m_pGPSDevice->oldDevice());
		
		if(newDevice)
		{
			delete m_pGPSDevice;
		}
	}
	
	if(newDevice)
	{
		switch(curDevice)
		{
			case 0:
				m_pGPSDevice = new FlytecDevice();
			break;
			case 1:
				m_pGPSDevice = new Flytec6015();
			break;
			default:
				m_pGPSDevice = new FlytecDevice;
				IFlyHighRC::pInstance()->setDeviceName(0);
			break;
		}
		
		m_pGPSDevice->setOldDevice(curDevice);
	}

	return m_pGPSDevice;
}

void IGPSDevice::open()
{
}

void IGPSDevice::close()
{
}

// memory
bool IGPSDevice::memoryRead(QByteArray &arr)
{
	(void)arr;
	return -1;
}

bool IGPSDevice::memoryWrite(QByteArray &arr)
{
	(void)arr;
	return -1;
}

void IGPSDevice::setLastModified(FieldType field)
{
	switch(field)
	{
		case Flights: case WayPoints: case Routes: case AirSpaces:
			m_lastModifiedList[field] = QDateTime::currentDateTime().toTime_t();
		break;
		default:
		break;
	}
}

int IGPSDevice::flightsLastModified()
{
	return m_lastModifiedList[Flights];
}

int IGPSDevice::wayPointsLastModified()
{
	return m_lastModifiedList[WayPoints];
}

int IGPSDevice::routesLastModified()
{
	return m_lastModifiedList[Routes];
}

int IGPSDevice::airspacesLastModified()
{
	return m_lastModifiedList[AirSpaces];
}

void IGPSDevice::setOldDevice(uint devNr)
{
	m_oldDevice = devNr;
}

uint IGPSDevice::oldDevice()
{
	return m_oldDevice;
}
