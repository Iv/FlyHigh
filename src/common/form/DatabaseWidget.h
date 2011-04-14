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

#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QWidget>
#include "DatabaseParameters.h"

class QComboBox;
class QSpinBox;
class QLineEdit;
class QPushButton;
class QLabel;
class QGroupBox;

/**
 * Database configuration widget
 * The source was lamely stolen and slipstreamed from www.digikam.org. Sorry & Thanks!
 */
class DatabaseWidget : public QWidget
{
	Q_OBJECT

public:

	DatabaseWidget(QWidget* parent=0);
	~DatabaseWidget();
	DatabaseParameters getDatabaseParameters();
	void setDatabaseInputFields(const QString&);

public Q_SLOTS:

	void slotHandleDBTypeIndexChanged(int index);
	void slotDBFileSelected();
	void checkDatabaseConnection();

private:

	QComboBox* m_pDBType;
	QSpinBox* m_pDBPort;
	QLineEdit* m_pDBName;
	QLineEdit* m_pDBPath;
	QLineEdit* m_pDBHostName;
	QLineEdit* m_pDBUserName;
	QLineEdit* m_pDBPassword;
	QPushButton* m_pFileButton;
	QLabel* m_pDBPathLabel;
	QGroupBox* m_pMySQLSettings;

};

#endif
