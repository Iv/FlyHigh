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
	"Can not connect to SQL Database.", "Make sure that DB Name = 'flyhigh', UserName = 'flyhigh',\nPassword = 'flyhigh', localhost is reachable, Port = 3306.",
	"Can not execute SQL command.", "Make sure that SQL database is setup correct.",
	"Can not connect to Flytec device.", "Make sure that the device is connected to the right port,\n and is set to menu mode.",
	"Can not execute command on Flytec device.", "Make sure that the device is connected to the right port,\n and is set to menu mode."
};

void Error::show(ErrorIdType error)
{
	QString msg;
	
	msg = m_errorList[error].errorText + "\n";
	msg += m_errorList[error].solution;
	
	QMessageBox::warning(NULL, "Error", msg, QMessageBox::Ok, QMessageBox::NoButton);
}

void Error::verify(bool expression, ErrorIdType error)
{
	if(!expression)
	{
		show(error);
	}
}
