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
 
#ifndef IGPSDevice_h
#define IGPSDevice_h

#include <qdatetime.h>
#include "IDataBase.h"

class IGPSDevice: public IDataBase
{
	public:
		IGPSDevice();
		virtual ~IGPSDevice();
	
		static IGPSDevice* pInstance();
		
		virtual bool open();

		virtual void close();

		// memory
		virtual bool memoryRead(QByteArray &arr);

		virtual bool memoryWrite(QByteArray &arr);
		
		// update
		int flightsLastModified();

		int wayPointsLastModified();

		int routesLastModified();

		int airspacesLastModified();
		
	protected:
		typedef enum {Flights, WayPoints, Routes, AirSpaces, NofFields}FieldType;
		
		void setLastModified(FieldType field);
		
	private:
		static IGPSDevice *m_pGPSDevice;
		int m_lastModifiedList[NofFields];
		uint m_oldDevice;

		uint oldDevice();

		void setOldDevice(uint devNr);
};

#endif
