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

#include <QLineEdit>
#include <QWidget>
#include <QString>
#include "ISql.h"
#include "Route.h"
#include "RouteFormImpl.h"
#include "WayPoint.h"

RouteFormImpl::RouteFormImpl(QWidget* parent, const QString &caption, Route *pRoute)
	:QDialog(parent)
{
	WayPoint *pWp;
	QString fullName;
	uint nItems;
	uint curItem;

	Q_CHECK_PTR(pRoute);
	m_pRoute = pRoute;
	m_readOnly = false;

  setupUi(this);
	setWindowTitle(caption);

	// database waypoints
	ISql::pInstance()->wayPointList(WayPoint::TypeBuoy, m_wpDbList);
	nItems = m_wpDbList.size();

	for(curItem=0; curItem<nItems; curItem++)
	{
		m_wpDbList.at(curItem).fullName(fullName);
		listBoxDBWayPoints->addItem(fullName);
	}

	// name
	lineEditName->setText(m_pRoute->name());

	// type
	comboBoxType->setCurrentIndex(m_pRoute->type());

	// waypoints
	nItems = m_pRoute->wayPointList().size();

	for(curItem=0; curItem<nItems; curItem++)
	{
		pWp = &(m_pRoute->wayPointList()[curItem]);
		m_wpRouteList.append(pWp);
	}

	showWpRoute(0);
}

void RouteFormImpl::setReadOnly()
{
	m_readOnly = true;

	lineEditName->setEnabled(false);
	comboBoxType->setEnabled(false);
	pushButtonUp->setEnabled(false);
	pushButtonDown->setEnabled(false);
	pushButtonToRight->setEnabled(false);
	pushButtonRemove->setEnabled(false);
}

void RouteFormImpl::accept()
{
	WayPoint *pWp;
	uint nItems;
	uint curItem;

	if(!m_readOnly)
	{
		m_pRoute->setName(lineEditName->text());
		m_pRoute->setType((Route::Type)comboBoxType->currentIndex());
		m_pRoute->wayPointList().clear();
		nItems = m_wpRouteList.count();

		for(curItem=0; curItem<nItems; curItem++)
		{
			pWp = m_wpRouteList.at(curItem);
			m_pRoute->wayPointList().push_back(*pWp);
		}
	}

	QDialog::accept();
}

void RouteFormImpl::down()
{
	int curItem;
	WayPoint *pWp;

	curItem = listBoxRouteWayPoints->currentRow();

	if(curItem < (m_wpRouteList.count() - 1))
	{
		pWp = m_wpRouteList.at(curItem);
		m_wpRouteList.removeAt(curItem);
		curItem++;
		m_wpRouteList.insert(curItem, pWp);
		showWpRoute(curItem);
	}
}

void RouteFormImpl::up()
{
	uint curItem;
	WayPoint *pWp;

	curItem = listBoxRouteWayPoints->currentRow();

	if(curItem > 0)
	{
		pWp = m_wpRouteList.at(curItem);
		m_wpRouteList.removeAt(curItem);
    curItem--;
		m_wpRouteList.insert(curItem, pWp);
		showWpRoute(curItem);
	}
}

void RouteFormImpl::toRight()
{
	uint curItem;
	uint count;
	WayPoint *pWp;

	curItem = listBoxDBWayPoints->currentRow();
	count = listBoxDBWayPoints->count();

	if(curItem < count)
	{
		pWp = &m_wpDbList[curItem];

		if(listBoxRouteWayPoints->count() > 0)
		{
			curItem = listBoxRouteWayPoints->currentRow() + 1;
		}
		else
		{
			curItem = 0;
		}

		m_wpRouteList.insert(curItem, pWp);
		showWpRoute(curItem);
	}
}

void RouteFormImpl::remove()
{
	uint curItem;

	curItem = listBoxRouteWayPoints->currentRow();
	m_wpRouteList.removeAt(curItem);

	if(curItem > 0)
	{
    curItem--;
	}
	else
	{
	  curItem = 0;
	}

	showWpRoute(curItem);
}

void RouteFormImpl::showWpRoute(uint curItem)
{
	QString wpName;
	uint nItems;
	uint itemNr;

	listBoxRouteWayPoints->clear();
	nItems = m_wpRouteList.count();

	for(itemNr=0; itemNr<nItems; itemNr++)
	{
		m_wpRouteList.at(itemNr)->fullName(wpName);
		listBoxRouteWayPoints->addItem(wpName);
	}

	listBoxRouteWayPoints->setCurrentRow(curItem);
}
