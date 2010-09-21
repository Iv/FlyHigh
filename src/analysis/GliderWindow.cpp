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
#include <qstringlist.h>
#include <q3table.h>
#include <qwidget.h>
#include "ContainerDef.h"
#include "GliderWindow.h"
#include "IDataBase.h"
#include "IGliderForm.h"
#include "ISql.h"
#include "ProgressDlg.h"

GliderWindow::GliderWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags)
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

        TableWindow::setWindowTitle("Gliders");
        TableWindow::setWindowIcon(QIcon(":/document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(Q3Table::SingleRow);
	m_pDb = ISql::pInstance();
	
	// header
	nameList += "Manufacturer";
	nameList += "Model";
	nameList += "Serial";
	nameList += "Flights";
	nameList += "Airtime [h]";

	setupHeader(nameList);
	
	pTable->setColumnWidth(Manufacturer, 110);
	pTable->setColumnWidth(Model, 120);
	pTable->setColumnWidth(Serial, 100);
	pTable->setColumnWidth(Flights, 100);
	pTable->setColumnWidth(Airtime, 100);

	m_glidersLastModified = 0;
	m_flightsLastModified = 0;
}

bool GliderWindow::periodicalUpdate()
{
	int glidersLastModified;
	int flightsLastModified;
	
	glidersLastModified = m_pDb->glidersLastModified();
	flightsLastModified = m_pDb->flightsLastModified();
	
	if((m_glidersLastModified < glidersLastModified) ||
			(m_flightsLastModified < flightsLastModified))
	{
		file_update();
		m_glidersLastModified = glidersLastModified;
		m_flightsLastModified = flightsLastModified;
	}
	
	return true;
}

void GliderWindow::file_new()
{
	Glider glider;
	IGliderForm newGlider(this, "New Glider", &glider);
	
	if(newGlider.exec())
	{
		ISql::pInstance()->add(glider);
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
	Q3Table *pTable = TableWindow::getTable();
	ProgressDlg progDlg(this);
	uint gliderNr;
	uint maxGliderNr;
	
	m_gliderList.clear();
	progDlg.beginProgress("reading gliders...", m_pDb);
	
	if(m_pDb->gliderList(m_gliderList))
	{
		maxGliderNr = m_gliderList.size();
		pTable->setNumRows(maxGliderNr);
		
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
	Q3Table *pTable = TableWindow::getTable();
	
	pTable->setText(row, Manufacturer, glider.manufacturer());
	pTable->setText(row, Model, glider.model());
	pTable->setText(row, Serial, glider.serial());
	str.sprintf("%i", glider.fligths());
	pTable->setText(row, Flights, str);
	str.sprintf("%.2f",  glider.airtime()/3600.0);
	pTable->setText(row, Airtime, str);
}
