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

#include <QFile>
#include <QFileDialog>
#include <QString>
#include <q3table.h>
#include <QDateTime>
#include <QMenuBar>
#include "IFlyHighRC.h"
#include "Error.h"
#include "FlightWindow.h"
#include "Glider.h"
#include "WayPoint.h"
#include "IGPSDevice.h"
#include "IGliderForm.h"
#include "IFlightForm.h"
#include "IFlyHighRC.h"
#include "IWayPointForm.h"
#include "ISql.h"
#include "Flight.h"
#include "IGCFileParser.h"
#include "KmlWriter.h"
#include "OLCOptimizer.h"
#include "OLCWebForm.h"
#include "ProgressDlg.h"
#include "MapView.h"

FlightWindow::FlightWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags, IDataBase::SourceType src)
	:TableWindow(parent, name, wflags)
{
	QString caption;
	QStringList nameList;
	Q3Table *pTable = TableWindow::getTable();

	QMenu* pFileMenu = menuBar()->addMenu(tr("&File"));

	QAction* pUpdateAct = new QAction(tr("&Update"), this);
	connect(pUpdateAct,SIGNAL(triggered()), this, SLOT(file_update()));
	pFileMenu->addAction(pUpdateAct);

	switch(src)
	{
	case IDataBase::SqlDB:
		{
			m_pDb = ISql::pInstance();
			caption = tr("Flights from DB");

			QAction* pNewAct = new QAction(tr("&New..."), this);
			connect(pNewAct,SIGNAL(triggered()), this, SLOT(file_new()));
			pFileMenu->addAction(pNewAct);
			QAction* pDelAct = new QAction(tr("&Delete"), this);
			connect(pDelAct,SIGNAL(triggered()), this, SLOT(file_delete()));
			pFileMenu->addAction(pDelAct);
			QAction* pImpAct = new QAction(tr("&Import..."), this);
			connect(pImpAct,SIGNAL(triggered()), this, SLOT(file_import()));
			pFileMenu->addAction(pImpAct);
		}
		break;
	case IDataBase::GPSdevice:
		{
			m_pDb = IGPSDevice::pInstance();
			caption = tr("Flights from GPS");

			QAction* pAddAct = new QAction(tr("&Add to DB..."), this);
			connect(pAddAct,SIGNAL(triggered()), this, SLOT(file_AddToSqlDB()));
			pFileMenu->addAction(pAddAct);
		}
		break;
	default:
		Q_ASSERT(false);
		break;
	}
	
	QAction* pExpIGCAct = new QAction(tr("&Export IGC..."), this);
	connect(pExpIGCAct,SIGNAL(triggered()), this, SLOT(file_exportIGC()));
	pFileMenu->addAction(pExpIGCAct);
	QAction* pExpKMLAct = new QAction(tr("&Export KML..."), this);
	connect(pExpKMLAct,SIGNAL(triggered()), this, SLOT(file_exportKML()));
	pFileMenu->addAction(pExpKMLAct);
	QAction* pExpAllAct = new QAction(tr("&Export all..."), this);
	connect(pExpAllAct,SIGNAL(triggered()), this, SLOT(exportTable()));
	pFileMenu->addAction(pExpAllAct);
	
/*	- ground speed / time
- vario / time
- altitude / time
- 3D plot of flight*/
	QMenu* pPlotMenu = menuBar()->addMenu(tr("&Plot"));

	QAction* pSpdTimeAct = new QAction("&Speed vs Time", this);
	connect(pSpdTimeAct,SIGNAL(triggered()), this, SLOT(plot_speedVsTime()));
	pPlotMenu->addAction(pSpdTimeAct);
	
	QAction* pAltTimeAct = new QAction("&Alt vs Time", this);
	connect(pAltTimeAct,SIGNAL(triggered()), this, SLOT(plot_altVsTime()));
	pPlotMenu->addAction(pAltTimeAct);
	QAction* pVarioTimeAct = new QAction("&Vario vs Time", this);
	connect(pVarioTimeAct,SIGNAL(triggered()), this, SLOT(plot_varioVsTime()));
	pPlotMenu->addAction(pVarioTimeAct);
	QAction* pOLCAct = new QAction("&OLC", this);
	connect(pOLCAct,SIGNAL(triggered()), this, SLOT(plot_OLC()));
	pPlotMenu->addAction(pOLCAct);
	QAction* pMapAct = new QAction("&Map View", this);
	connect(pMapAct,SIGNAL(triggered()), this, SLOT(showOnMap()));
	pPlotMenu->addAction(pMapAct);

	TableWindow::setWindowTitle(caption);
	TableWindow::setWindowIcon(QIcon(":/document.xpm"));
	
	// configure the table
	pTable->setReadOnly(true);
	pTable->setSelectionMode(Q3Table::SingleRow);
	
	// header
	nameList += "Nr";
	nameList += "Date\n[DD.MM.YYYY]";
	nameList += "Time\n[hh:mm:ss]";
	nameList += "Duration\n[hh:mm:ss]";
	nameList += "Glider";
	nameList += "Starting Point";
	nameList += "Landing Strip";
	nameList += "Distance\n[km]";
	nameList += "Comment";
	setupHeader(nameList);
	
	pTable->setColumnWidth(Nr, 50);
	pTable->setColumnWidth(Date, 115);
	pTable->setColumnWidth(Time, 95);
	pTable->setColumnWidth(Duration, 95);
	pTable->setColumnWidth(Model, 175);
	pTable->setColumnWidth(StartPt, 150);
	pTable->setColumnWidth(LandPt, 150);
	pTable->setColumnWidth(Distance, 70);
	pTable->setColumnWidth(Comment, 1000);
	
	m_fileName = "";
	m_lastModified = 0;
}

bool FlightWindow::periodicalUpdate()
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

void FlightWindow::file_update()
{
	Pilot pilot;
	Q3Table *pTable = TableWindow::getTable();
	ProgressDlg progDlg(this);
	uint flightNr;
	uint maxFlightNr;
	
	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);
	m_flightList.clear();
	progDlg.beginProgress(tr("reading flights..."), m_pDb);
	
	if(m_pDb->flightList(pilot, m_flightList))
	{
		maxFlightNr = m_flightList.size();
		pTable->setNumRows(maxFlightNr);
		
		for(flightNr=0; flightNr<maxFlightNr; flightNr++)
		{
			setFlightToRow(flightNr, m_flightList[flightNr]);
		}
	}
	
	progDlg.endProgress();
}

void FlightWindow::setFlightToRow(uint row, Flight &flight)
{
	Glider glider;
	WayPoint wp;
	QString str;
	QTime duration;
	Q3Table *pTable = TableWindow::getTable();
	
	str.sprintf("%i",flight.number());
	pTable->setText(row, Nr, str);
	pTable->setText(row, Date, flight.date().toString("dd.MM.yyyy"));
	pTable->setText(row, Time, flight.time().toString(Qt::ISODate));
	
	duration.setHMS(0, 0, 0);
	pTable->setText(row, Duration, duration.addSecs(flight.duration()).toString(Qt::ISODate));

	flight.glider().fullName(str);
	pTable->setText(row, Model, str);
	flight.startPt().fullName(str);
	pTable->setText(row, StartPt, str);
	flight.landPt().fullName(str);
	pTable->setText(row, LandPt, str);
	
	str.sprintf("%.3f", flight.distance()/1000.0);
	pTable->setText(row, Distance, str);
	
	pTable->setText(row, Comment, flight.comment());
}

void FlightWindow::file_AddToSqlDB()
{
	IFlightForm newFlightForm(this, tr("New Flight"));
	ProgressDlg progDlg(this);
	IGCFileParser igcParser;
	OLCOptimizer olcOptimizer;
	QTime time;
	QDate date;
	Flight flight;
	Glider glider;
	Pilot pilot;
	WayPoint wp;
	OLCOptimizer::FlightPointIndexListType fpIndexList;
	QString str;
	int row;
	uint gpsFlightNr;
	uint newFlightNr;
	int id;
	bool success;

	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		// pilot info
		ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);
	
		// load igc data
		progDlg.beginProgress(tr("read igc file..."), m_pDb);
		success = m_pDb->loadIGCFile(m_flightList[row]);
		progDlg.endProgress();
	
		if(success)
		{
			// parse and optimize
			igcParser.parse(m_flightList[row].igcData());
			olcOptimizer.setFlightPoints(igcParser.flightPointList(), 100, 200);
			progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);
			
			if(olcOptimizer.optimize())
			{
				// distance
				m_flightList[row].setDistance(olcOptimizer.freeDistance(fpIndexList));
			}
			
			progDlg.endProgress();
			
			// nr
			gpsFlightNr = getTable()->text(row, Nr).toUInt(); // store this for restore
			newFlightNr = ISql::pInstance()->newFlightNr(pilot);
			m_flightList[row].setNumber(newFlightNr);
			
			// start
			id = igcParser.flightPointList().firstValidFlightData();
			
			if(id >= 0)
			{
				// start time
				time = igcParser.flightPointList().at(id).time.addSecs(IFlyHighRC::pInstance()->utcOffset() * 3600);
				m_flightList[row].setTime(time);
			
				// start place
				wp = igcParser.flightPointList().at(id).wp;
				
				if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
				{
					IWayPointForm newWayPoint(this, tr("New WayPoint Starting Place"), &wp);
		
					if(newWayPoint.exec())
					{
						ISql::pInstance()->add(wp);
					}
				}
				m_flightList[row].setStartPt(wp);
			}
			
			// landing strip
			id = igcParser.flightPointList().lastValidFlightData();
			
			if(id >= 0)
			{
				wp = igcParser.flightPointList().at(id).wp;
				
				if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
				{
					IWayPointForm newWayPoint(this, tr("New WayPoint Landing Strip"), &wp);
		
					if(newWayPoint.exec())
					{
						ISql::pInstance()->add(wp);
					}
				}
				m_flightList[row].setLandPt(wp);
			}
	
			// glider
			if(!ISql::pInstance()->glider(igcParser.model(), glider))
			{
				IGliderForm newGlider(this, tr("New Glider"), &glider);
				
				if(newGlider.exec())
				{
					ISql::pInstance()->add(glider);
				}
			}
	
			m_flightList[row].setGlider(glider);
			
			// a new flight
			newFlightForm.setFlight(&m_flightList[row]);
			
			if(newFlightForm.exec())
			{
				ISql::pInstance()->add(m_flightList[row]);
				setFlightToRow(row, m_flightList[row]);
			}

			m_flightList[row].setNumber(gpsFlightNr); // restore original track nr or gps will be confused
		}
	}
}

void FlightWindow::file_import()
{
	IFlightForm newFlightForm(this, tr("New Flight"));
	ProgressDlg progDlg(this);
	IGCFileParser igcParser;
	OLCOptimizer olcOptimizer;
	QTime time;
	Flight flight;
	Glider glider;
	Pilot pilot;
	WayPoint wp;
	OLCOptimizer::FlightPointIndexListType fpIndexList;
	QString selected;
	QFile file;
	int nr;
	int startPtId;
	int landPtId;
	int duration;

	selected = QFileDialog::getOpenFileName(this,
																					tr("IGC file import"),
																					IFlyHighRC::pInstance()->lastDir(),
																					tr("IGC Files (*.igc; *.IGC)"));
	file.setFileName(selected);

	if(selected!="" && file.exists())
	{
		// pilot info
		ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);

		IFlyHighRC::pInstance()->setLastDir(QFileInfo(selected).absoluteDir().absolutePath());

		if(file.open(QIODevice::ReadOnly))
		{
			flight.setIgcData(file.readAll());
			file.close();
		
			// parse and optimize
			igcParser.parse(flight.igcData());
			olcOptimizer.setFlightPoints(igcParser.flightPointList(), 100, 200);
			progDlg.beginProgress("optimize flight...", &olcOptimizer);
			
			if(olcOptimizer.optimize())
			{
				// distance
				flight.setDistance(olcOptimizer.freeDistance(fpIndexList));
			}
			
			progDlg.endProgress();
	
			// nr
			nr = ISql::pInstance()->newFlightNr(pilot);
			flight.setNumber(nr);
			
			// date
			flight.setDate(igcParser.date());
		
			// start
			startPtId = igcParser.flightPointList().firstValidFlightData();
			
			if(startPtId >= 0)
			{
				// add UTC offset to start time
				time = igcParser.flightPointList().at(startPtId).time.addSecs(IFlyHighRC::pInstance()->utcOffset() * 3600);
				flight.setTime(time);
				
				// start place
				wp = igcParser.flightPointList().at(startPtId).wp;
				
				if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
				{
					IWayPointForm newWayPoint(this, tr("New WayPoint Starting Place"), &wp);
		
					if(newWayPoint.exec())
					{
						ISql::pInstance()->add(wp);
					}
				}
				flight.setStartPt(wp);
			}
			
			// landing strip
			landPtId = igcParser.flightPointList().lastValidFlightData();
			
			if(landPtId >= 0)
			{
				wp = igcParser.flightPointList().at(landPtId).wp;
				
				if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
				{
					IWayPointForm newWayPoint(this, tr("New WayPoint Landing Strip"), &wp);
		
					if(newWayPoint.exec())
					{
						ISql::pInstance()->add(wp);
					}
				}
				flight.setLandPt(wp);
			}
			
			// duration
			time = igcParser.flightPointList().at(startPtId).time;
			duration = time.secsTo(igcParser.flightPointList().at(landPtId).time);
			flight.setDuration(duration);
		
			// glider
			if(!ISql::pInstance()->glider(igcParser.model(), glider))
			{
				IGliderForm newGlider(this, tr("New Glider"), &glider);
				
				if(newGlider.exec())
				{
					ISql::pInstance()->add(glider);
				}
			}
			flight.setGlider(glider);
		
			// a new flight
			newFlightForm.setFlight(&flight);
			
			if(newFlightForm.exec())
			{
				ISql::pInstance()->add(flight);
			}
		}
	}
}

void FlightWindow::file_exportIGC()
{
	QFile file;
	QString olcFileName;
	QString selected;
	IGCFileParser igcParser;
	OLCOptimizer olcOptimizer;
	OLCOptimizer::FlightPointIndexListType fpIndexListFree;
	OLCOptimizer::FlightPointIndexListType fpIndexListFAI;
	OLCOptimizer::FlightPointIndexListType fpIndexListFlat;
	OLCWebForm olcWebForm;
	ProgressDlg progDlg(this);
	double distFree;
	double distFAI;
	double distFlat;
	double ptsFree;
	double ptsFAI;
	double ptsFlat;
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		if(m_pDb->loadIGCFile(m_flightList[row]))
		{
			// IGC file
			igcParser.parse(m_flightList[row].igcData());

			// fetch olc filename
			olcWebForm.setFlight(m_flightList[row]);
			olcWebForm.olcFileName(olcFileName);

			selected = QFileDialog::getSaveFileName(this,
																							tr("IGC file export"),
																							IFlyHighRC::pInstance()->lastDir() + QDir::separator()
																																								 + olcFileName
																																								 + ".igc",
																							"IGC Files (*.igc)");

			if(selected!="")
			{
				IFlyHighRC::pInstance()->setLastDir(QFileInfo(selected).absoluteDir().absolutePath());
				
				file.setFileName(selected);
				
				// assemble name for olc webform
				// use the same basename as the user has chosen
				if(selected.endsWith(".igc",Qt::CaseInsensitive))
				{
					olcFileName = selected.replace(".igc", ".html", Qt::CaseInsensitive);
				}
				else
				{
					// just append '.html'
					olcFileName = selected + ".html";
				}

				if(file.open(QIODevice::WriteOnly))
				{
					file.write(m_flightList[row].igcData());
					file.close();
				}
				
				// OLC file
				olcOptimizer.setFlightPoints(igcParser.flightPointList(), 100, 200);
				progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);
				
				if(olcOptimizer.optimize())
				{
					distFree = olcOptimizer.freeDistance(fpIndexListFree) / 1000.0;
					ptsFree = distFree * 1.5;
					distFAI = olcOptimizer.FAITriangle(fpIndexListFAI) / 1000.0;
					ptsFAI = distFAI  * 2.0;
					distFlat = olcOptimizer.flatTriangle(fpIndexListFlat) / 1000.0;
					ptsFlat = distFlat * 1.75;
					
					if((ptsFree > ptsFAI) && (ptsFree > ptsFlat))
					{
						olcWebForm.setDeparture(olcOptimizer.flyPointList().at(fpIndexListFree[0]));
						olcWebForm.set1stWayPoint(olcOptimizer.flyPointList().at(fpIndexListFree[1]).wp);
						olcWebForm.set2ndWayPoint(olcOptimizer.flyPointList().at(fpIndexListFree[2]).wp);
						olcWebForm.set3rdWayPoint(olcOptimizer.flyPointList().at(fpIndexListFree[3]).wp);
						olcWebForm.setFinish(olcOptimizer.flyPointList().at(fpIndexListFree[4]));
					}
					else if(ptsFlat > ptsFAI)
					{
						olcWebForm.setDeparture(olcOptimizer.flyPointList().at(fpIndexListFlat[0]));
						olcWebForm.set1stWayPoint(olcOptimizer.flyPointList().at(fpIndexListFlat[1]).wp);
						olcWebForm.set2ndWayPoint(olcOptimizer.flyPointList().at(fpIndexListFlat[2]).wp);
						olcWebForm.set3rdWayPoint(olcOptimizer.flyPointList().at(fpIndexListFlat[3]).wp);
						olcWebForm.setFinish(olcOptimizer.flyPointList().at(fpIndexListFlat[4]));
					}
					else
					{
						olcWebForm.setDeparture(olcOptimizer.flyPointList().at(fpIndexListFAI[0]));
						olcWebForm.set1stWayPoint(olcOptimizer.flyPointList().at(fpIndexListFAI[1]).wp);
						olcWebForm.set2ndWayPoint(olcOptimizer.flyPointList().at(fpIndexListFAI[2]).wp);
						olcWebForm.set3rdWayPoint(olcOptimizer.flyPointList().at(fpIndexListFAI[3]).wp);
						olcWebForm.setFinish(olcOptimizer.flyPointList().at(fpIndexListFAI[4]));
					}
					
					olcWebForm.save(olcFileName);
				}
				
				progDlg.endProgress();
			}
		}
	}
}

void FlightWindow::file_exportKML()
{
	QString fileName;
	QString selected;
	IGCFileParser igcParser;
	OLCOptimizer olcOptimizer;
	OLCOptimizer::FlightPointIndexListType fpIndexListFree;
	OLCOptimizer::FlightPointIndexListType fpIndexListFAI;
	OLCOptimizer::FlightPointIndexListType fpIndexListFlat;
	KmlWriter kmlWriter;
	
	ProgressDlg progDlg(this);
	double distFree;
	double distFAI;
	double distFlat;
	double ptsFree;
	double ptsFAI;
	double ptsFlat;
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		if(m_pDb->loadIGCFile(m_flightList[row]))
		{
			// IGC file
			igcParser.parse(m_flightList[row].igcData());

			// assemble suggestion
			fileName = m_flightList[row].startPt().name();
			fileName += m_flightList[row].date().toString("_dd_MM_yyyy");

			selected = QFileDialog::getSaveFileName(this,
																							tr("KML file export"),
																							IFlyHighRC::pInstance()->lastDir() + QDir::separator() +
																																								 fileName +
																																								 ".kml",
																							"KML Files (*.kml)");

			if(selected!="")
			{
				IFlyHighRC::pInstance()->setLastDir(QFileInfo(selected).absoluteDir().absolutePath());

				// OLC file
				olcOptimizer.setFlightPoints(igcParser.flightPointList(), 100, 200);
				progDlg.beginProgress("optimize flight...", &olcOptimizer);
				
				if(olcOptimizer.optimize())
				{
					distFree = olcOptimizer.freeDistance(fpIndexListFree) / 1000.0;
					ptsFree = distFree * 1.5;
					distFAI = olcOptimizer.FAITriangle(fpIndexListFAI) / 1000.0;
					ptsFAI = distFAI  * 2.0;
					distFlat = olcOptimizer.flatTriangle(fpIndexListFlat) / 1000.0;
					ptsFlat = distFlat * 1.75;
					
					if((ptsFree > ptsFAI) && (ptsFree > ptsFlat))
					{
						kmlWriter.setDeparture(olcOptimizer.flyPointList().at(fpIndexListFree[0]));
						kmlWriter.set1stWayPoint(olcOptimizer.flyPointList().at(fpIndexListFree[1]).wp);
						kmlWriter.set2ndWayPoint(olcOptimizer.flyPointList().at(fpIndexListFree[2]).wp);
						kmlWriter.set3rdWayPoint(olcOptimizer.flyPointList().at(fpIndexListFree[3]).wp);
						kmlWriter.setFinish(olcOptimizer.flyPointList().at(fpIndexListFree[4]));
						kmlWriter.setTriangle(false);
					}
					else if(ptsFlat > ptsFAI)
					{
						kmlWriter.setDeparture(olcOptimizer.flyPointList().at(fpIndexListFlat[0]));
						kmlWriter.set1stWayPoint(olcOptimizer.flyPointList().at(fpIndexListFlat[1]).wp);
						kmlWriter.set2ndWayPoint(olcOptimizer.flyPointList().at(fpIndexListFlat[2]).wp);
						kmlWriter.set3rdWayPoint(olcOptimizer.flyPointList().at(fpIndexListFlat[3]).wp);
						kmlWriter.setFinish(olcOptimizer.flyPointList().at(fpIndexListFlat[4]));
						kmlWriter.setTriangle(true);
					}
					else
					{
						kmlWriter.setDeparture(olcOptimizer.flyPointList().at(fpIndexListFAI[0]));
						kmlWriter.set1stWayPoint(olcOptimizer.flyPointList().at(fpIndexListFAI[1]).wp);
						kmlWriter.set2ndWayPoint(olcOptimizer.flyPointList().at(fpIndexListFAI[2]).wp);
						kmlWriter.set3rdWayPoint(olcOptimizer.flyPointList().at(fpIndexListFAI[3]).wp);
						kmlWriter.setFinish(olcOptimizer.flyPointList().at(fpIndexListFAI[4]));
						kmlWriter.setTriangle(true);
					}

					kmlWriter.setFlight(m_flightList[row]);
					kmlWriter.setFlightPoints(igcParser.flightPointList());
					kmlWriter.save(selected);
				}
				
				progDlg.endProgress();
			}
		}
	}
}

void FlightWindow::file_new()
{
	IFlightForm newFlightForm(this, tr("New Flight"));
	Pilot pilot;
	Flight flight;
	int nr;

	// pilot info
	ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);
	
	// nr
	nr = ISql::pInstance()->newFlightNr(pilot);
	flight.setNumber(nr);
	
	// current date and time
	flight.setDate(QDate::currentDate());
	flight.setTime(QTime::currentTime());
	flight.setDuration(QTime(0, 0, 0));

	// current glider
	flight.setGlider(pilot.glider());
	
	// a new flight
	newFlightForm.setFlight(&flight);
	
	if(newFlightForm.exec())
	{
		ISql::pInstance()->add(flight);
	}
}

void FlightWindow::file_delete()
{
	int row;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		m_pDb->delFlight(m_flightList[row]);
	}
}

void FlightWindow::plot_speedVsTime()
{
	QTime time;
	IGCFileParser igcParser;
	FlightPointList fpList;
	ProgressDlg progDlg(this);
	GnuPlot::TimeVectorType x;
	GnuPlot::PlotVectorType y;
	uint fpNr;
	uint fpListSize;
	int row;
	bool success;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		progDlg.beginProgress(tr("read igc file..."), m_pDb);
		success = m_pDb->loadIGCFile(m_flightList[row]);
		progDlg.endProgress();
	
		if(success)
		{
			igcParser.parse(m_flightList[row].igcData());
			fpList = igcParser.flightPointList();
			fpListSize = fpList.size();
			
			if(fpListSize > 0)
			{
				for(fpNr=0; fpNr<fpListSize; fpNr++)
				{
					x.push_back(fpList[fpNr].time);
					y.push_back(fpList.speedH(fpNr, fpNr+1)*3.6); // in km/h
				}
			
				m_plotter.clear();
				m_plotter.setLabelX("time [s]");
				m_plotter.setLabelY("speed [km/h]");
				m_plotter.plotXY(x, y, "Speed vs Time");
			}
		}
		
		TableWindow::unsetCursor();
	}
}

void FlightWindow::plot_altVsTime()
{
	QTime time;
	IGCFileParser igcParser;
	FlightPointList fpList;
	ProgressDlg progDlg(this);
	GnuPlot::TimeVectorType x;
	GnuPlot::PlotVectorType y;
	uint fpNr;
	uint fpListSize;
	int row;
	bool success;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		progDlg.beginProgress(tr("read igc file..."), m_pDb);
		success = m_pDb->loadIGCFile(m_flightList[row]);
		progDlg.endProgress();
		
		if(success)
		{
			igcParser.parse(m_flightList[row].igcData());
			fpList = igcParser.flightPointList();
			fpListSize = fpList.size();
			
			if(fpListSize > 0)
			{
				for(fpNr=0; fpNr<fpListSize; fpNr++)
				{
					x.push_back(fpList[fpNr].time);
					y.push_back(fpList[fpNr].wp.altitude()); // in m
				}
			
				m_plotter.clear();
				m_plotter.setLabelX("time [s]");
				m_plotter.setLabelY("altitude [m]");
				m_plotter.plotXY(x, y, "Altitude vs Time");
			}
		}
		
		TableWindow::unsetCursor();
	}
}

void FlightWindow::plot_varioVsTime()
{
	QTime time;
	IGCFileParser igcParser;
	FlightPointList fpList;
	GnuPlot::TimeVectorType x;
	GnuPlot::PlotVectorType y;
	ProgressDlg progDlg(this);
	uint fpNr;
	uint fpListSize;
	int row;
	bool success;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		progDlg.beginProgress(tr("read igc file..."), m_pDb);
		success = m_pDb->loadIGCFile(m_flightList[row]);
		progDlg.endProgress();
		
		if(success)
		{
			igcParser.parse(m_flightList[row].igcData());
			fpList = igcParser.flightPointList();
			fpListSize = fpList.size();
			
			if(fpListSize > 0)
			{
				for(fpNr=0; fpNr<fpListSize; fpNr++)
				{
					x.push_back(fpList[fpNr].time);
					y.push_back(fpList.speedV(fpNr, fpNr+1)); // in m/s
				}
			
				m_plotter.clear();
				m_plotter.setLabelX("time [s]");
				m_plotter.setLabelY("vario [m/s]");
				m_plotter.plotXY(x, y, "Vario vs Time");
			}
		}
		
		TableWindow::unsetCursor();
	}
}

void FlightWindow::plot_OLC()
{
	QString title;
	IGCFileParser igcParser;
	OLCOptimizer olcOptimizer;
	OLCOptimizer::FlightPointIndexListType fpIndexList;
	FlightPointList fpList;
	ProgressDlg progDlg(this);
	uint fpNr;
	uint dist;
	int row;
	bool success;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		progDlg.beginProgress(tr("read igc file..."), m_pDb);
		success = m_pDb->loadIGCFile(m_flightList[row]);
		progDlg.endProgress();
		
		if(success)
		{
			igcParser.parse(m_flightList[row].igcData());
	
			if(igcParser.flightPointList().size() > 0)
			{
				m_plotter.clear();
				m_plotter.setLabelX("latitude [deg.min]");
				m_plotter.setLabelY("longitude [deg.min]");
				m_plotter.setLabelZ("altitude [m]");
				plotFlighPointList(igcParser.flightPointList(), "track");
				
				olcOptimizer.setFlightPoints(igcParser.flightPointList(), 100, 200);
				progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);
				
				if(olcOptimizer.optimize())
				{
					// fai triangle
					dist = olcOptimizer.FAITriangle(fpIndexList);
					fpList.clear();
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[1]));
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[2]));
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[3]));
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[1]));
					title.sprintf("fai triangle: %.3f km (%.2f pts)", dist/1000.0, dist/1000.0*1.4);
					plotFlighPointList(fpList, title);
					
					// flat triangle
					dist = olcOptimizer.flatTriangle(fpIndexList);
					fpList.clear();
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[1]));
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[2]));
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[3]));
					fpList.add(olcOptimizer.flyPointList().at(fpIndexList[1]));
					title.sprintf("flat triangle: %.3f km (%.2f pts)", dist/1000.0, dist/1000.0*1.2);
					plotFlighPointList(fpList, title);
					
					// free distance
					dist = olcOptimizer.freeDistance(fpIndexList);
					fpList.clear();
					
					for(fpNr=0; fpNr<FLIGHT_POINT_INDEX_LIST_SIZE; fpNr++)
					{
						fpList.add(olcOptimizer.flyPointList().at(fpIndexList[fpNr]));
					}
					
					title.sprintf("free distance: %.3f km (%.2f pts)", dist/1000.0, dist/1000.0*1.0);
					plotFlighPointList(fpList, title);
				}

				progDlg.endProgress();
			}
		}
		
		TableWindow::unsetCursor();
	}
}

void FlightWindow::plotFlighPointList(FlightPointList &fpList, const QString& title)
{
	GnuPlot::PlotVectorType x;
	GnuPlot::PlotVectorType y;
	GnuPlot::PlotVectorType z;
	uint fpListSize;
	uint fpNr;
	
	fpListSize = fpList.size();
	
	for(fpNr=0; fpNr<fpListSize; fpNr++)
	{
		y.push_back(fpList[fpNr].wp.latitude());
		x.push_back(fpList[fpNr].wp.longitude());
		z.push_back(fpList[fpNr].wp.altitude());
	}
	
	m_plotter.plotXYZ(x, y, z, title);
}

void FlightWindow::showOnMap()
{
	QString title;
	IGCFileParser igcParser;
	ProgressDlg progDlg(this);
	int row;
	bool success;
	MapView *pView;
	WayPoint::WayPointListType wpList;
	uint fpListSize;
	uint fpNr;
	
	row = getTable()->currentRow();
	
	if(row >= 0)
	{
		progDlg.beginProgress(tr("read igc file..."), m_pDb);
		success = m_pDb->loadIGCFile(m_flightList[row]);
		progDlg.endProgress();
		
		if(success)
		{
			igcParser.parse(m_flightList[row].igcData());
			fpListSize = igcParser.flightPointList().size();

			if(fpListSize > 0)
			{
				for(fpNr=0; fpNr<fpListSize; fpNr++)
				{
					wpList.push_back(igcParser.flightPointList().at(fpNr).wp);
				}

				pView = new MapView();
				pView->showWayPointList(wpList);
				pView->showMaximized();
			}
		}
	}
}
