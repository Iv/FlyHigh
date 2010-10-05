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
 
#include <qcombobox.h>
#include <qlineedit.h>
 
#include "IFlyHighRC.h"
#include "IPortFrame.h"
#include "PortFrameImpl.h"

IPortFrame::IPortFrame(QWidget* parent)
{
	m_pFrame = new PortFrameImpl(parent);
	m_pFrame->lineEdit_Port->setText(IFlyHighRC::pInstance()->deviceLine());
        m_pFrame->comboBox_Baud->addItems(IFlyHighRC::pInstance()->deviceSpeedList());
}

IPortFrame::~IPortFrame()
{
	delete m_pFrame;
}

bool IPortFrame::show()
{
	bool ok;
	
	ok = (m_pFrame->exec() == QDialog::Accepted);
	
	if(ok)
	{
		IFlyHighRC::pInstance()->setDeviceLine(m_pFrame->lineEdit_Port->text());
                IFlyHighRC::pInstance()->setDeviceSpeed(m_pFrame->comboBox_Baud->currentIndex());
	}
	
	return ok;
}
