/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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
#include <qcstring.h>
#include "MainFrame.h"

class QProgressBar;
class Frame;

class MainFrameImpl: public MainFrame, public QThread
{
	Q_OBJECT

	public:
		MainFrameImpl(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
		~MainFrameImpl();

	public slots:
		virtual void addPage(QWidget * pFrame, int * pPos);

	protected:
		void open();
		void save();
		void read();
		void write();
		void run();

	private:
		typedef std::vector<Frame*> FrameListType;
		typedef enum CmdType{ReadConfig, WriteConfig, OpenConfig, SaveConfig};

		QByteArray m_flytecMem;
		FrameListType m_FrameList;
		QProgressBar *m_pProgressBar;
		QString m_fileName;
		CmdType m_cmd;

		void execThreadCmd(CmdType cmd);
		void updateFrames();
		void storeFrames();
};

#endif

