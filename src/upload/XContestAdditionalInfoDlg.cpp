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

  connect(pOkCancel,SIGNAL(accepted()),this,SLOT(accept()));
  connect(pOkCancel,SIGNAL(rejected()),this,SLOT(reject()));
}

QLayout* XContestAdditionalInfoDlg::buildDynamicGui() const
{
  QFormLayout* pElement = new QFormLayout();
  QFormLayout* pBoxLayout;
  QGroupBox* pBox;
  QWidget* pWidget;
  QVariantMap control;
  QString notes;
  QString label;

  for(QVariantMap::ConstIterator iter = m_controls.constBegin(); iter != m_controls.constEnd(); ++iter)
  {
    control = iter.value().toMap();
    label = control["label"].toString();

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
    pBoxLayout->addRow(label, pWidget);

    pElement->addRow(pBox);
  }

  return pElement;
}

QString XContestAdditionalInfoDlg::readNotes(const QVariantMap& control, const QString& key, const QString& prefix) const {
  QString result = "";

  if(control.find(key) != control.constEnd())
  {
    QVariantMap notes = control[key].toMap();
    for(QVariantMap::ConstIterator iter = notes.constBegin(); iter != notes.constEnd(); ++iter)
    {
      result += QString("(%1) %2: %3\n").arg(prefix,iter.key(),iter.value().toString());
    }
  }

  return result;
}
