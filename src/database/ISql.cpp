/***************************************************************************
 *   Copyright (C) 2006 by Alex Graf                                       *
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
#include <QFileInfo>
#include <QDir>
#include <QHostInfo>
#include <QSqlQuery>
#include <QStringList>
#include <QTimer>
#include "AirSpaceList.h"
#include "AirSpaces.h"
#include "CredentialsDlg.h"
#include "DatabaseUtilities.h"
#include "Error.h"
#include "ISql.h"
#include "Gliders.h"
#include "Flights.h"
#include "Pilots.h"
#include "Routes.h"
#include "Servicings.h"
#include "Upgrade.h"
#include "WayPoints.h"

ISql* ISql::m_pInst = NULL;

ISql::ISql()
{
  m_pTimer = NULL;

  // set default values
	setDBParameters(DatabaseParameters::defaultParameters());

	m_pAirSpaces = new AirSpaces(m_DefaultDB);
	m_pWayPoints = new WayPoints(m_DefaultDB);
	m_pGliders = new Gliders(m_DefaultDB);
	m_pFlights = new Flights(m_DefaultDB);
	m_pRoutes = new Routes(m_DefaultDB);
	m_pServicings = new Servicings(m_DefaultDB);
	m_pPilots = new Pilots(m_DefaultDB);
}

ISql::~ISql()
{
	m_DefaultDB.close();

	delete m_pAirSpaces;
	delete m_pWayPoints;
	delete m_pGliders;
	delete m_pFlights;
	delete m_pRoutes;
	delete m_pServicings;
	delete m_pPilots;
}

bool ISql::open()
{
  bool res;

	res = m_DefaultDB.isOpen();

  if(m_pTimer == NULL)
  {
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(checkModified()));
    m_pTimer->start(1000);
  }

	return res;
}

void ISql::close()
{
}

bool ISql::connectDb()
{
	bool ok;

	// close open db first
	if(open())
	{
    QString connection = m_DefaultDB.connectionName();
		m_DefaultDB.close();
    m_DefaultDB = QSqlDatabase();
    QSqlDatabase::removeDatabase(connection);
	}

	// which type of db?
	if(m_dbParameters.isMySQL())
	{
		// check for server host
		QHostInfo info = QHostInfo::fromName(m_dbParameters.dBHostName());

		if(info.error() != QHostInfo::NoError)
		{
			// todo: show error dialog
			qDebug() << info.errorString();
		}

		ok = connectDbPriv();
	}
	else if(m_dbParameters.isSQLite())
	{
    QFileInfo dbfile = QFileInfo(m_dbParameters.dBFile());

		// create directory if necessary
		if(!dbfile.dir().exists())
		{
			QDir().mkpath(dbfile.absolutePath());
		}

		ok = connectDbPriv();
	}
	else
	{
		qDebug() << "Unknown db type '" << m_dbParameters.dBType() << "'.";
		return false;
	}

	return ok;
}

bool ISql::reqPwdForCreateDb()
{
  return m_dbParameters.isMySQL();
}

bool ISql::createDb(const QString &usr, const QString &pwd)
{
  bool success = false;

	if(m_dbParameters.isMySQL())
	{
    success = DatabaseUtilities::createDb(m_dbParameters, usr, pwd);
  }
	else if(m_dbParameters.isSQLite())
	{
    success = DatabaseUtilities::createDb(m_dbParameters);
	}

	return success;
}

bool ISql::connectDbPriv()
{
	bool success;

  // create default db connection
	m_DefaultDB = QSqlDatabase::addDatabase(m_dbParameters.dBType());
	m_dbParameters.apply(m_DefaultDB);

  success = m_DefaultDB.open();
	Error::verify(success, Error::SQL_OPEN);

  if(success)
	{
    // enable foreign keys in sqlite
    if(m_dbParameters.isSQLite())
    {
      QSqlQuery query(m_DefaultDB);
      query.exec("PRAGMA foreign_keys = true;");
    }

    // check if there is something in the db
    // if not, the db needs to be created before upgrading
    if (m_DefaultDB.tables().count() > 0)
    {
      upgradeTables();
    }
    else
    {
      success = false;
    }
	}

	return success;
}

void ISql::setDBParameters(const DatabaseParameters& params)
{
	m_dbParameters = params;
}

ISql* ISql::pInstance()
{
	if(m_pInst == NULL)
	{
		m_pInst = new ISql();
	}

	return m_pInst;
}

bool ISql::add(WayPoint &wp)
{
  bool success;

	success = m_pWayPoints->add(wp);
	emit wayPointsChanged();

	return success;
}

bool ISql::add(WayPoint::WayPointListType &wpList)
{
  WayPoint::WayPointListType::iterator it;
  bool success = true;

  for(it=wpList.begin(); it!=wpList.end(); it++)
  {
    success &= m_pWayPoints->add(*it);
  }

	emit wayPointsChanged();

	return success;
}

bool ISql::update(WayPoint &wp)
{
  bool success;

	success = m_pWayPoints->update(wp);
	emit wayPointsChanged();

	return success;
}

bool ISql::update(WayPoint::WayPointListType &wpList)
{
  WayPoint::WayPointListType::iterator it;
  bool success = true;

  for(it=wpList.begin(); it!=wpList.end(); it++)
  {
    success &= m_pWayPoints->update(*it);
  }

	emit wayPointsChanged();

	return success;
}

bool ISql::delWayPoint(WayPoint &wp)
{
  bool success;

	success = m_pWayPoints->delWayPoint(wp);
	emit wayPointsChanged();

	return success;
}

bool ISql::delWayPoints(WayPoint::WayPointListType &wpList)
{
  WayPoint::WayPointListType::iterator it;
  bool success = true;

  for(it=wpList.begin(); it!=wpList.end(); it++)
  {
    success &= m_pWayPoints->delWayPoint(*it);
  }

	emit wayPointsChanged();

	return success;
}

bool ISql::findWayPoint(WayPoint &wp, uint radius)
{
	return m_pWayPoints->findWayPoint(wp, radius);
}

bool ISql::wayPointList(WayPoint::Type type, WayPoint::WayPointListType &wpList)
{
	return m_pWayPoints->wayPointList(type, wpList);
}

bool ISql::add(Glider &glider)
{
  bool success;

	success = m_pGliders->add(glider);
	emit glidersChanged();

  return success;
}

bool ISql::update(Glider &glider)
{
  bool success;

	success = m_pGliders->update(glider);
	emit glidersChanged();

  return success;
}

bool ISql::delGlider(Glider &glider)
{
  bool success;

	success = m_pGliders->delGlider(glider);
	emit glidersChanged();

	return success;
}

bool ISql::glider(const QString &model, Glider &glider)
{
	return m_pGliders->glider(model, glider);
}

bool ISql::gliderList(Glider::GliderListType &gliderList)
{
	return m_pGliders->gliderList(gliderList);
}

bool ISql::add(Flight &flight)
{
  bool success;

	success = m_pFlights->add(flight);
	emit flightsChanged();

	return success;
}

bool ISql::updateFlight(Flight &flight)
{
  bool success;

	success = m_pFlights->updateFlight(flight);
	emit flightsChanged();

	return success;
}

bool ISql::delFlight(Flight &flight)
{
  bool success;

	success = m_pFlights->delFlight(flight);
  emit flightsChanged();

	return success;
}

int ISql::newFlightNr(Pilot &pilot)
{
	return m_pFlights->newFlightNr(pilot);
}

bool ISql::flightList(Pilot &pilot, Flight::FlightListType &flightList)
{
	return m_pFlights->flightList(pilot, flightList);
}

bool ISql::flightsPerYear(Pilot &pilot, FlightsPerYearListType &fpyList)
{
	return m_pFlights->flightsPerYear(pilot, fpyList);
}

bool ISql::loadIGCFile(Flight &flight)
{
	return m_pFlights->loadIGCFile(flight);
}

bool ISql::add(Route &route)
{
  bool success;

	success = m_pRoutes->add(route);
	emit routesChanged();

	return success;
}

bool ISql::delRoute(Route &route)
{
  bool success;

	success = m_pRoutes->delRoute(route);
  emit routesChanged();

	return success;
}

bool ISql::routeList(Route::RouteListType &routeList)
{
	return m_pRoutes->routeList(routeList);
}

bool ISql::add(AirSpace &airspace)
{
  bool success;

	success = m_pAirSpaces->add(airspace);
	emit airSpacesChanged();

	return success;
}

bool ISql::add(AirSpaceList &airspaceList)
{
  AirSpaceList::iterator it;
  AirSpace *pAirSpace;
  bool success = true;

  for(it=airspaceList.begin(); it!=airspaceList.end(); it++)
  {
    pAirSpace = (*it);
    success &= m_pAirSpaces->add(*pAirSpace);
  }

	emit airSpacesChanged();

	return success;
}

bool ISql::update(AirSpace &airspace)
{
  bool success;

	success = m_pAirSpaces->updateAirSpace(airspace);
	emit airSpacesChanged();

	return success;
}

bool ISql::delAirSpace(AirSpace &airspace)
{
  bool success;

	success = m_pAirSpaces->delAirSpace(airspace);
	emit airSpacesChanged();

	return success;
}

bool ISql::delAirSpaces(AirSpaceList &airspaceList)
{
  AirSpaceList::iterator it;
  AirSpace *pAirSpace;
  bool success = true;

  for(it=airspaceList.begin(); it!=airspaceList.end(); it++)
  {
    pAirSpace = (*it);
    success &= m_pAirSpaces->delAirSpace(*pAirSpace);
  }

	emit airSpacesChanged();

	return success;
}

bool ISql::airspace(const QString &name, AirSpace &airspace)
{
	return m_pAirSpaces->airspace(name, airspace);
}

bool ISql::airspaceList(AirSpaceList &airspaceList)
{
	return m_pAirSpaces->airspaceList(airspaceList);
}

bool ISql::add(Servicing &servicing)
{
  bool success;

	success = m_pServicings->add(servicing);
	emit servicingsChanged();

	return success;
}

bool ISql::update(Servicing &servicing)
{
  bool success;

	success = m_pServicings->update(servicing);
	emit servicingsChanged();

	return success;
}

bool ISql::delServicing(Servicing &servicing)
{
  bool success;

	success = m_pServicings->delServicing(servicing);
  emit servicingsChanged();

	return success;
}

bool ISql::servicingList(Servicing::ServicingListType &servicingList)
{
	return m_pServicings->servicingList(servicingList);
}

bool ISql::add(Pilot &pilot)
{
	return m_pPilots->add(pilot);
}

bool ISql::setId(Pilot &pilot)
{
	return m_pPilots->setId(pilot);
}

bool ISql::update(Pilot &pilot)
{
	return m_pPilots->update(pilot);
}

bool ISql::pilot(int id, Pilot &pilot)
{
	return m_pPilots->pilot(id, pilot);
}

AirSpaces* ISql::pAirSpaceTable()
{
	return m_pAirSpaces;
}

WayPoints* ISql::pWayPointTable()
{
	return m_pWayPoints;
}

Gliders* ISql::pGliderTable()
{
	return m_pGliders;
}

Flights* ISql::pFlightTable()
{
	return m_pFlights;
}

Servicings* ISql::pServicingTable()
{
	return m_pServicings;
}

Pilots* ISql::pPilotTable()
{
	return m_pPilots;
}

void ISql::upgradeTables()
{
	Upgrade upgrade(m_DefaultDB);

	upgrade.upgrade();
}

void ISql::checkModified()
{
  if(m_pAirSpaces->checkModified())
  {
    emit airSpacesChanged();
  }

  if(m_pFlights->checkModified())
  {
    emit flightsChanged();
  }

  if(m_pGliders->checkModified())
  {
    emit glidersChanged();
  }

  if(m_pPilots->checkModified())
  {
    emit pilotsChanged();
  }

  if(m_pRoutes->checkModified())
  {
    emit routesChanged();
  }

  if(m_pServicings->checkModified())
  {
    emit servicingsChanged();
  }

  if(m_pWayPoints->checkModified())
  {
    emit wayPointsChanged();
  }
}
