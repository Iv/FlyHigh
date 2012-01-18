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

#include <QAction>
#include <QCursor>
#include <QTableWidget>
#include "ISql.h"
#include "Servicing.h"
#include "ServicingWindow.h"
#include "IServicingForm.h"

ServicingWindow::ServicingWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags)
	:TableWindow(parent, name, wflags)
{
	QStringList nameList;
	QTableWidget *pTable = TableWindow::getTable();
  QAction* pAction;

  pAction = new QAction(tr("&New..."), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(file_new()));
  MDIWindow::addAction(pAction);

  pAction = new QAction(tr("&Delete"), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(file_delete()));
  MDIWindow::addAction(pAction);

  pAction = new QAction(tr("&Export all..."), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(exportTable()));
  MDIWindow::addAction(pAction);

  TableWindow::setWindowTitle(tr("Servicings"));
  TableWindow::setWindowIcon(QIcon(":/document.xpm"));

	// configure the table
	pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	pTable->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pDb = ISql::pInstance();
	connect(m_pDb, SIGNAL(servicingsChanged()), this, SLOT(file_update()));

	// header
	nameList += tr("Glider");
	nameList += tr("Date");
	nameList += tr("Responsibility");
	nameList += tr("Comment");

	setupHeader(nameList);

	pTable->setColumnWidth(Glider, 160);
	pTable->setColumnWidth(Date, 100);
	pTable->setColumnWidth(Responsibility, 120);
	pTable->setColumnWidth(Comment, 1000);

	file_update();
}

void ServicingWindow::file_update()
{
	QTableWidget *pTable = TableWindow::getTable();
	QString gliderName;
	uint servNr;
	uint maxServNr;

	m_servicingsList.clear();
	TableWindow::setCursor(QCursor(Qt::WaitCursor));

	if(m_pDb->servicingList(m_servicingsList))
	{
		maxServNr = m_servicingsList.size();
		TableWindow::setNumRows(maxServNr);

		for(servNr=0; servNr<maxServNr; servNr++)
		{
			m_servicingsList[servNr].glider().fullName(gliderName);
			pTable->item(servNr, Glider)->setText(gliderName);
			pTable->item(servNr, Date)->setText(m_servicingsList[servNr].date().toString("dd.MM.yyyy"));
			pTable->item(servNr, Responsibility)->setText(m_servicingsList[servNr].responsibility());
			pTable->item(servNr, Comment)->setText(m_servicingsList[servNr].comment());
		}
	}

	TableWindow::unsetCursor();
}

void ServicingWindow::file_new()
{
	Servicing serv;
	IServicingForm newServicing(this, "New Servicing", &serv);

	if(newServicing.exec())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		ISql::pInstance()->add(serv);
		TableWindow::unsetCursor();
	}
}

void ServicingWindow::file_delete()
{
	int row;

	row = getTable()->currentRow();;

	if(row >= 0)
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->delServicing(m_servicingsList[row]);
		TableWindow::unsetCursor();
	}
}
