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
 
 #include <qlineedit.h>
 
#include "IPortFrame.h"
#include "PortFrameImpl.h"

IPortFrame::IPortFrame(QWidget* parent)
{
	m_pFrame = new PortFrameImpl(parent, "", Qt::WType_Modal);
}

IPortFrame::~IPortFrame()
{
	delete m_pFrame;
}

bool IPortFrame::show()
{
	return (m_pFrame->exec() == QDialog::Accepted);
}

void IPortFrame::getPort(QString &port)
{
	port = m_pFrame->lineEdit_Port->text();
}
