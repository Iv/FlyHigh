/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <qwidget.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include "MDIWindow.h"

MDIWindow::MDIWindow(QWidget* parent, const char* name, int wflags)
   :QMainWindow(parent, name, wflags)
{
}

MDIWindow::~MDIWindow()
{
}
