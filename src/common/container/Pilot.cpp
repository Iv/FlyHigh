/***************************************************************************
 *   Copyright (C) 2006 by Alex Graf                                     *
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
#include "Pilot.h"

Pilot::Pilot()
{
	m_id = -1;
	m_firstName = "";
	m_lastName = "";
	m_callSign = "";
}

Pilot::~Pilot()
{
}

void Pilot::setId(int id)
{
	m_id = id;
}

int Pilot::id()
{
	return m_id;
}

void Pilot::setFirstName(const QString& first)
{
	m_firstName = first;
}

QString& Pilot::firstName()
{
	return m_firstName;
}

void Pilot::setLastName(const QString& last)
{
	m_lastName = last;
}

QString& Pilot::lastName()
{
	return m_lastName;
}

void Pilot::fullName(QString& name)
{
	name = m_firstName;

	if(m_lastName != "")
	{
		name += " " + m_lastName;;
	}
}

void Pilot::setBirthDate(const QDate& date)
{
	m_birthDate = date;
}

QDate& Pilot::birthDate()
{
	return m_birthDate;
}

void Pilot::setCallSign(const QString& sign)
{
	m_callSign = sign;
}

QString& Pilot::callSign()
{
	return m_callSign;
}

void Pilot::setGlider(const Glider& glider)
{
	m_glider = glider;
}

Glider& Pilot::glider()
{
	return m_glider;
}
