/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <qmessagebox.h>

#include <qtable.h>
#include <qheader.h>
#include <qstringlist.h>
#include "TableWindow.h"

TableWindow::TableWindow(QWidget* parent, const char* name, int wflags)
    :MDIWindow(parent, name, wflags)
{
	QStringList nameList;
	
	m_pTable = new QTable(this);
	
	connect(m_pTable, SIGNAL(clicked(int, int, int, const QPoint&)),
			this, SLOT(clicked(int, int, int, const QPoint&)));

	m_pTable->show();
	
	setFocusProxy(m_pTable);
	setCentralWidget(m_pTable);
}

TableWindow::~TableWindow()
{
}

QTable* TableWindow::getTable()
{
	return m_pTable;
}

void TableWindow::setupHeader(const QStringList &colNameList)
{
	QHeader *pHeader;
	unsigned int colNr;
	unsigned int nofCols;
	
	// set the cols
	nofCols = colNameList.count();
	m_pTable->setNumCols(nofCols);
	pHeader = m_pTable->horizontalHeader();
	
	for(colNr=0; colNr<nofCols; colNr++)
	{
		pHeader->setLabel(colNr, colNameList[colNr]);
	}
}

void TableWindow::clicked(int row, int col, int button, const QPoint & mousePos)
{
}
