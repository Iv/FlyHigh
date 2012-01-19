/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#ifndef CREDENTIALSDLG_H
#define CREDENTIALSDLG_H

#include <QDialog>

class QLineEdit;

/**
 * Widget for entering username and password in one single dialog.
 */
class CredentialsDlg: public QDialog
{
  public:

    /**
     * Creates a credentials dialog
     * @param parent - the parent widget
     * @param text - the explanation text to show in the dialog
     */
    CredentialsDlg(QWidget* parent=0, const QString& text="");

    /**
     * destruction
     */
    virtual ~CredentialsDlg();

    /**
     * presets the username
     * @param user - the username
     */
    void setUsername(const QString& user);

    /**
     * Returns the username
     * @return the username
     */
    QString getUsername() const;

    /**
     * Returns the password
     * @return the password
     */
    QString getPassword() const;

  private:
    QLineEdit* m_pUsername;
    QLineEdit* m_pPassword;
};

#endif
