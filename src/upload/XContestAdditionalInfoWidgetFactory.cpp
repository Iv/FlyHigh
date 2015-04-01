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

#include <QPlainTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QWidget>
#include "XContestAdditionalInfoWidgetFactory.h"

#include <QDebug>


XContestAdditionalInfoWidgetFactory::XContestAdditionalInfoWidgetFactory()
{
}

QWidget* XContestAdditionalInfoWidgetFactory::createWidget(const QVariantMap &control)
{
  QWidget* pWidget = NULL;
  QString type = control["type"].toString();

  if(type == "TYPE_CHOOSE_ONE" || type == "TYPE_TEXT_CHOOSE_ONE" || type == "TYPE_CHOOSE_MORE")
  {
    QComboBox* pBox = new QComboBox();
    int selectedIdx = -1;
    int idx = 0;

    QVariantList options = control["options"].toList();

    for(QVariantList::ConstIterator iter = options.constBegin(); iter != options.constEnd(); ++iter)
    {
      QVariantMap option = iter->toMap();
      QString text = option["text"].toString();
      QString value = option["value"].toString();
      bool selected = option["selected"].toString() == "true";

      pBox->addItem(text,value);
      if(selected)
      {
        selectedIdx = idx;
      }
      ++idx;
    }

    pBox->setCurrentIndex(selectedIdx);

    if(type == "TYPE_TEXT_CHOOSE_ONE")
    {
      pBox->setEditable(true);
    }
    else if (type == "TYPE_CHOOSE_MORE")
    {
      qDebug() << "Multiselection not supported";
    }

    pWidget = pBox;
  } else if (type == "TYPE_LOGICAL")
  {
    QCheckBox* pBox = new QCheckBox();
    pWidget = pBox;
  } else if (type == "TYPE_TEXT")
  {
    QString value = control["value"].toString();
    pWidget = new QPlainTextEdit(value);
  } else
  {
    qDebug() << type << "not implemented";
  }

  return pWidget;
}
