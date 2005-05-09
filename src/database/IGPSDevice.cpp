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
#include "IGPSDevice.h"
#include "FlytecDevice.h"

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
	if(m_pGPSDevice != NULL)
	{
		delete m_pGPSDevice;
	}
}

void IGPSDevice::setDevice(GPSDeviceType dev)
{
	if(m_pGPSDevice != NULL)
	{
		delete m_pGPSDevice;
	}
	
	switch(dev)
	{
		case Flytec5020:
			m_pGPSDevice = new FlytecDevice;
		break;
		default:
			m_pGPSDevice = new FlytecDevice;
		break;
	}
}

IGPSDevice* IGPSDevice::pInstance()
{
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
	return -1;
}

bool IGPSDevice::memoryWrite(QByteArray &arr)
{
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
