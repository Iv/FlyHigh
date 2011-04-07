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

#include <iostream>
#include <QApplication>
#include <QIcon>
#include <QTextCodec>
#include <QHostInfo>
#include "MainWindow.h"
#include "IFlyHighRC.h"
#include "IGPSDevice.h"
#include "ISql.h"

#include <QInputDialog>
#include <QDir>

int main( int argc, char ** argv ) 
{
	QApplication appl(argc, argv);
	QString root;
	QString pwd;
	Q_INIT_RESOURCE(res);
	MainWindow* pMainWin;
	int res;
	bool ok;

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	IFlyHighRC::pInstance()->loadRC();

	// which type of db?
	QString dbtype = IFlyHighRC::pInstance()->dBType();
	if (dbtype.contains("mysql",Qt::CaseInsensitive))
	{
		QString dbhost = IFlyHighRC::pInstance()->dBHost();
		QHostInfo info = QHostInfo::fromName(dbhost);

		if(info.error()==QHostInfo::NoError)
		{
			ISql::pInstance()->setHostName(dbhost);
		}
		else
		{
			// todo: show error dialog
			std::cerr << info.errorString().toStdString() << std::endl;
		}

    ISql::pInstance()->setDriverName("QMYSQL");

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
	else if (dbtype.contains("sqlite",Qt::CaseInsensitive))
	{
    ISql::pInstance()->setDriverName("QSQLITE");
    // suggest db location in <userhome>/Flights/
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("Flights");

    // create directory if necessary
    if (!QDir(path).exists())
    {
      QDir().mkdir(path);
    }

    path.append(QDir::separator()).append("flyhigh_v2.sqlite");
    path = QDir::toNativeSeparators(path);
    ISql::pInstance()->setName(path);

    if(!ISql::pInstance()->connectDb())
    {
      ISql::pInstance()->createDb();
      ISql::pInstance()->connectDb();
    }
	}
	else
	{
		std::cerr << "Unknown db type '" << dbtype.toStdString() << "'." << std::endl;
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
