/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
 *   grafal@sourceforge.net                                                         *
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
 
#include <qcursor.h>
#include <qmenubar.h>
#include <q3table.h>
#include "ISql.h"
#include "Servicing.h"
#include "ServicingWindow.h"
#include "IServicingForm.h"

ServicingWindow::ServicingWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags)
	:TableWindow(parent, name, wflags)
{
	QStringList nameList;
	Q3Table *pTable = TableWindow::getTable();

        QMenu* pFileMenu = menuBar()->addMenu(tr("&File"));

        QAction* pNewAct = new QAction(tr("&New..."), this);
        connect(pNewAct,SIGNAL(triggered()), this, SLOT(file_new()));
        pFileMenu->addAction(pNewAct);
        QAction* pDelAct = new QAction(tr("&Delete"), this);
        connect(pDelAct,SIGNAL(triggered()), this, SLOT(file_delete()));
        pFileMenu->addAction(pDelAct);
        QAction* pExpAllAct = new QAction(tr("&Export all..."), this);
        connect(pExpAllAct,SIGNAL(triggered()), this, SLOT(exportTable()));
        pFileMenu->addAction(pExpAllAct);

        TableWindow::setWindowTitle("Servicings");
        TableWindow::setWindowIcon(QIcon(":/document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(Q3Table::SingleRow);
	m_pDb = ISql::pInstance();
	
	// header
	nameList += "Glider";
	nameList += "Date";
	nameList += "Responsibility";
	nameList += "Comment";

	setupHeader(nameList);

	pTable->setColumnWidth(Glider, 160);
	pTable->setColumnWidth(Date, 100);
	pTable->setColumnWidth(Responsibility, 120);
	pTable->setColumnWidth(Comment, 1000);
	
	m_lastModified = 0;
}

bool ServicingWindow::periodicalUpdate()
{
	int lastModified;
	
	lastModified = m_pDb->servicingsLastModified();
	
	if(m_lastModified < lastModified)
	{
		file_update();
		m_lastModified = lastModified;
	}
	
	return true;
}

void ServicingWindow::file_update()
{
	Q3Table *pTable = TableWindow::getTable();
	QString gliderName;
	uint servNr;
	uint maxServNr;
	
	m_servicingsList.clear();
	TableWindow::setCursor(QCursor(Qt::WaitCursor));
	
	if(m_pDb->servicingList(m_servicingsList))
	{
		maxServNr = m_servicingsList.size();
		pTable->setNumRows(maxServNr);
		
		for(servNr=0; servNr<maxServNr; servNr++)
		{
			m_servicingsList[servNr].glider().fullName(gliderName);
			pTable->setText(servNr, Glider, gliderName);
			pTable->setText(servNr, Date, m_servicingsList[servNr].date().toString("dd.MM.yyyy"));
			pTable->setText(servNr, Responsibility, m_servicingsList[servNr].responsibility());
			pTable->setText(servNr, Comment, m_servicingsList[servNr].comment());
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
