/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _MDIWindow_h_
#define _MDIWindow_h_

#include <QMainWindow>

class QWidget;
class QString;

class MDIWindow: public QMainWindow
{
	Q_OBJECT
	public:
		MDIWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags);
		
	signals:
		void message(const QString&, int);

		/**
		 * Emit this signal if underlying data has changed
		 */
		void dataChanged();
		
	protected slots:
		/**
		 * Refresh displayed data.
		 * Called when a dataChanged() signal was received
		 */
		virtual void refresh()=0;
};

#endif
