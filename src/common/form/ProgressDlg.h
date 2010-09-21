/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                     *
 *   grafal@sourceforge.net                                                         *
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
 
#ifndef ProgressDlg_h
#define ProgressDlg_h

#include <qobject.h>

class Q3ProgressDialog;
class QWidget;

/* 
add this to pProgObj
	public:
	signals:
	void progress(int percent); // show progress: emit progress(int);
	public slots:
	virtual void cancel(); // cancel the process
*/

class ProgressDlg: public QObject
{
	Q_OBJECT
	public:
		ProgressDlg(QWidget *pCreator);
		~ProgressDlg();
		
		void beginProgress(const QString &label, QObject *pProgObj);
		void endProgress();
		
	private:
		QWidget *m_pCreator;
		Q3ProgressDialog *m_pProgDlg;
		
	private slots:
		void progress(int percent);
};

#endif
