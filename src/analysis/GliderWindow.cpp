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
#include "GliderWindow.h"
#include "IDataBase.h"
#include "IGliderForm.h"
#include "ISql.h"
#include "ProgressDlg.h"

GliderWindow::GliderWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags)
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
	nameList += tr("Manufacturer");
	nameList += tr("Model");
	nameList += tr("Serial");
	nameList += tr("Flights");
	nameList += tr("Airtime [h]");

	setupHeader(nameList);

	pTable->setColumnWidth(Manufacturer, 110);
	pTable->setColumnWidth(Model, 120);
	pTable->setColumnWidth(Serial, 100);
	pTable->setColumnWidth(Flights, 100);
	pTable->setColumnWidth(Airtime, 100);

  connect(m_pDb, SIGNAL(glidersChanged()), this, SLOT(file_update()));

	file_update();
}

void GliderWindow::file_new()
{
  Glider::GliderListType gliders;
	Glider glider;

  ISql::pInstance()->gliderList(gliders);
	IGliderForm gliderForm(this, tr("New Glider"), &glider, gliders);

	if(gliderForm.exec())
	{
		ISql::pInstance()->add(glider);
	}
}

void GliderWindow::file_edit()
{
  Glider::GliderListType gliders;
  int row;

	row = getTable()->currentRow();

	if(row >= 0)
	{
    ISql::pInstance()->gliderList(gliders);
    IGliderForm gliderForm(this, tr("Edit Glider"), &m_gliderList[row], gliders);

    if(gliderForm.exec() && m_pDb->open())
    {
      ISql::pInstance()->update(m_gliderList[row]);
      m_pDb->close();
    }
	}
}

void GliderWindow::file_delete()
{
	int row;

	row = getTable()->currentRow();

	if(row >= 0)
	{
		m_pDb->delGlider(m_gliderList[row]);
	}
}

void GliderWindow::file_update()
{
	ProgressDlg progDlg(this);
	uint gliderNr;
	uint maxGliderNr;

	m_gliderList.clear();
	progDlg.beginProgress("reading gliders...", m_pDb);

	if(m_pDb->gliderList(m_gliderList))
	{
		maxGliderNr = m_gliderList.size();
		TableWindow::setNumRows(maxGliderNr);

		for(gliderNr=0; gliderNr<maxGliderNr; gliderNr++)
		{
			setGliderToRow(gliderNr, m_gliderList[gliderNr]);
		}
	}

	progDlg.endProgress();
}

void GliderWindow::setGliderToRow(uint row, Glider &glider)
{
	QString str;
	QTableWidget *pTable = TableWindow::getTable();

	pTable->item(row, Manufacturer)->setText(glider.manufacturer());
	pTable->item(row, Model)->setText(glider.model());
	pTable->item(row, Serial)->setText(glider.serial());
	str.sprintf("%i", glider.fligths());
	pTable->item(row, Flights)->setText(str);
	str.sprintf("%.2f",  glider.airtime() / 3600.0);
	pTable->item(row, Airtime)->setText(str);
}
