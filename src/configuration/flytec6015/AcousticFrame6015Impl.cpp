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

AcousticFrame6015Impl::AcousticFrame6015Impl(QWidget* parent, const char* name, WFlags fl)
: AcousticFrame6015(parent,name,fl)
{
}

AcousticFrame6015Impl::~AcousticFrame6015Impl()
{
}

void AcousticFrame6015Impl::update(QByteArray &arr)
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
	comboBox_RisePitch->setCurrentItem((uiValue & MASK_RISE_PITCH) >> POS_RISE_PITCH);

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

/*
	u_char c8value;
	int8_t i8value;
	u_int16_t c16value;
		
	// Base Volume
	slider_Volume->setValue(arr[BASE_VOL_BEEP_POS]);
	
	// Vario Offset
	spinBox_Threshold->setValue(arr[ACOUSTIC_LIFT_THR_POS]);

	// Up Base Frequency
	c16value = arr[UP_BASE_FRQ_POS] << 8;
	c16value += arr[UP_BASE_FRQ_POS+1] ;
	spinBox_UpFreq->setValue(c16value);

	// Frequency Modulation
	slider_FreqMod->setValue(arr[FRQ_MODULATION_POS]);

	// Acoustic Pitch
	slider_Pitch->setValue(arr[ACOUSTIC_PITCH_POS]);

	// Acoustic Integral
	slider_Integration->setValue(arr[ACOUSTIC_I_POS]);
	
	// Sink Acoustic Enabled
	c8value = arr[SINC_ACOUSTIC_EN_POS];
	checkBox_SinkAcoustic->setChecked(c8value);
	spinBox_Sink->setEnabled(c8value > 0);
	spinBox_DownFreq->setEnabled(c8value > 0);

	// Down Base Frequency
	c16value =  arr[DOWN_BASE_FRQ_POS] << 8;
	c16value += arr[DOWN_BASE_FRQ_POS+1];
	spinBox_DownFreq->setValue(c16value);

	// Sink
	i8value = arr[ACOUSTIC_SINK_POS];
	spinBox_Sink->setValue(i8value);
*/
}

void AcousticFrame6015Impl::store(QByteArray &arr)
{
/*
	u_int16_t c16value;
		
	// Base Volume
	arr[BASE_VOL_BEEP_POS] = (u_char)slider_Volume->value();
	
	// Vario Offset
	arr[ACOUSTIC_LIFT_THR_POS] = (u_char)spinBox_Threshold->value();

	// Up Base Frequency
	c16value = spinBox_UpFreq->value();
	arr[UP_BASE_FRQ_POS] = (u_char)(c16value >> 8);
	arr[UP_BASE_FRQ_POS+1] = (u_char)(c16value & 0xFF);

	// Frequency Modulation
	arr[FRQ_MODULATION_POS] = (u_char)slider_FreqMod->value();

	// Acoustic Pitch
	arr[ACOUSTIC_PITCH_POS] = (u_char)slider_Pitch->value();

	// Acoustic Integral
	arr[ACOUSTIC_I_POS] = (u_char)slider_Integration->value();
	
	// Sink Acoustic Enabled
	arr[SINC_ACOUSTIC_EN_POS] = (u_char)checkBox_SinkAcoustic->isChecked();

	// Down Base Frequency
	c16value = spinBox_DownFreq->value();
	arr[DOWN_BASE_FRQ_POS] = (u_char)(c16value >> 8);
	arr[DOWN_BASE_FRQ_POS+1] = (u_char)(c16value & 0xFF);
	
	// Sink
	arr[ACOUSTIC_SINK_POS] = (int8_t)spinBox_Sink->value();
*/
}


#include "AcousticFrame6015Impl.moc"

