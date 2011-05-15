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

#ifndef QUERYSTORE_H
#define QUERYSTORE_H

#include <QMap>
#include <QString>


class QSqlDatabase;

/**
 * Central store for sql queries.
 * Goal is to encapsulate database-dependent queries.
 */
class QueryStore
{

public:

	/**
	 * Singleton pattern: return the one and only
	 * QueryStore instance.
	 */
	static QueryStore* pInstance();

private:

	/**
	 * Construction.
	 */
	QueryStore();

	/**
	 * Populates the query map
	 */
	void init();

public:

	/**
	 * Returns the associated query, depending on the
	 * underlying database driver.
	 * @param name - name of the query to look up
	 * @param db - the target database
	 * @return the associated query
	 */
	QString getQuery(const QString& name, const QSqlDatabase& db) const;

private:

	/**
	 * Type definition for an associative container,
	 * mapping query names to query strings
	 */
	typedef QMap<QString,QString> TQueryMap;

	/**
	 * Map of queries common to all drivers
	 */
	 TQueryMap m_QueryMap;

	/**
	 * Map of queries specific to sqlite
	 */
	 TQueryMap m_QueryMapSqlite;

	/**
	 * Map of queries specific to mysql
	 */
	 TQueryMap m_QueryMapMysql;

	/**
	 * Singleton instance pointer
	 */
	static QueryStore* m_pInst;
};

#endif
