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

#include "DataBaseSub.h"

class Upgrade: public DataBaseSub
{
	public:
		Upgrade(QSqlDatabase *pDB);

		void upgradeFrom_0_3_1_to_0_3_2();
		void upgradeFrom_0_3_2_to_0_3_3();

	private:
		static const QString m_TableVersion_0_3_2;
		static const QString m_TableVersion_0_3_3;

		int tableVersion();
		void setTableVersion(const QString &tabVers);
		void testTables();
};

#endif
