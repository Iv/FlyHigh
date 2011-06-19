/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#include <QSqlDatabase>
#include "Upgrade.h"
#include "DatabaseParameters.h"
#include "DatabaseUtilities.h"


bool DatabaseUtilities::createDb(const DatabaseParameters& params,
																 const QString& root,
																 const QString& pwd)
{
	bool success;
	QString dbID = "root_db";

	// creating the QSqlDatabase object within a code block.
	// Assures proper cleanup when removing the db.
	{
		// create a parameter object with db admin credentials
		DatabaseParameters setupdbparams = params;
		setupdbparams.setDBUserName(root);
		setupdbparams.setDBPassword(pwd);
		// create a connection to a root db (using admin credentials)
		QSqlDatabase setupDb = QSqlDatabase::addDatabase(setupdbparams.dBType(), dbID);
		setupdbparams.apply(setupDb,true);
		// try accessing
		success = setupDb.open();
		if(success)
		{
			// create schema
			Upgrade upgrade(setupDb);
			upgrade.setup(params);
			setupDb.close();
		}
	}
	QSqlDatabase::removeDatabase(dbID);

	return success;
}
