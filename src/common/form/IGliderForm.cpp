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
 
#include "IGliderForm.h"
#include "GliderFormImpl.h"

IGliderForm::IGliderForm(QWidget* parent, const QString &caption, Glider *pGlider)
{
	m_pGliderForm = new GliderFormImpl(parent, caption, pGlider);
}

IGliderForm::~IGliderForm()
{
	delete m_pGliderForm;
}

bool IGliderForm::exec()
{
	return (m_pGliderForm->exec() == QDialog::Accepted);
}
