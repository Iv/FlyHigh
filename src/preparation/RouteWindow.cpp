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

#include <QAction>
#include <QCursor>
#include <QString>
#include <QTableWidget>
#include "AirSpaceList.h"
#include "IGPSDevice.h"
#include "ProgressDlg.h"
#include "IRouteForm.h"
#include "ISql.h"
#include "Route.h"
#include "RouteWindow.h"
#include "WayPoint.h"
#include "WebMapRouteView.h"

RouteWindow::RouteWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags)
{
	QStringList nameList;
	QAction *pAction;
	QTableWidget *pTable;

	pTable = TableWindow::getTable();

	switch(src)
	{
		case IDataBase::SqlDB:
		{
			m_pDb = ISql::pInstance();

			pAction = new QAction(tr("&New..."), this);
			connect(pAction, SIGNAL(triggered()), this, SLOT(file_new()));
			MDIWindow::addAction(pAction);

			pAction = new QAction(tr("New &Web Map..."), this);
			connect(pAction, SIGNAL(triggered()), this, SLOT(file_newWebMap()));
			MDIWindow::addAction(pAction);

      pAction = new QAction(tr("&Copy..."), this);
      connect(pAction, SIGNAL(triggered()), this, SLOT(file_copyFrom()));
      MDIWindow::addAction(pAction, true);
		}
		break;
		case IDataBase::GPSdevice:
		{
			m_pDb = IGPSDevice::pInstance();
/**
			pAction = new QAction(tr("&Add to DB..."), this);
			connect(pAction, SIGNAL(triggered()), this, SLOT(file_AddToSqlDB()));
			MDIWindow::addAction(pAction);
*/
		}
		break;
		default:
			Q_ASSERT(false);
		break;
	}

	pAction = new QAction(tr("&Delete"), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(file_delete()));
	MDIWindow::addAction(pAction);

	if(src == IDataBase::SqlDB)
	{
    pAction = new QAction(tr("&Add to GPS..."), this);
    connect(pAction, SIGNAL(triggered()), this, SLOT(file_AddToGPS()));
    MDIWindow::addAction(pAction, true);
	}

	pAction = new QAction(tr("&Update"), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(file_update()));
	MDIWindow::addAction(pAction);

  // import/export
	pAction = new QAction(this);
	pAction->setSeparator(true);
	MDIWindow::addAction(pAction);

	pAction = new QAction(tr("&Export all..."), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(exportTable()));
	MDIWindow::addAction(pAction);

  // view
	pAction = new QAction(this);
	pAction->setSeparator(true);
	MDIWindow::addAction(pAction, true);

	pAction = new QAction(tr("&View"), this);
	connect(pAction, SIGNAL(triggered()), this, SLOT(file_view()));
	MDIWindow::addAction(pAction, true);

	if(src == IDataBase::SqlDB)
	{
    pAction = new QAction(tr("View Web &Map"), this);
		connect(pAction, SIGNAL(triggered()), this, SLOT(file_viewWebMap()));
		MDIWindow::addAction(pAction, true);
	}

	TableWindow::setWindowIcon(QIcon(":/document.xpm"));

	// configure the table
	pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	pTable->setSelectionMode(QAbstractItemView::SingleSelection);

	// header
	nameList += tr("Name");
	nameList += tr("Type");
	setupHeader(nameList);

	pTable->setColumnWidth(Name, 200);
	pTable->setColumnWidth(Type, 300);

  connect(m_pDb, SIGNAL(routesChanged()), this, SLOT(file_update()));

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
	uint routeNr;
	uint maxRouteNr;

	m_routeList.clear();
	TableWindow::setNumRows(0);

	if(m_pDb->open())
	{
		progDlg.beginProgress("read routes...", m_pDb);
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->routeList(m_routeList);
		progDlg.endProgress();
		maxRouteNr = m_routeList.size();
		TableWindow::setNumRows(maxRouteNr);

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

	route.setType(Route::Competition);
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
	Route *pLatest = NULL;
  Route::RouteListType::iterator it;
	WayPoint wp;
  int maxId = -1;

	route.setName("New route");
	route.setType(Route::Straight);

  for(it=m_routeList.begin(); it!=m_routeList.end(); it++)
  {
    if(maxId < (*it).id())
    {
      maxId = (*it).id();
      pLatest = &(*it);
    }
  }

	if((pLatest != NULL) && (pLatest->wayPointList().size() > 0))
	{
	  // take two points around start of first route as initial
    wp = pLatest->wayPointList().at(0);
    wp.setLongitude(wp.longitude() - 0.25);
    route.wayPointList().push_back(wp);
    wp = pLatest->wayPointList().at(0);
    wp.setLongitude(wp.longitude() + 0.25);
    route.wayPointList().push_back(wp);
	}
	else
	{
	  // default
    route.wayPointList().push_back(WayPoint(47.0, 8.5));
    route.wayPointList().push_back(WayPoint(47.0, 9.0));
	}

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
  AirSpaceList airSpaceList;

	row = getTable()->currentRow();

	if(row >= 0)
	{
		route = m_routeList[row];
    pView = new WebMapRouteView(tr("View Route"));
    pView->setRoute(&route);
    ISql::pInstance()->airspaceList(airSpaceList);
    pView->setAirSpaceList(&airSpaceList);
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
	QTableWidget *pTable = TableWindow::getTable();

	pTable->item(row, Name)->setText(route.name());
	pTable->item(row, Type)->setText(route.typeAsText());
}

void RouteWindow::newWebMap(Route &route)
{
	WebMapRouteView *pView;
	AirSpaceList airSpaceList;

	pView = new WebMapRouteView(tr("Add Route to DB"));
	pView->setRoute(&route);
  ISql::pInstance()->airspaceList(airSpaceList);
  pView->setAirSpaceList(&airSpaceList);
	pView->loadMap();

	if((pView->exec() == QDialog::Accepted) && m_pDb->open())
	{
		// save new route
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
    m_pDb->add(route.wayPointList());
		m_pDb->add(route);
		m_pDb->close();
    TableWindow::unsetCursor();
	}
}
