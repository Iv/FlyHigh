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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QCursor>
#include <QMenuBar>
#include <QString>
#include <q3table.h>
#include "IGPSDevice.h"
#include "ProgressDlg.h"
#include "IRouteForm.h"
#include "ISql.h"
#include "Route.h"
#include "RouteWindow.h"
#include "WayPoint.h"
#include "WebMapRouteView.h"

#include <QDebug>

RouteWindow::RouteWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags)
{
	QString caption;
	QStringList nameList;
	Q3Table *pTable = TableWindow::getTable();

	QMenu* pFileMenu = menuBar()->addMenu(tr("&File"));

	switch(src)
	{
		case IDataBase::SqlDB:
		{
			m_pDb = ISql::pInstance();
			caption = "Routes from DB";

			QAction* pNewAct = new QAction(tr("&New..."), this);
			connect(pNewAct, SIGNAL(triggered()), this, SLOT(file_new()));
			pFileMenu->addAction(pNewAct);
			QAction* pNewWebMapAct = new QAction(tr("New &Web Map..."), this);
			connect(pNewWebMapAct, SIGNAL(triggered()), this, SLOT(file_newWebMap()));
			pFileMenu->addAction(pNewWebMapAct);
			QAction* pAddAct = new QAction(tr("&Add to GPS..."), this);
			connect(pAddAct, SIGNAL(triggered()), this, SLOT(file_AddToGPS()));
			pFileMenu->addAction(pAddAct);
		}
		break;
		case IDataBase::GPSdevice:
		{
			m_pDb = IGPSDevice::pInstance();
			caption = "Routes from GPS";
/**
			QAction* pAddAct = new QAction(tr("&Add to DB..."), this);
			connect(pAddAct, SIGNAL(triggered()), this, SLOT(file_AddToSqlDB()));
			pFileMenu->addAction(pAddAct);
*/
		}
		break;
		default:
			Q_ASSERT(false);
		break;
	}

  connect(m_pDb, SIGNAL(routesChanged()), this, SLOT(file_update()));

	QAction* pViewAct = new QAction(tr("&View"), this);
	connect(pViewAct, SIGNAL(triggered()), this, SLOT(file_view()));
	pFileMenu->addAction(pViewAct);

	if(src == IDataBase::SqlDB)
	{
    QAction* pViewWebMapAct = new QAction(tr("&View Web Map"), this);
		connect(pViewWebMapAct, SIGNAL(triggered()), this, SLOT(file_viewWebMap()));
		pFileMenu->addAction(pViewWebMapAct);
		QAction* pViewCopyFromAct = new QAction(tr("&Copy from..."), this);
		connect(pViewCopyFromAct, SIGNAL(triggered()), this, SLOT(file_copyFrom()));
		pFileMenu->addAction(pViewCopyFromAct);
	}

	QAction* pDelAct = new QAction(tr("&Delete"), this);
	connect(pDelAct, SIGNAL(triggered()), this, SLOT(file_delete()));
	pFileMenu->addAction(pDelAct);
	QAction* pUpdateAct = new QAction(tr("&Update"), this);
	connect(pUpdateAct, SIGNAL(triggered()), this, SLOT(file_update()));
	pFileMenu->addAction(pUpdateAct);
	QAction* pExpAllAct = new QAction(tr("&Export all..."), this);
	connect(pExpAllAct, SIGNAL(triggered()), this, SLOT(exportTable()));
	pFileMenu->addAction(pExpAllAct);

	TableWindow::setWindowTitle(caption);
	TableWindow::setWindowIcon(QIcon(":/document.xpm"));

	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(Q3Table::SingleRow);

	// header
	nameList += tr("Name");
	nameList += tr("Type");
	setupHeader(nameList);

	pTable->setColumnWidth(Name, 200);
	pTable->setColumnWidth(Type, 300);

	file_update();
}

void RouteWindow::file_delete()
{
	int row;

	row = getTable()->currentRow();

	if((row >= 0) && m_pDb->open())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->delRoute(m_routeList[row]);
		TableWindow::unsetCursor();
		m_pDb->close();
	}
}

void RouteWindow::file_update()
{
	ProgressDlg progDlg(this);
	Q3Table *pTable = TableWindow::getTable();
	uint routeNr;
	uint maxRouteNr;

	m_routeList.clear();
	pTable->setNumRows(0);

	if(m_pDb->open())
	{
		progDlg.beginProgress("read routes...", m_pDb);
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->routeList(m_routeList);
		progDlg.endProgress();
		maxRouteNr = m_routeList.size();
		pTable->setNumRows(maxRouteNr);

		for(routeNr=0; routeNr<maxRouteNr; routeNr++)
		{
			setRouteToRow(routeNr, m_routeList[routeNr]);
		}

		TableWindow::unsetCursor();
		m_pDb->close();
	}
}

void RouteWindow::file_new()
{
	Route route;
	IRouteForm routeForm(this, tr("Add Route to DB"), &route);

	if(routeForm.exec() && m_pDb->open())
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->add(route);
		TableWindow::unsetCursor();
		m_pDb->close();
	}
}

void RouteWindow::file_newWebMap()
{
	Route route;

	// default route
	route.setName("FIE_90");
	route.wayPointList().push_back(WayPoint(46.52439, 7.97287));
	route.wayPointList().push_back(WayPoint(46.52439, 8.29102));
	route.wayPointList().push_back(WayPoint(46.27975, 8.29102));
	route.wayPointList().push_back(WayPoint(46.27975, 7.97287));
	route.wayPointList().push_back(WayPoint(46.40207, 8.13194));
	newWebMap(route);
}

void RouteWindow::file_view()
{
	int row;
	Route route;

	row = getTable()->currentRow();

	if(row >= 0)
	{
		route = m_routeList[row];
		IRouteForm routeForm(this, tr("View Route"), &route);
		routeForm.execReadOnly();
	}
}

void RouteWindow::file_viewWebMap()
{
	WebMapRouteView *pView;
	int row;
	Route route;

	row = getTable()->currentRow();

	if(row >= 0)
	{
		route = m_routeList[row];
		pView = new WebMapRouteView(tr("View Route"));
		pView->setRoute(&route);
		pView->setEditable(false);
		pView->loadMap();
		pView->exec();
	}
}

void RouteWindow::file_copyFrom()
{
	int row;

	row = getTable()->currentRow();

	if(row >= 0)
	{
    newWebMap(m_routeList[row]);
	}
}

void RouteWindow::file_AddToGPS()
{
	ProgressDlg progDlg(this);
	Route route;
	int row;
	int nTps;
	int tpNr;

	row = getTable()->currentRow();

	if((row >= 0) && IGPSDevice::pInstance()->open())
	{
		// Route
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		progDlg.beginProgress("writing route...", IGPSDevice::pInstance());
		route.setName(m_routeList[row].name());
		nTps = m_routeList[row].wayPointList().size();

		for(tpNr=0; tpNr<nTps; tpNr++)
		{
			route.wayPointList().push_back(m_routeList[row].wayPointList().at(tpNr));
		}

		if(((nTps == 2) && (route.type() == Route::Free)) ||
				((nTps == 5) && (route.type() == Route::FlatOrFaiTri)))
		{
			route.wayPointList().push_back(m_routeList[row].wayPointList().at(0));
		}

		IGPSDevice::pInstance()->add(route);
		progDlg.endProgress();
		IGPSDevice::pInstance()->close();
		TableWindow::unsetCursor();
	}
}

void RouteWindow::file_AddToSqlDB()
{
/**
	QString str;
	WayPoint wp;
	double lon;
	double lat;
	int alt;
	int row;

	row = getTable()->currentRow();

	if(row >= 0)
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));

		// Coordinates
		str = getTable()->text(row, Name);

		for(....)
		{
		lon = str.toDouble();
		str = getTable()->text(row, Latitude);
		lat = str.toDouble();
		str = getTable()->text(row, Altitude);
		alt = str.toInt();
		wp.setCoordinates(lat, lon, alt);

		// Name
		str = getTable()->text(row, Name);
		wp.setName(str);

		IWayPointForm wayPointForm(this, tr("Add WayPoint to DB"), &wp);

		if(wayPointForm.exec() && m_pDb->open())
		{
			m_pDb->add(wp);
			m_pDb->close();
		}

		TableWindow::unsetCursor();
	}
*/
}

void RouteWindow::setRouteToRow(uint row, Route &route)
{
	Q3Table *pTable = TableWindow::getTable();

	pTable->setText(row, Name, route.name());
	pTable->setText(row, Type, route.typeAsText());
}

void RouteWindow::newWebMap(Route &route)
{
	WebMapRouteView *pView;
	WayPoint::WayPointListType::iterator it;

	pView = new WebMapRouteView(tr("Add Route to DB"));
	pView->setRoute(&route);
	pView->loadMap();

	if((pView->exec() == QDialog::Accepted) && m_pDb->open())
	{
		// save new route
		TableWindow::setCursor(QCursor(Qt::WaitCursor));

		for(it=route.wayPointList().begin(); it!=route.wayPointList().end(); it++)
		{
			ISql::pInstance()->add(*it);
		}

		m_pDb->add(route);
		TableWindow::unsetCursor();
		m_pDb->close();
	}
}
