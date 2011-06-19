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

#ifndef DATABASEUTILITIES_H
#define DATABASEUTILITIES_H

#include <QString>

class DatabaseParameters;

/**
 * A collection of utilities for db management and manipulation.
 */
class DatabaseUtilities
{
public:

	/**
	 * Creates a database for the given parameters.
	 * @param params - parameters for the new DB
	 * @param root - db administrator username
	 * @param pwd - db administrator password
	 * @return True if successful
	 */
	static bool createDb(const DatabaseParameters& params,
											 const QString& root="",
											 const QString& pwd="");
};

#endif
