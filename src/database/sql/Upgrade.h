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
#ifndef Upgrade_h
#define Upgrade_h

#include <QDateTime>
#include "DataBaseSub.h"

class QueryExecutor;
class DatabaseParameters;

class Upgrade: public DataBaseSub
{
	public:
		Upgrade(QSqlDatabase DB);

		/**
		 * Creates the database schema
		 * @param params - db connection parameters
		 * @return true if the schema was created successfully
		 */
		bool setup(const DatabaseParameters& params);

		bool upgrade();

	private:
		typedef QDateTime DataBaseVersion;
		static const DataBaseVersion DataBaseVersion_0_3_0;
		static const DataBaseVersion DataBaseVersion_0_3_1;
		static const DataBaseVersion DataBaseVersion_0_3_2;
		static const DataBaseVersion DataBaseVersion_0_5_0;
		static const DataBaseVersion DataBaseVersion_0_8_1;
		static const DataBaseVersion DataBaseVersion_0_8_2;
		static const DataBaseVersion DataBaseVersion_0_9_2;
    static const DataBaseVersion DataBaseVersion_0_9_5;

		QueryExecutor* m_pExecutor;

		DataBaseVersion dataBaseVersion();
		void setDataBaseVersion(const DataBaseVersion &vers);
};

#endif
