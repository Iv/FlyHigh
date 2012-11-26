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

#include "IPilotInfoFrame.h"
#include "PilotInfoFrameImpl.h"

IPilotInfoFrame::IPilotInfoFrame(QWidget* parent)
{
	m_pFrame = new PilotInfoFrameImpl(parent);
}

IPilotInfoFrame::~IPilotInfoFrame()
{
	delete m_pFrame;
}

bool IPilotInfoFrame::show()
{
	bool ok;
	
	ok = (m_pFrame->exec() == QDialog::Accepted);
	
	if(ok)
	{
		m_pFrame->ok();
	}
	
	return ok;
}
