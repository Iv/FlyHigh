/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#include <QCursor>
#include <QMenuBar>
#include <QStringList>
#include <QWidget>
#include <QTableWidget>
#include "ContainerDef.h"
#include "AccountWindow.h"
#include "IDataBase.h"
#include "IAccountForm.h"
#include "ISql.h"
#include "ProgressDlg.h"

AccountWindow::AccountWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags)
	:TableWindow(parent, name, wflags)
{
	QStringList nameList;
	QTableWidget *pTable;
	QAction* pAction;

  pTable = TableWindow::getTable();
	m_pDb = ISql::pInstance();

	pAction = new QAction(tr("&New..."), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(file_new()));
	MDIWindow::addAction(pAction);

	pAction = new QAction(tr("&Edit..."), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(file_edit()));
	MDIWindow::addAction(pAction, true);

	pAction = new QAction(tr("&Delete"), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(file_delete()));
	MDIWindow::addAction(pAction);

	pAction = new QAction(this);
	pAction->setSeparator(true);
	MDIWindow::addAction(pAction);

	pAction = new QAction(tr("&Export all..."), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(exportTable()));
	MDIWindow::addAction(pAction);

	// configure the table
	TableWindow::setWindowIcon(QIcon(":/document.xpm"));
	pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	pTable->setSelectionMode(QAbstractItemView::SingleSelection);

	// header
  nameList += tr("Username");
  nameList += tr("Contest");
  nameList += tr("Description");

	setupHeader(nameList);

  pTable->setColumnWidth(Username, 100);
  pTable->setColumnWidth(Contest, 100);
  pTable->setColumnWidth(Description, 200);

  connect(m_pDb, SIGNAL(accountsChanged()), this, SLOT(file_update()));

	file_update();
}

void AccountWindow::file_new()
{
  Account::AccountListType accounts;
  Account account;

  ISql::pInstance()->accountList(accounts);
  IAccountForm accountForm(this, tr("New Account"), &account, accounts);

  if(accountForm.exec())
	{
    ISql::pInstance()->add(account);
	}
}

void AccountWindow::file_edit()
{
  Account::AccountListType accounts;
  int row;

	row = getTable()->currentRow();

	if(row >= 0)
	{
    ISql::pInstance()->accountList(accounts);
    IAccountForm accountForm(this, tr("Edit Account"), &m_accountList[row], accounts);

    if(accountForm.exec() && m_pDb->open())
    {
      ISql::pInstance()->update(m_accountList[row]);
      m_pDb->close();
    }
	}
}

void AccountWindow::file_delete()
{
	int row;

	row = getTable()->currentRow();

	if(row >= 0)
	{
    m_pDb->delAccount(m_accountList[row]);
	}
}

void AccountWindow::file_update()
{
	ProgressDlg progDlg(this);
  uint accountNr;
  uint maxAccountNr;

  m_accountList.clear();
  progDlg.beginProgress("reading accounts...", m_pDb);

  if(m_pDb->accountList(m_accountList))
	{
    maxAccountNr = m_accountList.size();
    TableWindow::setNumRows(maxAccountNr);

    for(accountNr=0; accountNr<maxAccountNr; accountNr++)
		{
      setAccountToRow(accountNr, m_accountList[accountNr]);
		}
	}

	progDlg.endProgress();
}

void AccountWindow::setAccountToRow(uint row, Account &account)
{
	QTableWidget *pTable = TableWindow::getTable();

  pTable->item(row, Username)->setText(account.username());
  pTable->item(row, Contest)->setText(Account::typeAsString(account.type()));
  pTable->item(row, Description)->setText(account.description());
}
