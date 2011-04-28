/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#ifndef MIGRATOR_H
#define MIGRATOR_H

#include <QObject>
#include <QSqlDatabase>
#include "DatabaseParameters.h"

/**
 * Facilities for database migration
 */
class Migrator : public QObject
{
    Q_OBJECT

public:

    enum FinishStates
    {
        success,
        failed,
        canceled
    };

public:

		Migrator();
		~Migrator();

    void copyDatabases(DatabaseParameters fromDBParameters, DatabaseParameters toDBParameters);

Q_SIGNALS:

    void stepStarted(QString stepName);
    void smallStepStarted(int currValue, int maxValue);
    void finished(int finishState, QString errorMsg);

public Q_SLOTS:

    void stopProcessing();

private:

		void handleClosing(bool isstopThread);

private:

		QSqlDatabase m_FromDB;
		QSqlDatabase m_ToDB;
		bool m_stopProcessing;
};

#endif
