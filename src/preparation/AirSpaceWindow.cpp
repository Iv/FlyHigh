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
#include <qinputdialog.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qtable.h>
#include <qwidget.h>
#include "AirSpace.h"
#include "AirSpaceWindow.h"
#include "IGPSDevice.h"
#include "Images.h"
#include "IAirSpaceForm.h"
#include "IFlyHighRC.h"
#include "ISql.h"
#include "OpenAirFileParser.h"
#include "ProgressDlg.h"

AirSpaceWindow::AirSpaceWindow(QWidget* parent, const char* name, int wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags)
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
		break;
		case IDataBase::GPSdevice:
			m_pDb = IGPSDevice::pInstance();
			caption = "AirSpaces from GPS";

			pMenu->insertItem("&Delete", this, SLOT(file_delete()));
			pMenu->insertItem("&Update", this, SLOT(file_update()));
		break;
		case IDataBase::File:
			m_pDb = NULL;
			caption = "AirSpaces from File";
			pMenu->insertItem("&Import...", this, SLOT(file_open()));
			pMenu->insertItem("&Add to GPS...", this, SLOT(file_AddToGPS()));
		break;
	}
	
	pMenu->insertItem("&Export all...", this, SLOT(exportTable()));
	
	TableWindow::setCaption(caption);
	TableWindow::setIcon(Images::pInstance()->getImage("document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(QTable::SingleRow);

	// header	
	nameList += "Name";
	nameList += "Low";
	nameList += "High";
	nameList += "Class";
	setupHeader(nameList);
	
	pTable->setColumnWidth(Name, 200);
	pTable->setColumnWidth(High, 100);
	pTable->setColumnWidth(Low, 100);
	pTable->setColumnWidth(Class, 80);
	
	m_lastModified = 0;

	m_airSpaceView.setGeometry(QRect(0, 0, 200, 200));
	m_airSpaceView.show();

	connect(getTable(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

AirSpaceWindow::~AirSpaceWindow()
{
}

bool AirSpaceWindow::periodicalUpdate()
{
	int lastModified;

	if(m_pDb != NULL)
	{
		lastModified = m_pDb->airspacesLastModified();
	
		if(m_lastModified < lastModified)
		{
			file_update();
			m_lastModified = lastModified;
		}
	}
	
	return true;
}

void AirSpaceWindow::file_open()
{
	QTable *pTable = TableWindow::getTable();
	const QDir *pDir;
	QByteArray openAirData;
	QString fileName;
	QFile file;
	OpenAirFileParser parser;
	AirSpace airspace;
	uint airspaceNr;
	uint maxAirspaceNr;
	
	QFileDialog fileDlg(IFlyHighRC::pInstance()->lastDir(), "OpenAir Files (*.txt; *.fas)", this,
					"OpenAir Files", true);

	if(fileDlg.exec() == QDialog::Accepted)
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		pDir = fileDlg.dir();
		IFlyHighRC::pInstance()->setLastDir(pDir->absPath());
		delete pDir;
		file.setName(fileDlg.selectedFile());
		
		if(file.open(IO_ReadOnly))
		{
			openAirData = file.readAll();
			file.close();
		
			parser.parse(openAirData);
			m_airSpaceList = parser.airspaceList();
			m_airSpaceList.sort();
			maxAirspaceNr = m_airSpaceList.count();
			pTable->setNumRows(maxAirspaceNr);
			
			for(airspaceNr=0; airspaceNr<maxAirspaceNr; airspaceNr++)
			{
				setAirSpaceToRow(airspaceNr, *m_airSpaceList.at(airspaceNr));
			}
		
			pTable->selectRow(0);
			selectionChanged();
		}
		
		TableWindow::unsetCursor();
	}
}

void AirSpaceWindow::file_delete()
{
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->delAirSpace(getTable()->text(row, Name));
		TableWindow::unsetCursor();
	}
}

void AirSpaceWindow::file_update()
{
	QTable *pTable = TableWindow::getTable();
	uint airspaceNr;
	uint maxAirspaceNr;

	TableWindow::setCursor(QCursor(Qt::WaitCursor));
	
	m_airSpaceList.clear();
	m_pDb->airspaceList(m_airSpaceList);
	maxAirspaceNr = m_airSpaceList.count();
	pTable->setNumRows(maxAirspaceNr);
	
	for(airspaceNr=0; airspaceNr<maxAirspaceNr; airspaceNr++)
	{
		setAirSpaceToRow(airspaceNr, *m_airSpaceList.at(airspaceNr));
	}
	
	TableWindow::unsetCursor();
}

void AirSpaceWindow::file_AddToGPS()
{
	ProgressDlg progDlg(this);
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		progDlg.beginProgress("add airspace...", IGPSDevice::pInstance());
		IGPSDevice::pInstance()->add(*m_airSpaceList.at(row));
		progDlg.endProgress();
		TableWindow::unsetCursor();
	}
}

void AirSpaceWindow::selectionChanged()
{
	AirSpace *pAirSpace;
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		pAirSpace = m_airSpaceList.at(row);
		pAirSpace->createPointList();
		m_airSpaceView.setAirSpace(pAirSpace);
	}
}

void AirSpaceWindow::setAirSpaceToRow(uint row, AirSpace &airspace)
{
	QTable *pTable = TableWindow::getTable();

	pTable->setText(row, Name, airspace.name());
	pTable->setText(row, High, airspace.high());
	pTable->setText(row, Low, airspace.low());
	pTable->setText(row, Class, airspace.airspaceClass());
}
