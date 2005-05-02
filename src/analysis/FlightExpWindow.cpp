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

#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qstringlist.h>
#include <qtable.h>
#include <qwidget.h>
#include "ContainerDef.h"
#include "FlightExpWindow.h"
#include "IDataBase.h"
#include "Images.h"
#include "ISql.h"

FlightExpWindow::FlightExpWindow(QWidget* parent, const char* name, int wflags)
	:TableWindow(parent, name, wflags)
{
	QStringList nameList;
	QTable *pTable = TableWindow::getTable();
	QPopupMenu *pMenu;

	pMenu = new QPopupMenu(this);
	menuBar()->insertItem("&File", pMenu);
	pMenu->insertItem("&Print...", this, SLOT(print()));
	TableWindow::setCaption("Flight experience");
	TableWindow::setIcon(Images::pInstance()->getImage("document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(QTable::SingleRow);
	m_pDb = ISql::pInstance();
	
	// header
	nameList += "Year";
	nameList += "Number of flights";
	nameList += "Airtime [h]";

	setupHeader(nameList);
	
	pTable->setColumnWidth(Year, 100);
	pTable->setColumnWidth(NrFlights, 120);
	pTable->setColumnWidth(Airtime, 100);

	m_lastModified = 0;
}

bool FlightExpWindow::periodicalUpdate()
{
	int lastModified;
	
	lastModified = m_pDb->flightsLastModified();
	
	if(m_lastModified < lastModified)
	{
		file_update();
		m_lastModified = lastModified;
	}
	
	return true;
}

void FlightExpWindow::file_update()
{
	FlightsPerYearListType fpyList; 
	QTable *pTable = TableWindow::getTable();
	QString str;
	uint yearNr;
	uint maxYearNr;
	uint flightsTotal = 0;
	uint airtimeTotal = 0;

	pTable->setNumRows(0);
	
	if(m_pDb->flightsPerYear(fpyList))
	{
		maxYearNr = fpyList.size();
		
		if(maxYearNr > 0)
		{
			// statistics
			for(yearNr=0; yearNr<maxYearNr; yearNr++)
			{
				pTable->insertRows(yearNr);
				str.sprintf("%i", fpyList[yearNr].year);
				pTable->setText(yearNr, Year, str);
				str.sprintf("%i", fpyList[yearNr].nFlights);
				pTable->setText(yearNr, NrFlights, str);
				str.sprintf("%.2f",  fpyList[yearNr].airTimeSecs/3600.0);
				pTable->setText(yearNr, Airtime, str);
				
				flightsTotal += fpyList[yearNr].nFlights;
				airtimeTotal += fpyList[yearNr].airTimeSecs;
			}
			
			// separator
			pTable->insertRows(yearNr);
			pTable->setText(yearNr, Year, "_______________________________");
			pTable->setText(yearNr, NrFlights, "_______________________________");
			pTable->setText(yearNr, Airtime, "_______________________________");
				
			// sum
			pTable->insertRows(yearNr+1);
			str.sprintf("%i", fpyList[maxYearNr-1].year - fpyList[0].year + 1);
			pTable->setText(yearNr+1, Year, str);
			str.sprintf("%i", flightsTotal);
			pTable->setText(yearNr+1, NrFlights, str);
			str.sprintf("%.2f",  airtimeTotal/3600.0);
			pTable->setText(yearNr+1, Airtime, str);
		}
	}
}
