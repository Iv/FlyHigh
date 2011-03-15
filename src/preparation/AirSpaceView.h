/***************************************************************************
 *   Copyright (C) 2009 by Alex Graf                                     *
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

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QCloseEvent>

class AirSpace;
class AirSpaceList;

class AirSpaceView: public QWidget
{
	public:
		AirSpaceView();

		~AirSpaceView();

		void setAirSpaceList(AirSpaceList *pAirSpaceList, int selected);

		void setSelected(int selected);

	protected:
		void paintEvent(QPaintEvent *pEvent);

		void closeEvent(QCloseEvent *pEvent);

		void mousePressEvent(QMouseEvent *pEvent);

		void mouseReleaseEvent(QMouseEvent *pEvent);

		void mouseMoveEvent(QMouseEvent *pEvent);

		void wheelEvent(QWheelEvent * pEvent);

	private:
		enum {MinScale=1, MaxScale=10};

		AirSpaceList *m_pAirSpaceList;
		BoundBox m_bbox;
		QPoint m_offset;
		int m_scale;
		bool m_mouseDown;
		QPoint m_prevPos;

		int m_selected;

		void drawAirspace();

		void calcOffset(const QPoint &mousePos);
};

#endif
