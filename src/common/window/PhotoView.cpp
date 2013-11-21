/***************************************************************************
 *   Copyright (C) 2013 by Alex Graf                                       *
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

#include <QDir>
#include <QLabel>
#include <QPushButton>
#include "PhotoView.h"

PhotoView::PhotoView(QWidget* parent, const QString &caption, Qt::WindowFlags wflags)
  :QDialog(parent)
{
  m_pPhoto = new QLabel(this);
  m_pPrev = new QPushButton(this);
  m_pNext = new QPushButton(this);
  m_path = "";
  m_curPhoto = 0;

  m_pPhoto->setAlignment(Qt::AlignCenter);
  m_pPrev->setFlat(true);
  m_pNext->setFlat(true);

  connect(m_pPrev, SIGNAL(clicked()), this, SLOT(prev()));
  connect(m_pNext, SIGNAL(clicked()), this, SLOT(next()));

  setWindowTitle(caption);
  updateSize();
}

PhotoView::~PhotoView()
{
  delete m_pPhoto;
  delete m_pPrev;
  delete m_pNext;
}

void PhotoView::setFolder(const QString &path)
{
  QDir dir;
  QStringList filter;

  m_files.clear();
  m_path = path;
  m_curPhoto = 0;
  filter << "*.jpg" << "*.JPG" << "*.jpeg" << "*.JPEG" << "*.png";
  dir.cd(path);
  m_path = dir.absolutePath();
  m_files = dir.entryList(filter, QDir::Files);

  if(m_files.size() > 0)
  {
    updatePhoto();
  }
}

int PhotoView::photoCount() const
{
  return m_files.size();
}

void PhotoView::resizeEvent(QResizeEvent *pEvent)
{
  updateSize();
}

void PhotoView::updateSize()
{
  enum {NavWidth = 100};

  m_pPhoto->setGeometry(0, 0, width(), height());
  m_pPrev->setGeometry(0, 0, NavWidth, height());
  m_pNext->setGeometry(width() - NavWidth, 0, NavWidth, height());
  m_pPhoto->setPixmap(m_orgPhoto.scaled(width(), height(), Qt::KeepAspectRatio));
}

void PhotoView::prev()
{
  if(m_curPhoto > 0)
  {
    m_curPhoto--;
    updatePhoto();
  }
}

void PhotoView::next()
{
  if(m_curPhoto < (m_files.size() - 1))
  {
    m_curPhoto++;
    updatePhoto();
  }
}

void PhotoView::updatePhoto()
{
  QString path;

  path = m_path + "/" + m_files[m_curPhoto];
  m_orgPhoto.load(path);
  m_pPhoto->setPixmap(m_orgPhoto.scaled(width(), height(), Qt::KeepAspectRatio));
}
