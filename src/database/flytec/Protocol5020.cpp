/***************************************************************************
 *   Copyright (C) 2011 by Alex Graf                                       *
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

#include <QVariant>
#include <math.h>
#include "AirSpace.h"
#include "AirSpaceItem.h"
#include "Flight.h"
#include "Protocol5020.h"
#include "Route.h"
#include "Tokenizer.h"
#include "WayPoint.h"

#include <QtDebug>

Protocol5020::Protocol5020()
{
  m_memdump.resize(Flytec5020MemSize);
}

Protocol5020::~Protocol5020()
{
	close();
}

bool Protocol5020::open(const QString &dev, int baud)
{
	bool success;

	success = m_device.openDevice(dev, baud);

	return success;
}

void Protocol5020::close()
{
	m_device.closeDevice();
}

bool Protocol5020::devInfoReq()
{
	QString tlg;

	tlg = "$PBRSNP,";
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::devInfoRec(DeviceInfo &devInfo)
{
  Tokenizer tokenizer;
	QString token;
	QString tlg;
	bool valid = false;

  if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();
		tokenizer.getFirstToken(tlg, ',', token);
		valid = (token == "$PBRSNP");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
		  // identifier
			tokenizer.getNextToken(tlg, ',', token);
			devInfo.deviceIdent = token;

		  // pilot
			tokenizer.getNextToken(tlg, ',', token);
			devInfo.pilotName = ftString2qString(token);

		  // serial
			tokenizer.getNextToken(tlg, ',', token);
			devInfo.serialNr = token.toUInt();

		  // SW version
      tokenizer.getNextToken(tlg, '*', token);
			devInfo.swVersion = token;
		}
	}

  return valid;
}

bool Protocol5020::trackListReq()
{
	QString tlg;

	tlg = "$PBRTL,";
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::trackListRec(int &total, Flight &flight)
{
	Tokenizer tokenizer;
	QString token;
	QString tlg;
	bool valid = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();

		tokenizer.getFirstToken(tlg, ',', token);
		valid = (token == "$PBRTL");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
			// total
			tokenizer.getNextToken(tlg, ',', token);
			total = token.toUInt();

			// nr
			tokenizer.getNextToken(tlg, ',', token);
			flight.setNumber(token.toUInt());

			// date
			tokenizer.getNextToken(tlg, ',', token);
			flight.setDate(parseDate(token));

			// time
			tokenizer.getNextToken(tlg, ',', token);
			flight.setTime(parseTime(token));

			// duration
			tokenizer.getNextToken(tlg, '*', token);
			flight.setDuration(parseTime(token));
		}
	}

	return valid;
}

bool Protocol5020::trackReq(int trackNr)
{
	QString tlg;

	tlg = "$PBRTR,";
	tlg += QString::number(trackNr).rightJustified(2, '0');
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::trackRec(QString &line)
{
	bool success;

	success = m_device.recieveTlg(500, false);

	if(success)
	{
		line = m_device.getTlg();
	}

	return success;
}

bool Protocol5020::wpListReq()
{
	QString tlg;

	tlg = "$PBRWPS,";
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::wpListRec(WayPoint &wp)
{
	Tokenizer tokenizer;
	QString token;
	QString degToken;
	QString dirToken;
	QString tlg;
	bool valid = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();

		tokenizer.getFirstToken(tlg, ',', token);
		valid = (token == "$PBRWPS");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
			// latitude
			tokenizer.getNextToken(tlg, ',', degToken);
			tokenizer.getNextToken(tlg, ',', dirToken);
			wp.setLatitude(parseDeg(degToken, dirToken));

			// longitude
			tokenizer.getNextToken(tlg, ',', degToken);
			tokenizer.getNextToken(tlg, ',', dirToken);
			wp.setLongitude(parseDeg(degToken, dirToken));

			// short name
			tokenizer.getNextToken(tlg, ',', token); // skip

			// name
			tokenizer.getNextToken(tlg, ',', token);
			wp.setName(ftString2qString(token));

			// altitude
			tokenizer.getNextToken(tlg, '*', token);
			wp.setAltitude(token.toInt());
		}
	}

	return valid;
}

bool Protocol5020::wpSnd(const WayPoint &wp)
{
	QString tlg;
	bool success;

	tlg = "$PBRWPR,";

	// latitude
	tlg += latToString(wp.latitude(), 8);
	tlg += ",";

	// longitude
	tlg += lonToString(wp.longitude(), 9);
	tlg += ",";

	// skip short name
	tlg += ",";

	// name
	tlg += qString2ftString(wp.name(), 17);
	tlg += ",";

	// altitude
	tlg += QString::number(wp.altitude()).rightJustified(4, '0');

	addTail(tlg);
	success = m_device.sendTlg(tlg);
	usleep(100*1000);

qDebug() << tlg;

	return success;
}

bool Protocol5020::wpDel(const QString &name)
{
	QString tlg;
	bool success;

	tlg = "$PBRWPX,";
	tlg += qString2ftString(name, 17);
	addTail(tlg);

qDebug() << tlg;

	success = m_device.sendTlg(tlg);
	usleep(2000*1000);

	return success;
}

bool Protocol5020::wpDelAll()
{
	QString tlg;

	tlg = "$PBRWPX,,";
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::routeListReq()
{
	QString tlg;

	tlg = "$PBRRTS,";
	addTail(tlg);

	return m_device.sendTlg(tlg);
}

bool Protocol5020::routeListRec(uint &curSent, uint &totalSent, Route &route)
{
	Tokenizer tokenizer;
	QString token;
	QString tlg;
	int id;
	bool valid = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();
		tokenizer.getFirstToken(tlg, ',', token);
		valid = (token == "$PBRRTS");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
			// route number
			tokenizer.getNextToken(tlg, ',', token);
			id = token.toUInt();

			// total sentences
			tokenizer.getNextToken(tlg, ',', token);
			totalSent = token.toUInt();

			// cur sentence
			tokenizer.getNextToken(tlg, ',', token);
			curSent = token.toUInt();

			// name or waypoint
			if(curSent == 0)
			{
				// internal number as id
				route.setId(id);

				// name of route
				tokenizer.getNextToken(tlg, '*', token);
				route.setName(token);
			}
			else
			{
				WayPoint wp;

				// skip  compatible name
				tokenizer.getNextToken(tlg, ',', token);

				// name of waypoint
				tokenizer.getNextToken(tlg, '*', token);
				wp.setName(token);
				route.wayPointList().push_back(wp);
			}
		}
	}

	return valid;
}

bool Protocol5020::routeSnd(uint curSent, uint totalSent, Route &route)
{
	QString tlg;
	bool success;

	tlg = "$PBRRTR,";

	// route number
	tlg += QString::number(1).rightJustified(2, '0');
	tlg += ",";

	// total sentences
	tlg += QString::number(totalSent).rightJustified(2, '0');
	tlg += ",";

	// cur sentence
	tlg += QString::number(curSent).rightJustified(2, '0');
	tlg += ",";

	if(curSent == 0)
	{
		// name of route
		tlg += qString2ftString(route.name(), 17);
	}
	else
	{
		// skip compatible name
		tlg += ",";

		// name of waypoint
		tlg += qString2ftString(route.wayPointList().at(curSent - 1).name(), 17);
	}

	addTail(tlg);
	success = m_device.sendTlg(tlg);
	usleep(200*1000);

	return success;
}

bool Protocol5020::routeDel(const QString &name)
{
	QString tlg;
	bool success;

	// don't ask me why this won't work
	tlg = "$PBRRTX,";
	tlg += qString2ftString(name, 17);
	addTail(tlg);
	success = m_device.sendTlg(tlg);

	return success;
}

bool Protocol5020::ctrInfoReq()
{
	QString tlg;
	bool success;

	tlg = "$PBRCTRI";
	addTail(tlg);
	success = m_device.sendTlg(tlg);

	return success;
}

bool Protocol5020::ctrInfoRec(uint &nofCtr, uint &maxCtr, uint &nofFree)
{
	Tokenizer tokenizer;
	QString token;
	QString tlg;
	bool valid = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();
		tokenizer.getFirstToken(tlg, ',', token);
		valid = (token == "$PBRCTRI");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
		  // no of actual CTRs
      tokenizer.getNextToken(tlg, ',', token);
			nofCtr = token.toUInt();

      // no of max CTRs
      tokenizer.getNextToken(tlg, ',', token);
			maxCtr = token.toUInt();

      // no of free elements
      tokenizer.getNextToken(tlg, '*', token);
			nofFree = token.toUInt();
		}
	}

	return valid;
}

bool Protocol5020::ctrListReq()
{
	QString tlg;
	bool success;

	tlg = "$PBRCTR,";
	addTail(tlg);
	success = m_device.sendTlg(tlg);
	usleep(1000*1000);

	return success;
}

bool Protocol5020::ctrListRec(uint &curSent, uint &totalSent, AirSpace *pAirSpace)
{
	Tokenizer tokenizer;
	QString token;
	QString degToken;
	QString dirToken;
	QString tlg;
	AirSpaceItem *pItem = NULL;
	AirSpaceItemCircle *pCircle;
	AirSpaceItemSeg *pSegment;
	double lat;
	double lon;
	bool valid = false;

	if(m_device.recieveTlg(3000))
	{
		tlg = m_device.getTlg();
		tokenizer.getFirstToken(tlg, ',', token);
		valid = (token == "$PBRCTR");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
			// total sentences
			tokenizer.getNextToken(tlg, ',', token);
			totalSent = token.toUInt();

			// cur sentence
			tokenizer.getNextToken(tlg, ',', token);
			curSent = token.toUInt();

			if(curSent == 0)
			{
				// name
				tokenizer.getNextToken(tlg, ',', token);
				pAirSpace->setName(token);

				// warning distance
				tokenizer.getNextToken(tlg, '*', token);
				pAirSpace->setWarnDist(token.toUInt());
			}
			else if(curSent == 1)
			{
				// remark
				tokenizer.getNextToken(tlg, '*', token);
				pAirSpace->setRemark(token);
			}
			else
			{
				// type
				tokenizer.getNextToken(tlg, ',', token);

				if(token == "C")
				{
					pCircle = new AirSpaceItemCircle();

					// lat
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lat = parseDeg(degToken, dirToken);

					// lon
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lon = parseDeg(degToken, dirToken);

					// radius
					tokenizer.getNextToken(tlg, '*', token);
					pCircle->setRadius(token.toUInt());
					pItem = pCircle;
				}
				else if(token == "P")
				{
					pItem = new AirSpaceItemPoint(AirSpaceItem::Point);

					// lat
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lat = parseDeg(degToken, dirToken);

					// lon
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, '*', dirToken);
					lon = parseDeg(degToken, dirToken);
				}
				else if(token == "X")
				{
					pItem = new AirSpaceItemPoint(AirSpaceItem::Center);

					// lat
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lat = parseDeg(degToken, dirToken);

					// lon
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, '*', dirToken);
					lon = parseDeg(degToken, dirToken);
				}
				else if(token == "T")
				{
					pSegment = new AirSpaceItemSeg(AirSpaceItem::StartSegment);

					// lat
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lat = parseDeg(degToken, dirToken);

					// lon
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lon = parseDeg(degToken, dirToken);

					// dir
					tokenizer.getNextToken(tlg, '*', token);
					pSegment->setDir(token == "+");
					pItem = pSegment;
				}
				else if(token == "Z")
				{
					pSegment = new AirSpaceItemSeg(AirSpaceItem::StopSegment);

					// lat
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lat = parseDeg(degToken, dirToken);

					// lon
					tokenizer.getNextToken(tlg, ',', degToken);
					tokenizer.getNextToken(tlg, ',', dirToken);
					lon = parseDeg(degToken, dirToken);

					// dir
					tokenizer.getNextToken(tlg, '*', token);
					pSegment->setDir(token == "+");
					pItem = pSegment;
				}

				if(pItem != NULL)
				{
					pItem->setPoint(lat, lon);
					pAirSpace->airSpaceItemList().push_back(pItem);
				}
			}
		}
	}

	return valid;
}

bool Protocol5020::ctrSnd(uint curSent, uint totalSent, AirSpace &airspace)
{
	QString tlg;
	AirSpaceItem *pItem;
	AirSpaceItemSeg *pSegment;
	AirSpaceItemCircle *pCircle;
	bool success;

	tlg = "$PBRCTRW,";

	// total sentences
	tlg += QString::number(totalSent).rightJustified(3, '0');
	tlg += ",";

	// cur sentence
	tlg += QString::number(curSent).rightJustified(3, '0');
	tlg += ",";

	if(curSent == 0)
	{
		// name of airspace
		tlg += qString2ftString(airspace.name(), 17);
		tlg += ",";

		// warn distance
		tlg += QString::number(airspace.warnDist()).rightJustified(4, '0');
	}
	else if(curSent == 1)
	{
		// remark
		tlg += qString2ftString(airspace.remark(), 17);
	}
	else
	{
		pItem = airspace.airSpaceItemList().at(curSent - 2);

		if(pItem != NULL)
		{
			switch(pItem->type())
			{
				case AirSpaceItem::Point:
				case AirSpaceItem::Center:
					// type
					if(pItem->type() == AirSpaceItem::Point)
					{
						tlg += "P,";
					}
					else
					{
						tlg += "X,";
					}

					// latitude
					tlg += latToString(pItem->lat(), 8);
					tlg += ",";

					// longitude
					tlg += lonToString(pItem->lon(), 9);
				break;
				case AirSpaceItem::StartSegment:
				case AirSpaceItem::StopSegment:
					// type
					if(pItem->type() == AirSpaceItem::StartSegment)
					{
						tlg += "T,";
					}
					else
					{
						tlg += "Z,";
					}

					// latitude
					tlg += latToString(pItem->lat(), 8);
					tlg += ",";

					// longitude
					tlg += lonToString(pItem->lon(), 9);
					tlg += ",";

					// direction
					pSegment = (AirSpaceItemSeg*)pItem;

					if(pSegment->dir())
					{
						tlg += "+";
					}
					else
					{
						tlg += "-";
					}
				break;
				case AirSpaceItem::Circle:
					// type
					tlg += "C,";

					// latitude
					tlg += latToString(pItem->lat(), 8);
					tlg += ",";

					// longitude
					tlg += lonToString(pItem->lon(), 9);

					// radius
					pCircle = (AirSpaceItemCircle*)pItem;
					tlg += QString::number(pCircle->radius()).rightJustified(5, '0');
				break;
			}
		}
	}

	addTail(tlg);
	success = m_device.sendTlg(tlg);
	usleep(100*1000);

	return success;
}

bool Protocol5020::ctrDel(const QString &name)
{
	QString tlg;
	bool success;

	tlg = "$PBRCTRD,";
	tlg += qString2ftString(name, 17);
	addTail(tlg);
	success = m_device.sendTlg(tlg);

qDebug() << tlg;

	usleep(2000*1000);

	return success;
}

bool Protocol5020::recAck()
{
	Tokenizer tokenizer;
	QString token;
	QString tlg;
	int status;
	bool valid = false;

	if(m_device.recieveTlg(500))
	{
		tlg = m_device.getTlg();
    tokenizer.getFirstToken(tlg, ',', token);
		valid = (token == "$PBRANS");
		valid &= validateCheckSum(tlg);

		if(valid)
		{
			// status
			tokenizer.getFirstToken(tlg, '*', token);
			status = token.toUInt();
			valid = (status == 1);
		}
	}

	return valid;
}

bool Protocol5020::memoryRead(uint addr)
{
  Tokenizer tokenizer;
  QString tlg;
  QString token;
  uchar *pPage;
	uint byteNr;
	bool valid;
	bool success;
	bool ok;

	// valid address?
	if((addr + Flytec5020PageSize) > Flytec5020MemSize)
	{
		return false;
	}

	pPage = (u_char*)(m_memdump.data() + addr);
	tlg = "$PBRMEMR,";

  // address
	tlg += QString::number(addr, 16).rightJustified(4, '0').toUpper();
	addTail(tlg);
	success = m_device.sendTlg(tlg);

  if(success)
  {
    if(m_device.recieveTlg(3000))
    {
      tlg = m_device.getTlg();
      tokenizer.getFirstToken(tlg, ',', token);
      valid = (token == "$PBRMEMR");
      valid &= validateCheckSum(tlg);

      // address
      tokenizer.getNextToken(tlg, ',', token);
      valid &= (token.toUInt(&ok, 16) == addr);

      if(valid)
      {
        for(byteNr=0; byteNr<(Flytec5020PageSize - 1); byteNr++)
        {
          tokenizer.getNextToken(tlg, ',', token);
          pPage[byteNr] = token.toInt(&ok, 16);
        }

        tokenizer.getNextToken(tlg, '*', token);
        pPage[byteNr] = token.toInt(&ok, 16);
      }
    }
  }

  usleep(100 * 1000);

	return success;
}

bool Protocol5020::memoryWrite(uint addr)
{
  QString tlg;
  uchar *pPage;
  uint byteNr;
  bool success;

  // valid address?
	if((addr + Flytec5020PageSize) > Flytec5020MemSize)
	{
		return false;
	}

  pPage = (u_char*)(m_memdump.data() + addr);
  tlg = "$PBRMEMW,";

  // address
	tlg += QString::number(addr, 16).rightJustified(4, '0').toUpper();
	tlg += ",";

	// nof Bytes to be written
  tlg += QString::number(Flytec5020PageSize);

  // data
  for(byteNr=0; byteNr<Flytec5020PageSize; byteNr++)
	{
    tlg += ",";
	  tlg += QString::number(pPage[byteNr], 16).rightJustified(2, '0').toUpper();
	}

	addTail(tlg);
	success = m_device.sendTlg(tlg);
  usleep(100 * 2000);

  return success;
}

bool Protocol5020::updateConfiguration()
{
  QString tlg;
	bool success;

	tlg = "$PBRCONF,";
	addTail(tlg);
	success = m_device.sendTlg(tlg);

	return success;
}

bool Protocol5020::parWrite(int par, FtDataType dataType, const QVariant &value)
{
  u_int16_t ui16value;
  int16_t i16value;
  char* pString;
  bool success = true;

  switch(dataType)
	{
		case FtInt8:
      m_memdump[par] = (int8_t)value.toInt();
		break;
    case FtUInt8:
      m_memdump[par] = (u_int8_t)value.toUInt();
		break;
    case FtInt16:
      i16value = value.toInt();
      m_memdump[par] = (char)(i16value >> 8);
      m_memdump[par + 1] = (char)(i16value & 0xFF);
    break;
    case FtUInt16:
      ui16value = value.toUInt();
      m_memdump[par] = (u_char)(ui16value >> 8);
      m_memdump[par + 1] = (u_char)(ui16value & 0xFF);
    break;
/*
    case FtUInt32: case FtInt32:
		break;
*/
		case FtString:
      pString = m_memdump.data();
      qString2ftString(value.toString(), &pString[par], (int)FT_STRING_SIZE);
    break;
		case FtArray:
      m_memdump.replace(par, value.toByteArray().size(), value.toByteArray());
		break;
		default:
      success = false;
		break;
	}

	return success;
}

QVariant Protocol5020::parRead(int par, FtDataType dataType)
{
  QVariant value;
  u_int16_t ui16Value;
  int16_t i16Value;
  const char* pString;

  switch(dataType)
	{
		case FtInt8:
      value = (int8_t)m_memdump[par];
		break;
    case FtUInt8:
      value = (u_int8_t)m_memdump[par];
		break;
    case FtUInt16:
      ui16Value = m_memdump[par] << 8;
      ui16Value += m_memdump[par + 1];
      value = ui16Value;
    break;
    case FtInt16:
      i16Value = m_memdump[par] << 8;
      i16Value += m_memdump[par + 1];
      value = i16Value;
    break;
/*
    case FtUInt32: case FtInt32:
		break;
*/
		case FtString:
      pString = m_memdump.constData();
      value = ftString2qString(&pString[par], FT_STRING_SIZE);
    break;
		case FtArray:
      value = m_memdump.mid(par, getParLen(par));
		break;
	}

	return value;
}

QDate Protocol5020::parseDate(const QString &token) const
{
	Tokenizer tokenizer;
	QString timeToken;
	int year;
	int month;
	int day;

	tokenizer.getFirstToken(token, '.', timeToken);
	day = timeToken.toInt();
	tokenizer.getNextToken(token, '.', timeToken);
	month = timeToken.toInt();
	tokenizer.getNextToken(token, '\0', timeToken);
	year = timeToken.toInt() + 2000;

	return QDate(year, month, day);
}

QTime Protocol5020::parseTime(const QString &token) const
{
	Tokenizer tokenizer;
	QString timeToken;
	int hour;
	int min;
	int sec;

	tokenizer.getFirstToken(token, ':', timeToken);
	hour = timeToken.toInt();
	tokenizer.getNextToken(token, ':', timeToken);
	min = timeToken.toInt();
	tokenizer.getNextToken(token, '\0', timeToken);
	sec = timeToken.toInt();

	return QTime(hour, min, sec);
}

double Protocol5020::parseDeg(const QString &degToken, const QString &dirToken)
{
	Tokenizer subTokenizer;
	QString subToken;
	QChar dir;
	int intValue;
	double dValue;
	double deg;

	dValue = degToken.toDouble() / 100.0;
	intValue = floor(dValue);
	deg = intValue + (dValue - intValue) * 100 / 60;

	if((dirToken == "W") || (dirToken == "S"))
	{
		deg *= (-1); // negate values below equator and west
	}

	return deg;
}

QString Protocol5020::latToString(double deg, int size) const
{
	QString tlg;

	tlg = degToString(deg, size);

	if(deg < 0)
	{
		tlg += ",S";
	}
	else
	{
		tlg += ",N";
	}

	return tlg;
}

QString Protocol5020::lonToString(double deg, int size) const
{
	QString tlg;

	tlg = degToString(deg, size);

	if(deg < 0)
	{
		tlg += ",W";
	}
	else
	{
		tlg += ",E";
	}

	return tlg;
}

QString Protocol5020::degToString(double deg, int size) const
{
	int intValue;
	double value;

	intValue = floor(deg);
	value = intValue * 100 + (deg - intValue) * 60;

	return QString::number(value, 'f', 3).rightJustified(size, '0');
}

QString Protocol5020::qString2ftString(const QString &qString, int length)
{
	QString pad;
	QString ftString;

	if(qString.length() < length)
	{
		pad.fill(' ', length - qString.length());
		ftString = qString;
		ftString += pad;
	}
	else
	{
		ftString = qString;
		ftString.truncate(length);
	}

	return ftString;
}

QString Protocol5020::ftString2qString(const QString &ftString)
{
	int cpyLength;

	for(cpyLength=ftString.length(); cpyLength>0; cpyLength--)
	{
		if(ftString[cpyLength - 1] != ' ')
		{
			break;
		}
	}

	return ftString.left(cpyLength);
}

void Protocol5020::qString2ftString(const QString &qString, char *pftstr, int length)
{
  QString ftString;
  QString pad;

	if(qString.length() < length)
	{
		pad.fill(' ', length - qString.length());
		ftString = qString;
		ftString += pad;
	}
	else
	{
		ftString = qString;
		ftString.truncate(length);
	}

  memcpy(pftstr, ftString.toAscii().data(), ftString.length());
}

QString Protocol5020::ftString2qString(const char *pftstr, int length)
{
  char pstr[length];
	int strLen;

	for(strLen=length; strLen>0; strLen--)
	{
		if(pftstr[strLen - 1] != ' ')
		{
			break;
		}
	}

	memcpy(&pstr[0], &pftstr[0], strLen);
	pstr[strLen] = '\0';

  return pstr;
}

void Protocol5020::addTail(QString &tlg) const
{
	QString checkSum;

	tlg += "*";
	tlg += getCheckSum(tlg, tlg.size() - 1);
	tlg += "\r\n";
}

QString Protocol5020::getCheckSum(const QString &tlg, uint end) const
{
	uint charNr;
	char checkSum = 0;

	for(charNr=1; charNr<end; charNr++)
	{
		checkSum ^= tlg.at(charNr).toAscii();
	}

	return QString::number(checkSum, 16).rightJustified(2, '0').toUpper();
}

bool Protocol5020::validateCheckSum(const QString &tlg) const
{
	Tokenizer tokenizer;
	QString token;
	QString checkSum;

	tokenizer.getFirstToken(tlg, '*', token);
	tokenizer.getNextToken(tlg, '\r', token);
	checkSum = getCheckSum(tlg, tlg.size() - 5);

	return (checkSum == token);
}

int Protocol5020::getParLen(int par)
{
  int len;

  switch(par)
  {
    case USERFIELD_0_POS:
    case USERFIELD_1_POS:
    case USERFIELD_2_POS:
      len = 3;
    break;
    default:
      len = 0;
    break;
  }

  return len;
}
