/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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

#include <qsqlcursor.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include "Gliders.h" 

Gliders::Gliders(QSqlDatabase *pDB)
	:DataBaseSub(pDB)
{
}

bool Gliders::add(Glider &glider)
{
	QSqlCursor cur("Gliders");
	QSqlRecord *pRec;

	// insert record
	pRec = cur.primeInsert();
	pRec->setValue("Manufacturer", glider.manufacturer());
	pRec->setValue("Model", glider.model());
	cur.insert();
	
	DataBaseSub::setLastModified("'Gliders'");

	return true;
}

bool Gliders::glider(const QString &model, Glider &glider)
{
	QSqlCursor cur("Gliders");
	QString filter;
	bool exist;
	
	filter.sprintf("Model='%s'", model.ascii());
	cur.setFilter(filter);
	cur.select();
	exist = cur.first();
	
	if(exist)
	{
		glider.setManufacturer(cur.value("Manufacturer").toString());
		glider.setModel(cur.value("Model").toString());
	}

	return exist;
}

bool Gliders::gliderList(Glider::GliderListType &gliderList)
{
	Glider glider;
	QSqlQuery query(db());
	QString sqls = "SELECT * FROM Gliders ORDER BY Model ASC";
	bool success;
	
	success = query.exec(sqls);
	
	if(success)
	{
		while(query.next())
		{
			glider.setManufacturer(query.value(Manufacturer).toString());
			glider.setModel(query.value(Model).toString());

			gliderList.push_back(glider);
		}
	}
	
	return success;
}
