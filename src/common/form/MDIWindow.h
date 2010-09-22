/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _MDIWindow_h_
#define _MDIWindow_h_

#include <qmainwindow.h>

class QWidget;
class QString;
class QTimer;

class MDIWindow: public QMainWindow
{
	Q_OBJECT
	public:
		MDIWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags);
		
	signals:
		void message(const QString&, int);
		
	protected:
		virtual bool periodicalUpdate();
		
	private slots:
		void updateTimeout();
		
	private:
		QTimer *m_pUpdateTimer;
};

#endif
