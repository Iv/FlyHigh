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
#include <QCryptographicHash>
#include "simplecrypt.h"
#include "IFlyHighRC.h"
#include "Error.h"
#include "Accounts.h"
#include "ISql.h"

#include "QDebug"

Accounts::Accounts(QSqlDatabase DB)
	:DataBaseSub(DB)
{
  m_pilotId = IFlyHighRC::pInstance()->pilotId();
}

bool Accounts::add(Account &account)
{
  QSqlQuery query(db());
  bool success;

  success = query.prepare("INSERT INTO Accounts (Username, Password, Type, PilotId, Comment) "
                          "VALUES(:username, :password, :type, :pilotId, :comment)");
  query.bindValue(":username", account.username());
  query.bindValue(":password", scramble(account.password(), account.username()));
  query.bindValue(":type", account.type());
  query.bindValue(":pilotId", m_pilotId);
  query.bindValue(":comment", account.description());
  success &= query.exec();

  Error::verify(success, Error::SQL_CMD);
  DataBaseSub::setLastModified("Accounts");

  return success;
}

bool Accounts::update(Account &account)
{
  QSqlQuery query(db());
  bool success;

  success = query.prepare("UPDATE Accounts SET Username=:username, Password=:password, Type=:type, Comment=:comment "
                          "WHERE Id=:id");
  query.bindValue(":username", account.username());
  query.bindValue(":password", scramble(account.password(), account.username()));
  query.bindValue(":type", account.type());
  query.bindValue(":comment", account.description());
  query.bindValue(":id", account.id());
  success &= query.exec();

  DataBaseSub::setLastModified("Accounts");
  Error::verify(success, Error::SQL_CMD);

  return success;
}

bool Accounts::delAccount(Account &account)
{
  QSqlQuery query(db());
  bool success;

  success = query.prepare("DELETE FROM Accounts WHERE Id=:id");
  query.bindValue(":id", account.id());
  success &= query.exec();

  DataBaseSub::setLastModified("Accounts");
  Error::verify(success, Error::SQL_CMD);

  return success;
}

bool Accounts::accountList(Account::AccountListType &accountList)
{
  Account account;
  QSqlQuery query(db());
  bool success;

  success = query.prepare("SELECT Id, Username, Password, Type, PilotId, Comment FROM Accounts "
                          "WHERE PilotId = :pilotId "
                          "ORDER BY Username, Type ASC" );

  query.bindValue("pilotId", m_pilotId);
  success &= query.exec();

  if(success)
  {
    while(query.next())
    {
      account.setId(query.value(0).toInt());
      account.setUsername(query.value(1).toString());
      account.setPassword(unscramble(query.value(2).toString(), account.username()));
      account.setType((Account::OLCType)query.value(3).toInt());
      account.setDescription(query.value(5).toString());

      accountList.push_back(account);
    }
  }

  Error::verify(success, Error::SQL_CMD);

  return success;

}

bool Accounts::account(int id, Account &account)
{
  QSqlQuery query(db());
  bool success;

  success = query.prepare("SELECT Username, Password, Type, Comment FROM Accounts WHERE Id=:id AND PilotId=:pilotId");
  query.bindValue(":id", id);
  query.bindValue(":pilotId", m_pilotId);
  success &= (query.exec() && query.first());

  if(success)
  {
    account.setId(id);
    account.setUsername(query.value(0).toString());
    account.setPassword(unscramble(query.value(1).toString(),account.username()));
    account.setType((Account::OLCType)query.value(2).toInt());
    account.setDescription(query.value(3).toString());
  }
  else
  {
    Error::verify(success, Error::SQL_CMD);
  }

  return success;
}

bool Accounts::setAccountId(Account &account)
{
  QSqlQuery query(db());
  bool success;
  int id = -1;

  success = query.prepare("SELECT Id FROM Accounts "
                          "WHERE Username=:username AND Type=:type AND PilotId=:pilotId");
  query.bindValue(":username", account.username());
  query.bindValue(":type", account.type());
  query.bindValue(":pilotId", m_pilotId);

  success &= (query.exec() && query.first());

  if(success)
  {
    id = query.value(0).toInt();
  }
  else
  {
    Error::verify(success, Error::SQL_CMD);
  }

  account.setId(id);

  return success;
}

bool Accounts::checkModified()
{
  return DataBaseSub::checkModified("Accounts");
}

QString Accounts::scramble(QString toScramble, QString key)
{
  // attempt to hide account password a bit: use key to generate a sha1 hash,
  // then use this (hex string) hash as key for encryption
  // not at all secure, but probably ok to obscure passwords stored to database
  QByteArray hash = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1);
  SimpleCrypt cipher(QString(hash).toULongLong(NULL, 16));
  return cipher.encryptToString(toScramble);
}

QString Accounts::unscramble(QString toUnscramble, QString key)
{
  QByteArray hash = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1);
  SimpleCrypt cipher(QString(hash).toULongLong(NULL, 16));
  return cipher.decryptToString(toUnscramble);
}
