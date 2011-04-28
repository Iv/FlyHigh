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

#ifndef MIGRATORTHREAD_H
#define MIGRATORTHREAD_H

#include <QThread>
#include <DatabaseParameters.h>

class Migrator;

/**
 * Small class for threading db migration
 */
class MigratorThread : public QThread
{
		Q_OBJECT

public:

		MigratorThread(QWidget* parent);
		~MigratorThread();

		bool stop;
		void run();

		void init(DatabaseParameters fromDatabaseWidget, DatabaseParameters toDatabaseWidget);

		Migrator* migrator() const;

private:
		Migrator* m_pDBMigrator;
		DatabaseParameters m_FromDBParameters;
		DatabaseParameters m_ToDBParameters;
};

#endif
