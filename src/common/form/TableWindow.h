/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _TableWindow_h_
#define _TableWindow_h_

#include "MDIWindow.h"

class QStringList;
class QTable;

class TableWindow: public MDIWindow
{
	Q_OBJECT
	public:
		TableWindow(QWidget* parent, const char* name, int wflags);
		virtual ~TableWindow();

	protected:
		QTable* getTable();
		void setupHeader(const QStringList &colNameList);

	protected slots:
		virtual void clicked(int row, int col, int button, const QPoint &mousePos);

private:
	QTable* m_pTable;
};

#endif