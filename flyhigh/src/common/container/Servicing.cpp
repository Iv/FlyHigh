/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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
#include "Servicing.h"

Servicing::Servicing()
{
	m_glider = "";
	m_date = QDate::currentDate();
	m_resp = "";
	m_comment = "";
}

uint Servicing::nr()
{
	return m_nr;
}

void Servicing::setNr(uint nr)
{
	m_nr = nr;
}

const QString& Servicing::glider()
{
	return m_glider;
}

void Servicing::setGlider(const QString &glider)
{
	m_glider = glider;
}

const QDate& Servicing::date()
{
	return m_date;
}

void Servicing::setDate(const QDate &date)
{
	m_date = date;
}

const QString& Servicing::responsibility()
{
	return m_resp;
}

void Servicing::setResponsibility(const QString &resp)
{
	m_resp = resp;
}

const QString& Servicing::comment()
{
	return m_comment;
}

void Servicing::setComment(const QString &comm)
{
	m_comment = comm;
}
