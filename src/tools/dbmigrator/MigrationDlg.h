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

#ifndef MIGRATIONDLG_H
#define MIGRATIONDLG_H

#include <QDialog>

class QProgressBar;
class QLabel;
class QDialogButtonBox;
class DatabaseWidget;
class DatabaseParameters;
class MigratorThread;

/**
 * Dialog for migrating flyhigh databases
 * The source was lamely stolen and slipstreamed from www.digikam.org. Sorry & Thanks!
 */
class MigrationDlg: public QDialog
{
	Q_OBJECT

  public:

    /**
     * Creates a modal db migration dialog
     */
    MigrationDlg(QWidget* parent = 0);

    virtual ~MigrationDlg();

  private slots:
      void performCopy();

      void unlockInputFields();

      void lockInputFields();

      void handleFinish(int finishState, QString errorMsg);

      void handleStepStarted(const QString& stepName);

      void handleSmallStepStarted(int currValue, int maxValue);

      /**
       * Show a input dialog which provides db admin credentials
       */
      void handleRequestCredentials();

  signals:

      /**
       * User entered db admin credentials in dialog box
       */
      void credentialsEntered(QString,QString,bool);

  private:
    DatabaseWidget* m_pFromDBConfig;
    DatabaseWidget* m_pToDBConfig;
    QProgressBar* m_pProgressBar;
    QProgressBar* m_pProgressBarSmallStep;
    QPushButton* m_pMigrateButton;
    QPushButton* m_pCancelButton;
    QDialogButtonBox* m_pButtonBox;

    /**
     * Label
     */
    QLabel* m_pOverallStepTitle;

    /**
     * thread that does the work
     */
    MigratorThread* m_pMigratorThread;
};

#endif
