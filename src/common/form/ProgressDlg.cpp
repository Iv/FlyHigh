/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#include <QApplication>
#include <QEventLoop>
#include <QProgressDialog>
#include "ProgressDlg.h"

ProgressDlg::ProgressDlg(QWidget *pCreator)
{
	m_pCreator = pCreator;
	m_pProgDlg = NULL;
}

ProgressDlg::~ProgressDlg()
{
	endProgress();
}

void ProgressDlg::beginProgress(const QString &label, QObject *pProgObj)
{
	endProgress();

	m_pProgDlg = new QProgressDialog(label, "Cancel", 0, 100, m_pCreator);

	connect(pProgObj, SIGNAL(progress(int)), this, SLOT(progress(int)));
	connect(m_pProgDlg, SIGNAL(canceled()), pProgObj, SLOT(cancel()));

	m_pProgDlg->setAutoClose(false);
	m_pProgDlg->show();
}

void ProgressDlg::endProgress()
{
	if(m_pProgDlg != NULL)
	{
		delete m_pProgDlg;
		m_pProgDlg = NULL;
	}
}

void ProgressDlg::progress(int percent)
{
	m_pProgDlg->setValue(percent);
	QApplication::processEvents(QEventLoop::AllEvents);
}
