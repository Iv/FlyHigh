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

#include "Glider.h"
 
Glider::Glider()
{
	m_manu = "";
	m_model = "";
}

const QString& Glider::manufacturer()
{
	return m_manu;
}

void Glider::setManufacturer(const QString &manu)
{
	m_manu = manu;
}

const QString& Glider::model()
{
	return m_model;
}

void Glider::setModel(const QString &model)
{
	m_model = model;
}

Glider& Glider::operator=(const Glider &glider)
{
	m_manu = glider.m_manu;
	m_model = glider.m_model;
	
	return *this;
}
