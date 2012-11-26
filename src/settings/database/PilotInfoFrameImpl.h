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
#ifndef PilotInfoFrameImpl_h
#define PilotInfoFrameImpl_h

#include "ui_PilotInfoFrame.h"
#include "Glider.h"
#include "Pilot.h"

class PilotInfoFrameImpl: public QDialog, public Ui::PilotInfoFrame
{
	Q_OBJECT
	
	public:
		PilotInfoFrameImpl(QWidget* parent = 0);
		
		void updateGlider();
		void selectGlider(Glider &glider);
		Glider& currentGlider();
		void ok();

	protected slots:
		void newGlider();

	private:
		Pilot m_curPilot;
		Glider::GliderListType m_gliderList;
};

#endif
