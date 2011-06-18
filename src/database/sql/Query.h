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

#ifndef QUERY_H
#define QUERY_H

#include <QString>
#include <QStringList>

/**
 * Represents a list of sql statements.
 */
class Query
{

public:

	/**
	 * Default construction.
	 * Creates an query without statements
	 */
	Query();

	/**
	 * Construction.
	 * Creates an query with the given list of statements
	 */
	Query(const QStringList& list);

	/**
	 * Returns the associated list of sql statements
	 * @return the list of statements
	 */
	const QStringList& getStatements() const;

	/**
	 * Adds a sql statement.
	 * Statements will be run in the order of addition.
	 * @param statement - the sql statement
	 */
	void addStatement(const QString& statement);

	/**
	 * Adds a list of sql statements
	 * @param statements - the sql statements
	 */
	void addStatements(const QStringList& statements);

	/**
	 * Sets the transaction mode. Set to true if the query
	 * is to be run in a single transaction.
	 * @param trans - transaction
	 */
	void setTransaction(bool trans);

	/**
	 * If the query should run as a transaction
	 * @return true if the query should run as a transaction
	 */
	bool isTransaction() const;

private:

	/**
	 * The list of statements
	 */
	QStringList m_Statements;

	/**
	 * The transaction mode
	 */
	bool m_Transaction;
};

#endif
