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
 
#include <qapplication.h>
#include "MainWindow.h"
#include "IFlyHighRC.h"
#include "IGPSDevice.h"
#include "Images.h"
#include "ISql.h"

int main( int argc, char ** argv ) 
{
	QApplication appl(argc, argv);
	MainWindow* pMainWin;
	int res;

	IFlyHighRC::pInstance()->loadRC();
	IGPSDevice::pInstance()->open();
	ISql::pInstance()->open();
	
	pMainWin = new MainWindow();
	appl.setMainWidget(pMainWin);
	pMainWin->setIcon(Images::pInstance()->getImage("logo.png"));
	
	pMainWin->show();
	appl.connect(&appl, SIGNAL(lastWindowClosed()), &appl, SLOT(quit()));
	
	res = appl.exec();

	// exit
	IGPSDevice::pInstance()->close();
	IFlyHighRC::pInstance()->saveRC();
	
	return res;
}
