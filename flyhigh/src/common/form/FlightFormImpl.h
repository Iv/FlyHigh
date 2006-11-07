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

#ifndef FlightFormImpl_h
#define FlightFormImpl_h

#include <qthread.h>
#include "FlightForm.h"
#include "Glider.h"
#include "WayPoint.h"

class Flight;

class FlightFormImpl : public FlightForm
{
	Q_OBJECT

	public:
		FlightFormImpl(QWidget* parent, const QString &caption, Flight *pFlight);

		void setFlight(Flight *pFlight);
		void enableInput(bool b);

	protected slots:
		void accept();
		void newWayPoint();
		void newGlider();
		void limitDistance();
		
	private:
		Flight *m_pFlight;
		Glider::GliderListType m_gliderList;
		WayPoint::WayPointListType m_wpList;
		
		void updateWayPoints();
		void updateGlider();
		void selectStart();
		void selectLand();
		void selectGlider();
};

#endif
