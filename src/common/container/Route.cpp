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
 
#include "Route.h"

Route::Route()
{
	m_id = -1;
	m_name = "";
}

const QString& Route::name()
{
	return m_name;
}

int Route::id()
{
	return m_id;
}

void Route::setId(int id)
{
	m_id = id;
}

void Route::setName(const QString &name)
{
	m_name = name;
}

WayPoint::WayPointListType& Route::wayPointList()
{
	return m_wpList;
}

Route& Route::operator=(const Route &route)
{
	m_id = route.m_id;
	m_name = route.m_name;
	m_wpList = route.m_wpList;
	
	return *this;
}
