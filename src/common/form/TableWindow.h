/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _TableWindow_h_
#define _TableWindow_h_

#include <qdom.h>
#include <qprinter.h>
#include "MDIWindow.h"

class QStringList;
class QTable;

class TableWindow: public MDIWindow
{
	Q_OBJECT
	
	public:
		TableWindow(QWidget* parent, const char* name, int wflags);
		virtual ~TableWindow();

		void tableAsHTML(QDomDocument &doc);

	protected slots:
		void print();
		
	protected:
		QTable* getTable();
		void setupHeader(const QStringList &colNameList);

private:
	QTable* m_pTable;
	QPrinter m_printer;
};

#endif
