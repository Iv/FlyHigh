/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
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
#ifndef Protocol6015_h
#define Protocol6015_h

#include <qvariant.h>
#include "Defs6015.h"
#include "Device.h"
#include "Flight.h"
#include "Protocol.h"

class Flight;
class Route;
class WayPoint;

/**
	@author Alex Graf <grafal@sourceforge.net>
*/
class Protocol6015: public Protocol
{
	public:
		Protocol6015(IFlyHighRC::DeviceId id);

		~Protocol6015();

		bool open(const QString &dev, int baud);

		void close();

		/** Settings */
		bool parWrite(MemType memType, int par, FtDataType dataType, const QVariant &value);

		QVariant parRead(MemType memType, int par, FtDataType dataType);

		/**
			Tries to recieve a " Done\r\n". Waits 100ms until returns.
			@return true = done recieved
		*/
		bool recieveDone();

		/** Track */
		bool trackListReq();

		bool trackListRec(int &total, Flight &flight);

		bool trackReq(int trackNr);

		bool trackRec(QString &line);

		/** Waypoint */
		bool wpListReq();

		bool wpListRec(WayPoint &wp);

		bool wpSnd(const WayPoint &wp);

		bool wpDelAll();

		/** Route */
		bool routeListReq();

		bool routeListRec(uint &curSent, uint &totalSent, Route &route);

    bool routeSnd(uint &curSent, uint &totalSent, Route &route);

		bool routeDel(const QString &name);

	private:
		Device m_device;
		int m_pos;

		bool parseTrack(const QString &tlg, Flight &flight);

		bool parseWp(const QString &tlg, WayPoint &wp);

		QDate parseDate(const QString &token);

		QTime parseTime(const QString &token);

		void getWpSndTlg(const WayPoint &wp, QString &tlg);

		QString value2ftString(int value, int length);

		QString deg2ftString(double value, int length, char dir);

		double ftString2Deg(const QString &token);

		QString qString2ftString(const QString &qString, int length);

		QString ftString2qString(const QString &ftString);

		bool writeEnableFa();

		bool writePar(MemType memType, int par, const QString &value);

		bool writeParString(MemType memType, int par, const QString &value);

		bool writeParArray(MemType memType, int par, const QByteArray &value);

		bool requestPar(MemType memType, int par);

		int readParInt(MemType memType, int par, FtDataType dataType);

		QString readParString(MemType memType, int par);

		QByteArray readParArray(MemType memType, int par);
};

#endif
