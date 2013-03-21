#ifndef _ELEVATIONSET_H
/***************************************************************************
 *   Copyright (C) 2013 by Alex Graf                                       *
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
#define _ELEVATIONSET_H

#include <QObject>
#include <QString>
#include <QUrl>
#include "BoundBox.h"

class ElevationSet: public QObject
{
  Q_OBJECT

  public:
    ElevationSet();

    ElevationSet(const QString &target, const QString &url, const BoundBox &bbox);

    QString target(const QString &path) const;

    QString path(const QString &path) const;

    const QUrl& url() const;

    const BoundBox& boundBox() const;

  private:
    QString m_target;
    QUrl m_url;
    BoundBox m_bbox;
};
#endif
