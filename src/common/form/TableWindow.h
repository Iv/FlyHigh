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

#ifndef _TableWindow_h_
#define _TableWindow_h_

#include <QDomDocument>
#include <QPrinter>
#include "MDIWindow.h"

class QStringList;
class QTableWidget;

class TableWindow: public MDIWindow
{
	Q_OBJECT

	public:
		TableWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags);

		virtual ~TableWindow();

		void tableAsHTML(QDomDocument &doc);

	protected:
		QTableWidget* getTable();

		void setupHeader(const QStringList &colNameList);

		void setNumRows(int rows);

		void selectRow(int row);

	protected slots:
		void exportTable();

		virtual void selectionChanged();

	private:
		QTableWidget* m_pTable;
};

#endif
