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

#include <QFile>
#include <QFileDialog>
#include <q3header.h>
#include <QMenuBar>
#include <QString>
#include <QStringList>
#include <q3table.h>
#include "AirSpace.h"
#include "WebMapAirSpaceView.h"
#include "AirSpaceWindow.h"
#include "IGPSDevice.h"
#include "IAirSpaceForm.h"
#include "IFlyHighRC.h"
#include "ISql.h"
#include "OpenAirFileParser.h"
#include "ProgressDlg.h"

AirSpaceWindow::AirSpaceWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags)
{
	QString caption;
	QStringList nameList;
	Q3Table *pTable = TableWindow::getTable();

  m_pWebMapView = NULL;
  m_pAirSpaceView = NULL;
	QMenu* pFileMenu = menuBar()->addMenu(tr("&File"));

	switch(src)
	{
		case IDataBase::SqlDB:
		break;
		case IDataBase::GPSdevice:
		{
			m_pDb = IGPSDevice::pInstance();
			caption = "AirSpaces from GPS";

			QAction* pDelAct = new QAction(tr("&Delete"), this);
			connect(pDelAct,SIGNAL(triggered()), this, SLOT(file_delete()));
			pFileMenu->addAction(pDelAct);
			QAction* pUpdateAct = new QAction(tr("&Update"), this);
			connect(pUpdateAct,SIGNAL(triggered()), this, SLOT(file_update()));
			pFileMenu->addAction(pUpdateAct);
		}
		break;
		case IDataBase::File:
		{
			m_pDb = NULL;
			caption = "AirSpaces from File";

			QAction* pImpAct = new QAction(tr("&Import..."), this);
			connect(pImpAct,SIGNAL(triggered()), this, SLOT(file_open()));
			pFileMenu->addAction(pImpAct);
			QAction* pAddAct = new QAction(tr("&Add to GPS..."), this);
			connect(pAddAct,SIGNAL(triggered()), this, SLOT(file_AddToGPS()));
			pFileMenu->addAction(pAddAct);
		}
		break;
	}

	QAction* pExpAct = new QAction(tr("&Export all..."), this);
	connect(pExpAct,SIGNAL(triggered()), this, SLOT(exportTable()));
	pFileMenu->addAction(pExpAct);

  QAction* pAirSpaceViewAct = new QAction(tr("View Airspace..."), this);
	connect(pAirSpaceViewAct,SIGNAL(triggered()), this, SLOT(file_viewAirSpace()));
	pFileMenu->addAction(pAirSpaceViewAct);

/**
  QAction* pWebMapAct = new QAction(tr("View Webmap..."), this);
	connect(pWebMapAct,SIGNAL(triggered()), this, SLOT(file_viewWebMap()));
	pFileMenu->addAction(pWebMapAct);
*/

	TableWindow::setWindowTitle(caption);
	TableWindow::setWindowIcon(QIcon(":/document.xpm"));

	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(Q3Table::SingleRow);

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

  connect(getTable(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

	m_lastModified = 0;

	// read db
	emit dataChanged();
}

AirSpaceWindow::~AirSpaceWindow()
{
}

void AirSpaceWindow::closeEvent(QCloseEvent *pEvent)
{
  if(m_pAirSpaceView != NULL)
  {
    delete m_pAirSpaceView;
  }

  if(m_pWebMapView != NULL)
  {
    delete m_pWebMapView;
  }
}

void AirSpaceWindow::refresh()
{
	int lastModified;

	if(m_pDb != NULL)
	{
		lastModified = m_pDb->airspacesLastModified();

		if(m_lastModified < lastModified)
		{
			populateTable();
			m_lastModified = lastModified;
		}
	}
}

void AirSpaceWindow::file_update()
{
	populateTable();
}

void AirSpaceWindow::file_open()
{
	Q3Table *pTable = TableWindow::getTable();
	QByteArray openAirData;
	QString selected;
	QFile file;
	OpenAirFileParser parser;
	uint airspaceNr;
	uint maxAirspaceNr;

	selected = QFileDialog::getOpenFileName(this,
																					"Open OpenAir File",
																					IFlyHighRC::pInstance()->lastDir(),
																					"OpenAir Files (*.txt *.fas)");
	file.setFileName(selected);

	if(selected!="" && file.exists())
	{
		IFlyHighRC::pInstance()->setLastDir(QFileInfo(selected).absoluteDir().absolutePath());

		TableWindow::setCursor(QCursor(Qt::WaitCursor));

		if(file.open(QIODevice::ReadOnly))
		{
			openAirData = file.readAll();
			file.close();

			parser.parse(openAirData, m_airSpaceList);
			m_airSpaceList.sort();
			maxAirspaceNr = m_airSpaceList.size();
			pTable->setNumRows(maxAirspaceNr);

			for(airspaceNr=0; airspaceNr<maxAirspaceNr; airspaceNr++)
			{
				setAirSpaceToRow(airspaceNr, m_airSpaceList.at(airspaceNr));
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

	if((row >= 0) && m_pDb->open())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->delAirSpace(*m_airSpaceList[row]);
		TableWindow::unsetCursor();
		m_pDb->close();
	}
	// refill table
	emit dataChanged();
}

void AirSpaceWindow::populateTable()
{
	Q3Table *pTable = TableWindow::getTable();
	ProgressDlg progDlg(this);
	uint airspaceNr;
	uint maxAirspaceNr;

	m_airSpaceList.clear();
	pTable->setNumRows(0);

	if(m_pDb->open())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		progDlg.beginProgress("read airspaces...", m_pDb);
		m_pDb->airspaceList(m_airSpaceList);
		progDlg.endProgress();
		maxAirspaceNr = m_airSpaceList.size();
		pTable->setNumRows(maxAirspaceNr);

		for(airspaceNr=0; airspaceNr<maxAirspaceNr; airspaceNr++)
		{
			setAirSpaceToRow(airspaceNr, m_airSpaceList.at(airspaceNr));
		}

		pTable->selectRow(0);
		selectionChanged();
		m_pDb->close();
		TableWindow::unsetCursor();
	}
}

void AirSpaceWindow::file_AddToGPS()
{
	ProgressDlg progDlg(this);
	int row;

	row = getTable()->currentRow();

	if((row >= 0) && IGPSDevice::pInstance()->open())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		progDlg.beginProgress("add airspace...", IGPSDevice::pInstance());
		IGPSDevice::pInstance()->add(*m_airSpaceList[row]);
		progDlg.endProgress();
		TableWindow::unsetCursor();
		IGPSDevice::pInstance()->close();
	}
	// refill table
	emit dataChanged();
}

void AirSpaceWindow::file_viewAirSpace()
{
	if((m_airSpaceList.size() > 0) && (m_pAirSpaceView == NULL))
	{
	  m_pAirSpaceView = new AirSpaceView();
    m_pAirSpaceView->setGeometry(QRect(0, 0, 500, 500));
    m_pAirSpaceView->setWindowTitle(tr("View AirSpaces"));
    m_pAirSpaceView->setAirSpaceList(&m_airSpaceList, 0);
    connect(m_pAirSpaceView, SIGNAL(finished(int)), this, SLOT(airSpaceViewFinished(int)));
    m_pAirSpaceView->show();
    selectionChanged();
	}
}

void AirSpaceWindow::file_viewWebMap()
{
	if((m_airSpaceList.size() > 0) && (m_pWebMapView == NULL))
	{
		m_pWebMapView = new WebMapAirSpaceView(tr("View AirSpaces"));
		m_pWebMapView->setAirSpaceList(&m_airSpaceList);
		m_pWebMapView->loadMap();
    connect(m_pWebMapView, SIGNAL(airSpaceChanged(int)), this, SLOT(airSpaceChanged(int)));
    connect(m_pWebMapView, SIGNAL(finished(int)), this, SLOT(webMapFinished(int)));
    m_pWebMapView->setWindowModality(Qt::NonModal);
		m_pWebMapView->show();
		selectionChanged();
	}
}

void AirSpaceWindow::selectionChanged()
{
  if(m_pAirSpaceView != NULL)
  {
    m_pAirSpaceView->setSelected(getTable()->currentRow());
  }

	if(m_pWebMapView != NULL)
	{
	  m_pWebMapView->selectAirSpace(getTable()->currentRow());
	}
}

void AirSpaceWindow::setAirSpaceToRow(uint row, const AirSpace *pAirSpace)
{
	Q3Table *pTable = TableWindow::getTable();

	pTable->setText(row, Name, pAirSpace->name());
	pTable->setText(row, High, pAirSpace->high());
	pTable->setText(row, Low, pAirSpace->low());
	pTable->setText(row, Class, pAirSpace->airspaceClass());
}

void AirSpaceWindow::airSpaceViewFinished(int res)
{
  m_pAirSpaceView = NULL;
}

void AirSpaceWindow::webMapFinished(int res)
{
  m_pWebMapView = NULL;
}

void AirSpaceWindow::airSpaceChanged(int line)
{
  getTable()->selectRow(line);

  if(m_pAirSpaceView != NULL)
  {
    m_pAirSpaceView->setSelected(getTable()->currentRow());
  }
}
