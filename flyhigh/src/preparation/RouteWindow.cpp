/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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
#include <qheader.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qtable.h>
#include "IGPSDevice.h"
#include "Images.h"
#include "IRouteForm.h"
#include "ISql.h"
#include "Route.h"
#include "RouteWindow.h"
#include "WayPoint.h"

RouteWindow::RouteWindow(QWidget* parent, const char* name, int wflags, IDataBase::SourceType src)
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
			m_pDb = ISql::pInstance();
			caption = "Routes from DB";
			pMenu->insertItem("&New...", this, SLOT(file_new()));
			pMenu->insertItem("&Add to GPS...", this, SLOT(file_AddToGPS()));
		break;
		case IDataBase::GPSdevice:
			m_pDb = IGPSDevice::pInstance();
			caption = "Routes from GPS";
			pMenu->insertItem("&Add to DB...", this, SLOT(file_AddToSqlDB()));
		break;
	}
	
	pMenu->insertItem("&Delete", this, SLOT(file_delete()));
	pMenu->insertItem("&Update", this, SLOT(file_update()));
	
	TableWindow::setCaption(caption);
	TableWindow::setIcon(Images::pInstance()->getImage("document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(QTable::SingleRow);

	// header	
	nameList += "Name";
	setupHeader(nameList);
	
	pTable->setColumnWidth(Name, 140);
	
	m_lastModified = 0;
	startTimer(1);
}

void RouteWindow::timerEvent(QTimerEvent *pEvent)
{
	int lastModified;
	
	if(pEvent != NULL)
	{
		lastModified = m_pDb->routesLastModified();
		
		if(m_lastModified < lastModified)
		{
			file_update();
			m_lastModified = lastModified;
		}
	}
}

void RouteWindow::file_delete()
{
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		m_pDb->delRoute(getTable()->text(row, Name));
	}
}

void RouteWindow::file_update()
{
	Route::RouteListType routeList;
	Route route;
	QTable *pTable = TableWindow::getTable();
	uint routeNr;
	uint maxRouteNr;

	TableWindow::setCursor(QCursor(Qt::WaitCursor));
	
	pTable->setNumRows(0);
	m_pDb->routeList(routeList);
	maxRouteNr = routeList.size();
	
	for(routeNr=0; routeNr<maxRouteNr; routeNr++)
	{
		pTable->insertRows(routeNr);
		setRouteToRow(routeNr, routeList[routeNr]);
	}
	
	TableWindow::unsetCursor();
}

void RouteWindow::file_new()
{
	Route route;
	IRouteForm routeForm(this, tr("Add Route to DB"), &route);
	
	if(routeForm.exec())
	{
		ISql::pInstance()->add(route);
	}
}

void RouteWindow::file_AddToGPS()
{
	QString name;
	Route route;
	WayPoint wp;
	int row;
	uint wpNr;
	uint nofWp;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		name = getTable()->text(row, Name);
		
		if(ISql::pInstance()->route(name, route))
		{
			// make sure all WayPoints exist on GPS
			nofWp = route.wayPointList().size();
			
			for(wpNr=0; wpNr<nofWp; wpNr++)
			{
				name = *route.wayPointList().at(wpNr);
				ISql::pInstance()->wayPoint(name, wp);
				IGPSDevice::pInstance()->add(wp);
			}
			
			// Route
			IGPSDevice::pInstance()->add(route);
		}
	}
}

void RouteWindow::file_AddToSqlDB()
{
/*	QString str;
	WayPoint wp;
	double lon;
	double lat;
	int alt;
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
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
		
		if(wayPointForm.exec())
		{
			ISql::pInstance()->add(wp);
		}
	}
	*/
}

void RouteWindow::setRouteToRow(uint row, Route &route)
{
	QString str;
	QTable *pTable = TableWindow::getTable();
	QHeader *pHeader;
	uint maxWpNr;
	uint nCols;
	uint wpNr;
	
	pTable->setText(row, Name, route.name());
	maxWpNr = route.wayPointList().size();
	nCols = pTable->numCols();
	
	for(wpNr=0; wpNr<maxWpNr; wpNr++)
	{
		if(WayPoint1+wpNr >= nCols)
		{
			pTable->insertColumns(WayPoint1+wpNr);
			nCols++;
			pTable->setColumnWidth(WayPoint1+wpNr, 140);
			
			pHeader = pTable->horizontalHeader();
			str.sprintf("WayPoint %i", wpNr);
			pHeader->setLabel(WayPoint1+wpNr, str);
		}
	
		str = *(route.wayPointList().at(wpNr));
		pTable->setText(row, WayPoint1+wpNr, str);
	}
}
