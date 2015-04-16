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
    QVariantList options = control["options"].toList();
    int selectedIdx = -1;
    int idx = 0;

    // check if control allows an empty value
    if(control.contains("emptyValueLabel"))
    {
      pBox->addItem(control["emptyValueLabel"].toString(),"");
      selectedIdx = 0;
    }
    // fill in options
    for(QVariantList::ConstIterator iter = options.constBegin(); iter != options.constEnd(); ++iter)
    {
      QVariantMap option = iter->toMap();

      pBox->addItem(option["text"].toString(),option["value"].toString());
      // check for preselected option
      if(option["selected"].toString() == "true")
      {
        selectedIdx = idx;
      }
      ++idx;
    }

    pBox->setCurrentIndex(selectedIdx);

    if(type == "TYPE_TEXT_CHOOSE_ONE")
    {
      pBox->setEditable(true);
    } else if (type == "TYPE_CHOOSE_MORE")
    {
      // qt4 does not offer multiselection comboboxes
      // you have to live with a single choice
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
    // other types of controls not implemented, e.g. TYPE_FILE
  }

  return pWidget;
}
