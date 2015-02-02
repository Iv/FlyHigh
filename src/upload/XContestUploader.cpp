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
  QObject::connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleEvent(QNetworkReply*)));

  // create local copy of account
  m_Account = *pAccount;
  m_state = INIT;
  m_needTicket = true;
  m_Ticket = "";
  m_SessionId = "";
}

XContestUploader::~XContestUploader()
{
  qDebug() << "Deleting uploader for XContest (user:" << m_Account.username() << ")";
  delete m_pManager;
}


void XContestUploader::uploadFlight(Flight* pFlight)
{
  qDebug() << "Uploading flight to XContest";
  m_pFlight = pFlight;

  // call slot manually for kicking off state-machine
  handleEvent(NULL);
}

void XContestUploader::handleEvent(QNetworkReply* reply)
{

  if(reply == NULL && m_needTicket) {
    // probably invoked manually
    // send ticket request
    sendTicketRequest();
    return;
  }

  // Reading attributes of the reply
  // e.g. the HTTP status code
  QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  qDebug() << "Got reply with status code " << statusCodeV;

  if (reply->error() == QNetworkReply::NoError)
  {
    // read data from QNetworkReply here
    QByteArray bytes = reply->readAll();
    qDebug() << "Got response: " << QString(bytes);

    // try json parsing
    QJsonParseError e;
    QJsonDocument d = QJsonDocument::fromJson(bytes, &e);

    if(!d.isNull()) {
      qDebug() << "JSON response: " << d.toJson(QJsonDocument::Indented);

      QJsonObject obj = d.object();
      QJsonValue error = obj.value(QString("error"));

      if(!error.isNull())
      {
        QJsonObject errObj = error.toObject();
        qDebug() << "Error message: " << errObj["message"].toString();
        return;
      }

      if(m_needTicket) {
        // expecting response to ticket request
        m_needTicket = false;
        QJsonValue ticket = obj.value(QString("ticket"));
        if(!ticket.isNull())
        {
          qDebug() << "Ticket:" << ticket.toString();
          m_Ticket = ticket.toString();
        } else
        {
          qDebug() << "Not a valid Ticket";
        }
      }

      // state machine
      if(m_state == INIT)
      {
        // post request
        sendClaimRequest();
        m_state = CLAIM;
      } else if(m_state == CLAIM)
      {
        // expecting claim reply
        QJsonValue success = obj.value(QString("success"));
        if(!success.isNull())
        {
          bool ok = success.toBool();
          if(ok)
          {
            qDebug() << "Request successful";
            // read session id
            QJsonValue sessionId = obj.value(QString("authTicket"));
            if(!sessionId.isNull())
            {
              qDebug() << "Session ID:" << sessionId.toString();
              m_SessionId = sessionId.toString();

              QJsonValue form = obj.value(QString("form"));
              if(!form.isNull())
              {
                QJsonObject formObj = form.toObject();
                QJsonValue valid = formObj.value(QString("isValid"));
                if(!valid.isNull())
                {
                  qDebug() << "Form is valid: " << valid.toBool();
                  QJsonValue phase = formObj.value(QString("phase"));
                  if(!phase.isNull())
                  {
                    qDebug() << "Phase: " << phase.toDouble();
                  }
                }
              } else
              {
                qDebug() << "No valid form in response";
              }
            } else
            {
              qDebug() << "No valid session id";
            }
          } else
          {
            qDebug() << "Claim failed";
          }
        } else
        {
          qDebug() << "Response invalid";
        }
      }

    } else
    {
      qDebug() << "Could not parse response" << e.errorString();
    }
  } else
  {
    qDebug() << "Got error response: " << reply->errorString();
  }
}

void XContestUploader::sendTicketRequest()
{
  QNetworkRequest request(getTicketUrl());
  m_pManager->get(request);

  qDebug() << "Ticket request sent";
}

void XContestUploader::sendClaimRequest()
{
  QMap<QString,QString> params;
  QUrl reqUrl;
  QNetworkRequest request;
  QHttpMultiPart* pClaim;

  params.insert("key",XCONTEST_API_KEY);
  params.insert("ticket",m_Ticket);
  params.insert("hash",getHash(m_Ticket + XCONTEST_API_KEY));
  params.insert("authticket",m_SessionId);

  reqUrl = urlEncodeParams(XCONTEST_API_BASE_URL + XCONTEST_FLIGHT_URL, params);
  qDebug() << "URL string: " << reqUrl.toString();

  request = QNetworkRequest(reqUrl);

  pClaim = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  addAuthControls(pClaim);
  addFlightControls(pClaim);

  m_pManager->post(request,pClaim);
  qDebug() << "Flight claim request sent";
}

void XContestUploader::addAuthControls(QHttpMultiPart* pForm) const
{
  QString toHash;
  QHttpPart usernamePart;
  QHttpPart pwPart;

  usernamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"login[username]\""));
  usernamePart.setBody(m_Account.username().toUtf8());

  // login[hash] is SHA1 hash of: md5(password in lowercase)+ticket+api_key+shared_secret
  toHash = m_Account.password();
  toHash = toHash.toLower();
  toHash = QCryptographicHash::hash(toHash.toUtf8(), QCryptographicHash::Md5).toHex();
  toHash += m_Ticket;
  toHash += XCONTEST_API_KEY;

  pwPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"login[hash]\""));
  pwPart.setBody(getHash(toHash).toUtf8());

  pForm->append(usernamePart);
  pForm->append(pwPart);
}

void XContestUploader::addFlightControls(QHttpMultiPart *pForm) const
{
  QString gliderName;
  QHttpPart activePart;
  QHttpPart igcPart;
  QHttpPart commentPart;
  QHttpPart gliderNamePart;

  // publish flight (NO for test purposes!)
  activePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[is_active]\""));
  // TODO use flag from uploadform
  activePart.setBody("N");

  // add igc file
  igcPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[tracklog]\"; filename=\"flight.igc\""));
  igcPart.setBody(m_pFlight->igcData());

  // add comment
  commentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[comment]\""));
  commentPart.setBody(m_pFlight->comment().toUtf8());

  // add glider name: what in the igc stands might differ from flyhigh db. Lets trust the flyhigh db...
  gliderNamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[glider_name]\""));
  m_pFlight->glider().olcName(gliderName);
  gliderNamePart.setBody(gliderName.toUtf8());

  pForm->append(activePart);
  pForm->append(igcPart);
  pForm->append(commentPart);
  pForm->append(gliderNamePart);
}

QUrl XContestUploader::urlEncodeParams(const QString& baseUrl, QMap<QString,QString>& params)
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

QString XContestUploader::getHash(const QString& str) {
  QString toHash = str + "f218bd8158be7def6608ec9910dbf25c";
  QByteArray hash = QCryptographicHash::hash(toHash.toUtf8(), QCryptographicHash::Sha1);
  return hash.toHex();
}
