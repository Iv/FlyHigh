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

#include "Query.h"


Query::Query()
	: m_Transaction(false)
{
}

Query::Query(const QStringList& list)
	: m_Transaction(false)
{
	addStatements(list);
}

void Query::addStatement(const QString& statement)
{
	m_Statements.append(statement);
}

void Query::addStatements(const QStringList& list)
{
	// this would work for qt>=4.5:
	// m_Statements.append(list);
	// but we'll do it the hard way:
	for (QStringList::const_iterator iter=list.begin(); iter != list.end(); ++iter)
	{
		m_Statements.append(*iter);
	}
}

const QStringList& Query::getStatements() const
{
	return m_Statements;
}

void Query::setTransaction(bool trans)
{
	m_Transaction = trans;
}

bool Query::isTransaction() const
{
	return m_Transaction;
}
