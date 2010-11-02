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

#include <math.h>
#include <QApplication>
#include <QVariant>
#include <QAction>
#include <QActionGroup>
#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include "WebMap.h"
#include "WebMapFaiSector.h"
#include "WebMapLeg.h"
#include "WebMapWidget.h"
#include "WebMapRouteWindow.h"
#include "WebMapTurnPoint.h"

WebMapRouteWindow::WebMapRouteWindow(QWidget* parent, const char* name, Qt::WindowFlags wflags, IDataBase::SourceType src)
	:MDIWindow(parent, name, wflags)
{
	QFont font;
	QLabel *pLabel;
	QLabel *pLat;
	QLabel *pLon;
	QLabel *pDistMeter;
	QLabel *pDistPerc;
	QAction *pAction;
	uint itemNr;
	int posX;

	m_pGridLayoutWidgetDists = NULL;
	m_pGridLayoutWidgetTps = NULL;
	m_pWebMapWidget = NULL;

	font.setPointSize(16);

	resize(800, 600);

	/* Actions */

	// Goto
	action_Goto = new QAction(this);

	// Routentyp Actions
	for(itemNr=0; itemNr<NofRouteType; itemNr++)
	{
		pAction = new QAction(this);
		pAction->setCheckable(true);
		m_routeTypeActionList[itemNr] = pAction;
	}

	m_routeType = FlatOrFaiTriangle;
	m_routeTypeActionList[m_routeType]->setChecked(true);

	centralwidget = new QWidget(this);

	// Map
	m_pWebMapWidget = new WebMapWidget(centralwidget);
	posX = BoxWidth + Border;
	m_pWebMapWidget->setGeometry(QRect(posX, 0, width() - posX, height() - MenuHeight));

	// Distanz
	m_pDistanceWidget = new QWidget(centralwidget);
	m_pDistanceWidget->setGeometry(QRect(BoxPosX, DistPosY, BoxWidth, DistHeight));
	m_pDistanceLayout = new QHBoxLayout(m_pDistanceWidget);
	m_pDistanceLayout->setContentsMargins(0, 0, 0, 0);

	label_Distance = new QLabel(m_pDistanceWidget);
	label_Distance->setFont(font);
	m_pDistanceLayout->addWidget(label_Distance);

	label_Distance_Value = new QLabel(m_pDistanceWidget);
	label_Distance_Value->setFont(font);
	label_Distance_Value->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	m_pDistanceLayout->addWidget(label_Distance_Value);

	/* Distanzen */
	m_pGroupBoxDists = new QGroupBox(centralwidget);

	/* Turnpoints */
	m_pGroupBoxTps = new QGroupBox(centralwidget);

	setCentralWidget(centralwidget);

	/* Menu */
	menubar = new QMenuBar(this);
	menubar->setGeometry(QRect(0, 0, width(), MenuHeight));
	menuMenu = new QMenu(menubar);
	menu_FlightType = new QMenu(menuMenu);
	menubar->addAction(menuMenu->menuAction());
	menuMenu->addAction(menu_FlightType->menuAction());
	menuMenu->addAction(action_Goto);

	// Routentyp
	alignmentGroup = new QActionGroup(this);

	for(itemNr=0; itemNr<NofRouteType; itemNr++)
	{
		pAction = m_routeTypeActionList[itemNr];
		menu_FlightType->addAction(pAction);
		alignmentGroup->addAction(pAction);
	}

	connect(alignmentGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeRouteType(QAction*)));
	connect(m_pWebMapWidget->getMap(), SIGNAL(mapReady()), this, SLOT(mapReady()));

	setMenuBar(menubar);

	retranslateUi();

	setRouteType(m_routeType);
}

WebMapRouteWindow::~WebMapRouteWindow()
{
}

void WebMapRouteWindow::resizeEvent(QResizeEvent *pEvent)
{
	int posX;

	if(m_pWebMapWidget != NULL)
	{
		posX = BoxWidth + Border;
		m_pWebMapWidget->setGeometry(QRect(posX, 0, width() - posX, height() - MenuHeight));
	}
}

void WebMapRouteWindow::retranslateUi()
{
	QAction *pAction;
	QString strLabel;
	uint itemNr;

	setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));

	action_Goto->setText(QApplication::translate("MainWindow", "Goto...", 0, QApplication::UnicodeUTF8));

	// Routentyp
	pAction = m_routeTypeActionList[FreeFlight];
	pAction->setText(QApplication::translate("MainWindow", "Free flight", 0, QApplication::UnicodeUTF8));

	pAction = m_routeTypeActionList[FreeFlightOneTp];
	pAction->setText(QApplication::translate("MainWindow", "Free flight via a turnpoint", 0, QApplication::UnicodeUTF8));

	pAction = m_routeTypeActionList[FreeFlightTwoTp];
	pAction->setText(QApplication::translate("MainWindow", "Free flight via two turnpoints", 0, QApplication::UnicodeUTF8));

	pAction = m_routeTypeActionList[FreeFlightThreeTp];
	pAction->setText(QApplication::translate("MainWindow", "Free flight via three turnpoints", 0, QApplication::UnicodeUTF8));

	pAction = m_routeTypeActionList[FlatOrFaiTriangle];
	pAction->setText(QApplication::translate("MainWindow", "Flat or FAI triangle", 0, QApplication::UnicodeUTF8));

	// Distanz
	label_Distance->setText(QApplication::translate("MainWindow", "Distance:", 0, QApplication::UnicodeUTF8));

	/* Distances */
	m_pGroupBoxDists->setTitle(QApplication::translate("MainWindow", "Distances", 0, QApplication::UnicodeUTF8));

	/* Turnpoints */
	m_pGroupBoxTps->setTitle(QApplication::translate("MainWindow", "Turnpoints", 0, QApplication::UnicodeUTF8));

	// Menu
	menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", 0, QApplication::UnicodeUTF8));
	menu_FlightType->setTitle(QApplication::translate("MainWindow", "Type", 0, QApplication::UnicodeUTF8));
}

void WebMapRouteWindow::setRouteType(RouteType type)
{
	QColor tpColorList[] = {QColor(Qt::red), QColor(61, 143, 255), QColor(Qt::yellow)};
	qreal tpInitList[] = {47.03, 8.47, 47.03, 8.53, 46.97, 8.53, 46.97, 8.47, 47.0, 8.5};

	WebMapTurnPoint *pTp;
	WebMapLeg *pWebMapLeg;
	WebMapFaiSector *pSector;
	TpListItem tpListItem;
	DistListItem distListItem;
	uint itemNr;
	uint distRowNr = 0;
	uint tpCount = 0;
	uint distCount = 0;
	int distBoxPosY;
	int distBoxHeight;
	int tpBoxPosY;
	int tpBoxHeight;

	m_routeType = type;

	// MapOverlayItems und Anzeigen löschen
	m_pWebMapWidget->clear();
	m_distList.clear();
	m_tpList.clear();

	if(m_pGridLayoutWidgetDists != NULL)
	{
		delete m_pGridLayoutWidgetDists;
	}

	if(m_pGridLayoutWidgetTps != NULL)
	{
		delete m_pGridLayoutWidgetTps;
	}

	// MapOverlayItems erstellen
	switch(m_routeType)
	{
		case FreeFlight:
			tpCount = 2;
			distCount = 1;
		break;
		case FreeFlightOneTp:
			tpCount = 3;
			distCount = 2;
		break;
		case FreeFlightTwoTp:
			tpCount = 4;
			distCount = 3;
		break;
		case FreeFlightThreeTp:
			tpCount = 5;
			distCount = 4;
		break;
		case FlatOrFaiTriangle:
			tpCount = 5;
			distCount = 6;
		break;
	}

	// Distances GroupBox
	distBoxPosY = DistPosY + DistHeight;
	distBoxHeight = distCount * ItemHeight + BorderTop + Border;

	m_pGroupBoxDists->setGeometry(QRect(BoxPosX, distBoxPosY, BoxWidth, distBoxHeight));
	m_pGridLayoutWidgetDists = new QWidget(m_pGroupBoxDists);
	m_pGridLayoutWidgetDists->setGeometry(QRect(0, 0, BoxWidth, distBoxHeight));
	m_pGridLayoutDists = new QGridLayout(m_pGridLayoutWidgetDists);
	m_pGridLayoutDists->setContentsMargins(Border, BorderTop, Border, Border);

	// Turnpoints GroupBox
	tpBoxPosY = distBoxPosY + distBoxHeight + BoxOffset;
	tpBoxHeight = tpCount * ItemHeight + BorderTop + Border;

	m_pGroupBoxTps->setGeometry(QRect(BoxPosX, tpBoxPosY, BoxWidth, tpBoxHeight));
	m_pGridLayoutWidgetTps = new QWidget(m_pGroupBoxTps);
	m_pGridLayoutWidgetTps->setGeometry(QRect(0, 0, BoxWidth, tpBoxHeight));
	m_pGridLayoutTps = new QGridLayout(m_pGridLayoutWidgetTps);
	m_pGridLayoutTps->setContentsMargins(Border, BorderTop, Border, Border);

	for(itemNr=0; itemNr<tpCount; itemNr++)
	{
		pTp = new WebMapTurnPoint();
		pTp->setColor(QColor(Qt::gray));
		connect(pTp, SIGNAL(newLatLon(const WebMapTurnPoint*)), this, SLOT(newLatLon(const WebMapTurnPoint*)));

		// Beschriftung
		if(itemNr == 0)
		{
			pTp->setText('S');
			pTp->setColor(QColor(Qt::green));
		}
		else if(itemNr == (tpCount - 1))
		{
			pTp->setText('E');
			pTp->setColor(QColor(Qt::green));
		}
		else
		{
			pTp->setText(0x30 + itemNr);
			pTp->setColor(tpColorList[itemNr - 1]);
		}

		tpListItem = createTpListItem(pTp);
		addTpListItem(tpListItem);
		m_pWebMapWidget->addItem(pTp);
		pTp->setLatLon(tpInitList[itemNr * 2], tpInitList[itemNr * 2 + 1]);

		// Schenkel
		if(itemNr > 0)
		{
			pWebMapLeg = new WebMapLeg(m_tpList[itemNr-1].pTp, m_tpList[itemNr].pTp);
			distListItem = createDistListItem(m_tpList[itemNr-1].pTp, m_tpList[itemNr].pTp, pWebMapLeg);
			addDistListItem(distListItem, distRowNr);
			m_pWebMapWidget->addItem(pWebMapLeg);
			distRowNr++;

			if((m_routeType == FlatOrFaiTriangle) && (distRowNr == 3))
			{
				// Platz für Schliesser
				distRowNr++;
			}
		}
	}

	if(m_routeType == FlatOrFaiTriangle)
	{
		// FAI Sektoren
		m_pWebMapWidget->addItem(new WebMapFaiSector(m_tpList[1].pTp, m_tpList[2].pTp, m_tpList[3].pTp));
		m_pWebMapWidget->addItem(new WebMapFaiSector(m_tpList[2].pTp, m_tpList[3].pTp, m_tpList[1].pTp));
		m_pWebMapWidget->addItem(new WebMapFaiSector(m_tpList[3].pTp, m_tpList[1].pTp, m_tpList[2].pTp));

		// Schliesser
		pWebMapLeg = new WebMapLeg(m_tpList[3].pTp, m_tpList[1].pTp);
		pWebMapLeg->enableArrow(false);
		distListItem = createDistListItem(m_tpList[3].pTp, m_tpList[1].pTp, pWebMapLeg);
		addDistListItem(distListItem, 3);
		m_pWebMapWidget->addItem(pWebMapLeg);

		// Anzeige E to S
		distListItem = createDistListItem(m_tpList[4].pTp, m_tpList[0].pTp, NULL);
		addDistListItem(distListItem, 5);
	}

	updateDistValues(NULL);

	m_pGridLayoutWidgetDists->show();
	m_pGridLayoutWidgetTps->show();
}

void WebMapRouteWindow::changeRouteType(QAction *pAction)
{
	uint itemNr;

	for(itemNr=0; itemNr<NofRouteType; itemNr++)
	{
		if(m_routeTypeActionList[itemNr] == pAction)
		{
			setRouteType((RouteType)itemNr);
			break;
		}
	}
}

void WebMapRouteWindow::newLatLon(const WebMapTurnPoint *pTp)
{
	DistList::iterator it;
	WebMapTurnPoint *pTp1;
	WebMapTurnPoint *pTp2;
	qreal dist = 0;
	int tpNr;
	bool isFai;

	tpNr = findTpNr(pTp);

	if(tpNr >= 0)
	{
		m_tpList[tpNr].pLat->setText(QString::fromUtf8("%1 °").arg(pTp->getLat(), 0, 'f', 5));
		m_tpList[tpNr].pLon->setText(QString::fromUtf8("%1 °").arg(pTp->getLon(), 0, 'f', 5));

		updateDistValues(pTp);

		// glow
		isFai = isFaiTri();

		if(m_distList.size() > 3)
		{
			m_distList[1].pWebMapLeg->enableGlow(isFai);
			m_distList[2].pWebMapLeg->enableGlow(isFai);
			m_distList[3].pWebMapLeg->enableGlow(isFai);
		}
	}
}

void WebMapRouteWindow::mapReady()
{
	m_pWebMapWidget->getMap()->zoomTo(47.05, 8.53, 46.92, 8.47);
}

int WebMapRouteWindow::findTpNr(const WebMapTurnPoint *pTp)
{
	int itemNr;
	int tpNr = -1;

	for(itemNr=0; itemNr<m_tpList.size(); itemNr++)
	{
		if(pTp == m_tpList[itemNr].pTp)
		{
			tpNr = itemNr;
			break;
		}
	}

	return tpNr;
}

qreal WebMapRouteWindow::getTpDist(const WebMapTurnPoint *pTp1, const WebMapTurnPoint *pTp2)
{
	const qreal EarthRadius = 6371000.0;
	qreal dist;
	qreal angle; // in radians
	qreal lat1; // in radians
	qreal lat2; // in radians
	qreal deltaLong; // in radians
	
	// P1, L1 Latitude and Longitude of Point 1
	// P2, L2 Latitude and Longitude of Point 2
	lat1 = pTp1->getLat() * M_PI / 180;
	lat2 = pTp2->getLat() * M_PI / 180;
	deltaLong = (pTp2->getLon() - pTp1->getLon()) * M_PI / 180;
	
	// cos E = sin P1 * sin P2 + cos P1 * cos P2 * cos (L2 - L1)
	angle = sinl(lat1) * sinl(lat2) + cosl(lat1) * cosl(lat2) * cosl(deltaLong);

	// FAI Earth Radius is 6371 km
	dist = EarthRadius * acosl(angle);
	
	return dist;
}

WebMapRouteWindow::TpListItem WebMapRouteWindow::createTpListItem(WebMapTurnPoint *pTp)
{
	TpListItem item;
	QLabel *pLabel;
	QFont font;
	QString strLabel;

	font.setBold(true);
	font.setWeight(75);

	item.pTp = pTp;

	pLabel = new QLabel(m_pGridLayoutWidgetTps);
	pLabel->setFont(font);
	strLabel = QString::fromUtf8("TP %1:")
				.arg(pTp->getText());
	pLabel->setText(strLabel);
	item.pLabel = pLabel;

	pLabel = new QLabel(m_pGridLayoutWidgetTps);
	pLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	item.pLat = pLabel;
pLabel->setText("Lat");

	pLabel = new QLabel(m_pGridLayoutWidgetTps);
	pLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	item.pLon = pLabel;
pLabel->setText("Lon");

	return item;
}

void WebMapRouteWindow::addTpListItem(TpListItem &item)
{
	uint row;

	row = m_tpList.size();
	m_tpList.push_back(item);
	m_pGridLayoutTps->addWidget(item.pLabel, row, 0);
	m_pGridLayoutTps->addWidget(item.pLat, row, 1);
	m_pGridLayoutTps->addWidget(item.pLon, row, 2);
}

WebMapRouteWindow::DistListItem WebMapRouteWindow::createDistListItem(WebMapTurnPoint *pTp1, WebMapTurnPoint *pTp2, WebMapLeg *pWebMapLeg)
{
	DistListItem item;
	QLabel *pLabel;
	QFont font;
	QString strLabel;

	font.setBold(true);
	font.setWeight(75);

	item.pTp1 = pTp1;
	item.pTp2 = pTp2;
	item.pWebMapLeg = pWebMapLeg;

	pLabel = new QLabel(m_pGridLayoutWidgetDists);
	pLabel->setFont(font);
	strLabel = QString::fromUtf8("%1\342\206\222%2:")
				.arg(pTp1->getText())
				.arg(pTp2->getText());
	pLabel->setText(strLabel);
	item.pLabel = pLabel;

	pLabel = new QLabel(m_pGridLayoutWidgetDists);
	pLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	item.pDistMeter = pLabel;
pLabel->setText("Meter");

/*
	pLabel = new QLabel(m_pGridLayoutWidgetDists);
	pLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
	item.pDistPerc = pLabel;
pLabel->setText("Perc");
*/

	return item;
}

void WebMapRouteWindow::addDistListItem(DistListItem &item, uint row)
{
	m_distList.insert(row, item);
	m_pGridLayoutDists->addWidget(item.pLabel, row, 0);
	m_pGridLayoutDists->addWidget(item.pDistMeter, row, 1);
//	m_pGridLayoutDists->addWidget(item.pDistPerc, row, 2);
}

void WebMapRouteWindow::updateDistValues(const WebMapTurnPoint *pTp)
{
	DistList::iterator it;
	WebMapTurnPoint *pTp1;
	WebMapTurnPoint *pTp2;
	qreal dist;

	for(it=m_distList.begin(); it!=m_distList.end(); it++)
	{
		pTp1 = (*it).pTp1;
		pTp2 = (*it).pTp2;

		if((pTp == NULL) || (pTp == pTp1) || (pTp == pTp2))
		{
			dist = getTpDist(pTp1, pTp2);
			(*it).dist = dist;
			(*it).pDistMeter->setText(QString("%1 km").arg(dist / 1000, 0, 'f', 3));
		}
	}

	// Totale Distanz
	dist = 0;

	if(m_routeType == FlatOrFaiTriangle)
	{
		if(m_distList.size() == 6)
		{
			dist = m_distList[DistListFaiWebMapLeg1].dist + m_distList[DistListFaiWebMapLeg2].dist + m_distList[DistListFaiWebMapLeg3].dist;
			m_tpList[0].pTp->setCircleRadius(dist * 0.20);
			dist -= m_distList[DistListFaiTpEToTpS].dist;
		}
	}
	else
	{
		for(it=m_distList.begin(); it!=m_distList.end(); it++)
		{
			dist += (*it).dist;
		}
	}

	label_Distance_Value->setText(QString("%1 km").arg(dist / 1000, 0, 'f', 3));
}

bool WebMapRouteWindow::isFaiTri() const
{
	qreal minDist;
	qreal dist;
	bool isFai = false;

	if((m_routeType == FlatOrFaiTriangle) && (m_distList.size() == 6))
	{
		dist = m_distList[DistListFaiWebMapLeg1].dist + m_distList[DistListFaiWebMapLeg2].dist + m_distList[DistListFaiWebMapLeg3].dist;
		minDist = qMin(m_distList[DistListFaiWebMapLeg1].dist, m_distList[DistListFaiWebMapLeg2].dist);
		minDist = qMin(minDist, m_distList[DistListFaiWebMapLeg3].dist);
		isFai = (((minDist / dist) >= 0.28) && (m_distList[DistListFaiTpEToTpS].dist <= (dist * 0.20)));
	}

	return isFai;
}
