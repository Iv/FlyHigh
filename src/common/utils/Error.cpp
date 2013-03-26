/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                         *                                                     *
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

#include <qmessagebox.h>
#include "Error.h"

const Error::ErrorType Error::m_errorList[NOF_ERRORS] =
{
	{
		QObject::tr("Can not connect to SQL Database."),
		QObject::tr("Check that you have installed mysqld which is up\n"
		"and running. Will try to create database...")
	},
	{
		QObject::tr("Can not execute SQL command."),
		QObject::tr("Check the SQL database setup.")
	},
	{
		QObject::tr("Can not delete."),
		QObject::tr("Make sure that this record is not used by others.")
	},
	{
		QObject::tr("Can not get pilot information."),
		QObject::tr("Please set pilot information, or check\n"
		"the SQL database setup.")
	},
	{
		QObject::tr("Can not insert route."),
		QObject::tr("Verify that route name is unique!")
	},
	{
		QObject::tr("Can not connect to Flytec device."),
		QObject::tr("Check that the device is connected to the\n"
	 	"right port. Check your permissions\n"
		"on the device file!")
	},
	{
		QObject::tr("Can not execute command on Flytec device."),
		QObject::tr("Check that the device is connected to the\n"
		"right port, and is set to menu mode.")
	},
	{
		QObject::tr("Can not open gnuplot."),
    QObject::tr("Check if gnuplot is properly installed\n"
    "and listed in PATH.")
	},
	{
		QObject::tr("Parsing file failed."),
    QObject::tr("Check that file has correct format.\n")
	}
};

void Error::show(ErrorIdType error)
{
	QString msg;

	msg = m_errorList[error].errorText + "\n";
	msg += m_errorList[error].solution;

	QMessageBox::warning(NULL, "Error", msg, QMessageBox::Ok, Qt::NoButton);
}

void Error::verify(bool expression, ErrorIdType error)
{
	if(!expression)
	{
		show(error);
	}
}
