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
#ifndef Tokenizer_h
#define Tokenizer_h

class QString;

/**
	@author Alex Graf <grafal@sourceforge.net>
*/
class Tokenizer
{
	public:
		Tokenizer();

		~Tokenizer();

		bool getFirstToken(const QString &tlg, char delim, QString &token);

    bool getFirstToken(const QString &tlg, const QString &delim, QString &token);

		bool getNextToken(const QString &tlg, char delim, QString &token);

		bool getNextToken(const QString &tlg, const QString &delim, QString &token);

	private:
		int m_pos;
};

#endif
