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

#include <qcheckbox.h>
#include <qslider.h>
#include <qspinbox.h>
#include "AcousticFrameImpl.h"
#include "Flytec5020.h"

AcousticFrameImpl::AcousticFrameImpl(QWidget* parent, const char* name, Qt::WFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

AcousticFrameImpl::~AcousticFrameImpl()
{
}

void AcousticFrameImpl::sinkAcousticToggled(bool b)
{
	checkBox_SinkAcoustic->setChecked(b);
	spinBox_Sink->setEnabled(b);
	spinBox_DownFreq->setEnabled(b);
}

void AcousticFrameImpl::update()
{
  Flytec5020 *pFlytec;
  bool enabled;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Base Volume
	slider_Volume->setValue(pFlytec->parRead(MemUndef, BASE_VOL_BEEP_POS, FtUInt8).toUInt());

	// Vario Offset
	spinBox_Threshold->setValue(pFlytec->parRead(MemUndef, ACOUSTIC_LIFT_THR_POS, FtUInt8).toUInt());

	// Up Base Frequency
	spinBox_UpFreq->setValue(pFlytec->parRead(MemUndef, UP_BASE_FRQ_POS, FtInt16).toInt());

	// Frequency Modulation
	slider_FreqMod->setValue(pFlytec->parRead(MemUndef, FRQ_MODULATION_POS, FtUInt8).toUInt());

	// Acoustic Pitch
	slider_Pitch->setValue(pFlytec->parRead(MemUndef, ACOUSTIC_PITCH_POS, FtUInt8).toUInt());

	// Acoustic Integral
	slider_Integration->setValue(pFlytec->parRead(MemUndef, ACOUSTIC_I_POS, FtUInt8).toUInt());

	// Sink Acoustic Enabled
	enabled = (pFlytec->parRead(MemUndef, SINC_ACOUSTIC_EN_POS, FtUInt8).toUInt() == 1);
	checkBox_SinkAcoustic->setChecked(enabled);
	spinBox_Sink->setEnabled(enabled);
	spinBox_DownFreq->setEnabled(enabled);

	// Down Base Frequency
	spinBox_DownFreq->setValue(pFlytec->parRead(MemUndef, DOWN_BASE_FRQ_POS, FtInt16).toInt());

	// Sink
	spinBox_Sink->setValue(pFlytec->parRead(MemUndef, ACOUSTIC_SINK_POS, FtInt8).toInt());
}

void AcousticFrameImpl::store()
{
  Flytec5020 *pFlytec;

  pFlytec = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Base Volume
	pFlytec->parWrite(MemUndef, BASE_VOL_BEEP_POS, FtUInt8, slider_Volume->value());

	// Vario Offset
	pFlytec->parWrite(MemUndef, ACOUSTIC_LIFT_THR_POS, FtUInt8, spinBox_Threshold->value());

	// Up Base Frequency
	pFlytec->parWrite(MemUndef, UP_BASE_FRQ_POS, FtUInt16, spinBox_UpFreq->value());

	// Frequency Modulation
	pFlytec->parWrite(MemUndef, FRQ_MODULATION_POS, FtUInt8, slider_FreqMod->value());

	// Acoustic Pitch
  pFlytec->parWrite(MemUndef, ACOUSTIC_PITCH_POS, FtUInt8, slider_Pitch->value());

	// Acoustic Integral
	pFlytec->parWrite(MemUndef, ACOUSTIC_I_POS, FtUInt8, slider_Integration->value());

	// Sink Acoustic Enabled
	pFlytec->parWrite(MemUndef, SINC_ACOUSTIC_EN_POS, FtUInt8, checkBox_SinkAcoustic->isChecked());

	// Down Base Frequency
	pFlytec->parWrite(MemUndef, DOWN_BASE_FRQ_POS, FtUInt16, spinBox_DownFreq->value());

	// Sink
	pFlytec->parWrite(MemUndef, ACOUSTIC_SINK_POS, FtInt8, spinBox_Sink->value());
}
