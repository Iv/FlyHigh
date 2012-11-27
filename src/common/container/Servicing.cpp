/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
#include "Servicing.h"

Servicing::Servicing()
{
	m_id = -1;
	m_date = QDate::currentDate();
	m_resp = "";
	m_comment = "";
}

int Servicing::id()
{
	return m_id;
}

void Servicing::setId(int id)
{
	m_id = id;
}

Glider& Servicing::glider()
{
	return m_glider;
}

void Servicing::setGlider(Glider &glider)
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
