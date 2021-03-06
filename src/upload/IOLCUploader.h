/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                *
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

#ifndef IOLCUploader_h
#define IOLCUploader_h

#include <QObject>

class Flight;
class QString;

/**
 * Interface for all Online Contest Uploader classes
 */
class IOLCUploader : public QObject
{
public:

  virtual void uploadFlight(Flight* pFlight) = 0;

signals:
  /** upload process successfuly completed */
  virtual void finished() = 0;

  /** upload process aborted with error */
  virtual void error(const QString& message) = 0;

  /** upload process progress information. */
  virtual void step(const QString& step, const int& percent) = 0;

};

#endif
