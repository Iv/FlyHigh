/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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
#ifndef AirSpaceWindow_h
#define AirSpaceWindow_h

#include "AirSpace.h"
#include "AirSpaceList.h"
#include "AirSpaceView.h"
#include "IDataBase.h"
#include "TableWindow.h"

class QWidget;
class WebMapAirSpaceView;

class AirSpaceWindow: public TableWindow
{
	Q_OBJECT

	public:
		AirSpaceWindow(QWidget* parent, const QString &name, Qt::WindowFlags wflags, IDataBase::SourceType src);

		~AirSpaceWindow();

	protected:
    void closeEvent(QCloseEvent *pEvent);

		void selectionChanged();

	private slots:
		void file_open();

		void file_delete();

		void file_update();

		void file_AddToSqlDB();

		void file_AddToGPS();

    void file_viewAirSpace();

		void file_viewWebMap();

    void airSpaceViewFinished(int);

		void webMapFinished(int res);

    void airSpaceChanged(int line);

	private:
    typedef QVector<int> RowList;
		enum Fields{Name, Low, High, Class};

		IDataBase *m_pDb;
		AirSpaceList m_airSpaceList;
		AirSpaceView *m_pAirSpaceView;
    WebMapAirSpaceView *m_pWebMapView;
    bool m_externSelect;

    void setAirSpaceToRow(uint row, const AirSpace *pAirSpace);

    void selectionToList(AirSpaceList &airspaceList, RowList *pSelRows = NULL);
};

#endif
