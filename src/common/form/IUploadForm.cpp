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

#include "IUploadForm.h"
#include "UploadFormImpl.h"

IUploadForm::IUploadForm(QWidget* parent, const QString &caption, Flight *pFlight)
{
  m_pUploadForm = new UploadFormImpl(parent, caption, pFlight);
}

IUploadForm::~IUploadForm()
{
  delete m_pUploadForm;
}

bool IUploadForm::exec()
{
  return (m_pUploadForm->exec() == QDialog::Accepted);
}

void IUploadForm::enableInput(bool b)
{
  m_pUploadForm->enableInput(b);
}

void IUploadForm::setFlight(Flight *pFlight)
{
  m_pUploadForm->setFlight(pFlight);
}
