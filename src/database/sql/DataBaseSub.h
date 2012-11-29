/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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
#ifndef DataBaseSub_h
#define DataBaseSub_h

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class DataBaseSub: public QObject
{
  Q_OBJECT

	public:
		DataBaseSub(QSqlDatabase db);

		virtual ~DataBaseSub();

    virtual bool checkModified();

	protected:
		QSqlDatabase db();

		int newId(const QString &table);

		bool checkModified(const QString &field);

    void setLastModified(const QString &field);

    QString escape(const QString &str);

	private:
		QSqlDatabase m_DB;
		int m_lastModified;

    int lastModified(const QString &field);
};

#endif
