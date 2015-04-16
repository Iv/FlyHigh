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

#ifndef XContestUploader_h
#define XContestUploader_h

#include <QMap>
#include "Account.h"
#include "IOLCUploader.h"

class QNetworkReply;
class QUrl;
class QHttpMultiPart;
class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;
class QByteArray;
class QJsonDocument;
class QJsonObject;
class XContestAdditionalInfoDlg;

/**
 * Implements XContest uploading functionality
 */
class XContestUploader : public IOLCUploader
{
  Q_OBJECT
public:

  XContestUploader(Account* pAccount);
  virtual ~XContestUploader();

  virtual void uploadFlight(Flight* pFlight);

private:

  /** state type for state machine */
  typedef enum State {INIT,CLAIM,NEEDINFO,FINISH} State;

  void addAuthControls(QHttpMultiPart* pForm) const;
  void addFlightControls(QHttpMultiPart* pForm);

  void sendTicketRequest();
  void sendClaimRequest();
  void sendClarificationRequest();

  QNetworkRequest buildGateRequestUrl() const;

  bool parseResponse(const QByteArray& toParse, QJsonDocument& result, QString& error) const;
  bool checkErrorResponse(const QJsonDocument&jsonDoc, QString& errorMsg) const;
  bool readTicketResponse(const QJsonDocument&jsonDoc, QString& ticket) const;
  bool readGateResponse(const QJsonDocument&jsonDoc, bool& success, QString& sessionId, bool isFirst) const;
  bool readForm(const QJsonDocument&jsonDoc, bool& formValid, int& phase) const;
  bool readControls(const QJsonDocument&jsonDoc, QJsonObject& controls) const;

  static QString getHash(const QString& str);
  static QUrl urlEncodeParams(const QString& baseUrl, QMap<QString,QString>& params);
  static QUrl getTicketUrl();


private slots:
  void handleEvent(QNetworkReply* reply);

signals:
  void finished();
  void error(const QString& message);
  void step(const QString& step, const int& percent);


private:

  Account m_Account;
  State m_state;
  bool m_needTicket;
  QString m_Ticket;
  QString m_SessionId;
  Flight* m_pFlight;
  QNetworkAccessManager* m_pManager;
  XContestAdditionalInfoDlg* m_pInfoDlg;
  QMap<QString,QString> m_Controls;

  static const QString XCONTEST_API_BASE_URL;
  static const QString XCONTEST_TICKET_URL;
  static const QString XCONTEST_FLIGHT_URL;
  static const QString XCONTEST_API_KEY;
};

#endif
