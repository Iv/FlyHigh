/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _MainWindow_h_
#define _MainWindow_h_

#include <qmainwindow.h>

class QPopupMenu;
class QWorkspace;
class MDIWindow;

class TrackWindow;

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
		void waypoints_fromSQL();
		void waypoints_fromGPS();
		void routes_fromSQL();
		void routes_fromGPS();
		void windows_tile_horizontally();
		void windows_activated(int id);
		void settings_port();
		void settings_device(int id);
		void settings_configure_device();
		void help_about();
		void aboutToShow();
		
	private:
		QWorkspace* m_pWorkSpace;
		QPopupMenu* m_pWindowsMenu;
		QPopupMenu* m_pDevicesMenu;
		QPopupMenu* m_pSpecialMenu;
		MDIWindow* m_pActiveWin;
		int m_specialMenuId;
		uint m_selectedDevice;

		void showWindow(QMainWindow *pWin);
};

#endif
