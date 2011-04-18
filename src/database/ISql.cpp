/***************************************************************************
 *   Copyright (C) 2006 by Alex Graf                                     *
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
#include <QStringList>
#include <QHostInfo>
#include <QFileInfo>
#include <QDir>
#include <QInputDialog>
#include "AirSpaces.h"
#include "Error.h"
#include "ISql.h"
#include "WayPoints.h"
#include "Gliders.h"
#include "Flights.h"
#include "Pilots.h"
#include "Routes.h"
#include "Servicings.h"
#include "Upgrade.h"
#include "DatabaseParameters.h"
#include "ISql.h"

#include <QDebug>

ISql* ISql::m_pInst = NULL;

ISql::ISql()
{
  // set default values
	setDBParameters(DatabaseParameters::defaultParameters());

//	m_pAirSpaces = new AirSpaces(m_DefaultDB);
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

//	delete m_pAirSpaces;
	delete m_pWayPoints;
	delete m_pGliders;
	delete m_pFlights;
	delete m_pRoutes;
	delete m_pServicings;
	delete m_pPilots;
}

bool ISql::open()
{
	return m_DefaultDB.isOpen();
}

void ISql::close()
{
}

bool ISql::createAndConnect()
{
	QString root;
	QString pwd;
	bool ok;

	// close open db first
	if (open())
	{
		m_DefaultDB.close();
		QSqlDatabase::removeDatabase(m_DefaultDB.connectionName());
	}

	// which type of db?
	if (m_DriverName == "QMYSQL")
	{
		// check for server host
		QHostInfo info = QHostInfo::fromName(m_DBHostName);
		if(info.error()!=QHostInfo::NoError)
		{
			// todo: show error dialog
			qDebug() << info.errorString();
		}
		ok = connectDb();
		if(!ok)
		{
			root = QInputDialog::getText(NULL, tr("Name"), tr("MySQL root name:"), QLineEdit::Normal, "root", &ok);
			pwd = QInputDialog::getText(NULL, tr("Password"), tr("MySQL root pwd:"), QLineEdit::Password, "", &ok);

			if(ok)
			{
				createDb(root, pwd);
				ok = connectDb();
			}
		}
	}
	else if (m_DriverName == "QSQLITE")
	{
		QFileInfo dbfile = QFileInfo(m_DBName);

		// create directory if necessary
		if (!dbfile.dir().exists())
		{
			QDir().mkpath(dbfile.absolutePath());
		}

		ok = connectDb();
		if(!ok)
		{
			createDb();
			ok = connectDb();
		}
	}
	else
	{
		qDebug() << "Unknown db type '" << m_DriverName << "'.";
		return false;
	}
	return ok;
}

bool ISql::connectDb()
{
	bool success;

  // create default db connection
  m_DefaultDB = QSqlDatabase::addDatabase(m_DriverName);

  // set mysql db name or sqlite filename
  m_DefaultDB.setDatabaseName(m_DBName);

  // mysql needs additional configuration
  if (m_DriverName == "QMYSQL")
  {
    m_DefaultDB.setUserName(m_DBUserName);
    m_DefaultDB.setPassword(m_DBPassword);
    m_DefaultDB.setHostName(m_DBHostName);
    m_DefaultDB.setPort(m_DBPort);
  }

  success = m_DefaultDB.open();
	Error::verify(success, Error::SQL_OPEN);

  if(success)
	{
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

bool ISql::createDb(const QString &root, const QString &pwd)
{
	bool success;
	QString dbID = "root_db";

	// creating the QSqlDatabase object within a code block.
	// Assures proper cleanup when removing the db.
	{
		QSqlDatabase setupDb = QSqlDatabase::addDatabase(m_DriverName, dbID);
		if (m_DriverName == "QMYSQL")
		{
			setupDb.setUserName(root);
			setupDb.setPassword(pwd);
			setupDb.setHostName(m_DBHostName);
			setupDb.setPort(m_DBPort);
		}
		else if (m_DriverName == "QSQLITE")
		{
			setupDb.setDatabaseName(m_DBName);
		}

		success = setupDb.open();

		if(success)
		{
			Upgrade upgrade(setupDb);
			upgrade.setup(m_DBName, m_DBUserName, m_DBPassword);
			setupDb.close();
		}
	}
	QSqlDatabase::removeDatabase(dbID);

	return success;
}

void ISql::setName(const QString &name)
{
  m_DBName = name;
}

void ISql::setUserName(const QString &userName)
{
  m_DBUserName = userName;
}

void ISql::setPassword(const QString &passwd)
{
  m_DBPassword = passwd;
}

void ISql::setHostName(const QString &hostName)
{
  m_DBHostName = hostName;
}

void ISql::setPort(int port)
{
  m_DBPort = port;
}

void ISql::setDriverName(const QString& name)
{
	m_DriverName = name;
}

void ISql::setDBParameters(const DatabaseParameters& params)
{
	m_DriverName = params.dBType();
	if (params.isMySQL())
	{
		m_DBName = params.dBName();
	}
	else
	{
		m_DBName = params.dBFile();
	}
	m_DBUserName = params.dBUserName();
	m_DBPassword = params.dBPassword();
	m_DBHostName = params.dBHostName();
	m_DBPort = params.dBPort();
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
	return m_pWayPoints->add(wp);
}

bool ISql::update(WayPoint &wp)
{
	return m_pWayPoints->update(wp);
}

bool ISql::delWayPoint(WayPoint &wp)
{
	return m_pWayPoints->delWayPoint(wp);
}

bool ISql::findWayPoint(WayPoint &wp, uint radius)
{
	return m_pWayPoints->findWayPoint(wp, radius);
}

int ISql::wayPointsLastModified()
{
	return m_pWayPoints->lastModified("WayPoints");
}

bool ISql::wayPointList(WayPoint::WayPointListType &wpList)
{
	return m_pWayPoints->wayPointList(wpList);
}

bool ISql::add(Glider &glider)
{
	return m_pGliders->add(glider);
}

bool ISql::delGlider(Glider &glider)
{
	return m_pGliders->delGlider(glider);
}

bool ISql::glider(const QString &model, Glider &glider)
{
	return m_pGliders->glider(model, glider);
}

bool ISql::gliderList(Glider::GliderListType &gliderList)
{
	return m_pGliders->gliderList(gliderList);
}

int ISql::glidersLastModified()
{
	return m_pGliders->lastModified("Gliders");
}

bool ISql::add(Flight &flight)
{
	return m_pFlights->add(flight);
}

bool ISql::delFlight(Flight &flight)
{
	return m_pFlights->delFlight(flight);
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

int ISql::flightsLastModified()
{
	return m_pFlights->lastModified("Flights");
}

bool ISql::add(Route &route)
{
	return m_pRoutes->add(route);
}

bool ISql::delRoute(Route &route)
{
	return m_pRoutes->delRoute(route);
}

int ISql::routesLastModified()
{
	return m_pRoutes->lastModified("Routes");
}

bool ISql::routeList(Route::RouteListType &routeList)
{
	return m_pRoutes->routeList(routeList);
}
/*
bool ISql::add(AirSpace &airspace)
{
	return m_pAirSpaces->add(airspace);
}

bool ISql::delAirSpace(AirSpace &airspace)
{
	return m_pAirSpaces->delAirSpace(airspace);
}

bool ISql::airspace(const QString &name, AirSpace &airspace)
{
	return m_pAirSpaces->airspace(name, airspace);
}

int ISql::airspacesLastModified()
{
	return m_pAirSpaces->lastModified("AirSpaces");
}

bool ISql::airspaceList(AirSpace::AirSpaceListType &airspaceList)
{
	return m_pAirSpaces->airspaceList(airspaceList);
}
*/
bool ISql::add(Servicing &serv)
{
	return m_pServicings->add(serv);
}

bool ISql::delServicing(Servicing &servicing)
{
	return m_pServicings->delServicing(servicing);
}

bool ISql::servicingList(Servicing::ServicingListType &servicingList)
{
	return m_pServicings->servicingList(servicingList);
}

int ISql::servicingsLastModified()
{
	return m_pServicings->lastModified("Servicings");
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

int ISql::pilotsLastModified()
{
	return m_pPilots->lastModified("Pilots");
}

/*
AirSpaces* ISql::pAirSpaceTable()
{
	return m_pAirSpaces;
}*/

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
