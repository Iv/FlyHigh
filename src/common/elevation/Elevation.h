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
#ifndef _ELEVATION_H
#define _ELEVATION_H

#include <QObject>
#include "ElevationTileList.h"
#include "ElevationSetList.h"

class QFileInfo;
class QNetworkAccessManager;
class QNetworkReply;
class ElevationDef;
class FlightPointList;

class Elevation: public QObject
{
  Q_OBJECT

  public:
    Elevation();

    ~Elevation();

    void elevations(FlightPointList &fpList);

    void nextDownload(bool doit);

	signals:
		void progress(int percent);

		void confirmDownload(Elevation *pElevation, const QString &question);

	public slots:
		void cancel();

  private:
    typedef QList<ElevationSet*> DownloadList;

    typedef enum States{StatDownload, StatReqConfirm, StatWaitConfirm}States;

    ElevationTileList m_tileList;
    ElevationSetList m_setList;
    DownloadList m_downloadList;
    QNetworkAccessManager* m_pNetMgr;
    QNetworkReply *m_pReply;
    States m_state;
    bool m_download;

    void scanTiles();

    void addTile(const QFileInfo &fileInfo);

    bool unzip(const QString &fileName, const ElevationSet *pSet);

  private slots:
    void netReply(QNetworkReply *pReply);

    void downloadProgress(qint64 recieved, qint64 total);
};
#endif
