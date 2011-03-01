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

#include "Error.h"
#include "Flytec5020.h"
#include "IFlyHighRC.h"
#include "Protocol5020.h"

Flytec5020::Flytec5020()
{
	m_protocol = new Protocol5020();
}

Flytec5020::~Flytec5020()
{
	delete m_protocol;
}

bool Flytec5020::open()
{
	bool success;

	success = m_protocol->open(IFlyHighRC::pInstance()->deviceLine(),
					IFlyHighRC::pInstance()->deviceSpeedString().toUInt());
	Error::verify(success, Error::FLYTEC_OPEN);

DeviceInfo devInfo;

m_protocol->devInfoReq();
m_protocol->devInfoRec(devInfo);

	return success;
}

void Flytec5020::close()
{
	m_protocol->close();
}
