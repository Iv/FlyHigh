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
#include <qstring.h>
#include <qstringlist.h>
#include <qtable.h>
#include <qdatetime.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include "IDataBase.h"
#include "Images.h"
#include "ISql.h"
#include "IGPSDevice.h"
#include "ProgressDlg.h"
#include "IWayPointForm.h"
#include "WayPointWindow.h"
#include "WayPoint.h"

WayPointWindow::WayPointWindow(QWidget* parent, const char* name, int wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags)
{
	QString caption;
	QStringList nameList;
	QTable *pTable = TableWindow::getTable();
	QPopupMenu *pMenu;

	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&File", pMenu);
	pMenu->insertItem("&Update", this, SLOT(file_update()));
	
	switch(src)
	{
		case IDataBase::SqlDB:
			m_pDb = ISql::pInstance();
			caption = "WayPoints from DB";
			pMenu->insertItem("&Add to GPS...", this, SLOT(file_AddToGps()));
		break;
		case IDataBase::GPSdevice:
			m_pDb = IGPSDevice::pInstance();
			caption = "WayPoints from GPS";
			pMenu->insertItem("&Add to DB...", this, SLOT(file_AddToSqlDB()));
		break;
		default:
			Q_ASSERT(false);
		break;
	}
	
	pMenu->insertItem("&Delete", this, SLOT(file_delete()));
	pMenu->insertItem("&Export all...", this, SLOT(exportTable()));
	
	TableWindow::setCaption(caption);
	TableWindow::setIcon(Images::pInstance()->getImage("document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(QTable::SingleRow);

	// header	
	nameList += "Name";
	nameList += "Longitude\n[°,min]";
	nameList += "Latitude\n[°,min]";
	nameList += "Altitude\n[m]";
	setupHeader(nameList);
	
	pTable->setColumnWidth(Name, 120);
	pTable->setColumnWidth(Longitude, 80);
	pTable->setColumnWidth(Latitude, 80);
	pTable->setColumnWidth(Altitude, 70);
	
	m_lastModified = 0;
}

bool WayPointWindow::periodicalUpdate()
{
	int lastModified;
	
	lastModified = m_pDb->wayPointsLastModified();
	
	if(m_lastModified < lastModified)
	{
		file_update();
		m_lastModified = lastModified;
	}
	
	return true;
}

void WayPointWindow::file_update()
{
	WayPoint::WayPointListType wpList;
	WayPoint wp;
	QTable *pTable = TableWindow::getTable();
	ProgressDlg progDlg(this);
	uint wpNr;
	uint maxWpNr;
	
	pTable->setNumRows(0); // clear table, because of different nr of waypoints
	progDlg.beginProgress("reading waypoints...", m_pDb);
	m_pDb->wayPointList(wpList);
	progDlg.endProgress();
	maxWpNr = wpList.size();
	pTable->setNumRows(maxWpNr);
	
	for(wpNr=0; wpNr<maxWpNr; wpNr++)
	{
		setWpToRow(wpNr, wpList[wpNr]);
	}
}

void WayPointWindow::file_AddToGps()
{
	WayPoint wp;
	
	if(setCurRowToWp(wp))
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		IGPSDevice::pInstance()->add(wp);
		TableWindow::unsetCursor();
	}
}

void WayPointWindow::file_delete()
{
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		TableWindow::setCursor(QCursor(Qt::WaitCursor));
		m_pDb->delWayPoint(getTable()->text(row, Name));
		TableWindow::unsetCursor();
	}
}

void WayPointWindow::file_AddToSqlDB()
{
	WayPoint wp;
	
	if(setCurRowToWp(wp))
	{
		IWayPointForm wayPointForm(this, tr("Add WayPoint to DB"), &wp);
		
		if(wayPointForm.exec())
		{
			TableWindow::setCursor(QCursor(Qt::WaitCursor));
			ISql::pInstance()->add(wp);
			TableWindow::unsetCursor();
		}
	}
}

void WayPointWindow::setWpToRow(uint row, WayPoint &wp)
{
	QString str;
	QTable *pTable = TableWindow::getTable();
	
	pTable->setText(row, Name, wp.name());
	
	str.sprintf("%.5f", wp.longitude());
	pTable->setText(row, Longitude, str);
	
	str.sprintf("%.5f", wp.latitude());
	pTable->setText(row, Latitude, str);
	
	str.sprintf("%i", wp.altitude());
	pTable->setText(row, Altitude, str);
}

bool WayPointWindow::setCurRowToWp(WayPoint &wp)
{
	QString str;
	double lon;
	double lat;
	int alt;
	int row;
	bool success;
	
	row = getTable()->currentRow();
	success = (row >= 0);
	
	if(success)
	{
		// Coordinates
		str = getTable()->text(row, Longitude);
		lon = str.toDouble();
		str = getTable()->text(row, Latitude);
		lat = str.toDouble();
		str = getTable()->text(row, Altitude);
		alt = str.toInt();
		wp.setCoordinates(lat, lon, alt);
		
		// Name
		str = getTable()->text(row, Name);
		wp.setName(str);
	}
	
	return success;
}
