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

FlightExpWindow::FlightExpWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags)
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
	uint flightsTotal = 0;
	uint airtimeTotal = 0;
  uint fpyCount;
	uint fpyNr;
  uint itemNr;

	TableWindow::setCursor(QCursor(Qt::WaitCursor));

	// pilot info
	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);

	if(m_pDb->flightsPerYear(pilot, fpyList))
	{
		fpyCount = fpyList.size();
		TableWindow::setNumRows(fpyCount + 2);

		if(fpyCount > 0)
		{
			// statistics, newest first
			for(fpyNr=0; fpyNr<fpyCount; fpyNr++)
			{
			  itemNr = (fpyCount - fpyNr - 1);
				str.sprintf("%i", fpyList[fpyNr].year);
				pTable->item(itemNr, Year)->setText(str);
				str.sprintf("%i", fpyList[fpyNr].nFlights);
				pTable->item(itemNr, NrFlights)->setText(str);
				str.sprintf("%.2f",  fpyList[fpyNr].airTimeSecs / 3600.0);
				pTable->item(itemNr, Airtime)->setText(str);

				flightsTotal += fpyList[fpyNr].nFlights;
				airtimeTotal += fpyList[fpyNr].airTimeSecs;
			}

			// separator
			itemNr = fpyCount;
			pTable->item(itemNr, Year)->setText("________");
			pTable->item(itemNr, NrFlights)->setText("_____________________");
			pTable->item(itemNr, Airtime)->setText("_______________");

			// sum
			itemNr++;
			str.sprintf("%i", fpyList[fpyCount-1].year - fpyList[0].year + 1);
			pTable->item(itemNr, Year)->setText(str);
			str.sprintf("%i", flightsTotal);
			pTable->item(itemNr, NrFlights)->setText(str);
			str.sprintf("%.2f",  airtimeTotal / 3600.0);
			pTable->item(itemNr, Airtime)->setText(str);
		}
	}

	TableWindow::unsetCursor();
}
