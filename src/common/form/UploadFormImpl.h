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

#ifndef UploadFormImpl_h
#define UploadFormImpl_h

#include "ui_UploadForm.h"
#include "Account.h"

class Flight;
class QAbstractButton;

class UploadFormImpl : public QDialog, public Ui::UploadForm
{
	Q_OBJECT

	public:
    UploadFormImpl(QWidget* parent, const QString &caption, Flight *pFlight);

		void setFlight(Flight *pFlight);

	protected slots:
    void upload();
    void close();
    void newAccount();
    void validate();
    void uploadSuccessful();
    void errorOnUpload(const QString& msg);
    void updateProgress(const QString& msg, const int& percent);

  private:
    void updateAccount();

  private:
		Flight *m_pFlight;
    Account::AccountListType m_accountList;
    bool m_upload;
};

#endif
