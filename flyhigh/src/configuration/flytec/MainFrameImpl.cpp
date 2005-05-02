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

#include <qcursor.h>
#include <qmessagebox.h>
#include <qtoolbox.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qstatusbar.h>
#include <qprogressbar.h>
#include <qfile.h>
#include <qfiledialog.h>

#include "CorrFrameImpl.h"
#include "VarioFrameImpl.h"
#include "AcousticFrameImpl.h"
#include "VelocityFrameImpl.h"
#include "GPSFrameImpl.h"
#include "DisplayFrameImpl.h"
#include "FlyCalcFrameImpl.h"
#include "DeviceFrameImpl.h"
#include "IFlyHighRC.h"
#include "MemoryFrameImpl.h"
#include "PolareFrameImpl.h"
#include "UnitFrameImpl.h"
#include "Frame.h"
#include "MainFrameImpl.h"
#include "IGPSDevice.h"

MainFrameImpl::MainFrameImpl(QWidget* parent, const char* name, WFlags fl)
	:MainFrame(parent,name,fl)
{
	QWidget *pWidget;
	VelocityFrameImpl *pVelocityFrame;
	AcousticFrameImpl *pAcousticFrame;
	VarioFrameImpl *pVarioFrame;
	CorrFrameImpl *pCorrFrame;
	GPSFrameImpl *pGPSFrame;
	DisplayFrameImpl *pDisplayFrame;
	FlyCalcFrameImpl *pFlyCalcFrame;
	DeviceFrameImpl *pDeviceFrame;
	MemoryFrameImpl *pMemoryFrame;
	PolareFrameImpl *pPolareFrame;
	UnitFrameImpl *pUnitFrame;
	int pos = 0;
	
	m_fileName = "";
	
	// make a clean table
	pWidget = toolBox->item(0);
	toolBox->removeItem(pWidget);
	pWidget = widgetStack->widget(0);
	widgetStack->removeWidget(pWidget);
	
	// Correction
	pCorrFrame = new CorrFrameImpl(widgetStack);
	addPage(pCorrFrame, &pos);
	pCorrFrame->setEnabled(false);
	m_FrameList.push_back(pCorrFrame);
	
	// Vario
	pVarioFrame = new VarioFrameImpl(widgetStack);
	addPage(pVarioFrame, &pos);
	m_FrameList.push_back(pVarioFrame);

	// Acoustic
	pAcousticFrame = new AcousticFrameImpl(widgetStack);
	addPage(pAcousticFrame, &pos);
	m_FrameList.push_back(pAcousticFrame);
	
	// Velocity
	pVelocityFrame = new VelocityFrameImpl(widgetStack);
	addPage(pVelocityFrame, &pos);
	m_FrameList.push_back(pVelocityFrame);
	
	// GPS
	pGPSFrame = new GPSFrameImpl(widgetStack);
	addPage(pGPSFrame, &pos);
	m_FrameList.push_back(pGPSFrame);

	// Fly Calculator (not available for 5020)
	pFlyCalcFrame = new FlyCalcFrameImpl(widgetStack);
	addPage(pFlyCalcFrame, &pos);
	pFlyCalcFrame->setEnabled(false);
	m_FrameList.push_back(pFlyCalcFrame);
	
	// Device
	pDeviceFrame = new DeviceFrameImpl(widgetStack);
	addPage(pDeviceFrame, &pos);
	m_FrameList.push_back(pDeviceFrame);
	
	// Memory
	pMemoryFrame = new MemoryFrameImpl(widgetStack);
	addPage(pMemoryFrame, &pos);
	m_FrameList.push_back(pMemoryFrame);
	
	// Display
	pDisplayFrame = new DisplayFrameImpl(widgetStack);
	addPage(pDisplayFrame, &pos);
	m_FrameList.push_back(pDisplayFrame);
	
	// Polare
	pPolareFrame = new PolareFrameImpl(widgetStack);
	addPage(pPolareFrame, &pos);
	m_FrameList.push_back(pPolareFrame);
	
	// Unit
	pUnitFrame = new UnitFrameImpl(widgetStack);
	addPage(pUnitFrame, &pos);
	m_FrameList.push_back(pUnitFrame);
	
/*	m_pProgressBar = new QProgressBar(statusBar());
	m_pProgressBar->setProgress(0, ft_MemSize/ft_PageSize);
	m_pProgressBar->setShown(false);
	statusBar()->addWidget(m_pProgressBar, 1, true);*/
}

MainFrameImpl::~MainFrameImpl()
{
}

void MainFrameImpl::addPage( QWidget * pFrame, int * pPos)
{
	QWidget *pWidget;
	
	widgetStack->addWidget(pFrame, *pPos);
	(*pPos)++;
	pWidget = new QWidget(toolBox,  pFrame->caption());
	pWidget->setBackgroundMode(QWidget::PaletteBackground);
	toolBox->addItem(pWidget,  pFrame->caption());
}

void MainFrameImpl::open()
{
	QFileDialog fileDlg(IFlyHighRC::pInstance()->lastDir(), "Flytec Config Files (*.flt)", this,
					"Flytec config file open", true);

	if(fileDlg.exec() == QDialog::Accepted)
	{
		IFlyHighRC::pInstance()->setLastDir(fileDlg.dirPath());
		m_fileName = fileDlg.selectedFile();
		execThreadCmd(OpenConfig);
	}
}

void MainFrameImpl::save()
{
	QFileDialog fileDlg(IFlyHighRC::pInstance()->lastDir(), "Flytec Config Files (*.flt)", this,
					"Flytec config file open", true);

	if(fileDlg.exec() == QDialog::Accepted)
	{
		IFlyHighRC::pInstance()->setLastDir(fileDlg.dirPath());
		m_fileName = fileDlg.selectedFile();
		execThreadCmd(SaveConfig);
	}
}

void MainFrameImpl::read()
{
	execThreadCmd(ReadConfig);
}

void MainFrameImpl::write()
{
	if(QMessageBox::question(this, tr("write configuration"), 
		tr("Write current configuration to the device?"), 1, 2) == 1)
	{
		execThreadCmd(WriteConfig);
	}
}

void MainFrameImpl::execThreadCmd(CmdType cmd)
{
	m_cmd = cmd;
	QThread::start();
}

void MainFrameImpl::run()
{
	QFile file;
	
/*	m_pProgressBar->setProgress(0);
	m_pProgressBar->setShown(true);
	*/
	MainFrame::setCursor(QCursor(Qt::WaitCursor));
	
	switch(m_cmd)
	{
		case OpenConfig:
			file.setName(m_fileName);
			
			if(file.open(IO_ReadOnly))
			{
				m_flytecMem = file.readAll();
				file.close();
				updateFrames();
			}
		break;
		case SaveConfig:
			file.setName(m_fileName);
		
			if(file.open(IO_WriteOnly))
			{
				storeFrames();
				file.writeBlock(m_flytecMem);
				file.close();
			}
		break;
		case ReadConfig:
			if(IGPSDevice::pInstance()->memoryRead(m_flytecMem))
			{
				updateFrames();
			}
		break;
		case WriteConfig:
			storeFrames();
			IGPSDevice::pInstance()->memoryWrite(m_flytecMem);
		break;
	}
	
	MainFrame::unsetCursor();
	
//	m_pProgressBar->setShown(false);
}

void MainFrameImpl::storeFrames()
{
	FrameListType::iterator it;
	
	// update Frames
	for(it=m_FrameList.begin(); it!=m_FrameList.end(); it++)
	{
		(*it)->store(m_flytecMem);
	}
}

void MainFrameImpl::updateFrames()
{
	FrameListType::iterator it;
	
	// update Frames
	for(it=m_FrameList.begin(); it!=m_FrameList.end(); it++)
	{
		(*it)->update(m_flytecMem);
	}
}

#include "MainFrameImpl.moc"

