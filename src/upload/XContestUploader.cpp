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

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCryptographicHash>
#include "QJsonDocument.h"
#include "QJsonParseError.h"
#include "QJsonObject.h"
#include "QJsonValue.h"
#include "Account.h"
#include "Flight.h"
#include "XContestUploader.h"

#include <QDebug>

const QString XContestUploader::XCONTEST_API_BASE_URL = "http://www.xcontest.org/api/gate";
const QString XContestUploader::XCONTEST_TICKET_URL = "/ticket/?key=%1&hash=%2";
const QString XContestUploader::XCONTEST_API_KEY = "75F62615B235B6A0-C139EAE8D66FA6DD-632DD7669D2F2AD0";


XContestUploader::XContestUploader(Account* pAccount)
{
  qDebug() << "Creating uploader for XContest";
}

XContestUploader::~XContestUploader()
{
  qDebug() << "Deleting uploader for XContest";
}


void XContestUploader::uploadFlight(Flight* pFlight)
{
  qDebug() << "Uploading flight to XContest";

  QNetworkAccessManager* pManager = new QNetworkAccessManager();
  QObject::connect(pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

  QString urlstr = XCONTEST_API_BASE_URL + XCONTEST_TICKET_URL.arg(XCONTEST_API_KEY, getHash(XCONTEST_API_KEY));
  qDebug() << "URL string: " << urlstr;

  QUrl url(urlstr);
  QNetworkRequest request(url);
  pManager->get(request);


  //QNetworkReply reply = pManager->post(request, params.encodedQuery());


}

void XContestUploader::finishedSlot(QNetworkReply* reply)
{
    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this

    qDebug() << "Status code: " << statusCodeV;

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here

        // Example 2: Reading bytes form the reply
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        qDebug() << "Got response: " << string;

        // try json parsing
        QJsonParseError e;
        QJsonDocument d = QJsonDocument::fromJson(bytes, &e);
        if(!d.isNull()) {
          qDebug() << d.toJson(QJsonDocument::Compact);
          QJsonObject obj = d.object();
          QJsonValue ticket = obj.value(QString("ticket"));
          QJsonValue error = obj.value(QString("error"));
          if(!ticket.isNull()) {
            qDebug() << "Ticket:" << ticket.toString();
          } else if(!error.isNull()) {
            QJsonObject errObj = error.toObject();
            qDebug() << "Error message: " << errObj["message"].toString();
          }

        } else {
          qDebug() << e.errorString();
        }

    } else  {

      qDebug() << "Got error response: " << reply->errorString();
        // handle errors here
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    //delete reply;
}

QString XContestUploader::getHash(QString str) {
  QString toHash = str + "f218bd8158be7def6608ec9910dbf25c";
  QByteArray hash = QCryptographicHash::hash(toHash.toUtf8(), QCryptographicHash::Sha1);
  return hash.toHex();
}
