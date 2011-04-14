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

#include <QApplication>
#include <QIcon>
#include <QTextCodec>
#include <QHostInfo>
#include <QInputDialog>
#include <QDir>
#include <QDebug>
#include "MainWindow.h"
#include "IFlyHighRC.h"
#include "IGPSDevice.h"
#include "ISql.h"


int main( int argc, char ** argv ) 
{
	QApplication appl(argc, argv);
	QString root;
	QString pwd;
	QFileInfo dbfile;
	Q_INIT_RESOURCE(res);
	MainWindow* pMainWin;
	int res;
	bool ok;

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	// read config file
	IFlyHighRC::pInstance()->loadRC();

	DatabaseParameters dbparam = IFlyHighRC::pInstance()->getDBParameters();
	ISql::pInstance()->setDBParameters(dbparam);

	// which type of db?
	if (dbparam.isMySQL())
	{
		// check for server host
		QHostInfo info = QHostInfo::fromName(dbparam.dBHostName());
		if(info.error()!=QHostInfo::NoError)
		{
			// todo: show error dialog
			qDebug() << info.errorString();
		}

		if(!ISql::pInstance()->connectDb())
		{
			root = QInputDialog::getText(NULL, QObject::tr("Name"), QObject::tr("MySQL root name:"), QLineEdit::Normal, "root", &ok);
			pwd = QInputDialog::getText(NULL, QObject::tr("Password"), QObject::tr("MySQL root pwd:"), QLineEdit::Password, "", &ok);

			if(ok)
			{
				ISql::pInstance()->createDb(root, pwd);
				ISql::pInstance()->connectDb();
			}
		}
	}
	else if (dbparam.isSQLite())
	{
		dbfile = QFileInfo(dbparam.dBFile());

    // create directory if necessary
		if (!dbfile.dir().exists())
    {
			QDir().mkpath(dbfile.absolutePath());
    }

    if(!ISql::pInstance()->connectDb())
    {
      ISql::pInstance()->createDb();
      ISql::pInstance()->connectDb();
    }
	}
	else
	{
		qDebug() << "Unknown db type '" << dbparam.dBType() << "'.";
		return 1;
	}
	
	pMainWin = new MainWindow();
	pMainWin->setWindowIcon(QIcon(":/flyhigh.png"));
	
	pMainWin->show();
	appl.connect(&appl, SIGNAL(lastWindowClosed()), &appl, SLOT(quit()));
	
	res = appl.exec();

	// exit
	IGPSDevice::pInstance()->close();
	IFlyHighRC::pInstance()->saveRC();

	delete IGPSDevice::pInstance();
	delete IFlyHighRC::pInstance();
	delete ISql::pInstance();
	
	return res;
}
