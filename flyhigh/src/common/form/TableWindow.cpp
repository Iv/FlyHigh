/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <qmessagebox.h>

#include <qtable.h>
#include <qheader.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qrect.h>
#include <qsimplerichtext.h>
#include <qstringlist.h>
#include "TableWindow.h"

#include <qfile.h>

TableWindow::TableWindow(QWidget* parent, const char* name, int wflags)
    :MDIWindow(parent, name, wflags)
{
	QStringList nameList;
	
	m_pTable = new QTable(this);
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
	QHeader *pHeader;
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
	txt = doc.createTextNode(caption());
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

void TableWindow::print()
{
#ifndef QT_NO_PRINTER
	QFont font("Arial", 8);
	QString string;
	m_printer.setFullPage(true);
	QPaintDeviceMetrics screen(this);
	QDomDocument doc;
	int page = 1;
	
	m_printer.setResolution(screen.logicalDpiY());
	m_printer.setOrientation(QPrinter::Landscape);
	m_printer.setPrintProgram("kprinter"); // this is a workaround, because qt won't print with cups
	
	if(m_printer.setup(this))
	{
		QPainter p(&m_printer);
		QPaintDeviceMetrics metrics(p.device());
		int dpix = metrics.logicalDpiX();
		int dpiy = metrics.logicalDpiY();
		const int margin = 15;
		
		p.setFont(font);
		
		QRect body(margin * dpix / 72, margin * dpiy / 72,
						metrics.width() - margin * dpix / 72 * 2,
						metrics.height() - margin * dpiy / 72 * 2);

		tableAsHTML(doc);
		string = doc.toString();

/*	
		QFile f("table.html");
		f.open(IO_WriteOnly);
		f.writeBlock(string, qstrlen(string));
		f.close();
*/
		QSimpleRichText richText(string, font);
		richText.setWidth(&p, body.width());
		QRect view(body);
		
		while(true)
		{
			richText.draw(&p, body.left(), body.top(), view, colorGroup());
			view.moveBy(0, body.height());
			p.translate(0 , -body.height());
			p.setFont(font);
			p.drawText(view.right() - p.fontMetrics().width(QString::number(page)),
			view.bottom() + p.fontMetrics().ascent() + 5, QString::number(page));
			
			if (view.top() >= richText.height())
			{
				break;
			}
			m_printer.newPage();
			page++;
		}
	}
#endif
}
