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
#include "Query.h"


class QSqlDatabase;

/**
 * Central store for sql queries.
 * Goal is to encapsulate database-dependent queries.
 * Queries are store in a 'map of maps'. The outer map
 * associates driver names to the query container, while
 * the inner map associates query names to the actual queries.
 * Database-independent queries are associated with 'common'
 * as driver name.
 */
class QueryStore
{

public:

	/**
	 * Singleton pattern: return the one and only
	 * QueryStore instance.
	 */
	static QueryStore* pInstance();

  /**
   * Destroys the one and only QueryStore instance.
   */
  static void deleteInstance();

private:

	/**
	 * Construction.
	 */
	QueryStore();

  /**
   * Destruction
   */
  virtual ~QueryStore();

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
	Query getQuery(const QString& name, const QSqlDatabase& db) const;

private:

	/**
	 * Adds a query.
	 * If the query is driver-independent, use 'common' as driver name.
	 * @param name - name of the query
	 * @param driver - the driver name (or 'common')
	 * @param query - the query
	 */
	void addQuery(const QString& name, const QString& driver, const Query& query);

	/**
	 * Adds a query. This is a conveinience method for adding
	 * queries consisting of just one single statement.
	 * If the query is driver-independent, use 'common' as driver name.
	 * @param name - name of the query
	 * @param driver - the driver name (or 'common')
	 * @param sql - the query string
	 */
	void addQuery(const QString& name, const QString& driver, const QString& sql);

	/**
	* Returns the associated query, depending on the driver name.
	* @param name - name of the query to look up
	* @param driver - the driver name (or 'common')
	* @return the associated query
	*/
	Query getQuery(const QString& name, const QString& driver) const;

private:

	/**
	 * Type definition for an associative container,
	 * mapping query names to queries
	 */
	typedef QMap<QString,Query> TQueryMap;

	/**
	 * Type	definition for an associative container,
	 * mapping driver names to query maps
	 */
	typedef QMap<QString,TQueryMap> TDriverMap;

	/**
	 * Map of query maps
	 */
	 TDriverMap m_QueryMap;

	/**
	 * Singleton instance pointer
	 */
	static QueryStore* m_pInst;

};

#endif
