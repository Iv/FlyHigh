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

#ifndef DbSettingsDlg_H
#define DbSettingsDlg_H

#include <QDialog>

class QTabWidget;
class QDialogButtonBox;
class QVBoxLayout;
class DatabaseWidget;
class DatabaseParameters;
class DeviceConnectionWidget;
class DeviceConnectionParameters;

class DbSettingsDlg: public QDialog
{
  public:

    /**
     * Creates a modal preferences dialog
     */
    DbSettingsDlg(QWidget* parent = 0);

    ~DbSettingsDlg();

    /**
     * Access to the db config
     */
    const DatabaseParameters getDBParameters() const;

    /**
     * Access to the device config
     */
    const DeviceConnectionParameters getDeviceConnectionParameters() const;

private:

    /**
     * Tab container
     */
    QTabWidget* m_pTabWidget;

    /**
     * Ok/Cancel button
     */
    QDialogButtonBox* m_pButtonBox;

    /**
     * Layout manager
     */
    QVBoxLayout* m_pLayout;

    /**
     * Database configuration dialog
     */
    DatabaseWidget* m_pDBConfig;

    /**
     * Device connection dialog
     */
    DeviceConnectionWidget* m_pDeviceConfig;
};

#endif
