/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#ifndef _MainWindow_h_
#define _MainWindow_h_

#include <qmainwindow.h>
#include <QCloseEvent>

class QMdiArea;
class QSignalMapper;
class MDIWindow;

class MainWindow: public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow();
		virtual ~MainWindow();

	protected:
		void closeEvent(QCloseEvent *);
		
	private slots:
		void flights_fromSQL();
		void flights_fromGPS();
		void flights_experience();
		void analysis_gliders();
		void analysis_servicing();
		void waypoints_fromSQL();
		void waypoints_fromGPS();
		void routes_fromSQL();
		void routes_fromGPS();
//		void airspaces_fromSQL();
		void airspaces_fromGPS();
		void airspaces_fromFile();
		void windows_tile_horizontally();
		void windows_activated(int id);
		void settings_port();
		void settings_device();
		void settings_configure_device();
		void settings_pilotInfo();
		void help_about();
		void aboutToShow();
		void setActiveSubWindow(QWidget *window);
		
	private:
		QMdiArea* m_pMdiArea;
		QSignalMapper* m_pWinMapper;
		QMenu* m_pWindowsMenu;
		QMenu* m_pDevicesMenu;
		MDIWindow* m_pActiveWin;
		QAction* m_pCascade;
		QAction* m_pTile;
		QAction* m_pTileHor;
		QAction* m_pWinSeparator;

		void showWindow(QMainWindow *pWin);

		MDIWindow* activeMdiChild();
};

#endif
