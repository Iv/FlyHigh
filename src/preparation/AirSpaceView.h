/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
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
#ifndef AirSpaceView_h
#define AirSpaceView_h

#include <qwidget.h>

class AirSpace;

class AirSpaceView: public QWidget
{
	public:
		AirSpaceView();

		~AirSpaceView();

		void setAirSpaceList(AirSpace::AirSpaceListType *pAirSpaceList, int selected);

		void setSelected(int selected);

	protected:
		void paintEvent(QPaintEvent *pEvent);

		void closeEvent(QCloseEvent *pEvent);

	private:
		AirSpace::AirSpaceListType *m_pAirSpaceList;
		BoundBox m_bbox;
		int m_selected;

		void drawAirspace();
};

#endif
