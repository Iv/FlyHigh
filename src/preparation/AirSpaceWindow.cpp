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
#include <qdialog.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qheader.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qtable.h>
#include "AirSpace.h"
#include "AirSpaceWindow.h"
#include "IGPSDevice.h"
#include "Images.h"
#include "IAirSpaceForm.h"
#include "ISql.h"
#include "OpenAirFileParser.h"

AirSpaceWindow::AirSpaceWindow(QWidget* parent, const char* name, int wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags),
	m_curAirSpace(NULL, "Current Airspace", NULL)
{
	QString caption;
	QStringList nameList;
	QTable *pTable = TableWindow::getTable();
	QPopupMenu *pMenu;

	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&File", pMenu);
	
	switch(src)
	{
		case IDataBase::SqlDB:
			m_pDb = ISql::pInstance();
			caption = "CTRs from DB";
			pMenu->insertItem("&New...", this, SLOT(file_new()));
			pMenu->insertItem("&Import...", this, SLOT(file_import()));
			pMenu->insertItem("&Add to GPS...", this, SLOT(file_AddToGPS()));
		break;
		case IDataBase::GPSdevice:
			m_pDb = IGPSDevice::pInstance();
			caption = "CTRs from GPS";
			pMenu->insertItem("&Add to DB...", this, SLOT(file_AddToSqlDB()));
		break;
	}
	
	pMenu->insertItem("&Delete", this, SLOT(file_delete()));
	pMenu->insertItem("&Update", this, SLOT(file_update()));
	pMenu->insertItem("&Print...", this, SLOT(print()));
	TableWindow::printer().setOrientation(QPrinter::Landscape);
	
	TableWindow::setCaption(caption);
	TableWindow::setIcon(Images::pInstance()->getImage("document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(QTable::SingleRow);

	// header	
	nameList += "Name";
	nameList += "High";
	nameList += "Low";
	nameList += "Class";
	setupHeader(nameList);
	
	pTable->setColumnWidth(Name, 200);
	pTable->setColumnWidth(High, 100);
	pTable->setColumnWidth(Low, 100);
	pTable->setColumnWidth(Class, 60);
	
//	m_curAirSpace.show();

	connect(pTable, SIGNAL(currentChanged(int, int)),
			this, SLOT(currentChanged(int, int)));
	m_curAirSpace.setModal(false);
	
	m_lastModified = 0;
	startTimer(1);
}

void AirSpaceWindow::timerEvent(QTimerEvent *pEvent)
{
/*	int lastModified;
	
	if(pEvent != NULL)
	{
		lastModified = m_pDb->airspacesLastModified();
		
		if(m_lastModified < lastModified)
		{
			file_update();
			m_lastModified = lastModified;
		}
	}*/
}

void AirSpaceWindow::file_new()
{
}

void AirSpaceWindow::file_import()
{
	QTable *pTable = TableWindow::getTable();
	QByteArray openAirData;
	QString fileName;
	QFile file;
	OpenAirFileParser parser;
	AirSpace airspace;
	uint airspaceNr;
	uint maxAirspaceNr;
	
	fileName = QFileDialog::getOpenFileName("", "OpenAir Files (*.*)", this);
	file.setName(fileName);

	if(file.open(IO_ReadOnly))
	{
		openAirData = file.readAll();
		file.close();
	}
	
	parser.parse(openAirData);
	m_airSpaceList = parser.airspaceList();
	maxAirspaceNr = m_airSpaceList.size();// parser.airspaceList().size();
	
	for(airspaceNr=0; airspaceNr<maxAirspaceNr; airspaceNr++)
	{
		pTable->insertRows(airspaceNr);
		setAirSpaceToRow(airspaceNr, m_airSpaceList.at(airspaceNr));//parser.airspaceList().at(airspaceNr));
	}
	
	pTable->selectRow(0);
	m_curAirSpace.show();
}

void AirSpaceWindow::file_delete()
{
}

void AirSpaceWindow::file_update()
{
/*	Airspace::AirSpaceListType airspaceList;
	AirSpace airspace;
	QTable *pTable = TableWindow::getTable();
	uint airspaceNr;
	uint maxAirspaceNr;

	TableWindow::setCursor(QCursor(Qt::WaitCursor));
	
	pTable->setNumRows(0);
	m_pDb->airspaceList(airspaceList);
	maxAirspaceNr = airspaceList.size();
	
	for(airspaceNr=0; airspaceNr<maxAirspaceNr; airspaceNr++)
	{
		pTable->insertRows(airspaceNr);
		setAirSpaceToRow(airspaceNr, airspaceList[airspaceNr]);
	}
	
	TableWindow::unsetCursor();*/
}

void AirSpaceWindow::file_AddToGPS()
{
}

void AirSpaceWindow::file_AddToSqlDB()
{
}

void AirSpaceWindow::setAirSpaceToRow(uint row, AirSpace &airspace)
{
	QTable *pTable = TableWindow::getTable();
	
	pTable->setText(row, Name, airspace.name());
	pTable->setText(row, High, airspace.high());
	pTable->setText(row, Low, airspace.low());
	pTable->setText(row, Class, airspace.airspaceClass());
}

void AirSpaceWindow::currentChanged(int row, int col)
{
	m_curAirSpace.setAirSpace(&(m_airSpaceList.at(row)));
}
