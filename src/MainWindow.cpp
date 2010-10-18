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

#include <qmenubar.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qobject.h>
#include <qmdiarea.h>
#include <qmdisubwindow.h>
#include <QSignalMapper>
#include <QCloseEvent>

#include "AirSpaceWindow.h"
#include "GliderWindow.h"
#include "IFlyHighRC.h"
#include "IFlytecConfig.h"
#include "IFlytec6015Config.h"
#include "MainWindow.h"
#include "IFlyHighRC.h"
#include "IPortFrame.h"
#include "FlightWindow.h"
#include "FlightExpWindow.h"
#include "IFlyHighRCFrame.h"
#include "MDIWindow.h"
#include "IDataBase.h"
#include "IGPSDevice.h"
#include "IRouteForm.h"
#include "RouteWindow.h"
#include "ServicingWindow.h"
#include "WayPointWindow.h"
#include "WebMapRouteWindow.h"

MainWindow::MainWindow()
//	:QMainWindow(0, Qt::WDestructiveClose)
	:QMainWindow(0)
{
	QString devName;
	int id;
	uint devNr;
	uint curDev;
	uint maxDevNr;

	// Workspace
	m_pMdiArea = new QMdiArea(this);
	setCentralWidget(m_pMdiArea);
	QPalette palette;
	palette.setColor(m_pMdiArea->backgroundRole(),Qt::lightGray);
	m_pMdiArea->setPalette(palette);

	m_pWinMapper = new QSignalMapper(this);
	connect(m_pWinMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
	
	m_pActiveWin = NULL;
	QMainWindow::setWindowTitle("FlyHigh");
	
	// Menu File
	QMenu* pFileMenu = menuBar()->addMenu(tr("&File"));
	QAction* pQuitAct = new QAction(tr("&Quit"), this);
	connect(pQuitAct,SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
	pFileMenu->addAction(pQuitAct);


	// Analysis
	QMenu* pAnalysisMenu = menuBar()->addMenu(tr("&Analysis"));


	QAction* pFlDBAct = new QAction(tr("&Flights (DB)"), this);
	connect(pFlDBAct,SIGNAL(triggered()), this, SLOT(flights_fromSQL()));
	pAnalysisMenu->addAction(pFlDBAct);
	QAction* pFlDevAct = new QAction(tr("Flights (G&PS)"), this);
	connect(pFlDevAct,SIGNAL(triggered()), this, SLOT(flights_fromGPS()));
	pAnalysisMenu->addAction(pFlDevAct);
	pAnalysisMenu->addSeparator();
	QAction* pExpAct = new QAction(tr("Flight &Exp"), this);
	connect(pExpAct,SIGNAL(triggered()), this, SLOT(flights_experience()));
	pAnalysisMenu->addAction(pExpAct);
	pAnalysisMenu->addSeparator();
	QAction* pGlidAct = new QAction(tr("&Gliders"), this);
	connect(pGlidAct,SIGNAL(triggered()), this, SLOT(analysis_gliders()));
	pAnalysisMenu->addAction(pGlidAct);
	QAction* pServAct = new QAction(tr("&Servicing"), this);
	connect(pServAct,SIGNAL(triggered()), this, SLOT(analysis_servicing()));
	pAnalysisMenu->addAction(pServAct);


	// Preparation
	QMenu* pPrepMenu = menuBar()->addMenu(tr("&Preparation"));

	QAction* pWPDBAct = new QAction(tr("&WayPoints (DB)"), this);
	connect(pWPDBAct,SIGNAL(triggered()), this, SLOT(waypoints_fromSQL()));
	pPrepMenu->addAction(pWPDBAct);
	QAction* pWPDevAct = new QAction(tr("WayPoints (G&PS)"), this);
	connect(pWPDevAct,SIGNAL(triggered()), this, SLOT(waypoints_fromGPS()));
	pPrepMenu->addAction(pWPDevAct);
	pPrepMenu->addSeparator();
	QAction* pRtDBAct = new QAction(tr("&Routes (DB)"), this);
	connect(pRtDBAct,SIGNAL(triggered()), this, SLOT(routes_fromSQL()));
	pPrepMenu->addAction(pRtDBAct);
	QAction* pRtDevAct = new QAction(tr("Routes (&GPS)"), this);
	connect(pRtDevAct,SIGNAL(triggered()), this, SLOT(routes_fromGPS()));
	pPrepMenu->addAction(pRtDevAct);
	QAction* pWRtDBAct = new QAction(tr("&WRoutes (DB)"), this);
	connect(pWRtDBAct,SIGNAL(triggered()), this, SLOT(wroute_fromSQL()));
	pPrepMenu->addAction(pWRtDBAct);
	pPrepMenu->addSeparator();
	QAction* pAirDevAct = new QAction(tr("&Airspaces (GPS)"), this);
	connect(pAirDevAct,SIGNAL(triggered()), this, SLOT(airspaces_fromGPS()));
	pPrepMenu->addAction(pAirDevAct);
	QAction* pAirFileAct = new QAction(tr("Airspaces (&File)"), this);
	connect(pAirFileAct,SIGNAL(triggered()), this, SLOT(airspaces_fromFile()));
	pPrepMenu->addAction(pAirFileAct);

	// Menu Configuration
	QMenu* pConfMenu = menuBar()->addMenu(tr("&Configuration"));

	// Submenu Port
	QAction* pPortAct = new QAction(tr("&Port..."), this);
	connect(pPortAct,SIGNAL(triggered()), this, SLOT(settings_port()));
	pConfMenu->addAction(pPortAct);
	
	// Submenu Settings>Device
	m_pDevicesMenu = pConfMenu->addMenu(tr("&Device"));
	// create a radiobutton group
	QActionGroup* pDevActGrp = new QActionGroup(this);
	maxDevNr = IFlyHighRC::pInstance()->deviceNameList().size();
	curDev = IFlyHighRC::pInstance()->deviceName();

	// add all currently supported devices
	for(devNr=0; devNr<maxDevNr; devNr++)
	{
		devName = IFlyHighRC::pInstance()->deviceNameList().at(devNr);
		QAction* pDevAct = new QAction(devName,this);
		pDevAct->setData(devNr);
		pDevAct->setCheckable(true);
		connect(pDevAct,SIGNAL(triggered()),this,SLOT(settings_device()));
		// enable current device
		if(devNr == curDev)
		{
			pDevAct->setChecked(true);
		}
		pDevActGrp->addAction(pDevAct);
	}

	// add buttons to menu
	m_pDevicesMenu->addActions(pDevActGrp->actions());

	QAction* pConfAct = new QAction(tr("&Configure Device..."), this);
	connect(pConfAct,SIGNAL(triggered()), SLOT(settings_configure_device()));
	pConfMenu->addAction(pConfAct);

	QAction* pPilotAct = new QAction(tr("Pilot &Info..."), this);
	connect(pPilotAct,SIGNAL(triggered()), SLOT(settings_pilotInfo()));
	pConfMenu->addAction(pPilotAct);

	// Menu Windows
	m_pWindowsMenu = menuBar()->addMenu(tr("&Windows"));

	m_pCascade = new QAction(tr("&Cascade"), this);
	connect(m_pCascade, SIGNAL(triggered()), m_pMdiArea, SLOT(cascadeSubWindows()));
	
	m_pTile = new QAction(tr("&Tile"), this);
	connect(m_pTile, SIGNAL(triggered()), m_pMdiArea, SLOT(tileSubWindows()));
	
	m_pTileHor = new QAction(tr("Tile &Horizontally"), this);
	connect(m_pTileHor, SIGNAL(triggered()), SLOT(windows_tile_horizontally()));

	m_pWinSeparator = new QAction(this);
	m_pWinSeparator->setSeparator(true);

	connect(m_pWindowsMenu, SIGNAL(aboutToShow()),this, SLOT(aboutToShow()));

	// Menu Help
	menuBar()->addSeparator();
	QMenu* pHelpMenu = menuBar()->addMenu(tr("&Help"));

	QAction* pAboutAct = new QAction(tr("&About"), this);
	connect(pAboutAct,SIGNAL(triggered()), SLOT(help_about()));
	pHelpMenu->addAction(pAboutAct);

	statusBar()->showMessage("Ready", 2000);

	// if pilot info is not set
	if(IFlyHighRC::pInstance()->pilotId() < 0)
	{
		settings_pilotInfo();
	}
}

MainWindow::~MainWindow()
{
}

void MainWindow::flights_fromGPS()
{
        MDIWindow* pWin = new FlightWindow(m_pMdiArea,
                                           0,
                                           0, //Qt::WDestructiveClose|Qt::Window,
                                           IDataBase::GPSdevice);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::flights_fromSQL()
{
	MDIWindow *pWin;

	pWin = new FlightWindow(m_pMdiArea, "Flights", 0, IDataBase::SqlDB);
	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::flights_experience()
{
	MDIWindow *pWin;

	pWin = new FlightExpWindow(m_pMdiArea, "Flight Experience", 0);
	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::analysis_gliders()
{
	MDIWindow* pWin;

	pWin = new GliderWindow(m_pMdiArea, "Glider", 0);
	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::analysis_servicing()
{
	MDIWindow* pWin;

	pWin = new ServicingWindow(m_pMdiArea, "Servicing", 0);
	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::waypoints_fromSQL()
{
	MDIWindow* pWin = new WayPointWindow(m_pMdiArea, "WayPoints from DB", 0, IDataBase::SqlDB);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::waypoints_fromGPS()
{
	MDIWindow* pWin = new WayPointWindow(m_pMdiArea, "WayPoints from GPS", 0, IDataBase::GPSdevice);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::routes_fromSQL()
{
	MDIWindow* pWin = new RouteWindow(m_pMdiArea, "Routes from DB", 0, IDataBase::SqlDB);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::routes_fromGPS()
{
	MDIWindow* pWin = new RouteWindow(m_pMdiArea, "Routes from GPS", 0, IDataBase::GPSdevice);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::wroute_fromSQL()
{
	MDIWindow* pWin = new WebMapRouteWindow(m_pMdiArea, "Route from DB", 0, IDataBase::SqlDB);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

/*
void MainWindow::airspaces_fromSQL()
{
	MDIWindow* pWin = new AirSpaceWindow(m_pWorkSpace, "Airspaces from DB", WDestructiveClose, IDataBase::SqlDB);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}
*/

void MainWindow::airspaces_fromGPS()
{
	MDIWindow* pWin = new AirSpaceWindow(m_pMdiArea, "Airspaces from GPS", 0, IDataBase::GPSdevice);

	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::airspaces_fromFile()
{
	MDIWindow* pWin;

	pWin = new AirSpaceWindow(m_pMdiArea, "Airspaces from OpenAirTextFile", 0, IDataBase::File);
	connect(pWin, SIGNAL(message(const QString&, int)), statusBar(), SLOT(message(const QString&, int)));
	showWindow(pWin);
}

void MainWindow::help_about()
{
	QMessageBox::about(this, IFlyHighRC::pInstance()->versionInfo(),
			"Copyright (c): 2004-2010 by Alex Graf <grafal@sf.net> and \n"
			"Jurg Allemann <ja_kern@sf.net>.\n"
			"FlyHigh is distributed under the terms of the General Public\n"
			"License (GPL). Visit www.gnu.org for more information.");
}

void MainWindow::aboutToShow()
{
	QList<QMdiSubWindow *> winList;
	unsigned int nofWin;
	unsigned int winNr;
	int menuItemId;
	bool hasMdiChild;

	m_pWindowsMenu->clear();
	m_pWindowsMenu->addAction(m_pCascade);
	m_pWindowsMenu->addAction(m_pTile);
	m_pWindowsMenu->addAction(m_pTileHor);
	m_pWindowsMenu->addAction(m_pWinSeparator);

	// set enabled
	hasMdiChild = !m_pMdiArea->subWindowList().isEmpty();
	m_pCascade->setEnabled(hasMdiChild);
	m_pTile->setEnabled(hasMdiChild);
	m_pTileHor->setEnabled(hasMdiChild);

	QList<QMdiSubWindow *> windows = m_pMdiArea->subWindowList();
	m_pWinSeparator->setVisible(!windows.isEmpty());

	for (int i = 0; i < windows.size(); ++i)
	{
			MDIWindow *child = qobject_cast<MDIWindow *>(windows.at(i)->widget());

			QString text;
			if (i < 9) {
					text = tr("&%1 %2").arg(i + 1)
															.arg(child->windowTitle());
			} else {
					text = tr("%1 %2").arg(i + 1)
														.arg(child->windowTitle());
			}
			QAction *action  = m_pWindowsMenu->addAction(text);
			action->setCheckable(true);
			action ->setChecked(child == activeMdiChild());
			connect(action, SIGNAL(triggered()), m_pWinMapper, SLOT(map()));
			m_pWinMapper->setMapping(action, windows.at(i));
	}
/**

	// show window list
        m_pWindowsMenu->addSeparator();
        winList = m_pMdiArea->subWindowList();
	nofWin = winList.count();
	
	for(winNr=0; winNr<nofWin; winNr++)
	{
                menuItemId = m_pWindowsMenu->insertItem(winList.at(winNr)->windowTitle(),
					 this, SLOT(windows_activated(int)));
		m_pWindowsMenu->setItemParameter(menuItemId, winNr);
                m_pWindowsMenu->setItemChecked(menuItemId, m_pMdiArea->activeSubWindow() == winList.at(winNr));
	}
*/
}

void MainWindow::windows_activated(int id)
{
        QWidget* pWin = m_pMdiArea->subWindowList().at(id);

	if(pWin != NULL)
	{
		pWin->showNormal();
		pWin->setFocus();
	}
}

void MainWindow::windows_tile_horizontally()
{
        QList<QMdiSubWindow *> winList = m_pMdiArea->subWindowList();
	QWidget *pWin;
	unsigned int winNr;
	unsigned int nofWin;
	int heightForEach;
	int preferredHeight;
	int actHeight;
	int y = 0;

	// if 0 or 1 windows do normal tiling
	if(winList.count() < 2)
	{
		m_pMdiArea->tileSubWindows();
		return;
	}
  
	// primitive horizontal tiling
	nofWin = winList.count();
	heightForEach = m_pMdiArea->height() / nofWin;

	for(winNr= 0; winNr<nofWin; winNr++)
	{
		pWin = winList.at(winNr);
		
		if(pWin->isMaximized())
		{
			// prevent flicker
			pWin->hide();
			pWin->showNormal();
		}

		// set new height
		pWin->resize(m_pMdiArea->width(), heightForEach);
		pWin->move(0,y);
		y += heightForEach;
	}
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	QList<QMdiSubWindow *> winList = m_pMdiArea->subWindowList();
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
	m_pMdiArea->addSubWindow(pWin);
	pWin->setWindowState(Qt::WindowNoState|Qt::WindowActive);
	// show the very first window in maximized mode
	if(m_pMdiArea->subWindowList().count()==1)
	{
		m_pMdiArea->activeSubWindow()->showMaximized();
	}
}

void MainWindow::settings_device()
{
	QAction* pAct = qobject_cast<QAction*>(sender());
	
	IGPSDevice::pInstance()->close();
	IFlyHighRC::pInstance()->setDeviceName(pAct->data().toUInt());
	IGPSDevice::pInstance()->open();
}

void MainWindow::settings_port()
{
	IPortFrame portFrame;
	QString value;

	if(portFrame.show())
	{
		IGPSDevice::pInstance()->close();
		IGPSDevice::pInstance()->open();
	}
}

void MainWindow::settings_configure_device()
{
	IFlytecConfig *pFrame;
	IFlytec6015Config *pConfig6015;
	
	switch(IFlyHighRC::pInstance()->deviceName())
	{
		case IFlyHighRC::DevFlytec5020:
			pFrame = new IFlytecConfig();
			pFrame->show();
		break;
		case IFlyHighRC::DevFlytec6015:
			pConfig6015 = new IFlytec6015Config();
			pConfig6015->show();
		break;
			/*
			handle here other devices
			case 1:
			break;
			*/
	}
}

void MainWindow::settings_pilotInfo()
{
	IFlyHighRCFrame rcFrame;
	
	rcFrame.show();
}

MDIWindow* MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = m_pMdiArea->activeSubWindow())
        return qobject_cast<MDIWindow *>(activeSubWindow->widget());
    return 0;
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    m_pMdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
