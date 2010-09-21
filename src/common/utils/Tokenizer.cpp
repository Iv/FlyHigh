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
#include <qstring.h>
#include "Tokenizer.h"

Tokenizer::Tokenizer()
{
	m_pos = 0;
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::getFirstToken(const QString &tlg, char delim, QString &token)
{
	m_pos = 0;

	return getNextToken(tlg, delim, token);
}

bool Tokenizer::getNextToken(const QString &tlg, char delim, QString &token)
{
	int end;
	bool found;

	token = "";
        end = tlg.indexOf(delim, m_pos);
	found = (end > 0);

	if(found)
	{
		token = tlg.mid(m_pos, (end - m_pos));
		m_pos = (end + 1);
	}
	else
	{
		token = tlg.mid(m_pos, (tlg.length() - m_pos));
	}

	return found;
}
