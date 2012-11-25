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

#include "IFlyHighRC.h"
#include "DeviceConnectionParameters.h"

DeviceConnectionParameters::DeviceConnectionParameters()
{
}

DeviceConnectionParameters::DeviceConnectionParameters(const QString &type,
                                                       const QString& port,
                                                       const QString& speed)
  : m_deviceType(type)
  , m_port(port)
  , m_lineSpeed(speed)
{
}

bool DeviceConnectionParameters::operator==(const DeviceConnectionParameters& other) const
{
  return m_deviceType == other.m_deviceType &&
         m_port       == other.m_port &&
         m_lineSpeed  == other.m_lineSpeed;
}

bool DeviceConnectionParameters::operator!=(const DeviceConnectionParameters& other) const
{
	return !operator==(other);
}


void DeviceConnectionParameters::readFromConfig()
{
  m_deviceType = IFlyHighRC::pInstance()->deviceNameString();
  m_port = IFlyHighRC::pInstance()->deviceLine();
  m_lineSpeed = IFlyHighRC::pInstance()->deviceSpeedString();
}

void DeviceConnectionParameters::writeToConfig() const
{
	IFlyHighRC::pInstance()->loadRC();
  IFlyHighRC::pInstance()->setDeviceName(m_deviceType);
  IFlyHighRC::pInstance()->setDeviceLine(m_port);
  IFlyHighRC::pInstance()->setDeviceSpeed(m_lineSpeed);
	IFlyHighRC::pInstance()->saveRC();
}

DeviceConnectionParameters DeviceConnectionParameters::defaultParameters()
{
  DeviceConnectionParameters parameters;

  parameters.m_deviceType = IFlyHighRC::DevFlytec6015;
  parameters.m_port = IFlyHighRC::pInstance()->deviceLineList().first();
  parameters.m_lineSpeed = IFlyHighRC::pInstance()->deviceSpeedList().first();

	return parameters;
}

QString DeviceConnectionParameters::deviceType() const
{
  return m_deviceType;
}

const QString& DeviceConnectionParameters::port() const
{
  return m_port;
}

const QString& DeviceConnectionParameters::lineSpeed() const
{
  return m_lineSpeed;
}

