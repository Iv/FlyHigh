/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
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

#ifndef MainFrameImpl_h
#define MainFrameImpl_h

#include <vector>
#include <qthread.h>
#include "ui_MainFrame.h"

class Q3ProgressBar;
class Frame;

class MainFrameImpl: public QMainWindow, public Ui::MainFrame
{
	Q_OBJECT

	public:
		MainFrameImpl(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);

	public slots:
		virtual void addPage(QWidget * pFrame, int * pPos);

	protected slots:
		void open();
		void save();
		void read();
		void write();

	private:
		typedef std::vector<Frame*> FrameListType;
		typedef enum CmdType{ReadConfig, WriteConfig, OpenConfig, SaveConfig}CmdType;

		QByteArray m_flytecMem;
		FrameListType m_FrameList;
		Q3ProgressBar *m_pProgressBar;
		QString m_fileName;
		CmdType m_cmd;

		void execThreadCmd(CmdType cmd);
		void updateFrames();
		void storeFrames();
};

#endif

