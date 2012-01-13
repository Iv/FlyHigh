/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#ifndef RouteFormImpl_h
#define RouteFormImpl_h

#include <QList>
#include "ui_RouteForm.h"
#include "WayPoint.h"

class QWidget;
class QString;
class Route;

class RouteFormImpl: public QDialog, public Ui_RouteForm
{
	Q_OBJECT

	public:
		RouteFormImpl(QWidget* parent, const QString &caption, Route *pRoute);

		void setReadOnly();

	public slots:
		void accept();

		void down();

		void up();

		void toRight();

		void remove();

	private:
		bool m_readOnly;
		Route *m_pRoute;
		WayPoint::WayPointListType m_wpDbList;
		QList<WayPoint*> m_wpRouteList;

		void showWpRoute(uint curItem);
};

#endif

