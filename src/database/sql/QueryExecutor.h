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

#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QSqlQuery>

class QueryStore;

/**
 * Represents a list of sql statements.
 */
class QueryExecutor
{
public:

	/**
	 * Type definition for a associative container, mapping
	 * strings to various data types.
	 */
	typedef QMap<QString,QVariant> TBindMap;

	/**
	* Type definition for a associative container, mapping
	* strings to strings.
	 */
	typedef QMap<QString,QString> TReplaceMap;

public:

	/**
	 * Default construction.
	 * Creates an QueryExecutor
	 */
	QueryExecutor();

  /**
   * Destruction
   */
  virtual ~QueryExecutor();

	/**
	 * Runs a query.
	 * @param name - the name of the query to run
	 * @return The result of the last statement in the query
	 */
	QSqlQuery executeQuery(const QString& name,
												 QSqlDatabase db);

	/**
	 * Runs a query.
	 * A map for named binding using placeholders may be provided.
	 * @param name - the name of the query to run
	 * @param bindings - map with values to bind
	 * @return The result of the last statement in the query
	 */
	QSqlQuery executeQuery(const QString& name,
												 const TBindMap& bindings,
												 QSqlDatabase db);

	/**
	 * Runs a query.
	 * A map with pairs for replacing any tokens may be provided.
	 * In statements of the query, all occurences of the keys
	 * provided in the replacement map are replaced by their
	 * associated values.
	 * @param name - the name of the query to run
	 * @param replacements - map of tokens to replace in the query
	 * @return The result of the last statement in the query
	 */
	QSqlQuery executeQuery(const QString& name,
												 const TReplaceMap& replacements,
												 QSqlDatabase db);

	/**
	 * Runs a query.
	 * A map for named binding using placeholders may be provided.
	 * A map with pairs for replacing any tokens may be provided.
	 * In statements of the query, all occurences of the keys
	 * provided in the replacement map are replaced by their
	 * associated values.
	 * @param name - the name of the query to run
	 * @param bindings - map with values to bind
	 * @param replacements - map of tokens to replace in the query
	 * @return The result of the last statement in the query
	 */
	QSqlQuery executeQuery(const QString& name,
												 const TBindMap& bindings,
												 const TReplaceMap& replacements,
												 QSqlDatabase db);

private:

	/**
	 * Runs a sql statement.
	 * @param sql - the sql statement to run
	 * @param bindings - map with values to bind
	 * @return The query result
	 */
	QSqlQuery executeStatement(const QString& sql,
														 const TBindMap& bindings,
														 QSqlDatabase db);

private:

	/**
	 * The query store
	 * (singleton, but we keep a pointer here for conveinence
	 */
	QueryStore* m_pQueryStore;

};

#endif
