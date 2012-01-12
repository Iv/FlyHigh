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

#include <QDate>
#include <QFile>
#include <QRegExp>
#include <QFileDialog>
#include <QHeaderView>
#include <QStringList>
#include <QTableWidget>
#include <QTextStream>
#include "IFlyHighRC.h"
#include "TableWindow.h"

TableWindow::TableWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags)
    :MDIWindow(parent, name, wflags)
{
	m_pTable = new QTableWidget(this);
//  m_pTable->verticalHeader()->hide();
  m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  setWidget(m_pTable);

	connect(m_pTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
}

TableWindow::~TableWindow()
{
}

QTableWidget* TableWindow::getTable()
{
	return m_pTable;
}

void TableWindow::setupHeader(const QStringList &colNameList)
{
	uint nofCols;

	// set the cols
	nofCols = colNameList.count();
	m_pTable->setColumnCount(nofCols);
	m_pTable->setHorizontalHeaderLabels(colNameList);
}

void TableWindow::setNumRows(int rows)
{
  QTableWidgetItem *pItem;
  int row;
  int col;

  m_pTable->setRowCount(rows);

  for(row=0; row<rows; row++)
  {
    for(col=0; col<m_pTable->columnCount(); col++)
    {
      pItem = new QTableWidgetItem();
      m_pTable->setItem(row, col, pItem);
    }

    m_pTable->resizeRowToContents(row);
  }

}

void TableWindow::selectRow(int row)
{
  m_pTable->clearSelection();
  m_pTable->selectRow(row);
  m_pTable->scrollToItem(m_pTable->item(row, 0));
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
	QDomElement meta;
	QDomText txt;
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
	meta = doc.createElement("META");
	meta.setAttribute("content","text/html; charset=UTF-8");
	head.appendChild(meta);
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

	nrows = m_pTable->rowCount();
	ncols = m_pTable->columnCount();

	// header
	tableRow = doc.createElement("tr");
	table.appendChild(tableRow);

	for(col=0; col<ncols; col++)
	{
		cell = doc.createElement("td");
		tableRow.appendChild(cell);
		bold = doc.createElement("b");
		txt = doc.createTextNode(m_pTable->horizontalHeaderItem(col)->text());
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
			txt = doc.createTextNode(m_pTable->item(row, col)->text());
			cell.appendChild(txt);
			tableRow.appendChild(cell);
		}
	}
}

void TableWindow::exportTable()
{
	QFile file;
	QString fileName;
	QString selected;
	QString string;
	QDomDocument doc;

	// suggest window title as filename, but replace uncommon characters
	fileName = this->windowTitle().replace(QRegExp("[^a-zA-Z0-9-_]"), "_");
	// and append date
	fileName += QDate::currentDate().toString("_dd_MM_yyyy");

	selected = QFileDialog::getSaveFileName(this,
																					tr("HTML file export"),
																					IFlyHighRC::pInstance()->lastDir() + QDir::separator()
																																						 + fileName
																																						 + ".html",
																					"HTML Files (*.html *.htm)");

	if(selected!="")
	{
		file.setFileName(selected);

		if(file.open(QFile::WriteOnly | QFile::Truncate))
		{
			QTextStream out(&file);

			tableAsHTML(doc);
			string = doc.toString();
			out << string.toLocal8Bit();
			out.flush();
			file.close();
		}
	}
}

void TableWindow::selectionChanged()
{
}
