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
#include <QNetworkConfigurationManager>
#include <QEventLoop>
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
#include "XContestAdditionalInfoDlg.h"
#include "XContestUploader.h"

#include <QDebug>

const QString XContestUploader::XCONTEST_API_BASE_URL = "http://www.xcontest.org";
const QString XContestUploader::XCONTEST_TICKET_URL = "/api/gate/ticket/?key=%1&hash=%2";
const QString XContestUploader::XCONTEST_FLIGHT_URL = "/api/gate/request/?flight";
const QString XContestUploader::XCONTEST_API_KEY = "75F62615B235B6A0-C139EAE8D66FA6DD-632DD7669D2F2AD0";


XContestUploader::XContestUploader(Account* pAccount)
{
  m_pManager = new QNetworkAccessManager();
  // with these 2 lines, network availability may be checked _before_ doing any requests:
  QNetworkConfigurationManager netConfig;
  m_pManager->setConfiguration(netConfig.defaultConfiguration());

  // network reply triggers our state machine:
  QObject::connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleEvent(QNetworkReply*)));

  // create local copy of account
  m_Account = *pAccount;
  m_state = INIT;
  m_needTicket = true;
  m_Ticket = "";
  m_SessionId = "";
  m_pInfoDlg = NULL;
}

XContestUploader::~XContestUploader()
{
  delete m_pManager;
}


void XContestUploader::uploadFlight(Flight* pFlight)
{
  QEventLoop pause;
  // upload is done if either finished() or error() signal is emitted
  connect(this, SIGNAL(finished()),&pause,SLOT(quit()));
  connect(this, SIGNAL(error(QString)),&pause,SLOT(quit()));

  m_pFlight = pFlight;

  // check network availability
  if(m_pManager->networkAccessible()==QNetworkAccessManager::NotAccessible)
  {
    emit error(tr("Network not accessible. Check connections!"));
    return;
  }

  // call slot manually for kicking off state-machine
  handleEvent(NULL);

  // wait until upload process is finished
  pause.exec();
}

void XContestUploader::handleEvent(QNetworkReply* reply)
{
  QString errorMsg;
  QVariant httpStatus;
  QByteArray bytes;
  QJsonDocument jsonDoc;
  QJsonObject controls;
  bool valid;
  bool success;
  bool formValid;
  int phase;

  if(reply == NULL && m_needTicket) {
    // invoked directly / initial call
    // send ticket request
    emit step(tr("Connecting to xcontest.org"), 10);
    sendTicketRequest();
    return;
  }

  // check for basic errors
  if (reply->error() != QNetworkReply::NoError)
  {
    // network reply had errors
    errorMsg = reply->errorString();
    emit error(errorMsg);
    return;
  }

  // Read the HTTP status code
  httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  if(!httpStatus.isValid())
  {
    // invalid http status code... how could this happen??
    errorMsg = "Invalid http reply received";
    emit error(errorMsg);
    return;
  } else
  {
    if(httpStatus.toInt()!=200)
    {
      // we're only happy with http status ok. Don't know how to deal with redirects and such...
      errorMsg = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
      emit error(errorMsg);
      return;
    }
  }

  // ok so far
  // read data from QNetworkReply
  bytes = reply->readAll();

  // try json parsing
  valid = parseResponse(bytes,jsonDoc, errorMsg);
  if(!valid)
  {
    emit error(errorMsg);
    return;
  }

  // so far we've got a good http response with a valid json body
  qDebug() << "JSON response: " << jsonDoc.toJson(QJsonDocument::Indented);

  valid = checkErrorResponse(jsonDoc,errorMsg);
  if(!valid)
  {
    emit error(errorMsg);
    return;
  }

  // sub-state machine: before each gate post, we need a gate ticket
  if(m_needTicket) {
    m_needTicket = false;

    // we're expecting the response to a gate ticket request
    valid = readTicketResponse(jsonDoc, m_Ticket);
    if(!valid)
    {
      errorMsg = "Protocol error - gate ticket reply expected";
      emit error(errorMsg);
      return;
    }
    if(m_SessionId.isEmpty())
    {
      // notify only in first ticket request
      emit step(tr("Application authorized"),20);
    }
  }

  // state machine
  if(m_state == INIT)
  {
    // post request
    emit step(tr("Login for user %1 succeeded").arg(m_Account.username()),40);
    sendClaimRequest();
    emit step(tr("IGC file sent"),60);
    m_state = CLAIM;
  } else if(m_state == CLAIM)
  {
    // expecting claim reply
    valid = readGateResponse(jsonDoc, success, m_SessionId, m_SessionId.isEmpty());
    if(!valid)
    {
      errorMsg = "Protocol error - gate response not valid";
      emit error(errorMsg);
      return;
    }

    // read form controls
    valid = readForm(jsonDoc,formValid,phase);
    if(!valid)
    {
      errorMsg = "Protocol error - form not valid";
      emit error(errorMsg);
      return;
    }

    if(success)
    {
      if(phase == 3) {
        // ok
        m_state = FINISH;
        emit finished();
        return;
      }
      qDebug() << "Whats up? Phase" << phase << "not expected here";
      errorMsg = "Protocol error - form not valid";
      emit error(errorMsg);
      return;
    } else
    {
      // maybe additional info is required (aka autocomplete not successful)
      if(phase == 2) {

        valid = readControls(jsonDoc, controls);
        if(!valid)
        {
          errorMsg = "Protocol error - controls not valid";
          emit error(errorMsg);
          return;
        }

        // create dialog (but don't show yet)
        m_pInfoDlg = new XContestAdditionalInfoDlg(controls.toVariantMap());

        m_state = NEEDINFO;
        emit step(tr("Additional info is needed"),80);
        m_needTicket = true;
        sendTicketRequest();
      } else
      {
        errorMsg = "Protocol error - unknown phase";
        emit error(errorMsg);
        return;
      }
    }
  } else if(m_state == NEEDINFO)
  {
    // show dialog
    if(m_pInfoDlg->exec()!= QDialog::Accepted)
    {
      errorMsg = "Aborted by user";
      emit error(errorMsg);
      return;
    }

    // read clarifications from gui
    const QMap<QString,QString>& clarifications = m_pInfoDlg->clarifications();
    // don't use QMap::unite(), it might create multiple values per key
    for(QMap<QString,QString>::ConstIterator iter = clarifications.constBegin(); iter != clarifications.constEnd(); ++iter)
    {
      m_Controls.insert(iter.key(),iter.value());
    }

    // send data
    sendClarificationRequest();
    emit step(tr("Additional info sent"),90);

    // back to claim state
    m_state = CLAIM;
  }
}

void XContestUploader::sendTicketRequest()
{
  QNetworkRequest request(getTicketUrl());
  m_pManager->get(request);
}

void XContestUploader::sendClaimRequest()
{
  QNetworkRequest request;
  QHttpMultiPart* pClaim;

  request = buildGateRequestUrl();

  pClaim = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  addAuthControls(pClaim);
  addFlightControls(pClaim);

  m_pManager->post(request,pClaim);
}

void XContestUploader::sendClarificationRequest()
{
  QNetworkRequest request;
  QHttpMultiPart* pClarification;

  request = buildGateRequestUrl();

  pClarification = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  for(QMap<QString,QString>::ConstIterator iter = m_Controls.constBegin(); iter != m_Controls.constEnd(); ++iter)
  {
    QHttpPart part;

    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg(iter.key())));
    part.setBody(iter.value().toUtf8());
    pClarification->append(part);
  }

  m_pManager->post(request,pClarification);
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

void XContestUploader::addFlightControls(QHttpMultiPart *pForm)
{
  QString gliderName;
  QHttpPart activePart;
  QHttpPart igcPart;
  QHttpPart commentPart;
  QHttpPart gliderNamePart;

  // publish flight (NO for test purposes!)
  activePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[is_active]\""));
  activePart.setBody("Y");
  m_Controls.insert("flight[is_active]","Y");

  // add igc file
  igcPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[tracklog]\"; filename=\"flight.igc\""));
  igcPart.setBody(m_pFlight->igcData());
  // don't add igc to subsequent requests

  // add comment
  commentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[comment]\""));
  commentPart.setBody(m_pFlight->comment().toUtf8());
  m_Controls.insert("flight[comment]",m_pFlight->comment());

  // add glider name: what in the igc stands might differ from flyhigh db. Lets trust the flyhigh db...
  gliderNamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"flight[glider_name]\""));
  m_pFlight->glider().olcName(gliderName);
  gliderNamePart.setBody(gliderName.toUtf8());
  m_Controls.insert("flight[glider_name]",gliderName);

  pForm->append(activePart);
  pForm->append(igcPart);
  pForm->append(commentPart);
  pForm->append(gliderNamePart);
}

QNetworkRequest XContestUploader::buildGateRequestUrl() const
{
  QMap<QString,QString> params;
  QUrl reqUrl;

  params.insert("key",XCONTEST_API_KEY);
  params.insert("ticket",m_Ticket);
  params.insert("hash",getHash(m_Ticket + XCONTEST_API_KEY));
  params.insert("authticket",m_SessionId);

  reqUrl = urlEncodeParams(XCONTEST_API_BASE_URL + XCONTEST_FLIGHT_URL, params);

  return QNetworkRequest(reqUrl);
}

/**
 * Parses input and assign resulting Json document to jsonDoc.
 * @param input - input bytes to parse
 * @param jsonDoc - output QJsonDocument
 * @param error - error message if a parsing error occurs. Not changed if parsing is successful
 * @return true if parsing succeeds, false otherwise
 */
bool XContestUploader::parseResponse(const QByteArray& input, QJsonDocument& jsonDoc, QString& error) const
{
  QJsonParseError e;
  QJsonDocument d;

  d = QJsonDocument::fromJson(input, &e);
  if(!d.isNull()){
    jsonDoc = d;
    return true;
  } else
  {
    error = e.errorString();
    return false;
  }
}

/**
 * Checks if the provided json document has the value error set. if yes, the respective message is assigned to errorMsg.
 * @param jsonDoc - input the json document to traverse
 * @param errorMsg - output error message that belongs to the error value
 * @return true if there is _NO_ error found, false if there IS an error
 */
bool XContestUploader::checkErrorResponse(const QJsonDocument&jsonDoc, QString& errorMsg) const
{
  QJsonObject obj = jsonDoc.object();
  QJsonValue e = obj.value(QString("error"));

  if(!e.isNull())
  {
    QJsonObject errObj = e.toObject();
    errorMsg = errObj["message"].toString();
    return false;
  } else
  {
    // no error
    return true;
  }
}

/**
 * Read the ticket value from the provided json document. If found, the ticket will be assigned to ticket
 * @param jsonDoc - input the json document to traverse
 * @param ticket - output where the ticket value will be assigned
 * @return true if the ticket value has been found, false otherwise
 */
bool XContestUploader::readTicketResponse(const QJsonDocument&jsonDoc, QString& ticket) const
{
  QJsonObject obj = jsonDoc.object();
  QJsonValue t = obj.value(QString("ticket"));

  if(!t.isNull())
  {
    ticket = t.toString();
    return true;
  } else
  {
    return false;
  }
}

/**
 * Read the success and session id value from the provided json document. If found, the values will be assigned to success and sessionId
 * @param jsonDoc - input the json document to traverse
 * @param success - output where the success value will be assigned
 * @param sessionId - output where the sessionId value will be assigned
 * @param isFirst - true if this is the first gate response of the session (only first response carries a authTicket control)
 * @return true if both the success and sessionId values have been found, false otherwise
 */
bool XContestUploader::readGateResponse(const QJsonDocument&jsonDoc, bool& success, QString& sessionId, bool isFirst) const
{
  QJsonObject obj = jsonDoc.object();
  QJsonValue s = obj.value(QString("success"));
  QJsonValue a = obj.value(QString("authTicket"));

  if(!s.isNull())
  {
    success = s.toBool();
  }
  if(isFirst)
  {
    if(!a.isNull())
    {
      sessionId = a.toString();
    }
    return !(s.isNull() || a.isNull());
  } else
  {
    return !s.isNull();
  }
}

/**
 * Read the form values from the provided json document. If found, the values will be assigned to formValid and phase
 * @param jsonDoc - input the json document to traverse
 * @param formValid - output where the from.isValid value will be assigned
 * @param phase - output where the form.phase value will be assigned
 * @return true if both formValid and phase values have been found, false otherwise
 */
bool XContestUploader::readForm(const QJsonDocument&jsonDoc, bool& formValid, int& phase) const
{
  QJsonObject obj = jsonDoc.object();
  QJsonValue f = obj.value(QString("form"));
  QJsonObject form;
  QJsonValue v;
  QJsonValue p;

  if(!f.isNull())
  {
    form = f.toObject();
    v = form.value(QString("isValid"));
    if(!v.isNull())
    {
      formValid = v.toBool();
    }
    p = form.value(QString("phase"));
    if(!p.isNull())
    {
      phase = (int)p.toDouble();
    }
  }
  return !(f.isNull() || v.isNull() || p.isNull());
}

/**
 * Read the controls object from the provided json document. If found, the object will be assigned to controls
 * @param jsonDoc - input the json document to traverse
 * @param controls - output where the from.controls object will be assigned
 * @return true if controls has been found, false otherwise
 */
bool XContestUploader::readControls(const QJsonDocument&jsonDoc, QJsonObject& controls) const
{
  QJsonObject obj = jsonDoc.object();
  QJsonValue f = obj.value(QString("form"));
  QJsonValue c;
  QJsonObject form;

  if(!f.isNull())
  {
    form = f.toObject();
    c = form.value(QString("controls"));
    if(!c.isNull())
    {
      controls = c.toObject();
    }
  }
  return !(f.isNull() || c.isNull());
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
