/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <QWidget>
#include <QString>
#include "MDIWindow.h"


MDIWindow::MDIWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags)
	:QMainWindow(parent, wflags)
{
	setWindowTitle(name);

	connect(this,SIGNAL(dataChanged()),this,SLOT(refresh()));
}
