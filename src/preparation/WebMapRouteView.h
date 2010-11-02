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
#ifndef WebMapRouteWindow_h
#define WebMapRouteWindow_h

#include <QMainWindow>
#include "IDataBase.h"

class QAction;
class QActionGroup;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QMenuBar;
class QMenu;
class QWidget;
class WebMapLeg;
class WebMapWidget;
class WebMapTurnPoint;

/**
	@author Alex Graf <alex@grafitation.ch>
*/
class WebMapRouteWindow: public QMainWindow
{
	Q_OBJECT

	public:
		WebMapRouteWindow(QWidget* parent, const QString &name, IDataBase::SourceType src);

		~WebMapRouteWindow();

	protected:
		void resizeEvent(QResizeEvent *pEvent);

	private:
		enum {BoxPosX = 0, DistPosY = 0, BoxWidth = 190, BoxOffset = 10, DistHeight = 30, ItemHeight = 22,
					Border = 10, BorderTop = 20, MenuHeight = 30};

		enum {DistListFaiTpSToTp1 = 0, DistListFaiWebMapLeg1 = 1, DistListFaiWebMapLeg2 = 2, DistListFaiWebMapLeg3 = 3, DistListFaiTp3ToTpE = 4, DistListFaiTpEToTpS = 5};

		typedef enum RouteType{FreeFlight, FreeFlightOneTp, FreeFlightTwoTp, FreeFlightThreeTp, FlatOrFaiTriangle, NofRouteType}RouteType;

		typedef struct TpListItem
		{
			QLabel *pLabel;
			QLabel *pLat;
			QLabel *pLon;
			WebMapTurnPoint *pTp;
		}TpListItem;

		typedef QList<TpListItem> TpList;

		typedef struct DistListItem
		{
			QLabel *pLabel;
			QLabel *pDistMeter;
			QLabel *pDistPerc;
			WebMapTurnPoint *pTp1;
			WebMapTurnPoint *pTp2;
			WebMapLeg *pWebMapLeg;
			qreal dist;
		}DistListItem;

		typedef QList<DistListItem> DistList;

		QAction *action_Goto;
		QAction* m_routeTypeActionList[NofRouteType];

		QWidget *centralwidget;
		WebMapWidget *m_pWebMapWidget;
		QWidget *m_pDistanceWidget;
		QHBoxLayout *m_pDistanceLayout;

		QLabel *label_Distance;
		QLabel *label_Distance_Value;

		QGroupBox *m_pGroupBoxDists;
		QWidget *m_pGridLayoutWidgetDists;
		QGridLayout *m_pGridLayoutDists;

		QGroupBox *m_pGroupBoxTps;
		QWidget *m_pGridLayoutWidgetTps;
		QGridLayout *m_pGridLayoutTps;

		QMenuBar *menubar;
		QMenu *menuMenu;
		QMenu *menu_FlightType;
		QActionGroup *alignmentGroup;

		RouteType m_routeType;
		TpList m_tpList;
		DistList m_distList;

		void retranslateUi();

		void setRouteType(RouteType type);

		uint getTpCount() const;

		uint getTpNr(const WebMapTurnPoint* pTp) const;

		qreal getDist() const;

		qreal getTpDist(uint tp1, uint tp2) const;

		qreal getTpLat(uint tp) const;

		qreal getTpLon(uint tp) const;

		int findTpNr(const WebMapTurnPoint *pTp);

		qreal getTpDist(const WebMapTurnPoint *pTp1, const WebMapTurnPoint *pTp2);

		TpListItem createTpListItem(WebMapTurnPoint *pTp);

		void addTpListItem(TpListItem &item);

		DistListItem createDistListItem(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapLeg *pWebMapLeg);

		void addDistListItem(DistListItem &item, uint row);

		void updateDistValues(const WebMapTurnPoint *pTp);

		bool isFaiTri() const;

		private slots:
			void changeRouteType(QAction *pAction);

			void newLatLon(const WebMapTurnPoint *pTp);

			void mapReady();
};

#endif
