/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#include <qworkspace.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qobjectlist.h>
#include <qvbox.h>

#include "IFlytecConfig.h"
#include "MainWindow.h"
#include "IPortFrame.h"
#include "FlightWindow.h"
#include "FlightExpWindow.h"
#include "MDIWindow.h"
#include "Images.h"
#include "IDataBase.h"
#include "IGPSDevice.h"
#include "IRouteForm.h"
#include "RouteWindow.h"
#include "ServicingWindow.h"
#include "WayPointWindow.h"

MainWindow::MainWindow()
	:QMainWindow(0, "FlyHigh", WDestructiveClose)
{
	QPopupMenu *pMenu;
	QVBox *pVBox;
	int id;
	
	m_pActiveWin = NULL;
	m_selectedDevice = 0;
	
	// Menu File
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&File", pMenu);
	pMenu->insertSeparator();
	pMenu->insertItem("&Quit", qApp, SLOT(closeAllWindows()));

	// Analysis
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Analysis", pMenu);
	pMenu->insertItem("&Flights (DB)", this, SLOT(flights_fromSQL()));
	pMenu->insertItem("&Flights (GPS)", this, SLOT(flights_fromGPS()));
	pMenu->insertItem("&Flight Exp", this, SLOT(flights_experience()));
	pMenu->insertItem("&Servicing", this, SLOT(servicing()));

	// Preparation
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Preparation", pMenu);
	pMenu->insertItem("&WayPoints (DB)", this, SLOT(waypoints_fromSQL()));
	pMenu->insertItem("&WayPoints (GPS)", this, SLOT(waypoints_fromGPS()));
	pMenu->insertItem("&Routes (DB)", this, SLOT(routes_fromSQL()));
	pMenu->insertItem("&Routes (GPS)", this, SLOT(routes_fromGPS()));

	// Menu Configuration
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Configuration", pMenu);
	pMenu->insertItem("Port...", this, SLOT(settings_port()));
	
	// Menu Settings>Device
	m_pDevicesMenu = new QPopupMenu(this);
	m_pDevicesMenu->setCheckable(true);
	id = m_pDevicesMenu->insertItem("Flytec 5020", this, SLOT(settings_device(int)));
	m_pDevicesMenu->setItemChecked(id, true);
	/*
		add here other devices 
		pSubMenu->insertItem("Flytec 5030", this, SLOT(settings_device(int)));
	*/
	pMenu->insertItem("&Device", m_pDevicesMenu);
	pMenu->insertItem("Configure Device...", this, SLOT(settings_configure_device()));

	// Menu Windows
	m_pWindowsMenu = new QPopupMenu(this);
	m_pWindowsMenu->setCheckable(true);
	connect(m_pWindowsMenu, SIGNAL(aboutToShow()),
			this, SLOT(aboutToShow()));
	menuBar()->insertItem("&Windows", m_pWindowsMenu);

	// Menu Help
	menuBar()->insertSeparator();
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Help", pMenu);
	pMenu->insertItem( "&About", this, SLOT(help_about()));

	// Frame
	pVBox = new QVBox(this);
	pVBox->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	setCentralWidget(pVBox);
	
	// Workspace
	m_pWorkSpace = new QWorkspace(pVBox);
	m_pWorkSpace->setScrollBarsEnabled(true);
	m_pWorkSpace->setPaletteBackgroundColor(Qt::lightGray);
	
	statusBar()->message("Ready", 2000);
}

MainWindow::~MainWindow()
{
}

void MainWindow::flights_fromGPS()
{
	MDIWindow* pWin = new FlightWindow(m_pWorkSpace, 0, WDestructiveClose, IDataBase::GPSdevice);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::flights_fromSQL()
{
	MDIWindow* pWin = new FlightWindow(m_pWorkSpace, "Flights", WDestructiveClose, IDataBase::SqlDB);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::flights_experience()
{
	MDIWindow* pWin = new FlightExpWindow(m_pWorkSpace, "Flight Experience", WDestructiveClose);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::servicing()
{
	MDIWindow* pWin = new ServicingWindow(m_pWorkSpace, "Servicing", WDestructiveClose);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::waypoints_fromSQL()
{
	MDIWindow* pWin = new WayPointWindow(m_pWorkSpace, "WayPoints from DB", WDestructiveClose, IDataBase::SqlDB);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::waypoints_fromGPS()
{
	MDIWindow* pWin = new WayPointWindow(m_pWorkSpace, "WayPoints from GPS", WDestructiveClose, IDataBase::GPSdevice);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::routes_fromSQL()
{
	MDIWindow* pWin = new RouteWindow(m_pWorkSpace, "Routes from DB", WDestructiveClose, IDataBase::SqlDB);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::routes_fromGPS()
{
	MDIWindow* pWin = new RouteWindow(m_pWorkSpace, "Routes from GPS", WDestructiveClose, IDataBase::GPSdevice);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::help_about()
{
	QMessageBox::about(this, "FlyHigh Beta Version",
			"Copyright (c):  2005 by Alex Graf, <grafal@sourceforge.net>\n"
			"This file is distributed under the terms of the General Public\n"
			"License. See the file gpl.txt for the Licence or visit www.gnu.org\n"
			"for more information.");
}

void MainWindow::aboutToShow()
{
	QWidgetList winList;
	unsigned int nofWin;
	unsigned int winNr;
	int cascadeId;
	int tileId;
	int horTileId;
	int menuItemId;

	// add menu items
	m_pWindowsMenu->clear();
	cascadeId = m_pWindowsMenu->insertItem("&Cascade", m_pWorkSpace, SLOT(cascade()));
	tileId = m_pWindowsMenu->insertItem("&Tile", m_pWorkSpace, SLOT(tile()));
	horTileId = m_pWindowsMenu->insertItem("Tile &Horizontally", this, SLOT(windows_tile_horizontally()));

	// set enabled
	if(m_pWorkSpace->windowList().isEmpty())
	{
		m_pWindowsMenu->setItemEnabled(cascadeId, false);
		m_pWindowsMenu->setItemEnabled(tileId, false);
		m_pWindowsMenu->setItemEnabled(horTileId, false);
	}

	// show window list
	m_pWindowsMenu->insertSeparator();
	winList = m_pWorkSpace->windowList();
	nofWin = winList.count();
	
	for(winNr=0; winNr<nofWin; winNr++)
	{
		menuItemId = m_pWindowsMenu->insertItem(winList.at(winNr)->caption(),
					 this, SLOT(windows_activated(int)));
		m_pWindowsMenu->setItemParameter(menuItemId, winNr);
		m_pWindowsMenu->setItemChecked(menuItemId, m_pWorkSpace->activeWindow() == winList.at(winNr));
	}
}

void MainWindow::windows_activated(int id)
{
	QWidget* pWin = m_pWorkSpace->windowList().at(id);

	if(pWin != NULL)
	{
		pWin->showNormal();
		pWin->setFocus();
	}
}

void MainWindow::windows_tile_horizontally()
{
	QWidgetList winList = m_pWorkSpace->windowList();
	QWidget *pWin;
	unsigned int winNr;
	unsigned int nofWin;
	int heightForEach;
	int preferredHeight;
	int actHeight;
	int y = 0;

	if(winList.count() == 0)
	{
		return;
	}
  
	// primitive horizontal tiling  
	heightForEach = m_pWorkSpace->height() / winList.count();
	nofWin = winList.count();

	for(winNr= 0; winNr<nofWin; winNr++)
	{
		pWin = winList.at(winNr);
		
		if(pWin->testWState(WState_Maximized))
		{
			// prevent flicker
			pWin->hide();
			pWin->showNormal();
		}

		// set new height
		preferredHeight = pWin->minimumHeight() + pWin->parentWidget()->baseSize().height();
		actHeight = QMAX(heightForEach, preferredHeight);
		pWin->parentWidget()->setGeometry( 0, y, m_pWorkSpace->width(), actHeight);
		y += actHeight;
	}
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	QWidgetList winList = m_pWorkSpace->windowList();
	QWidget *pWin;
	unsigned int nofWin = winList.count();
	unsigned int winNr;
	
	if(nofWin > 0)
	{
		for(winNr=0; winNr<nofWin; winNr++)
		{
	    pWin = winList.at(winNr);
			
	    if(!pWin->close())
			{
				e->ignore();
				return;
	    }
		}
	}
	
	QMainWindow::closeEvent(e);
}

void MainWindow::showWindow(QMainWindow *pWin)
{
	// show the very first window in maximized mode
	if(m_pWorkSpace->windowList().isEmpty())
	{
		pWin->showMaximized();
	}
	else
	{
		pWin->show();
	}
}

void MainWindow::settings_device(int id)
{
	unsigned int itemNr;
	unsigned int nofItems = m_pDevicesMenu->count();
	int itemId;
	bool selected;
	
	// check the selected item
	for(itemNr=0; itemNr<nofItems; itemNr++)
	{
		itemId = m_pDevicesMenu->idAt(itemNr);
		
		selected = (itemId == id);
		m_pDevicesMenu->setItemChecked(itemId, selected);
		
		if(selected)
		{
			m_selectedDevice = itemNr;
		}
	}
}

void MainWindow::settings_port()
{
	IPortFrame portFrame;
	QString port;

	if(portFrame.show())
	{
		portFrame.getPort(port);
		IGPSDevice::pInstance()->setPort(port);
	}
}

void MainWindow::settings_configure_device()
{
	IFlytecConfig *pFrame;
	
	switch(m_selectedDevice)
	{
		case 0:
			pFrame = new IFlytecConfig();
			pFrame->show();
		break;
			/*
			handle here other devices
			case 1:
			break;
			*/
	}
}
