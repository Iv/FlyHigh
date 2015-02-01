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
#include <QHttpMultiPart>
#include <QHttpPart>
#include "QJsonDocument.h"
#include "QJsonArray.h"
#include "QJsonParseError.h"
#include "QJsonObject.h"
#include "QJsonValue.h"
#include "Account.h"
#include "Flight.h"
#include "XContestUploader.h"

#include <QDebug>

const QString XContestUploader::XCONTEST_API_BASE_URL = "http://www.xcontest.org";
const QString XContestUploader::XCONTEST_TICKET_URL = "/api/gate/ticket/?key=%1&hash=%2";
const QString XContestUploader::XCONTEST_FLIGHT_URL = "/api/gate/request/?flight";
const QString XContestUploader::XCONTEST_API_KEY = "75F62615B235B6A0-C139EAE8D66FA6DD-632DD7669D2F2AD0";


XContestUploader::XContestUploader(Account* pAccount)
{
  m_pManager = new QNetworkAccessManager();
  QObject::connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(reply(QNetworkReply*)));

  m_Account = *pAccount;
  m_state = INIT;
}

XContestUploader::~XContestUploader()
{
  qDebug() << "Deleting uploader for XContest (user:" << m_Account.username() << ")";
  delete m_pManager;
}


void XContestUploader::uploadFlight(Flight* pFlight)
{
  qDebug() << "Uploading flight to XContest";


  // send ticket request
  QNetworkRequest request(getTicketUrl());
  m_pManager->get(request);

  qDebug() << "Ticket request sent";
}

void XContestUploader::reply(QNetworkReply* reply)
{
  qDebug() << "Get reply";

  // Reading attributes of the reply
  // e.g. the HTTP status code
  QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

  qDebug() << "Status code: " << statusCodeV;

  // no error received?
  if (reply->error() == QNetworkReply::NoError)
  {
    // read data from QNetworkReply here

    // Example 2: Reading bytes form the reply
    QByteArray bytes = reply->readAll(); // bytes
    QString string(bytes); // string
    qDebug() << "Got response: " << string;

    // try json parsing
    QJsonParseError e;
    QJsonDocument d = QJsonDocument::fromJson(bytes, &e);
    if(!d.isNull()) {
      qDebug() << d.toJson(QJsonDocument::Indented);
      QJsonObject obj = d.object();
      QJsonValue ticket = obj.value(QString("ticket"));
      QJsonValue error = obj.value(QString("error"));
      if(!ticket.isNull()) {
        qDebug() << "Ticket:" << ticket.toString();

        QMap<QString,QString> params;
        params.insert("key",XCONTEST_API_KEY);
        params.insert("ticket",ticket.toString());
        params.insert("hash",getHash(ticket.toString() + XCONTEST_API_KEY));
        params.insert("authticket","");

        QUrl reqUrl = urlEncodeParams(XCONTEST_API_BASE_URL + XCONTEST_FLIGHT_URL, params);
        qDebug() << "URL string: " << reqUrl.toString();

        QNetworkRequest request(reqUrl);

        QHttpMultiPart* pLogin = getLogin(ticket.toString());
        m_pManager->post(request,pLogin);
        qDebug() << "User Auth request sent";

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
}

QHttpMultiPart* XContestUploader::getLogin(QString ticket) const
{
  QHttpMultiPart* pLogin = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  QHttpPart usernamePart;
  usernamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"login[username]\""));
  usernamePart.setBody(m_Account.username().toUtf8());

  QString toHash = m_Account.password();
  toHash = toHash.toLower();
  QString loginStr = QCryptographicHash::hash(toHash.toUtf8(), QCryptographicHash::Md5).toHex();
  loginStr += ticket;
  loginStr += XCONTEST_API_KEY;

  QHttpPart pwPart;
  pwPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"login[hash]\""));
  pwPart.setBody(getHash(loginStr).toUtf8());

  pLogin->append(usernamePart);
  pLogin->append(pwPart);

  return pLogin;
}

QUrl XContestUploader::urlEncodeParams(QString baseUrl, QMap<QString,QString>& params)
{
  QString request(baseUrl);
  if(!params.isEmpty())
  {
    QMapIterator<QString,QString> iter(params);
    while(iter.hasNext())
    {
      iter.next();
      request.append("&");
      request.append(QUrl::toPercentEncoding(iter.key()));
      request.append("=");
      request.append(QUrl::toPercentEncoding(iter.value()));
    }
  }
  return QUrl(request);
}

QUrl XContestUploader::getTicketUrl()
{
  return QUrl(XCONTEST_API_BASE_URL + XCONTEST_TICKET_URL.arg(XCONTEST_API_KEY, getHash(XCONTEST_API_KEY)));
}

QString XContestUploader::getHash(QString str) {
  QString toHash = str + "f218bd8158be7def6608ec9910dbf25c";
  QByteArray hash = QCryptographicHash::hash(toHash.toUtf8(), QCryptographicHash::Sha1);
  return hash.toHex();
}
