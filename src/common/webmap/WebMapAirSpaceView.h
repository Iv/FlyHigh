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

#ifndef _WebMapRouteView_h
#define _WebMapRouteView_h

#include <QDialog>

class WebMap;
class AirSpaceList;

class WebMapAirSpaceView: public QDialog
{
	Q_OBJECT

	public:
		WebMapAirSpaceView(const QString &name);

		~WebMapAirSpaceView();

		void setAirSpaceList(AirSpaceList *pAirSpaceList);

		void loadMap();

		void selectAirSpace(int nr);

	signals:
		void airSpaceChanged(int line);

	protected:
		void resizeEvent(QResizeEvent *pEvent);

	private:
		WebMap *m_pWebMap;
		AirSpaceList *m_pAirSpaceList;

	private slots:
		void mapReady();

		void finished(int res);

		void lineChanged(int line);
};

#endif