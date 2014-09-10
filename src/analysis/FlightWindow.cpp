/***************************************************************************
* Copyright (C) 2004 by Alex Graf                                          *
* grafal@sourceforge.net                                                   *
*                                                                          *
* This program is free software; you can redistribute it and/or modify     *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the                            *
* Free Software Foundation, Inc.,                                          *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
***************************************************************************/

#include <QAction>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTableWidget>
#include "AirSpaceList.h"
#include "IFlyHighRC.h"
#include "Elevation.h"
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
#include "PhotoView.h"
#include "ProgressDlg.h"
#include "MapView.h"
#include "qexifimageheader.h"
#include "WebMapFlightView.h"

FlightWindow::FlightWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags, IDataBase::SourceType src)
:TableWindow(parent, name, wflags)
{
  QStringList nameList;
  QTableWidget *pTable;
  QAction *pAction;

  pTable = TableWindow::getTable();

    // menu
  switch(src)
  {
    case IDataBase::SqlDB:
    {
      m_pDb = ISql::pInstance();

      pAction = new QAction(tr("&New..."), this);
      connect(pAction, SIGNAL(triggered()), this, SLOT(file_new()));
      MDIWindow::addAction(pAction);

      pAction = new QAction(tr("&Edit..."), this);
      connect(pAction, SIGNAL(triggered()), this, SLOT(file_edit()));
      MDIWindow::addAction(pAction, true);

      pAction = new QAction(tr("&Delete"), this);
      connect(pAction, SIGNAL(triggered()), this, SLOT(file_delete()));
      MDIWindow::addAction(pAction);
    }
    break;
    case IDataBase::GPSdevice:
    {
      m_pDb = IGPSDevice::pInstance();

      pAction = new QAction(tr("&Add to DB..."), this);
      connect(pAction, SIGNAL(triggered()), this, SLOT(file_AddToSqlDB()));
      MDIWindow::addAction(pAction, true);
    }
    break;
    default:
      Q_ASSERT(false);
    break;
  }

  pAction = new QAction(tr("&Update"), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(file_update()));
  MDIWindow::addAction(pAction);

    // import/export
  pAction = new QAction(this);
  pAction->setSeparator(true);
  MDIWindow::addAction(pAction, true);

  if(src == IDataBase::SqlDB)
  {
    pAction = new QAction(tr("&Import..."), this);
    connect(pAction, SIGNAL(triggered()), this, SLOT(file_import()));
    MDIWindow::addAction(pAction);
  }

  pAction = new QAction(tr("&Export IGC..."), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(file_exportIGC()));
  MDIWindow::addAction(pAction, true);

  pAction = new QAction(tr("&Export KML..."), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(file_exportKML()));
  MDIWindow::addAction(pAction);

  pAction = new QAction(tr("&Export all..."), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(exportTable()));
  MDIWindow::addAction(pAction);

  /* - ground speed / time
  - vario / time
  - altitude / time
  - 3D plot of flight*/
  pAction = new QAction(this);
  pAction->setSeparator(true);
  MDIWindow::addAction(pAction, true);

  pAction = new QAction(tr("&Plot Air Data"), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(plot_airData()));
  MDIWindow::addAction(pAction);

  pAction = new QAction(tr("&OLC"), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(plot_OLC()));
  MDIWindow::addAction(pAction);

  pAction = new QAction(tr("&View Map"), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(showOnMap()));
  MDIWindow::addAction(pAction);

  pAction = new QAction(tr("View &Web Map"), this);
  connect(pAction, SIGNAL(triggered()), this, SLOT(showOnWebMap()));
  MDIWindow::addAction(pAction, true);

  if(src == IDataBase::SqlDB)
  {
    pAction = new QAction(tr("View &Photos"), this);
    connect(pAction, SIGNAL(triggered()), this, SLOT(showPhotos()));
    MDIWindow::addAction(pAction, true);
  }

  // configure the table
  TableWindow::setWindowIcon(QIcon(":/document.xpm"));
  pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  pTable->setSelectionMode(QAbstractItemView::SingleSelection);

  // header
  nameList += tr("Id");
  nameList += tr("Nr");
  nameList += tr("");
  nameList += tr("Date\n[DD.MM.YYYY]");
  nameList += tr("Time\n[hh:mm:ss]");
  nameList += tr("Duration\n[hh:mm:ss]");
  nameList += tr("Glider");
  nameList += tr("Starting Point");
  nameList += tr("Landing Strip");
  nameList += tr("Distance\n[km]");
  nameList += tr("Comment");
  setupHeader(nameList);

  pTable->setColumnHidden(Id, true);
  pTable->setColumnWidth(Nr, 50);
  pTable->setColumnWidth(Icon, 22);
  pTable->setColumnWidth(Date, 115);
  pTable->setColumnWidth(Time, 95);
  pTable->setColumnWidth(Duration, 95);
  pTable->setColumnWidth(Model, 175);
  pTable->setColumnWidth(StartPt, 150);
  pTable->setColumnWidth(LandPt, 150);
  pTable->setColumnWidth(Distance, 70);
  pTable->setColumnWidth(Comment, 1000);
  pTable->sortByColumn(Nr, Qt::DescendingOrder);
  pTable->setSortingEnabled(true);

  connect(m_pDb, SIGNAL(flightsChanged()), this, SLOT(file_update()));
  connect(m_pDb, SIGNAL(wayPointsChanged()), this, SLOT(file_update()));

  m_fileName = "";
  file_update();

  m_plotter.execCmd("set terminal qt size 800,600");
}

void FlightWindow::file_update()
{
  QTableWidget *pTable;
  Pilot pilot;
  ProgressDlg progDlg(this);
  uint flightNr;
  uint maxFlightNr;

  pTable = TableWindow::getTable();
  ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);

  // prevent sorting on modification
  pTable->setSortingEnabled(false);
  m_flightList.clear();
  TableWindow::setNumRows(0);

  if(m_pDb->open())
  {
    progDlg.beginProgress(tr("reading flights..."), m_pDb);

    if(m_pDb->flightList(pilot, m_flightList))
    {
      maxFlightNr = m_flightList.size();
      TableWindow::setNumRows(maxFlightNr);

      for(flightNr=0; flightNr<maxFlightNr; flightNr++)
      {
        setFlightToRow(flightNr, m_flightList[flightNr]);
      }
    }

    progDlg.endProgress();
    m_pDb->close();
  }

  pTable->setSortingEnabled(true);
}

void FlightWindow::setFlightToRow(uint row, Flight &flight)
{
  Glider glider;
  WayPoint wp;
  QString str;
  QTime duration;
  QTableWidget *pTable;
  QIcon icon(":/camera.png");

  pTable = TableWindow::getTable();
  pTable->item(row, Id)->setData(Qt::DisplayRole, flight.id());
  pTable->item(row, Nr)->setData(Qt::DisplayRole, flight.number());
  pTable->item(row, Date)->setText(flight.date().toString("dd.MM.yyyy"));
  pTable->item(row, Time)->setText(flight.time().toString(Qt::ISODate));

  if(flight.photoPath() != "")
  {
    pTable->item(row, Icon)->setIcon(icon);
  }

  duration.setHMS(0, 0, 0);
  pTable->item(row, Duration)->setText(duration.addSecs(flight.duration()).toString(Qt::ISODate));
  flight.glider().fullName(str);
  pTable->item(row, Model)->setText(str);
  flight.startPt().fullName(str);
  pTable->item(row, StartPt)->setText(str);
  flight.landPt().fullName(str);
  pTable->item(row, LandPt)->setText(str);
  str = QString("%1").arg(flight.distance() / 1000.0, 0, 'f', 3);
  pTable->item(row, Distance)->setText(str);
  pTable->item(row, Comment)->setText(flight.comment());
}

void FlightWindow::file_AddToSqlDB()
{
  IFlightForm newFlightForm(this, tr("New Flight"));
  ProgressDlg progDlg(this);
  IGCFileParser igcParser;
  OLCOptimizer olcOptimizer;
  Glider::GliderListType gliders;
  FlightPointList simpleFpList;
  QTime time;
  Glider glider;
  Pilot pilot;
  WayPoint wp;
  OLCOptimizer::FlightPointIndexListType fpIndexList;
  int row;
  uint gpsFlightNr;
  uint newFlightNr;
  int id;
  bool success = false;

  row = getCurrentFlightIndex();

  if(row >= 0)
  {
    // pilot info
    ISql::pInstance()->pilot(IFlyHighRC::pInstance()->pilotId(), pilot);

    // load igc data
    if(m_pDb->open())
    {
      progDlg.beginProgress(tr("read igc file..."), m_pDb);
      success = m_pDb->loadIGCFile(m_flightList[row]);
      m_pDb->close();
      progDlg.endProgress();
    }

    if(success)
    {
      // parse and optimize
      igcParser.parse(m_flightList[row].igcData());
      igcParser.flightPointList().simplify(simpleFpList);
      olcOptimizer.setFlightPoints(simpleFpList, 100, 200);
      progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);

      if(olcOptimizer.optimize())
      {
        // distance
        m_flightList[row].setDistance(olcOptimizer.freeDistance(fpIndexList));
      }

      progDlg.endProgress();

      // nr
      gpsFlightNr = getTable()->item(row, Nr)->text().toUInt(); // store this for restore
      newFlightNr = ISql::pInstance()->newFlightNr(pilot);
      m_flightList[row].setNumber(newFlightNr);

      // start
      id = igcParser.flightPointList().firstValidFlightData();

      if(id >= 0)
      {
        // start time
        time = igcParser.flightPointList().at(id)->time().addSecs(IFlyHighRC::pInstance()->utcOffset() * 3600);
        m_flightList[row].setTime(time);

        // start place
        wp.setPos(igcParser.flightPointList().at(id)->pos());
        wp.setAlt(igcParser.flightPointList().at(id)->alt());
        wp.setType(WayPoint::TypeStartLand);

        if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
        {
          wp.setName("");
          wp.setDescription("");
          wp.setCountry("");
          wp.setSpot("");
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
        wp.setPos(igcParser.flightPointList().at(id)->pos());
        wp.setAlt(igcParser.flightPointList().at(id)->alt());
        wp.setType(WayPoint::TypeStartLand);

        if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
        {
          wp.setName("");
          wp.setDescription("");
          wp.setCountry("");
          wp.setSpot("");
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
        ISql::pInstance()->gliderList(gliders);
        IGliderForm newGlider(this, tr("New Glider"), &glider, gliders);

        if(newGlider.exec())
        {
          if(!ISql::pInstance()->glider(glider.model(), glider))
          {
            ISql::pInstance()->add(glider);
          }
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

    m_pDb->close();
  }
}

void FlightWindow::file_import()
{
  IFlightForm newFlightForm(this, tr("New Flight"));
  ProgressDlg progDlg(this);
  IGCFileParser igcParser;
  OLCOptimizer olcOptimizer;
  Glider::GliderListType gliders;
  FlightPointList simpleFpList;
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
      igcParser.flightPointList().simplify(simpleFpList);
      olcOptimizer.setFlightPoints(simpleFpList, 100, 200);
      progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);

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
        time = igcParser.flightPointList().at(startPtId)->time().addSecs(IFlyHighRC::pInstance()->utcOffset() * 3600);
        flight.setTime(time);

        // start place
        wp.setPos(igcParser.flightPointList().at(startPtId)->pos());
        wp.setAlt(igcParser.flightPointList().at(startPtId)->alt());
        wp.setType(WayPoint::TypeStartLand);

        if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
        {
          wp.setName("");
          wp.setDescription("");
          wp.setCountry("");
          wp.setSpot("");
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
        wp.setPos(igcParser.flightPointList().at(landPtId)->pos());
        wp.setAlt(igcParser.flightPointList().at(landPtId)->alt());
        wp.setType(WayPoint::TypeStartLand);

        if(!ISql::pInstance()->findWayPoint(wp, WayPoint::startLandRadius))
        {
          wp.setName("");
          wp.setDescription("");
          wp.setCountry("");
          wp.setSpot("");
          IWayPointForm newWayPoint(this, tr("New WayPoint Landing Strip"), &wp);

          if(newWayPoint.exec())
          {
            ISql::pInstance()->add(wp);
          }
        }

        flight.setLandPt(wp);
      }

      // duration
      time = igcParser.flightPointList().at(startPtId)->time();
      duration = time.secsTo(igcParser.flightPointList().at(landPtId)->time());
      flight.setDuration(duration);

      // glider
      if(!ISql::pInstance()->glider(igcParser.model(), glider))
      {
        ISql::pInstance()->gliderList(gliders);
        IGliderForm newGlider(this, tr("New Glider"), &glider, gliders);

        if(newGlider.exec())
        {
          ISql::pInstance()->add(glider);
        }
      }

      flight.setGlider(glider);

      // a new flight
      newFlightForm.setFlight(&flight);

      if(newFlightForm.exec() && m_pDb->open())
      {
        m_pDb->add(flight);
        m_pDb->close();
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
  FlightPointList simpleFpList;
  OLCWebForm olcWebForm;
  ProgressDlg progDlg(this);
  double distFree;
  double distFAI;
  double distFlat;
  double ptsFree;
  double ptsFAI;
  double ptsFlat;
  int row;
  bool success;

  row = getCurrentFlightIndex();

  if((row >= 0) && m_pDb->open())
  {
    progDlg.beginProgress(tr("read igc file..."), m_pDb);
    success = m_pDb->loadIGCFile(m_flightList[row]);

    if(success)
    {
      progDlg.endProgress();

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

      if(selected != "")
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
        igcParser.flightPointList().simplify(simpleFpList);
        olcOptimizer.setFlightPoints(simpleFpList, 100, 200);
        progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);

        if(olcOptimizer.optimize())
        {
          distFree = olcOptimizer.freeDistance(fpIndexListFree) / 1000.0;
          ptsFree = distFree * 1.5;
          distFAI = olcOptimizer.FAITriangle(fpIndexListFAI) / 1000.0;
          ptsFAI = distFAI * 2.0;
          distFlat = olcOptimizer.flatTriangle(fpIndexListFlat) / 1000.0;
          ptsFlat = distFlat * 1.75;

          if((ptsFree > ptsFAI) && (ptsFree > ptsFlat))
          {
            olcWebForm.setDeparture(*olcOptimizer.flyPointList().at(fpIndexListFree[0]));
            olcWebForm.set1stTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFree[1]));
            olcWebForm.set2ndTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFree[2]));
            olcWebForm.set3rdTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFree[3]));
            olcWebForm.setFinish(*olcOptimizer.flyPointList().at(fpIndexListFree[4]));
          }
          else if(ptsFlat > ptsFAI)
          {
            olcWebForm.setDeparture(*olcOptimizer.flyPointList().at(fpIndexListFlat[0]));
            olcWebForm.set1stTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFlat[1]));
            olcWebForm.set2ndTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFlat[2]));
            olcWebForm.set3rdTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFlat[3]));
            olcWebForm.setFinish(*olcOptimizer.flyPointList().at(fpIndexListFlat[4]));
          }
          else
          {
            olcWebForm.setDeparture(*olcOptimizer.flyPointList().at(fpIndexListFAI[0]));
            olcWebForm.set1stTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFAI[1]));
            olcWebForm.set2ndTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFAI[2]));
            olcWebForm.set3rdTurnPoint(*olcOptimizer.flyPointList().at(fpIndexListFAI[3]));
            olcWebForm.setFinish(*olcOptimizer.flyPointList().at(fpIndexListFAI[4]));
          }

          olcWebForm.save(olcFileName);
        }
      }

      progDlg.endProgress();
    }

    m_pDb->close();
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
  FlightPointList simpleFpList;

  ProgressDlg progDlg(this);
  double distFree;
  double distFAI;
  double distFlat;
  double ptsFree;
  double ptsFAI;
  double ptsFlat;
  int row;

  row = getCurrentFlightIndex();

  if((row >= 0) && m_pDb->open())
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

      if(selected != "")
      {
        IFlyHighRC::pInstance()->setLastDir(QFileInfo(selected).absoluteDir().absolutePath());

        // OLC file
        igcParser.flightPointList().simplify(simpleFpList);
        olcOptimizer.setFlightPoints(simpleFpList, 100, 200);
        progDlg.beginProgress("optimize flight...", &olcOptimizer);

        if(olcOptimizer.optimize())
        {
          distFree = olcOptimizer.freeDistance(fpIndexListFree) / 1000.0;
          ptsFree = distFree * 1.5;
          distFAI = olcOptimizer.FAITriangle(fpIndexListFAI) / 1000.0;
          ptsFAI = distFAI * 2.0;
          distFlat = olcOptimizer.flatTriangle(fpIndexListFlat) / 1000.0;
          ptsFlat = distFlat * 1.75;

          if((ptsFree > ptsFAI) && (ptsFree > ptsFlat))
          {
            kmlWriter.setDeparture(fpIndexListFree[0]);
            kmlWriter.set1stTurnPoint(fpIndexListFree[1]);
            kmlWriter.set2ndTurnPoint(fpIndexListFree[2]);
            kmlWriter.set3rdTurnPoint(fpIndexListFree[3]);
            kmlWriter.setFinish(fpIndexListFree[4]);
            kmlWriter.setTriangle(false);
          }
          else if(ptsFlat > ptsFAI)
          {
            kmlWriter.setDeparture(fpIndexListFlat[0]);
            kmlWriter.set1stTurnPoint(fpIndexListFlat[1]);
            kmlWriter.set2ndTurnPoint(fpIndexListFlat[2]);
            kmlWriter.set3rdTurnPoint(fpIndexListFlat[3]);
            kmlWriter.setFinish(fpIndexListFlat[4]);
            kmlWriter.setTriangle(true);
          }
          else
          {
            kmlWriter.setDeparture(fpIndexListFAI[0]);
            kmlWriter.set1stTurnPoint(fpIndexListFAI[1]);
            kmlWriter.set2ndTurnPoint(fpIndexListFAI[2]);
            kmlWriter.set3rdTurnPoint(fpIndexListFAI[3]);
            kmlWriter.setFinish(fpIndexListFAI[4]);
            kmlWriter.setTriangle(true);
          }

          kmlWriter.setFlight(m_flightList[row]);
          kmlWriter.setFlightPoints(igcParser.flightPointList());
          kmlWriter.save(selected);
        }

        progDlg.endProgress();
      }
    }

    m_pDb->close();
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

  if(newFlightForm.exec() && m_pDb->open())
  {
    ISql::pInstance()->add(flight);
    m_pDb->close();
  }
}

int FlightWindow::getCurrentFlightIndex()
{
  QTableWidget *pTable = getTable();
  Flight::FlightListType::iterator it;
  int ret = -1;
  int row;
  int id;
  int index;

  row = pTable->currentRow();

  if(row >=0)
  {
    id = pTable->item(row, Id)->data(Qt::DisplayRole).toInt();
    index = 0;

    for(it=m_flightList.begin(); it<=m_flightList.end(); ++it)
    {
      if(it->id() == id)
      {
        ret = index;
        break;
      }

      index++;
    }
  }

  return ret;
}

void FlightWindow::file_edit()
{
  IFlightForm flightForm(this, tr("Edit Flight"));
  int row;

  row = getCurrentFlightIndex();

  if(row >= 0)
  {
    flightForm.setFlight(&m_flightList[row]);

    if(flightForm.exec() && m_pDb->open())
    {
      ISql::pInstance()->updateFlight(m_flightList[row]);
      m_pDb->close();
    }
  }
}

void FlightWindow::file_delete()
{
  int row;

  row = getCurrentFlightIndex();

  if((row >= 0) && m_pDb->open())
  {
    m_pDb->delFlight(m_flightList[row]);
    m_pDb->close();
  }
}

void FlightWindow::plot_airData()
{
  QTime time;
  IGCFileParser igcParser;
  ProgressDlg progDlg(this);
  GnuPlot::TimeVectorType x;
  GnuPlot::PlotVectorType y1;
///  GnuPlot::PlotVectorType y2;
  GnuPlot::PlotVectorType y3;
  GnuPlot::PlotVectorType y4;
///  GnuPlot::PlotVectorType y5;
  uint fpNr;
  uint tpListSize;
  int row;
  bool success;

  row = getCurrentFlightIndex();

  if((row >= 0) && m_pDb->open())
  {
    progDlg.beginProgress(tr("read igc file..."), m_pDb);
    success = m_pDb->loadIGCFile(m_flightList[row]);
    progDlg.endProgress();

    if(success)
    {
      igcParser.parse(m_flightList[row].igcData());
      tpListSize = igcParser.flightPointList().size();

      if(tpListSize > 0)
      {
        for(fpNr=0; fpNr<tpListSize; fpNr++)
        {
          x.push_back(igcParser.flightPointList().at(fpNr)->time());
          y1.push_back(igcParser.flightPointList().at(fpNr)->alt()); // alt in m
///          y2.push_back(igcParser.flightPointList().at(fpNr)->baroAlt()); // alt in m
          y3.push_back(igcParser.flightPointList().speedV(fpNr, fpNr + 1)); // vario in m/s
          y4.push_back(igcParser.flightPointList().speedH(fpNr, fpNr+1) * 3.6); // SOG in km/h
///          y5.push_back(igcParser.flightPointList().at(fpNr)->trueAirspeed() * 3.6); // TAS in km/h
        }

        m_plotter.clear();
        m_plotter.execCmd("set lmargin 10");
        m_plotter.execCmd("set rmargin 5");
        m_plotter.setMultiplot(3, 1, tr("Airdata"));

        // alt
        m_plotter.setLabelX("");
        m_plotter.setLabelY(tr("alt [m]"));
        m_plotter.multiplotXY(x, y1);

/** with two plots in one subfigure
m_plotter.setLabelX(tr("time"));
m_plotter.setLabelY(tr("speed [km/h]"));
m_plotter.multiplotXY(x, y1, tr("SOG"), y2, tr("TAS"));
*/

        // vario
        m_plotter.setLabelX("");
        m_plotter.setLabelY(tr("vario [m/s]"));
        m_plotter.multiplotXY(x, y3);

        // speed
        m_plotter.setLabelX(tr("time"));
        m_plotter.setLabelY(tr("speed [km/h]"));
        m_plotter.multiplotXY(x, y4);

/** with two plots in one subfigure
m_plotter.setLabelX(tr("time [s]"));
m_plotter.setLabelY(tr("speed [km/h]"));
m_plotter.multiplotXY(x, y4, tr("SOG"), y5, tr("TAS"));
*/
      }
    }

    m_pDb->close();
    TableWindow::unsetCursor();
  }
}

void FlightWindow::plot_OLC()
{
  QString title;
  IGCFileParser igcParser;
  OLCOptimizer olcOptimizer;
  OLCOptimizer::FlightPointIndexListType fpIndexList;
  FlightPointList tpList;
  FlightPointList simpleFpList;
  ProgressDlg progDlg(this);
  int fpNr;
  uint dist;
  int row;
  bool success;

  row = getCurrentFlightIndex();

  if((row >= 0) && m_pDb->open())
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
        m_plotter.setLabelX(tr("latitude [deg.min]"));
        m_plotter.setLabelY(tr("longitude [deg.min]"));
        m_plotter.setLabelZ(tr("altitude [m]"));
        plotFlighPointList(igcParser.flightPointList(), tr("track"));
                igcParser.flightPointList().simplify(simpleFpList);
        olcOptimizer.setFlightPoints(simpleFpList, 100, 200);
        progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);

        if(olcOptimizer.optimize())
        {
          // fai triangle
          dist = olcOptimizer.FAITriangle(fpIndexList);
          tpList.clear();
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[1])));
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[2])));
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[3])));
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[1])));
          title.sprintf("fai triangle: %.3f km (%.2f pts)", dist/1000.0, dist/1000.0*1.4);
          plotFlighPointList(tpList, title);

          // flat triangle
          dist = olcOptimizer.flatTriangle(fpIndexList);
          tpList.clear();
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[1])));
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[2])));
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[3])));
          tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[1])));
          title.sprintf("flat triangle: %.3f km (%.2f pts)", dist/1000.0, dist/1000.0*1.2);
          plotFlighPointList(tpList, title);

          // free distance
          dist = olcOptimizer.freeDistance(fpIndexList);
          tpList.clear();

          for(fpNr=0; fpNr<fpIndexList.size(); fpNr++)
          {
            tpList.push_back(new FlightPoint(olcOptimizer.flyPointList().at(fpIndexList[fpNr])));
          }

          title.sprintf("free distance: %.3f km (%.2f pts)", dist/1000.0, dist/1000.0*1.0);
          plotFlighPointList(tpList, title);
        }

        progDlg.endProgress();
      }
    }

    m_pDb->close();
    TableWindow::unsetCursor();
  }
}

void FlightWindow::plotFlighPointList(FlightPointList &tpList, const QString& title)
{
  GnuPlot::PlotVectorType x;
  GnuPlot::PlotVectorType y;
  GnuPlot::PlotVectorType z;
  uint tpListSize;
  uint fpNr;

  tpListSize = tpList.size();

  for(fpNr=0; fpNr<tpListSize; fpNr++)
  {
    y.push_back(tpList[fpNr]->pos().lat());
    x.push_back(tpList[fpNr]->pos().lon());
    z.push_back(tpList[fpNr]->alt());
  }

  m_plotter.plotXYZ(x, y, z, title);
}

void FlightWindow::showOnWebMap()
{
  IGCFileParser igcParser;
  OLCOptimizer olcOptimizer;
  OLCOptimizer::FlightPointIndexListType fpIndexList;
  ProgressDlg progDlg(this);
  WebMapFlightView *pView;
  WayPoint::WayPointListType tpList;
  FlightPointList::SogListType sogList;
  FlightPointList::VarioListType varioList;
  FlightPointList simpleFpList;
  Elevation elevation;
  AirSpaceList airSpaceList;
  Photo::PhotoListType photoList;
  float score;
  uint dist;
  int tpListSize;
  int fpNr;
  int row;
  bool success;

  row = getCurrentFlightIndex();

  if((row >= 0) && m_pDb->open())
  {
    progDlg.beginProgress(tr("read igc file..."), m_pDb);
    success = m_pDb->loadIGCFile(m_flightList[row]);
    progDlg.endProgress();

    if(success)
    {
      igcParser.parse(m_flightList[row].igcData());
      tpListSize = igcParser.flightPointList().size();

      if(tpListSize > 0)
      {
        pView = new WebMapFlightView(tr("View Flight"));
        pView->setLocation(m_flightList[row].startPt().name());

        ISql::pInstance()->airspaceList(airSpaceList);
        pView->setAirSpaceList(&airSpaceList);

        // set flight points
        connect(&elevation, SIGNAL(confirmDownload(Elevation*, const QString&)),
                this, SLOT(confirmDownload(Elevation*, const QString&)));
        progDlg.beginProgress(tr("download set..."), &elevation);
        elevation.elevations(igcParser.flightPointList());
        progDlg.endProgress();

        pView->setFlightPointList(m_flightList[row].date(), &igcParser.flightPointList());

        // optimize flight
        igcParser.flightPointList().simplify(simpleFpList);
        olcOptimizer.setFlightPoints(simpleFpList, 100, 200);
        progDlg.beginProgress(tr("optimize flight..."), &olcOptimizer);

        if(olcOptimizer.optimize())
        {
          getBestOlcTurnPts(olcOptimizer, tpList, dist, score);
          pView->setTurnPointList(tpList);
          pView->loadMap();
        }

        // speed
        for(fpNr=0; fpNr<tpListSize; fpNr++)
        {
          sogList.push_back(igcParser.flightPointList().speedH(fpNr, fpNr + 1) * 3.6); // in km/h
        }

        pView->setSogList(sogList);

        // vario
        for(fpNr=0; fpNr<tpListSize; fpNr++)
        {
          varioList.push_back(igcParser.flightPointList().speedV(fpNr, fpNr + 1)); // in m/s
        }

        pView->setVarioList(varioList);

        // photos
        if(m_flightList[row].photoPath() != "")
        {
          resolvePhotos(m_flightList[row].photoPath(), igcParser.flightPointList(), photoList);
          pView->setPhotoList(photoList);
        }

        progDlg.endProgress();
        pView->exec();
      }
    }

    m_pDb->close();
  }
}

void FlightWindow::showOnMap()
{
  IGCFileParser igcParser;
  ProgressDlg progDlg(this);
  int row;
  bool success;
  MapView *pView;
  WayPoint::WayPointListType wpList;

  row = getCurrentFlightIndex();

  if((row >= 0) && m_pDb->open())
  {
    progDlg.beginProgress(tr("read igc file..."), m_pDb);
    success = m_pDb->loadIGCFile(m_flightList[row]);
    progDlg.endProgress();

    if(success)
    {
    igcParser.parse(m_flightList[row].igcData());
          pView = new MapView();
          pView->showFlightPointList(igcParser.flightPointList());
          pView->show();
    }

    m_pDb->close();
  }
}

void FlightWindow::showPhotos()
{
  PhotoView view(NULL, tr("Photos"));
  int row;

  row = getCurrentFlightIndex();

  if(row >= 0)
  {
    view.resize(800, 600);
    view.setFolder(m_flightList[row].photoPath());

    if(view.photoCount() > 0)
    {
      view.exec();
    }
  }
}

void FlightWindow::confirmDownload(Elevation *pElevation, const QString &question)
{
  QMessageBox msgBox;
  int ret;

  msgBox.setText(question);
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  msgBox.setIcon(QMessageBox::Question);
  ret = msgBox.exec();
  pElevation->nextDownload(ret == QMessageBox::Yes);
}

Route::Type FlightWindow::getBestOlcTurnPts(const OLCOptimizer &optimizer,
                                     WayPoint::WayPointListType &tpList,
                                     uint &bestDist, float &bestScore)
{
  Route::Type scoreType;
  OLCOptimizer::FlightPointIndexListType tpIndexList;
  WayPoint wp;
  uint dist;
  float score;
  int tpNr;

  // free
  scoreType = Route::Free;
  dist = optimizer.freeDist();
  bestDist = dist;
  bestScore = (dist / 1000.0) * 1.0;
  optimizer.freeDistIndex(tpIndexList);

  // straight
  dist = optimizer.straightDist();
  score = (dist / 1000.0) * 1.2;

  if(score > bestScore)
  {
    scoreType = Route::Straight;
    bestDist = dist;
    bestScore = score;
    tpIndexList.clear();
    optimizer.straightDistIndex(tpIndexList);
  }

  // flat
  dist = optimizer.flatDist();
  score = (dist / 1000.0) * 1.2;

  if(score > bestScore)
  {
    scoreType = Route::FlatTri;
    bestDist = dist;
    bestScore = score;
    tpIndexList.clear();
    optimizer.flatDistIndex(tpIndexList);
  }

  // fai
  dist = optimizer.faiDist();
  score = (dist / 1000.0) * 1.3;

  if(score > bestScore)
  {
    scoreType = Route::FaiTri;
    bestDist = dist;
    bestScore = score;
    tpIndexList.clear();
    optimizer.faiDistIndex(tpIndexList);
  }

  for(tpNr=0; tpNr<tpIndexList.size(); tpNr++)
  {
    wp.setPos(optimizer.flyPointList().at(tpIndexList[tpNr])->pos());
    tpList.push_back(wp);
  }

  return scoreType;
}

void FlightWindow::resolvePhotos(const QString &path, const FlightPointList &fpList,
                                 Photo::PhotoListType &photoList)
{
  QDir dir;
  QStringList filter;
  QStringList files;
  QStringList::iterator it;
  Photo photo;
  QExifImageHeader exif;
  QVector<QExifURational> posvec;
  QString strValue;
  QString filename;
  QDateTime imgTime;
  LatLng pos;
  double dValue;
  bool hasPos;

  filter << "*.jpg" << "*.JPG" << "*.jpeg" << "*.JPEG";
  dir.cd(path);
  files = dir.entryList(filter, QDir::Files);

  if(files.size() > 0)
  {
    for(it=files.begin(); it!=files.end(); it++)
    {
      filename = path + "/" + *it;

      if(exif.loadFromJpeg(filename))
      {
        // lat
        hasPos = (exif.contains(QExifImageHeader::GpsLatitude) &&
                  exif.contains(QExifImageHeader::GpsLatitudeRef));

        if(hasPos)
        {
          posvec = exif.value(QExifImageHeader::GpsLatitude).toRationalVector();
          dValue = (double)posvec[0].first / (double)posvec[0].second;
          dValue += ((double)posvec[1].first / (double)posvec[1].second) / 60;
          dValue += ((double)posvec[2].first / (double)posvec[2].second) / 3600;

          strValue = exif.value(QExifImageHeader::GpsLatitudeRef).toString();

          if(strValue == "S")
          {
            dValue = -dValue;
          }

          pos.setLat(dValue);
        }

        // lon
        hasPos &= (exif.contains(QExifImageHeader::GpsLongitude) &&
                   exif.contains(QExifImageHeader::GpsLongitudeRef));

        if(hasPos)
        {
          posvec = exif.value(QExifImageHeader::GpsLongitude).toRationalVector();
          dValue = (double)posvec[0].first / (double)posvec[0].second;
          dValue += ((double)posvec[1].first / (double)posvec[1].second) / 60;
          dValue += ((double)posvec[2].first / (double)posvec[2].second) / 3600;

          strValue = exif.value(QExifImageHeader::GpsLongitudeRef).toString();

          if(strValue == "W")
          {
            dValue = -dValue;
          }

          pos.setLon(dValue);
        }

        if(!hasPos)
        {
          hasPos = exif.contains(QExifImageHeader::DateTimeOriginal);

          if(hasPos)
          {
            strValue = exif.value(QExifImageHeader::DateTimeOriginal).toString();
            imgTime = QDateTime::fromString(strValue, "yyyy:MM:dd hh:mm:ss").toUTC();
            pos = fpList.at(imgTime.time())->pos();
          }
        }

        if(hasPos)
        {
          photo.setPath(filename);
          photo.setPos(pos);
          photoList.push_back(photo);
        }
      }
    }
  }
}
