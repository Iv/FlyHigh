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
#ifndef ISql_h
#define ISql_h

#include <QString>
#include <QSqlDatabase>
#include "AirSpace.h"
#include "ContainerDef.h"
#include "IDataBase.h"
#include "WayPoint.h"
#include "Flight.h"
#include "Glider.h"
#include "Route.h"
#include "Pilot.h"
#include "Servicing.h"
#include "DatabaseParameters.h"

class QTimer;
class AirSpaces;
class WayPoints;
class Gliders;
class Flights;
class Routes;
class Servicings;
class Pilots;

/**
@author Alex Graf
*/
class ISql: public IDataBase
{
  Q_OBJECT

	public:
		static ISql* pInstance();

		virtual ~ISql();

		bool open();

		void close();

		/**
		 * Connects to the configured db.
		 * If the db does not yet exist, it will be created.
		 */
		bool connectDb();

		bool reqPwdForCreateDb();

		bool createDb(const QString &usr = "", const QString &pwd = "");

		void setDBParameters(const DatabaseParameters& params);

		bool add(WayPoint &wp);

		bool add(WayPoint::WayPointListType &wpList);

    bool update(WayPoint &wp);

    bool update(WayPoint::WayPointListType &wpList);

		bool delWayPoint(WayPoint &wp);

		bool delWayPoints(WayPoint::WayPointListType &wpList);

		bool findWayPoint(WayPoint &wp, uint radius);

		bool wayPointList(WayPoint::Type type, WayPoint::WayPointListType &wpList);

		bool add(Glider &glider);

		bool update(Glider &glider);

		bool delGlider(Glider &glider);

		bool glider(const QString &model, Glider &glider);

		bool gliderList(Glider::GliderListType &gliderList);

		bool add(Flight &flight);

		bool updateFlight(Flight &flight);

		bool delFlight(Flight &flight);

		int newFlightNr(Pilot &pilot);

		bool flightList(Pilot &pilot, Flight::FlightListType &flightList);

		bool flightsPerYear(Pilot &pilot, FlightsPerYearListType &fpyList);

		bool loadIGCFile(Flight &flight);

		bool add(Route &route);

		bool delRoute(Route &route);

		bool routeList(Route::RouteListType &routeList);

		bool add(AirSpace &airspace);

		bool add(AirSpaceList &airspaceList);

		bool update(AirSpace &airspace);

		bool delAirSpace(AirSpace &airspace);

		bool delAirSpaces(AirSpaceList &airspaceList);

		bool airspace(const QString &name, AirSpace &airspace);

		bool airspaceList(AirSpaceList &airspaceList);

		bool add(Servicing &serv);

		bool delServicing(Servicing &servicing);

		bool servicingList(Servicing::ServicingListType &servicingList);

		bool add(Pilot &pilot);

		bool update(Pilot &pilot);

		bool pilot(int id, Pilot &pilot);

		bool setId(Pilot &pilot);

  signals:
    void glidersChanged();

    void pilotsChanged();

		void servicingsChanged();

	protected:
		AirSpaces* pAirSpaceTable();

		WayPoints* pWayPointTable();

		Gliders* pGliderTable();

		Flights* pFlightTable();

		Servicings* pServicingTable();

		Pilots* pPilotTable();

	private:
		static ISql* m_pInst;
    QTimer *m_pTimer;
		AirSpaces* m_pAirSpaces;
		WayPoints* m_pWayPoints;
		Gliders* m_pGliders;
		Flights* m_pFlights;
		Routes* m_pRoutes;
		Servicings* m_pServicings;
		Pilots* m_pPilots;
		QSqlDatabase m_DefaultDB;

		/**
		 * database connection parameters
		 */
		DatabaseParameters m_dbParameters;

		ISql();

    bool connectDbPriv();

		void upgradeTables();

  private slots:
    void checkModified();

		friend class AirSpaces;
		friend class WayPoints;
		friend class Gliders;
		friend class Flights;
		friend class Servicings;
		friend class Pilots;
		friend class Routes;
};

#endif
