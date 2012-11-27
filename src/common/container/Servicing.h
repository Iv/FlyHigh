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

#ifndef Servicing_h
#define Servicing_h

#include <QDateTime>
#include <QString>
#include <QList>
#include "Glider.h"

class Servicing
{
	public:
		typedef QList<Servicing> ServicingListType;

		Servicing();

		int id();

		void setId(int id);

		Glider& glider();

		void setGlider(Glider& glider);

		const QDate& date();

		void setDate(const QDate &date);

		const QString& responsibility();

		void setResponsibility(const QString &resp);

		const QString& comment();

		void setComment(const QString &comm);

	private:
		int m_id;
		uint m_nr;
		Glider m_glider;
		QDate m_date;
		QString m_resp;
		QString m_comment;
};

#endif
