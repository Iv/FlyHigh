/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org
	for more information.
*/

#ifndef _TableWindow_h_
#define _TableWindow_h_

#include <QDomDocument>
#include <QPrinter>
#include "MDIWindow.h"

class QStringList;
class Q3Table;

class TableWindow: public MDIWindow
{
	Q_OBJECT

	public:
		TableWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags);

		virtual ~TableWindow();

		void tableAsHTML(QDomDocument &doc);

	protected:
		Q3Table* getTable();

		void setupHeader(const QStringList &colNameList);

		void selectRow(int row);

	private:
		/**
		 * Fills in data into the table object
		 */
		virtual void populateTable()=0;

	protected slots:
		void exportTable();

		virtual void selectionChanged();

	private:
		Q3Table* m_pTable;
};

#endif
