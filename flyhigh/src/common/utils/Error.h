/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

 #ifndef Error_h
#define Error_h

#include <qstring.h>
#include <qvaluelist.h>

class Error
{
	public:
		typedef enum ErrorIdType
		{
			SQL_OPEN,
			SQL_CMD,
			FLYTEC_OPEN,
			FLYTEC_CMD,
			NOF_ERRORS
		};
	
		static void show(ErrorIdType error);
		static void verify(bool expression, ErrorIdType error);

	private:
		typedef struct
		{
			QString errorText;
			QString solution;
		}ErrorType;
		
		static const ErrorType m_errorList[NOF_ERRORS];
};

#endif
