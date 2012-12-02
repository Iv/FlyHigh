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

#include <QDebug>
#include <QSqlError>
#include "QueryStore.h"
#include "QueryExecutor.h"

QueryExecutor::~QueryExecutor()
{
  QueryStore::deleteInstance();
}

QSqlQuery QueryExecutor::executeQuery(const QString& name,
																			const TBindMap& bindings,
																			const TReplaceMap& replacements,
																			QSqlDatabase db)
{
	// fetch statements
  Query query = QueryStore::pInstance()->getQuery(name,db);
	const QStringList& statements = query.getStatements();

	// only single-statement queries work as expected with bindings/replacements
	if (!bindings.empty())
	{
		if (statements.size()!=1)
		{
			qDebug() << "you should not use bindings with multi-statement queries";
		}
	}

	// in a transaction?
	if (query.isTransaction())
	{
		// start transaction
		qDebug() << "transactions are not yet implemented. Sorry!";
	}

	// loop over statements
	QSqlQuery result;
	for(QStringList::const_iterator iter = statements.constBegin(); iter != statements.constEnd(); ++iter)
	{
		QString statement = *iter;
		// handle replacement tokens
		for(TReplaceMap::const_iterator repiter = replacements.constBegin(); repiter != replacements.constEnd(); ++repiter)
		{
			statement.replace(repiter.key(),repiter.value());
		}

		// run the statement
		result = executeStatement(statement,bindings,db);

		// check result
		if (result.lastError().type()!=QSqlError::NoError)
		{
			// error: don't run more statements
			break;
		}
	}

	return result;
}

QSqlQuery QueryExecutor::executeQuery(const QString& name,
																			QSqlDatabase db)
{
	return executeQuery(name,TBindMap(),TReplaceMap(),db);
}

QSqlQuery QueryExecutor::executeQuery(const QString& name,
																			const TBindMap& bindings,
																			QSqlDatabase db)
{
	return executeQuery(name,bindings,TReplaceMap(),db);
}

QSqlQuery QueryExecutor::executeQuery(const QString& name,
																			const TReplaceMap& replacements,
																			QSqlDatabase db)
{
	return executeQuery(name,TBindMap(),replacements,db);
}

QSqlQuery QueryExecutor::executeStatement(const QString& sql,
																					const TBindMap& bindings,
																					QSqlDatabase db)
{
	// prepare the sql statement
	QSqlQuery query(db);
	query.prepare(sql);

	if (!bindings.empty())
	{
		// binding parameters must follow this pattern:
		QRegExp paramPat(":[A-Za-z0-9]+");
		int pos=0;

		// check for all parameters
		while ((pos=paramPat.indexIn(sql, pos))!=-1)
		{
				QString param = paramPat.cap(0);

				TBindMap::const_iterator binditer = bindings.constFind(param);
				if (binditer == bindings.constEnd())
				{
					qDebug() << "no placeholder '" << param << "' in bindings map, the sql statement is probably ill-defined.";
				}
				else
				{
					// bind value
					query.bindValue(param, binditer.value());
				}
				// move on
				pos += paramPat.matchedLength();
		}
	}

	// run the query
	if (!query.exec())
	{
		qDebug() << "Executing the query '" << query.executedQuery() << "' failed. DB says " <<
								query.lastError().databaseText();
	}

	return query;
}
