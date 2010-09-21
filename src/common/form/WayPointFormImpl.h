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

#ifndef WayPointFormImpl_h
#define WayPointFormImpl_h

#include <qstring.h>
#include "ui_WayPointForm.h"
#include "WayPoint.h"

class WayPointFormImpl: public QDialog, public Ui::WayPointForm
{
	Q_OBJECT

	public:
		WayPointFormImpl(QWidget* parent, const QString &caption, WayPoint *pWayPoint);

	protected slots:
		void accept();
		void limitLatitude();
		void limitLongitude();
		
	private:
		WayPoint *m_pWayPoint;
};

#endif

