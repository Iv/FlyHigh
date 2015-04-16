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

#include <QDialogButtonBox>
#include <QLabel>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QAbstractButton>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QStringList>
#include "XContestAdditionalInfoWidgetFactory.h"
#include "XContestAdditionalInfoDlg.h"

#include <QDebug>


XContestAdditionalInfoDlg::XContestAdditionalInfoDlg(const QVariantMap& controls)
{
  selectControls(controls);
  buildGui();
}

/**
 * Choose which controls will be included in the dialog. We don't show
 * all of them (all controls are present, including autocompleted and already sumbitted ones),
 * but just those that are:
 * - marked as invalid (these where causing the additional request)
 * - dependent on invalid ones
 */
void XContestAdditionalInfoDlg::selectControls(const QVariantMap& controls)
{
  QVariantMap control;
  bool isValid;
  QString alternative;

  for(QVariantMap::ConstIterator iter = controls.constBegin(); iter != controls.constEnd(); ++iter)
  {
    control = iter.value().toMap();
    isValid = control["isValid"].toString() == "true";

    if(isValid == false)
    {
      m_controls.insert(iter.key(),iter.value());

      alternative = control["alternativeFor"].toString();

      if(!alternative.isNull() && !alternative.isEmpty())
      {
        QVariantMap::ConstIterator altIt = controls.find(alternative);
        if(altIt != controls.end() && !m_controls.contains(altIt.key()))
        {
          m_controls.insert(altIt.key(), altIt.value());
        }
      }
    }
  }
}

void XContestAdditionalInfoDlg::clarifications(QMap<QString,QString>& clarifications) const
{
  clarifications = m_clarifications;
}

void XContestAdditionalInfoDlg::buildGui()
{
  QLayout* pLayout;
  QDialogButtonBox* pOkCancel;

  QWidget::setWindowTitle(tr("Additional info required"));
  QGridLayout* pGrid = new QGridLayout();
  setLayout(pGrid);

  // create dynamic content
  pLayout = buildDynamicGui();

  pOkCancel = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  pGrid->addLayout(pLayout, 0,0,1,2);
  pGrid->addWidget(pOkCancel, 2, 1);

  adjustSize();

  connect(pOkCancel,SIGNAL(accepted()),this,SLOT(submit()));
  connect(pOkCancel,SIGNAL(rejected()),this,SLOT(reject()));
}

QLayout* XContestAdditionalInfoDlg::buildDynamicGui()
{
  QFormLayout* pElement = new QFormLayout();
  QFormLayout* pBoxLayout;
  QGroupBox* pBox;
  QWidget* pWidget;
  QVariantMap control;
  QString notes;
  QString label;
  QString name;

  for(QVariantMap::ConstIterator iter = m_controls.constBegin(); iter != m_controls.constEnd(); ++iter)
  {
    control = iter.value().toMap();
    if(control.contains("label"))
    {
      label = control["label"].toString();
    }
    else
    {
      // fallback if control has no label. we'd like to add a proper title to the widget
      label = control["ident"].toString();
    }
    name = control["name"].toString();

    pBox = new QGroupBox(label);
    pBoxLayout = new QFormLayout();
    pBoxLayout->setLabelAlignment(Qt::AlignLeft);
    pBox->setLayout(pBoxLayout);

    notes = "";
    notes += readNotes(control,"errors","E");
    notes += readNotes(control,"warnings","W");

    if(notes.isEmpty())
    {
      notes = "None";
    }

    pBoxLayout->addRow("Notes:", new QLabel(notes));

    pWidget = XContestAdditionalInfoWidgetFactory::createWidget(control);
    m_widgets.insert(pWidget,name);
    pBoxLayout->addRow(label, pWidget);

    pElement->addRow(pBox);
  }

  return pElement;
}

QString XContestAdditionalInfoDlg::readNotes(const QVariantMap& control, const QString& key, const QString& prefix) const {
  QString result = "";
  QVariantMap messageMap;

  // fill messages map first
  QVariantList messages = control["messages"].toList();
  for(QVariantList::ConstIterator iter = messages.constBegin(); iter != messages.constEnd(); ++iter)
  {
    QVariantMap message = iter->toMap();
    messageMap.insert(message["code"].toString(), message["label"]);
  }

  if(control.find(key) != control.constEnd())
  {
    QVariantMap notes = control[key].toMap();
    for(QVariantMap::ConstIterator iter = notes.constBegin(); iter != notes.constEnd(); ++iter)
    {
      if(messageMap.contains(iter.key()))
      {
        result += QString("(%1) %2\n     %3").arg(prefix,iter.value().toString(), messageMap[iter.key()].toString());
      }
      else
      {
        result += QString("(%1) %2\n").arg(prefix,iter.value().toString());
      }
    }
  }

  return result;
}

void XContestAdditionalInfoDlg::submit()
{
  QString value;
  // read values from widgets and fill into clarifications

  for(QMap<QWidget*,QString>::Iterator iter = m_widgets.begin(); iter != m_widgets.end(); ++iter)
  {
    if(qobject_cast<QComboBox*>(iter.key())!=NULL)
    {
      // is a combobox
      QComboBox* pBox = dynamic_cast<QComboBox*>(iter.key());
      if(pBox->isEditable())
      {
        value = pBox->currentText();
      }
      else
      {
        value = pBox->itemData(pBox->currentIndex()).toString();
      }
    }
    else if(qobject_cast<QAbstractButton*>(iter.key())!=NULL)
    {
      // is a checkbox
      QAbstractButton* pCheckBox = dynamic_cast<QAbstractButton*>(iter.key());
      value = pCheckBox->isChecked()?"Y":"N";
    }
    else if(qobject_cast<QPlainTextEdit*>(iter.key())!=NULL)
    {
      // is a text field
      QPlainTextEdit* pTextEdit = dynamic_cast<QPlainTextEdit*>(iter.key());
      value = pTextEdit->toPlainText();
    }
    if(!value.isEmpty())
    {
      qDebug() << "Add clarification " << iter.value() << "=" << value;
      m_clarifications.insert(iter.value(),value);
    }
  }

  QDialog::accept();
}
