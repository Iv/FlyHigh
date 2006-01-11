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
#ifndef FlyHighRCFrameImpl_h
#define FlyHighRCFrameImpl_h

#include "FlyHighRCFrame.h"

class FlyHighRCFrameImpl: public FlyHighRCFrame
{
	Q_OBJECT
	
	public:
		FlyHighRCFrameImpl(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
		
		void updateGlider();
		void selectGlider(const QString &name);

	protected slots:
		void newGlider();
};

#endif
