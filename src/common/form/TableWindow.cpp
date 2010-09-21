/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <qmessagebox.h>
#include <qdir.h>
#include <qfile.h>
#include <q3filedialog.h>
#include <q3table.h>
#include <q3header.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <qrect.h>
#include <q3simplerichtext.h>
#include <qstringlist.h>
#include "IFlyHighRC.h"
#include "TableWindow.h"

TableWindow::TableWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags)
    :MDIWindow(parent, name, wflags)
{
	QStringList nameList;
	
	m_pTable = new Q3Table(this);
	m_pTable->show();
	setFocusProxy(m_pTable);
	setCentralWidget(m_pTable);

	connect(m_pTable, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

TableWindow::~TableWindow()
{
}

Q3Table* TableWindow::getTable()
{
	return m_pTable;
}

void TableWindow::setupHeader(const QStringList &colNameList)
{
	Q3Header *pHeader;
	uint colNr;
	uint nofCols;
	
	// set the cols
	nofCols = colNameList.count();
	m_pTable->setNumCols(nofCols);
	pHeader = m_pTable->horizontalHeader();
	
	for(colNr=0; colNr<nofCols; colNr++)
	{
		pHeader->setLabel(colNr, colNameList[colNr]);
	}
}

void TableWindow::tableAsHTML(QDomDocument &doc)
{
	QDomElement root;
	QDomElement table;
	QDomElement head;
	QDomElement body;
	QDomElement tableRow;
	QDomElement cell;
	QDomElement bold;
	QDomElement elem;
	QDomText txt;
	Q3Header *pHeader;
	int row;
	int nrows;
	int col;
	int ncols;
	
/*
	<html>
	<head>
		<title>"caption"</title>
	</head>
	<body>
	</body>
	*/
	root = doc.createElement("html");
	doc.appendChild(root);
	head = doc.createElement("head");
	root.appendChild(head);
	elem = doc.createElement("title");
        txt = doc.createTextNode(windowTitle());
	elem.appendChild(txt);
	head.appendChild(elem);
	body = doc.createElement("body");
	root.appendChild(body);
	
	// table
	table = doc.createElement("table");
	table.setAttribute("border", 1);
	body.appendChild(table);
	
	nrows = m_pTable->numRows();
	ncols = m_pTable->numCols();
	
	// header
	pHeader = m_pTable->horizontalHeader();
	tableRow = doc.createElement("tr");
	table.appendChild(tableRow);
	
	for(col=0; col<ncols; col++)
	{
		cell = doc.createElement("td");
		tableRow.appendChild(cell);
		bold = doc.createElement("b");
		txt = doc.createTextNode(pHeader->label(col));
		bold.appendChild(txt);
		cell.appendChild(bold);
	}
	
	// data
	for(row=0; row<nrows; row++)
	{
		tableRow = doc.createElement("tr");
		table.appendChild(tableRow);
		
		for(col=0; col<ncols; col++)
		{
			cell = doc.createElement("td");
			txt = doc.createTextNode(m_pTable->text(row, col));
			cell.appendChild(txt);
			tableRow.appendChild(cell);
		}
	}
}

void TableWindow::exportTable()
{
	const QDir *pDir;
	QFile file;
	QString fileName;
	QString string;
	QDomDocument doc;
	Q3FileDialog fileDlg(IFlyHighRC::pInstance()->lastDir(), "HTML Files (html.*)", this,
					"HTML file export", true);

	fileDlg.setMode(Q3FileDialog::AnyFile);

	if(fileDlg.exec() == QDialog::Accepted)
	{
		pDir =  fileDlg.dir();
		IFlyHighRC::pInstance()->setLastDir(pDir->absolutePath());
		delete pDir;
		fileName = fileDlg.selectedFile();
                file.setFileName(fileName + ".html");
		
		if(file.open(QIODevice::WriteOnly))
		{
                  QDataStream out(&file);
                  
			tableAsHTML(doc);
			string = doc.toString();
                        out << string;
			//file.write(string,  string.length());
			file.close();
		}
	}
}

void TableWindow::selectionChanged()
{
}
