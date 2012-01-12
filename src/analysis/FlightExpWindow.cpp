/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
#include <QStringList>
#include <QTableWidget>
#include <QWidget>
#include "ContainerDef.h"
#include "FlightExpWindow.h"
#include "IDataBase.h"
#include "IFlyHighRC.h"
#include "ISql.h"

FlightExpWindow::FlightExpWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags)
	:TableWindow(parent, name, wflags)
{
	QStringList nameList;
  QAction* pAction;
	QTableWidget *pTable;

  pTable = TableWindow::getTable();

  pAction = new QAction(tr("&Export all..."), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(exportTable()));
  MDIWindow::addAction(pAction);

  TableWindow::setWindowTitle(tr("Flight experience"));
  TableWindow::setWindowIcon(QIcon(":/document.xpm"));

	// configure the table
	pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	pTable->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pDb = ISql::pInstance();
	connect(m_pDb, SIGNAL(flightsChanged()), this, SLOT(file_update()));

	// header
	nameList += tr("Year");
	nameList += tr("Number of flights");
	nameList += tr("Airtime [h]");

	setupHeader(nameList);

	pTable->setColumnWidth(Year, 60);
	pTable->setColumnWidth(NrFlights, 140);
	pTable->setColumnWidth(Airtime, 100);

	file_update();
}

void FlightExpWindow::file_update()
{
	FlightsPerYearListType fpyList;
	Pilot pilot;
	QTableWidget *pTable = TableWindow::getTable();
	QString str;
	uint yearNr;
	uint maxYearNr;
	uint flightsTotal = 0;
	uint airtimeTotal = 0;

	TableWindow::setCursor(QCursor(Qt::WaitCursor));

	// pilot info
	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);

	if(m_pDb->flightsPerYear(pilot, fpyList))
	{
		maxYearNr = fpyList.size();
		TableWindow::setNumRows(maxYearNr + 2);

		if(maxYearNr > 0)
		{
			// statistics
			for(yearNr=0; yearNr<maxYearNr; yearNr++)
			{
				str.sprintf("%i", fpyList[yearNr].year);
				pTable->item(yearNr, Year)->setText(str);
				str.sprintf("%i", fpyList[yearNr].nFlights);
				pTable->item(yearNr, NrFlights)->setText(str);
				str.sprintf("%.2f",  fpyList[yearNr].airTimeSecs / 3600.0);
				pTable->item(yearNr, Airtime)->setText(str);

				flightsTotal += fpyList[yearNr].nFlights;
				airtimeTotal += fpyList[yearNr].airTimeSecs;
			}

			// separator
			pTable->item(yearNr, Year)->setText("________");
			pTable->item(yearNr, NrFlights)->setText("_____________________");
			pTable->item(yearNr, Airtime)->setText("_______________");

			// sum
			yearNr++;
			str.sprintf("%i", fpyList[maxYearNr-1].year - fpyList[0].year + 1);
			pTable->item(yearNr, Year)->setText(str);
			str.sprintf("%i", flightsTotal);
			pTable->item(yearNr, NrFlights)->setText(str);
			str.sprintf("%.2f",  airtimeTotal / 3600.0);
			pTable->item(yearNr, Airtime)->setText(str);
		}
	}

	TableWindow::unsetCursor();
}
