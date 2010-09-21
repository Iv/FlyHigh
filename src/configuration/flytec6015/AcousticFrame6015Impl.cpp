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

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qslider.h>
#include <qspinbox.h>

#include "AcousticFrame6015Impl.h"
#include "Flytec6015.h"

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
	Flytec6015 *pDev;
	uint uiValue;
	int iValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Volume
	uiValue = pDev->memoryRead(MemFa, AUDIO_VOLUME, UInt8).toUInt();
	slider_Volume->setValue(uiValue);

	// Rise acoustic, sink alarm, stall alarm, rise pitch
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();
	checkBox_SinkAlarm->setChecked(uiValue & MASK_SINK_ALARM);
	checkBox_RiseAcoustic->setChecked(uiValue & MASK_RISE_ACC);
	checkBox_StallAlarm->setChecked(uiValue & MASK_STALL_ALARM);
        comboBox_RisePitch->setCurrentIndex((uiValue & MASK_RISE_PITCH) >> POS_RISE_PITCH);

	// Frequency gain
	uiValue = pDev->memoryRead(MemFa, FREQ_GAIN, UInt8).toUInt();
	slider_FreqGain->setValue(uiValue);

	// Pitch gain
	uiValue = pDev->memoryRead(MemFa, PITCH_GAIN, UInt8).toUInt();
	slider_PitchGain->setValue(uiValue);

	// Rise frequency
	uiValue = pDev->memoryRead(MemFa, B_FREQ_RISE, UInt16).toUInt();
	spinBox_UpFreq->setValue(uiValue);

	// Sink frequency
	uiValue = pDev->memoryRead(MemFa, B_FREQ_SINK, UInt16).toUInt();
	spinBox_DownFreq->setValue(uiValue);

	// Rise threshold
	iValue = pDev->memoryRead(MemFa, AUDIO_RISE, Int16).toInt();
	spinBox_ThresUp->setValue(iValue);

// Sink threshold
iValue = pDev->memoryRead(MemFa, AUDIO_SINK, Int16).toInt();
spinBox_ThresDown->setValue(iValue);

	// Sink alarm threshold
	iValue = pDev->memoryRead(MemFa, SINK_ALARM, Int16).toInt();
	spinBox_ThresSinkAlarm->setValue(iValue);
}

void AcousticFrame6015Impl::store()
{
	Flytec6015 *pDev;
	uint uiValue;
	int iValue;

	pDev = static_cast<Flytec6015*>(IGPSDevice::pInstance());

	// Volume
	uiValue = slider_Volume->value();
	pDev->memoryWrite(MemFa, AUDIO_VOLUME, UInt8, uiValue);

	// Rise acoustic, sink alarm, stall alarm, rise pitch
	uiValue = pDev->memoryRead(MemFa, DIV_FLAGS, UInt16).toUInt();

	uiValue = (uiValue & ~MASK_SINK_ALARM) | (-checkBox_SinkAlarm->isChecked() & MASK_SINK_ALARM);
	uiValue = (uiValue & ~MASK_RISE_ACC) | (-checkBox_RiseAcoustic->isChecked() & MASK_RISE_ACC);
	uiValue = (uiValue & ~MASK_STALL_ALARM) | (-checkBox_StallAlarm->isChecked() & MASK_STALL_ALARM);

	uiValue &= ~MASK_RISE_PITCH;
        uiValue |= (comboBox_RisePitch->currentIndex() << POS_RISE_PITCH);

	pDev->memoryWrite(MemFa, DIV_FLAGS, UInt16, uiValue);

	// Frequency gain
	uiValue = slider_FreqGain->value();
	pDev->memoryWrite(MemFa, FREQ_GAIN, UInt8, uiValue);

	// Pitch gain
	uiValue = slider_PitchGain->value();
	pDev->memoryWrite(MemFa, PITCH_GAIN, UInt8, uiValue);

	// Rise frequency
	uiValue = spinBox_UpFreq->value();
	pDev->memoryWrite(MemFa, B_FREQ_RISE, UInt16, uiValue);

	// Sink frequency
	uiValue = spinBox_DownFreq->value();
	pDev->memoryWrite(MemFa, B_FREQ_SINK, UInt16, uiValue);

	// Rise threshold
	iValue = spinBox_ThresUp->value();
	pDev->memoryWrite(MemFa, AUDIO_RISE, Int16, iValue);

// Sink threshold
iValue = spinBox_ThresDown->value();
pDev->memoryWrite(MemFa, AUDIO_SINK, Int16, iValue);

	// Sink alarm threshold
	iValue = spinBox_ThresSinkAlarm->value();
	pDev->memoryWrite(MemFa, SINK_ALARM, Int16, iValue);
}


#include "moc_AcousticFrame6015Impl.cxx"

