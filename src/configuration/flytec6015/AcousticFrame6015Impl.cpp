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
#include <qcombobox.h>
#include <qslider.h>
#include <qspinbox.h>

#include "AcousticFrame6015Impl.h"
#include "Flytec5020.h"

AcousticFrame6015Impl::AcousticFrame6015Impl(QWidget* parent, const char* name, Qt::WFlags fl)
: QWidget(parent)
{
  setupUi(this);
}

AcousticFrame6015Impl::~AcousticFrame6015Impl()
{
}

void AcousticFrame6015Impl::update()
{
	Flytec5020 *pDev;
	uint uiValue;
	int iValue;

	pDev = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Volume
	uiValue = pDev->parRead(MemFa, AUDIO_VOLUME, FtUInt8).toUInt();
	slider_Volume->setValue(uiValue);

	// Rise acoustic, sink alarm, stall alarm, rise pitch
	uiValue = pDev->parRead(MemFa, DIV_FLAGS, FtUInt16).toUInt();
	checkBox_SinkAlarm->setChecked(uiValue & MASK_SINK_ALARM);
	checkBox_RiseAcoustic->setChecked(uiValue & MASK_RISE_ACC);
	checkBox_StallAlarm->setChecked(uiValue & MASK_STALL_ALARM);
  comboBox_RisePitch->setCurrentIndex((uiValue & MASK_RISE_PITCH) >> POS_RISE_PITCH);

	// Frequency gain
	uiValue = pDev->parRead(MemFa, FREQ_GAIN, FtUInt8).toUInt();
	slider_FreqGain->setValue(uiValue);

	// Pitch gain
	uiValue = pDev->parRead(MemFa, PITCH_GAIN, FtUInt8).toUInt();
	slider_PitchGain->setValue(uiValue);

	// Rise frequency
	uiValue = pDev->parRead(MemFa, B_FREQ_RISE, FtUInt16).toUInt();
	spinBox_UpFreq->setValue(uiValue);

	// Sink frequency
	uiValue = pDev->parRead(MemFa, B_FREQ_SINK, FtUInt16).toUInt();
	spinBox_DownFreq->setValue(uiValue);

	// Rise threshold
	iValue = pDev->parRead(MemFa, AUDIO_RISE, FtInt16).toInt();
	spinBox_ThresUp->setValue(iValue);

// Sink threshold
iValue = pDev->parRead(MemFa, AUDIO_SINK, FtInt16).toInt();
spinBox_ThresDown->setValue(iValue);

	// Sink alarm threshold
	iValue = pDev->parRead(MemFa, SINK_ALARM, FtInt16).toInt();
	spinBox_ThresSinkAlarm->setValue(iValue);
}

void AcousticFrame6015Impl::store()
{
	Flytec5020 *pDev;
	uint uiValue;
	int iValue;

	pDev = static_cast<Flytec5020*>(IGPSDevice::pInstance());

	// Volume
	uiValue = slider_Volume->value();
	pDev->parWrite(MemFa, AUDIO_VOLUME, FtUInt8, uiValue);

	// Rise acoustic, sink alarm, stall alarm, rise pitch
	uiValue = pDev->parRead(MemFa, DIV_FLAGS, FtUInt16).toUInt();

	uiValue = (uiValue & ~MASK_SINK_ALARM) | (-checkBox_SinkAlarm->isChecked() & MASK_SINK_ALARM);
	uiValue = (uiValue & ~MASK_RISE_ACC) | (-checkBox_RiseAcoustic->isChecked() & MASK_RISE_ACC);
	uiValue = (uiValue & ~MASK_STALL_ALARM) | (-checkBox_StallAlarm->isChecked() & MASK_STALL_ALARM);

	uiValue &= ~MASK_RISE_PITCH;
  uiValue |= (comboBox_RisePitch->currentIndex() << POS_RISE_PITCH);

	pDev->parWrite(MemFa, DIV_FLAGS, FtUInt16, uiValue);

	// Frequency gain
	uiValue = slider_FreqGain->value();
	pDev->parWrite(MemFa, FREQ_GAIN, FtUInt8, uiValue);

	// Pitch gain
	uiValue = slider_PitchGain->value();
	pDev->parWrite(MemFa, PITCH_GAIN, FtUInt8, uiValue);

	// Rise frequency
	uiValue = spinBox_UpFreq->value();
	pDev->parWrite(MemFa, B_FREQ_RISE, FtUInt16, uiValue);

	// Sink frequency
	uiValue = spinBox_DownFreq->value();
	pDev->parWrite(MemFa, B_FREQ_SINK, FtUInt16, uiValue);

	// Rise threshold
	iValue = spinBox_ThresUp->value();
	pDev->parWrite(MemFa, AUDIO_RISE, FtInt16, iValue);

// Sink threshold
iValue = spinBox_ThresDown->value();
pDev->parWrite(MemFa, AUDIO_SINK, FtInt16, iValue);

	// Sink alarm threshold
	iValue = spinBox_ThresSinkAlarm->value();
	pDev->parWrite(MemFa, SINK_ALARM, FtInt16, iValue);
}
