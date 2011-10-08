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
 *   along with this program; if not,  write to the                        *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QCursor>
#include <QString>
#include <QStringList>
#include <q3table.h>
#include <QDateTime>
#include <QMenuBar>
#include "IDataBase.h"
#include "ISql.h"
#include "IGPSDevice.h"
#include "ProgressDlg.h"
#include "IWayPointForm.h"
#include "WayPointWindow.h"
#include "WayPoint.h"
#include "WebMapWayPointView.h"

WayPointWindow::WayPointWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags)
{
	QString caption;
	QStringList nameList;
	Q3Table *pTable = TableWindow::getTable();

  m_pWayPointView = NULL;
  m_externSelect = false;
	QMenu* pFileMenu = menuBar()->addMenu(tr("&File"));
	QAction* pUpdateAct = new QAction(tr("&Update"), this);
	connect(pUpdateAct,SIGNAL(triggered()), this, SLOT(file_update()));
	pFileMenu->addAction(pUpdateAct);

	switch(src)
	{
		case IDataBase::SqlDB:
		{
			m_pDb = ISql::pInstance();
			caption = "WayPoints from DB";
			QAction* pAddAct = new QAction(tr("Add to GPS..."), this);
			connect(pAddAct, SIGNAL(triggered()), this, SLOT(file_AddToGps()));
			pFileMenu->addAction(pAddAct);
      QAction* pEditAct = new QAction(tr("Edit..."), this);
			connect(pEditAct, SIGNAL(triggered()), this, SLOT(file_Edit()));
			pFileMenu->addAction(pEditAct);
		}
		break;
		case IDataBase::GPSdevice:
		{
			m_pDb = IGPSDevice::pInstance();
			caption = "WayPoints from GPS";
			QAction* pAddAct = new QAction(tr("Add to DB..."), this);
			connect(pAddAct, SIGNAL(triggered()), this, SLOT(file_AddToSqlDB()));
			pFileMenu->addAction(pAddAct);
		}
		break;
		default:
			Q_ASSERT(false);
		break;
	}

  connect(m_pDb, SIGNAL(wayPointsChanged()), this, SLOT(file_update()));

	QAction* pNewAct = new QAction(tr("New..."), this);
	connect(pNewAct, SIGNAL(triggered()), this, SLOT(file_addNewWp()));
	pFileMenu->addAction(pNewAct);

	QAction* pDelAct = new QAction(tr("Delete"), this);
	connect(pDelAct, SIGNAL(triggered()), this, SLOT(file_delete()));
	pFileMenu->addAction(pDelAct);

  if(src == IDataBase::GPSdevice)
  {
    QAction* pDelAct = new QAction(tr("Delete All"), this);
    connect(pDelAct, SIGNAL(triggered()), this, SLOT(file_deleteAll()));
    pFileMenu->addAction(pDelAct);
  }

	QAction* pViewWebMapAct = new QAction(tr("View WebMap..."), this);
	connect(pViewWebMapAct, SIGNAL(triggered()), this, SLOT(file_viewWebMap()));
	pFileMenu->addAction(pViewWebMapAct);

	QAction* pEditWebMapAct = new QAction(tr("Edit WebMap..."), this);
	connect(pEditWebMapAct, SIGNAL(triggered()), this, SLOT(file_editWebMap()));
	pFileMenu->addAction(pEditWebMapAct);

	TableWindow::setWindowTitle(caption);
	TableWindow::setWindowIcon(QIcon(":/document.xpm"));

	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(Q3Table::MultiRow);

	// header
	nameList += "Name";
	nameList += "Country";
	nameList += "Spot";
	nameList += "Longitude\n[Deg,min]";
	nameList += "Latitude\n[Deg,min]";
	nameList += "Altitude\n[m]";
	nameList += "Description";
	setupHeader(nameList);

	pTable->setColumnWidth(Name, 150);
	pTable->setColumnWidth(Country, 70);
	pTable->setColumnWidth(Spot, 120);
	pTable->setColumnWidth(Longitude, 90);
	pTable->setColumnWidth(Latitude, 90);
	pTable->setColumnWidth(Altitude, 70);
	pTable->setColumnWidth(Description, 500);

	file_update();
}

WayPointWindow::~WayPointWindow()
{
}

void WayPointWindow::closeEvent(QCloseEvent *pEvent)
{
  if(m_pWayPointView != NULL)
  {
    delete m_pWayPointView;
    m_pWayPointView = NULL;
  }
}

void WayPointWindow::selectionChanged()
{
  int row;

	if((m_pWayPointView != NULL) && !m_externSelect)
	{
	  row = getTable()->currentRow();

	  if(row >= 0)
	  {
      m_pWayPointView->selectWayPoint(m_wpList[row].id());
	  }
	}
}

void WayPointWindow::file_update()
{
	Q3Table *pTable = TableWindow::getTable();
	ProgressDlg progDlg(this);
	uint wpNr;
	uint maxWpNr;

	m_wpList.clear();
	pTable->setNumRows(0); // clear table, because of different nr of waypoints

	if(m_pDb->open())
	{
		progDlg.beginProgress("reading waypoints...", m_pDb);
		m_pDb->wayPointList(m_wpList);
		progDlg.endProgress();
		m_pDb->close();
	}

	maxWpNr = m_wpList.size();
	pTable->setNumRows(maxWpNr);

	for(wpNr=0; wpNr<maxWpNr; wpNr++)
	{
		setWpToRow(wpNr, m_wpList[wpNr]);
	}
}

void WayPointWindow::file_AddToGps()
{
	int numSel;
	int sel;
	int topRow;
	int bottomRow;
	int row;

	numSel = getTable()->numSelections();

	if((numSel > 0) && IGPSDevice::pInstance()->open())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));

		for(sel=0; sel<numSel; sel++)
		{
			topRow = getTable()->selection(sel).topRow();
			bottomRow = getTable()->selection(sel).bottomRow();

			for(row=topRow; row<=bottomRow; row++)
			{
				IGPSDevice::pInstance()->add(m_wpList[row]);
			}
		}

		TableWindow::unsetCursor();
		IGPSDevice::pInstance()->close();
	}
}

void WayPointWindow::file_delete()
{
	int numSel;
	int sel;
	int topRow;
	int bottomRow;
	int row;

	numSel = getTable()->numSelections();

	if((numSel > 0) && m_pDb->open())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));

		for(sel=0; sel<numSel; sel++)
		{
			topRow = getTable()->selection(sel).topRow();
			bottomRow = getTable()->selection(sel).bottomRow();

			for(row=topRow; row<=bottomRow; row++)
			{
				m_pDb->delWayPoint(m_wpList[row]);
			}
		}

		TableWindow::unsetCursor();
		m_pDb->close();
	}
}

void WayPointWindow::file_deleteAll()
{
	if(m_pDb->open())
	{
    TableWindow::setCursor(QCursor(Qt::WaitCursor));
    m_pDb->delAllWayPoints();
    TableWindow::unsetCursor();
		m_pDb->close();
	}
}

void WayPointWindow::file_addNewWp()
{
	WayPoint wp;
	IWayPointForm wayPointForm(this, tr("New WayPoint"), &wp);

	if(wayPointForm.exec() && m_pDb->open())
	{
		m_pDb->add(wp);
		m_pDb->close();
	}
}

void WayPointWindow::file_AddToSqlDB()
{
	int row;

	row = getTable()->currentRow();

	if((row >= 0) && ISql::pInstance()->open())
	{
		IWayPointForm wayPointForm(this, tr("Add WayPoint to DB"), &m_wpList[row]);

		if(wayPointForm.exec())
		{
			TableWindow::setCursor(QCursor(Qt::WaitCursor));
			ISql::pInstance()->add(m_wpList[row]);
			TableWindow::unsetCursor();
		}

		ISql::pInstance()->close();
	}
}

void WayPointWindow::file_Edit()
{
	int row;

	row = getTable()->currentRow();

	if(row >= 0)
	{
    IWayPointForm wayPointForm(this, tr("New WayPoint"), &m_wpList[row]);

    if(wayPointForm.exec() && m_pDb->open())
    {
      m_pDb->update(m_wpList[row]);
      m_pDb->close();
    }
	}
}

void WayPointWindow::file_editWebMap()
{
	if((m_pWayPointView == NULL) && (m_wpList.size() >= 0))
	{
		m_pWayPointView = new WebMapWayPointView(tr("Edit WayPoints"));
    connect(m_pWayPointView, SIGNAL(finished(int)), this, SLOT(wayPointViewFinished(int)));
		connect(m_pWayPointView, SIGNAL(wayPointChanged(const WayPoint&)), this,
            SLOT(wayPointChanged(const WayPoint&)));
    connect(m_pWayPointView, SIGNAL(wayPointChanged(int)), this, SLOT(wayPointChanged(int)));
		m_pWayPointView->setWayPointList(&m_wpList);
		m_pWayPointView->loadMap();
    m_pWayPointView->setWindowModality(Qt::NonModal);
		m_pWayPointView->show();
	}
}

void WayPointWindow::file_viewWebMap()
{
	if((m_pWayPointView == NULL) && (m_wpList.size() >= 0))
	{
		m_pWayPointView = new WebMapWayPointView(tr("View WayPoints"));
    connect(m_pWayPointView, SIGNAL(finished(int)), this, SLOT(wayPointViewFinished(int)));
		connect(m_pWayPointView, SIGNAL(wayPointChanged(const WayPoint&)), this,
            SLOT(wayPointChanged(const WayPoint&)));
    connect(m_pWayPointView, SIGNAL(wayPointChanged(int)), this, SLOT(wayPointChanged(int)));
		m_pWayPointView->setWayPointList(&m_wpList);
		m_pWayPointView->loadMap();
    m_pWayPointView->setWindowModality(Qt::NonModal);
		m_pWayPointView->show();
	}
}

void WayPointWindow::wayPointViewFinished(int res)
{
  m_pWayPointView = NULL;
}

void WayPointWindow::wayPointChanged(const WayPoint &wp)
{
  WayPoint locWp;

  locWp = wp;

  if(m_pDb->open())
  {
    m_externSelect = true;
    m_pDb->update(locWp);
    m_externSelect = false;
    m_pDb->close();
  }
}

void WayPointWindow::wayPointChanged(int id)
{
  int row;
  int listSize;

  listSize = m_wpList.size();

  for(row=0; row<listSize; row++)
  {
    if(m_wpList[row].id() == id)
    {
      m_externSelect = true;
      TableWindow::selectRow(row);
      m_externSelect = false;
      break;
    }
  }
}

void WayPointWindow::setWpToRow(uint row, WayPoint &wp)
{
	QString str;
	Q3Table *pTable = TableWindow::getTable();

	pTable->setText(row, Country, wp.country());
	pTable->setText(row, Spot, wp.spot());
	pTable->setText(row, Name, wp.name());

	str.sprintf("%.5f", wp.longitude());
	pTable->setText(row, Longitude, str);

	str.sprintf("%.5f", wp.latitude());
	pTable->setText(row, Latitude, str);

	str.sprintf("%i", wp.altitude());
	pTable->setText(row, Altitude, str);

	pTable->setText(row, Description, wp.description());
}
