/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

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
#include "MDIWindow.h"
#include "Images.h"
#include "IDataBase.h"
#include "IGPSDevice.h"
#include "IRouteForm.h"
#include "RouteWindow.h"
#include "WayPointWindow.h"

MainWindow::MainWindow()
	:QMainWindow(0, "KFlyChart", WDestructiveClose)
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
	pMenu->insertItem("&Quit", qApp, SLOT(closeAllWindows()), CTRL+Key_Q);

	// Flights
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Flights", pMenu);
	pMenu->insertItem("&From DB", this, SLOT(flights_fromSQL()), CTRL+Key_F);
	pMenu->insertItem("&From GPS", this, SLOT(flights_fromGPS()), CTRL+Key_N);

	// WayPoints
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&WayPoints", pMenu);
	pMenu->insertItem("&From DB", this, SLOT(waypoints_fromSQL()), CTRL+Key_N);
	pMenu->insertItem("&From GPS", this, SLOT(waypoints_fromGPS()), CTRL+Key_N);
	
	// Routes
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Routes", pMenu);
	pMenu->insertItem("&From DB", this, SLOT(routes_fromSQL()), CTRL+Key_N);
	pMenu->insertItem("&From GPS", this, SLOT(routes_fromGPS()), CTRL+Key_N);
	
	// Menu Windows
	m_pWindowsMenu = new QPopupMenu(this);
	m_pWindowsMenu->setCheckable(true);
	connect(m_pWindowsMenu, SIGNAL(aboutToShow()),
			this, SLOT(aboutToShow()));
	menuBar()->insertItem("&Windows", m_pWindowsMenu);

	// Menu Settings
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Settings", pMenu);
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
	pMenu->insertItem("&Device", m_pDevicesMenu, CTRL+Key_P);
	
	pMenu->insertItem("Configure Device...", this, SLOT(settings_configure_device()));

	// Menu Help
	menuBar()->insertSeparator();
	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&Help", pMenu);
	pMenu->insertItem( "&About", this, SLOT(help_about()), Key_F1);

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
	QMessageBox::about(this, "KFlyChart Beta Version",
			"Copyright (c):  2004 by Alex Graf\n"
			"This file is distributed under the terms of the General Public\n"
			"Licence. See the file gpl.txt for the Licence or visit gnu.org\n"
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
