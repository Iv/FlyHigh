/***************************************************************************
 *   Copyright (C) 2004 by grafal,,,                                       *
 *   grafal@spirit                                                         *
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
#include <qwidget.h>
#include <qstring.h>
#include "ISql.h"
#include "Route.h"
#include "RouteFormImpl.h"
#include "WayPoint.h"

RouteFormImpl::RouteFormImpl(QWidget* parent, const QString &caption, Route *pRoute)
	:RouteForm(parent, caption, true)
{
	WayPoint::WayPointListType wpList;
	uint nItems;
	uint curItem;
	
	Q_CHECK_PTR(pRoute);
	m_pRoute = pRoute;
	
	RouteForm::setCaption(caption);
	ISql::pInstance()->wayPointList(wpList);
	nItems = wpList.size();
	
	for(curItem=0; curItem<nItems; curItem++)
	{
		listBoxDBWayPoints->insertItem(wpList[curItem].name());
	}
}

void RouteFormImpl::accept()
{
	QString wpName;
	uint nItems;
	uint curItem;
	
	m_pRoute->setName(lineEditName->text());
	nItems = listBoxRouteWayPoints->count();
	
	for(curItem=0; curItem<nItems; curItem++)
	{
		wpName = listBoxRouteWayPoints->text(curItem);
		m_pRoute->wayPointList().push_back(wpName);
	}

	QDialog::accept();
}

void RouteFormImpl::down()
{
	QString curText;
	QString downText;
	uint curItem;

	curItem = listBoxRouteWayPoints->currentItem();
	
	if(curItem < (listBoxRouteWayPoints->count() - 1))
	{
		curText = listBoxRouteWayPoints->text(curItem);
		downText = listBoxRouteWayPoints->text(curItem+1);
		listBoxRouteWayPoints->changeItem(curText, curItem+1);
		listBoxRouteWayPoints->changeItem(downText, curItem);
		listBoxRouteWayPoints->setCurrentItem(curItem+1);
	}
}

void RouteFormImpl::up()
{
	QString curText;
	QString upText;
	uint curItem;

	curItem = listBoxRouteWayPoints->currentItem();
	
	if(curItem > 0)
	{
		curText = listBoxRouteWayPoints->text(curItem);
		upText = listBoxRouteWayPoints->text(curItem-1);
		listBoxRouteWayPoints->changeItem(curText, curItem-1);
		listBoxRouteWayPoints->changeItem(upText, curItem);
		listBoxRouteWayPoints->setCurrentItem(curItem-1);
	}
}

void RouteFormImpl::toRight()
{
	QString str;
	
	str = listBoxDBWayPoints->currentText();
	listBoxRouteWayPoints->insertItem(str);
}

void RouteFormImpl::remove()
{
	uint curItem;
	
	curItem = listBoxRouteWayPoints->currentItem();
	listBoxRouteWayPoints->removeItem(curItem);
}

#include "RouteFormImpl.moc"
