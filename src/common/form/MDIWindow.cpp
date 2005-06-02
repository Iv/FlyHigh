/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <qapplication.h>
#include <qeventloop.h>
#include <qprogressdialog.h>
#include <qwidget.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qtimer.h>
#include "MDIWindow.h"

#define UPDATE_TIME 1000 // 1 second

MDIWindow::MDIWindow(QWidget* parent, const char* name, int wflags)
	:QMainWindow(parent, name, wflags)
{
	m_pProgressDlg = NULL;
	m_pUpdateTimer = new QTimer(this);
	connect(m_pUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTimeout()));
	m_pUpdateTimer->start(0, true);
}

bool MDIWindow::periodicalUpdate()
{
	return false;
}

void MDIWindow::updateTimeout()
{
	if(periodicalUpdate())
	{
		m_pUpdateTimer->start(UPDATE_TIME, true);
	}
}
