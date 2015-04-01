/***************************************************************************
 *   Copyright (C) 2010 by Alex Graf                                       *
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

#ifndef XContestAdditionalInfoDlg_h
#define XContestAdditionalInfoDlg_h

#include <QMap>
#include <QVariant>
#include <QDialog>

class QLayout;

/**
 * Dialog class that asks for additional info if the data uploaded
 * to xcontest is ambiguos
 */
class XContestAdditionalInfoDlg: public QDialog
{
	Q_OBJECT

	public:
    XContestAdditionalInfoDlg(const QVariantMap& controls);
    void clarifications(QMap<QString,QString>& clarifications) const;

  protected slots:
    void submit();

  private:
    void selectControls(const QVariantMap& controls);
    void buildGui();
    QLayout* buildDynamicGui();
    QString readNotes(const QVariantMap& control, const QString& key, const QString& prefix) const;

  private:
    /** collected clarifications to be used as additional info, outcome of this dialog */
    QMap<QString,QString> m_clarifications;

    /** controls selected for display */
    QVariantMap m_controls;

    /** collection of widgets for retrieval of user input */
    QMap<QWidget*,QString> m_widgets;
};

#endif
