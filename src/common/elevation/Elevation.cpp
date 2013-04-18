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
#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QNetworkReply>
#include <QProcess>
#include <QUrl>
#include <QString>
#include <unistd.h>
#include "Elevation.h"
#include "ElevationDef.h"
#include "FlightPoint.h"
#include "FlightPointList.h"
#include "IFlyHighRC.h"

#include <QDebug>

Elevation::Elevation()
{
  QDir dir;
  QString path;
  ElevationDef def;

  m_pReply = NULL;
  m_download = false;
  path = IFlyHighRC::pInstance()->elevationDir();
  dir.setPath(path);

  if(!dir.exists())
  {
    dir.mkpath(path);
  }

  m_pNetMgr = new QNetworkAccessManager(this);
  QObject::connect(m_pNetMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(netReply(QNetworkReply*)));

  def.addEuropeSet(m_setList);
  def.addWorldSet(m_setList);
  scanTiles();
}

Elevation::~Elevation()
{
  delete m_pNetMgr;
}

void Elevation::elevations(FlightPointList &fpList)
{
  BoundBox bbox;
  ElevationSetList::iterator setIt;
  ElevationSet *pSet;
  ElevationTileList::iterator tileIt;
  ElevationTileList affTileList(false);
  FlightPoint *pFp;
  QDir dir;
  QString question;
  QString path;
  double elevation;
  double prevElevation = 0;
  uint item;
  bool download;
  bool first = true;

  path = IFlyHighRC::pInstance()->elevationDir();

  // update affected sets
  fpList.boundBox(bbox);

  for(setIt=m_setList.begin(); setIt!=m_setList.end(); setIt++)
  {
    if(bbox.intersect((*setIt)->boundBox()) && !dir.exists((*setIt)->target(path)))
    {
      m_downloadList.push_back(*setIt);
    }
  }

  m_pReply = NULL;
  download = (m_downloadList.size() > 0);
  m_state = StatReqConfirm;

  if(download)
  {
    // wait for download
    while(download)
    {
      switch(m_state)
      {
        case StatReqConfirm:
          download = (m_downloadList.size() > 0);

          if(download)
          {
            pSet = m_downloadList.first();
            m_state = StatWaitConfirm;
            question = tr("Download set");
            question += QString(" (%1; %2) /").arg(pSet->boundBox().south()).arg(pSet->boundBox().west());
            question += QString(" (%3; %4)").arg(pSet->boundBox().north()).arg(pSet->boundBox().east());
            emit confirmDownload(this, question);
          }
        break;
        case StatDownload:
          if(m_pReply == NULL)
          {
            m_state = StatReqConfirm;
          }
        break;
        default:
        break;
      }

      qApp->processEvents();
      usleep(10000);
    }

    scanTiles();
  }

  // read elevations
qDebug() << "read elevations";

  for(tileIt=m_tileList.begin(); tileIt!=m_tileList.end(); tileIt++)
  {
    if(bbox.intersect((*tileIt)->boundBox()))
    {
      (*tileIt)->open();
qDebug() << "use" << (*tileIt)->path() << " " << (*tileIt)->type();
      affTileList.push_back(*tileIt);
    }
  }

  // lookup elevation for each FlightPoint
  for(item=0; item<fpList.size(); item++)
  {
    pFp = fpList[item];

    for(tileIt=affTileList.begin(); tileIt!=affTileList.end(); tileIt++)
    {
      if((*tileIt)->boundBox().isInside(pFp->pos()))
      {
        elevation = (*tileIt)->elevation(pFp->pos());

        if(first)
        {
          prevElevation = elevation;
          first = false;
        }

        if(elevation == -32768)
        {
          elevation = prevElevation;
        }
        else if(abs(prevElevation - elevation) > 1000)
        {
          elevation = prevElevation;
        }
        else
        {
          prevElevation = elevation;
        }

        pFp->setElevation(elevation);
        break;
      }
    }
  }

  // cleanup
  for(tileIt=affTileList.begin(); tileIt!=affTileList.end(); tileIt++)
  {
    (*tileIt)->close();
  }

  affTileList.clear();
}

void Elevation::nextDownload(bool doit)
{
  QNetworkRequest request;
  ElevationSet *pSet;

  if(m_state == StatWaitConfirm)
  {
    pSet = m_downloadList.first();
    m_downloadList.removeFirst();

    if(doit)
    {
      m_state = StatDownload;
      request.setUrl(pSet->url());
      request.setOriginatingObject(pSet);
      m_pReply = m_pNetMgr->get(request);
qDebug() << "send request " << request.url();

      connect(m_pReply, SIGNAL(downloadProgress(qint64, qint64)),
              this, SLOT(downloadProgress(qint64, qint64)));
    }
    else
    {
      m_state = StatReqConfirm;
    }
  }
}

void Elevation::cancel()
{
  if(m_pReply != NULL)
  {
    m_pReply->abort();
    m_downloadList.clear();
  }
}

void Elevation::scanTiles()
{
  QStringList fileList;
  QStringList::iterator fileIt;
  QFileInfo fileInfo;
  QDir dir;
  QString rootPath;
  QString path;
  ElevationSetList::iterator setIt;

  rootPath = IFlyHighRC::pInstance()->elevationDir();
  m_tileList.clear();

  for(setIt=m_setList.begin(); setIt!=m_setList.end(); setIt++)
  {
    path = (*setIt)->target(rootPath);

    if(dir.exists(path))
    {
      fileInfo.setFile(path);

      if(fileInfo.isDir())
      {
        QDirIterator iterator(path, QDirIterator::Subdirectories);

        while(iterator.hasNext())
        {
          iterator.next();

          if(iterator.fileInfo().isFile())
          {
            addTile(iterator.fileInfo());
          }
        }
      }
      else
      {
        addTile(fileInfo);
      }
    }
  }
}

void Elevation::addTile(const QFileInfo &fileInfo)
{
  ElevationTile *pTile;
  QString coord;
  double lat;
  double lon;

  pTile = new ElevationTile(fileInfo.filePath());
  coord = fileInfo.baseName();

  // p.e. N66W038
  if(coord.size() == 7)
  {
    lat = coord.mid(1, 2).toDouble();

    if(coord.mid(0, 1) == "S")
    {
      lat = -lat;
    }

    lon = coord.mid(4, 3).toDouble();

    if(coord.mid(3, 1) == "W")
    {
      lon = -lon;
    }

    if(fileInfo.size() == (3601 * 3601 * 2)) // SRTM 1
    {
      pTile->setBoundBox(BoundBox(LatLng(lat, lon), LatLng(lat + 1, lon + 1)));
      pTile->setType(ElevationTile::Srtm1);
    }
    else if(fileInfo.size() == (1201 * 1201 * 2)) // SRTM 3
    {
      pTile->setBoundBox(BoundBox(LatLng(lat, lon), LatLng(lat + 1, lon + 1)));
      pTile->setType(ElevationTile::Srtm3);
    }
  }

  m_tileList.push_back(pTile);
}

bool Elevation::unzip(const QString &fileName, const ElevationSet *pSet)
{
  enum RetCode{RetNormal = 0, RetNoMatchFile = 11};
  QProcess proc(this);
  QStringList arguments;
  QString rootPath;
  QString path;
  QDir dir;
  int ret;

  rootPath = IFlyHighRC::pInstance()->elevationDir();

  // if dir does not exist, create it
  arguments << "-qql" << fileName << pSet->path("");
qDebug() << "test dir: " << arguments;

  proc.start("unzip", arguments);
  proc.waitForFinished();
  ret = proc.exitCode();
qDebug() << "exit code: " << ret;

  if(ret == RetNoMatchFile)
  {
    path = pSet->path(rootPath);

    if(!dir.exists(path))
    {
      dir.mkdir(path);
qDebug() << "mkdir " << path;
    }
  }

  // unzip
  arguments.clear();
  arguments << "-o" << "-d" << path << fileName;
qDebug() << "unzip: " << arguments;

  proc.start("unzip", arguments);
  proc.waitForFinished();

ret = proc.exitCode();
qDebug() << "exit code: " << ret;

  return (ret == RetNormal);
}

void Elevation::netReply(QNetworkReply *pReply)
{
  QString rootPath;
  QString path;
  QString fileName;
  QFile file;
  ElevationSet *pSet;
  int status;
  bool success = false;

qDebug() << "netReply";

  if(pReply->error() == QNetworkReply::NoError)
  {
    status = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();

    if((status == 200) && pReply->isReadable())
    {
      path = pReply->request().url().path();
      fileName = QFileInfo(path).fileName();

      if(!fileName.isEmpty())
      {
        rootPath = IFlyHighRC::pInstance()->elevationDir();
        file.setFileName(rootPath + "/" + fileName);

        if(file.open(QIODevice::WriteOnly))
        {
int count;
          count = file.write(pReply->readAll());
          file.close();

qDebug() << "saved " << count << " bytes to " << file.fileName();

          // unzip
          pSet = qobject_cast<ElevationSet*>(pReply->request().originatingObject());
          unzip(file.fileName(), pSet);
          success = true;
        }
      }
    }
  }

  if(!success)
  {
    // fail
  }

  pReply->deleteLater();
  m_pReply = NULL;
}

void Elevation::downloadProgress(qint64 recieved, qint64 total)
{
  int prog;

  prog = (int)((double)recieved * 100.0 / (double)total);
/// qDebug() << "progress: " << prog;

  emit progress(prog);
}
