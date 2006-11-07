/***************************************************************************
 *   Copyright (C) 2004 by Alex Graf                                       *
 *   grafal@sourceforge.net                                                         *
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

#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qstring.h>
#include "ISql.h"
#include "Route.h"
#include "RouteFormImpl.h"
#include "WayPoint.h"

RouteFormImpl::RouteFormImpl(QWidget* parent, const QString &caption, Route *pRoute)
	:RouteForm(parent, caption, true)
{
	WayPoint *pWp;
	QString fullName;
	uint nItems;
	uint curItem;
	
	Q_CHECK_PTR(pRoute);
	m_pRoute = pRoute;
	m_readOnly = false;
	
	RouteForm::setCaption(caption);

	// database waypoints
	ISql::pInstance()->wayPointList(m_wpDbList);
	nItems = m_wpDbList.size();
	
	for(curItem=0; curItem<nItems; curItem++)
	{
		m_wpDbList.at(curItem).fullName(fullName);
		listBoxDBWayPoints->insertItem(fullName);
	}

	// route
	lineEditName->setText(m_pRoute->name());
	nItems = m_pRoute->wayPointList().size();
	
	for(curItem=0; curItem<nItems; curItem++)
	{
		pWp = &(m_pRoute->wayPointList().at(curItem));
		m_wpRouteList.append(pWp);
	}

	showWpRoute();
}

void RouteFormImpl::setReadOnly()
{
	m_readOnly = true;

	lineEditName->setEnabled(false);
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
	uint curItem;
	WayPoint *pWp;

	curItem = listBoxRouteWayPoints->currentItem();
	
	if(curItem < (m_wpRouteList.count() - 1))
	{
		pWp = m_wpRouteList.at(curItem);
		m_wpRouteList.remove(curItem);
		m_wpRouteList.insert(curItem+1, pWp);
		showWpRoute();
	}
}

void RouteFormImpl::up()
{
	uint curItem;
	WayPoint *pWp;

	curItem = listBoxRouteWayPoints->currentItem();
	
	if(curItem > 0)
	{
		pWp = m_wpRouteList.at(curItem);
		m_wpRouteList.remove(curItem);
		m_wpRouteList.insert(curItem-1, pWp);
		showWpRoute();
	}
}

void RouteFormImpl::toRight()
{
	uint curItem;
	uint count;
	WayPoint *pWp;

	curItem = listBoxDBWayPoints->currentItem();
	count = listBoxDBWayPoints->count();

	if(curItem < count)
	{
		pWp = &m_wpDbList.at(curItem);
	
		if(listBoxRouteWayPoints->count() > 0)
		{
			curItem = listBoxRouteWayPoints->currentItem() + 1;
		}
		else
		{
			curItem = 0;
		}
	
		m_wpRouteList.insert(curItem, pWp);
		showWpRoute();
	}
}

void RouteFormImpl::remove()
{
	uint curItem;
	
	curItem = listBoxRouteWayPoints->currentItem();
	m_wpRouteList.remove(curItem);
	showWpRoute();
}

void RouteFormImpl::showWpRoute()
{
	QString wpName;
	uint nItems;
	uint selItem;
	uint curItem;

	selItem = m_wpRouteList.at();
	listBoxRouteWayPoints->clear();
	nItems = m_wpRouteList.count();
	
	for(curItem=0; curItem<nItems; curItem++)
	{
		m_wpRouteList.at(curItem)->fullName(wpName);
		listBoxRouteWayPoints->insertItem(wpName);
	}

	listBoxRouteWayPoints->setCurrentItem(selItem);
}


#include "RouteFormImpl.moc"
