/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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

#include "RouteForm.h"

class QWidget;
class QString;
class Route;

class RouteFormImpl: public RouteForm
{
	Q_OBJECT

	public:
		RouteFormImpl(QWidget* parent, const QString &caption, Route *pRoute);
	
	public slots:
		void accept();
		void down();
		void up();
		void toRight();
		void remove();
		
	private:
		Route *m_pRoute;
};

#endif

