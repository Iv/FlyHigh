/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                *
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

#include <QMenu>
#include <QString>
#include "MDIWindow.h"

MDIWindow::MDIWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags)
	:QMdiSubWindow(parent, wflags)
{
  m_pMenu = new QMenu(name, this);
	setWindowTitle(name);
	setAttribute(Qt::WA_DeleteOnClose);
	setMinimumSize(400, 200);
}

void MDIWindow::addAction(QAction *pAction)
{
  m_pMenu->addAction(pAction);
}

QMenu* MDIWindow::menu()
{
  return m_pMenu;
}
