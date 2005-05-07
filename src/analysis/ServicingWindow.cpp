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
 
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtable.h>
#include "Images.h"
#include "ISql.h"
#include "Servicing.h"
#include "ServicingWindow.h"
#include "IServicingForm.h"

ServicingWindow::ServicingWindow(QWidget* parent, const char* name, int wflags)
	:TableWindow(parent, name, wflags)
{
	QStringList nameList;
	QTable *pTable = TableWindow::getTable();
	QPopupMenu *pMenu;

	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&File", pMenu);
	pMenu->insertItem("&New...", this, SLOT(file_new()));
	pMenu->insertItem("&Delete", this, SLOT(file_delete()));
	pMenu->insertItem("&Print...", this, SLOT(print()));
	TableWindow::setCaption("Flight experience");
	TableWindow::setIcon(Images::pInstance()->getImage("document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(QTable::SingleRow);
	m_pDb = ISql::pInstance();
	
	// header
	nameList += "Nr";
	nameList += "Glider";
	nameList += "Date";
	nameList += "Responsibility";
	nameList += "Comment";

	setupHeader(nameList);

	pTable->setColumnWidth(Nr, 40);
	pTable->setColumnWidth(Glider, 100);
	pTable->setColumnWidth(Date, 80);
	pTable->setColumnWidth(Responsibility, 100);
	pTable->setColumnWidth(Comment, 800);
	
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
	Servicing::ServicingListType servList;
	QTable *pTable = TableWindow::getTable();
	QString str;
	uint servNr;
	uint maxServNr;

	if(m_pDb->servicingList(servList))
	{
		maxServNr = servList.size();
		pTable->setNumRows(maxServNr);
		
		for(servNr=0; servNr<maxServNr; servNr++)
		{
			str.sprintf("%i", servList[servNr].nr());
			pTable->setText(servNr, Nr, str);
			pTable->setText(servNr, Glider, servList[servNr].glider());
			pTable->setText(servNr, Date, servList[servNr].date().toString("dd.MM.yyyy"));
			pTable->setText(servNr, Responsibility, servList[servNr].responsibility());
			pTable->setText(servNr, Comment, servList[servNr].comment());
		}
	}
}

void ServicingWindow::file_new()
{
	Servicing serv;
	IServicingForm newServicing(this, "New Servicing", &serv);
	
	if(newServicing.exec())
	{
		ISql::pInstance()->add(serv);
	}
}

void ServicingWindow::file_delete()
{
	int row;
	uint nr;
	
	row = getTable()->currentRow();;
	
	if(row >= 0)
	{
		nr = getTable()->text(row, Nr).toInt();
		m_pDb->delServicing(nr);
	}
}
