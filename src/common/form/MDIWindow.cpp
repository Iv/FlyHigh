/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <qapplication.h>
#include <qeventloop.h>
#include <qwidget.h>
#include <qstring.h>
#include <qtimer.h>
#include "MDIWindow.h"

#define UPDATE_TIME 1000 // 1 second

MDIWindow::MDIWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags)
	:QMainWindow(parent, wflags)
{
	setWindowTitle(name);
	m_pUpdateTimer = new QTimer(this);
	connect(m_pUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTimeout()));
	m_pUpdateTimer->setSingleShot(true);
	m_pUpdateTimer->start(0);
}

bool MDIWindow::periodicalUpdate()
{
	return false;
}

void MDIWindow::updateTimeout()
{
	if(periodicalUpdate())
	{
		m_pUpdateTimer->setSingleShot(true);
		m_pUpdateTimer->start(UPDATE_TIME);
	}
}
