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
#ifndef Pilot_h
#define Pilot_h

#include <q3valuelist.h>
#include "qdatetime.h"
#include "Glider.h"

/**
@author Alex Graf
*/
class Pilot
{
	public:
		typedef Q3ValueList<Pilot> PilotListType;

		Pilot();
		~Pilot();

		void setId(int id);
		int id();
		void setFirstName(const QString& first);
		QString& firstName();
		void setLastName(const QString& last);
		QString& lastName();
		void fullName(QString& name);
		void setBirthDate(const QDate& date);
		QDate& birthDate();
		void setCallSign(const QString& sign);
		QString& callSign();
		void setGlider(const Glider& glider);
		Glider& glider();

	private:
		int m_id;
		QString m_firstName;
		QString m_lastName;
		QDate m_birthDate;
		QString m_callSign;
		Glider m_glider;
};

#endif
