/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "Error.h"
#include "Accounts.h"
#include "ISql.h"

Accounts::Accounts(QSqlDatabase DB)
	:DataBaseSub(DB)
{
}

bool Accounts::add(Account &account)
{

	return true;
}

bool Accounts::update(Account &account)
{
  QSqlQuery query(db());
	bool success;


	return success;
}

bool Accounts::delAccount(Account &account)
{
  QSqlQuery query(db());
	bool success;


	return success;
}

bool Accounts::accountList(Account::AccountListType &accountList)
{
  bool success = false;

	return success;
}

bool Accounts::account(int id, Account &account)
{
  QSqlQuery query(db());
	bool success;


	return success;
}

bool Accounts::setAccountId(Account &account)
{
  QSqlQuery query(db());
	bool success;

	return success;
}

bool Accounts::checkModified()
{
  return DataBaseSub::checkModified("Accounts");
}
