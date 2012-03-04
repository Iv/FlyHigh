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

#include "GliderFormImpl.h"
#include "Glider.h"

GliderFormImpl::GliderFormImpl(QWidget* parent, const QString &caption, Glider *pGlider,
                               const Glider::GliderListType& gliderList)
  :QDialog(parent)
{
  QStringList manus;
  QStringList models;
  QStringList serials;
  int gliderNr;

	setupUi(this);
	setWindowTitle(caption);
	Q_CHECK_PTR(pGlider);
	m_pGlider = pGlider;

	for(gliderNr=0; gliderNr<gliderList.count(); gliderNr++)
	{
	  manus.push_back(gliderList.value(gliderNr).manufacturer());
	  models.push_back(gliderList.value(gliderNr).model());
	}

  // manufacturers
	manus.removeDuplicates();
	manus.sort();
  manus.removeOne("");
  comboBoxManu->addItems(manus);

  // models
  models.removeDuplicates();
  models.sort();
  models.removeOne("");
  comboBoxModel->addItems(models);

	connect(comboBoxManu, SIGNAL(activated(int)), this, SLOT(updateGlider(int)));
	connect(comboBoxModel, SIGNAL(activated(int)), this, SLOT(updateGlider(int)));
}

void GliderFormImpl::accept()
{
	m_pGlider->setManufacturer(comboBoxManu->currentText());
	m_pGlider->setModel(comboBoxModel->currentText());
	m_pGlider->setSerial(lineEditSerial->text());

  QDialog::accept();
}

void GliderFormImpl::updateGlider(int index)
{
  // adjust all combo boxes to show glider data of the selected index
  comboBoxManu->setCurrentIndex(index);
  comboBoxModel->setCurrentIndex(index);
}
