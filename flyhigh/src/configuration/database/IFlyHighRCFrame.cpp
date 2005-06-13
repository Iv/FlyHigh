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

#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qlineedit.h>
#include "IFlyHighRC.h"
#include "IFlyHighRCFrame.h"
#include "FlyHighRCFrameImpl.h"

IFlyHighRCFrame::IFlyHighRCFrame(QWidget* parent)
{
	m_pFrame = new FlyHighRCFrameImpl(parent, "", Qt::WType_Modal);
	
	m_pFrame->lineEdit_PilotName->setText(IFlyHighRC::pInstance()->pilotName());
	m_pFrame->dateEditDateOfBirth->setDate(IFlyHighRC::pInstance()->pilotBirth());
	m_pFrame->lineEdit_GliderID->setText(IFlyHighRC::pInstance()->callsign());
}

IFlyHighRCFrame::~IFlyHighRCFrame()
{
	delete m_pFrame;
}

bool IFlyHighRCFrame::show()
{
	bool ok;
	
	ok = (m_pFrame->exec() == QDialog::Accepted);
	
	if(ok)
	{
		IFlyHighRC::pInstance()->setPilotName(m_pFrame->lineEdit_PilotName->text());
		IFlyHighRC::pInstance()->setPilotBirth(m_pFrame->dateEditDateOfBirth->date());
		IFlyHighRC::pInstance()->setCallsign(m_pFrame->lineEdit_GliderID->text());
	}
	
	return ok;
}
