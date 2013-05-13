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

#include <QFile>
#include <QString>
#include <QTextStream>
#include "Tokenizer.h"
#include "WptFileParser.h"

WptFileParser::WptFileParser()
{
}

bool WptFileParser::parse(const QString &fileName, WayPoint::WayPointListType &wayPointList)
{
  typedef enum Parser{ParserUndef, ParserCup, ParserWpt}Parser;
  QFile file(fileName);
  QTextStream inStream(&file);
  QString line;
  WayPoint wpt;
  Parser parser = ParserUndef;
  int id = 0;
  bool success;

  wayPointList.clear();
  success = file.open(QIODevice::ReadOnly | QIODevice::Text);

  if(success)
  {
    if(fileName.endsWith(".cup"))
    {
      parser = ParserCup;

      // skip first line
      line = inStream.readLine();
    }
    else if(fileName.endsWith(".wpt"))
    {
      parser = ParserWpt;

      // skip first line
      line = inStream.readLine();
    }

    line = inStream.readLine();

    while(!line.isNull())
    {
      line += "\n"; // removed by readline, for tokenizer needed

      switch(parser)
      {
        case ParserCup:
          success = parseLineCup(line, wpt);
        break;
        case ParserWpt:
          success = parseLineWpt(line, wpt);
        break;
        default:
        break;
      }

      if(success)
      {
        id++;
        wpt.setId(id);
        wpt.setType(WayPoint::TypeBuoy);
        wayPointList.push_back(wpt);
      }

      line = inStream.readLine();
    }
  }

  return (wayPointList.size() > 0);
}

bool WptFileParser::parseLineCup(const QString &line, WayPoint &wpt)
{
  Tokenizer tokenizer;
  QString token;
  double lat;
  double lon;
  double dValue;
  int iValue;
  bool success;

  // Title,Code,Country,Latitude,Longitude,Elevation,Style,Direction,Length,Frequency,Description
  // "B01188 RUERAS-MILEZ",B01188,,4640.029N,00843.138E,1880.0m,1,,,,
  success = tokenizer.getFirstToken(line, ',', token);

  // Title
  if(success && token.startsWith("\""))
  {
    token.remove('"');
    wpt.setName(token);
    success = tokenizer.getNextToken(line, ',', token);
  }

  // Code
  if(success)
  {
    success = tokenizer.getNextToken(line, ',', token); // skip
  }

  // Country
  if(success)
  {
    wpt.setCountry(token);
    success = tokenizer.getNextToken(line, ',', token);
  }

  // lat
  if(success)
  {
    dValue = token.left(token.size() - 1).toDouble() / 100.0;
    iValue = (int)dValue;
    lat = iValue + (dValue - iValue) * 100 / 60;

    if(token.endsWith("S"))
    {
      lat = -lat;
    }

    wpt.setLat(lat);
    success = tokenizer.getNextToken(line, ',', token);
  }

  // lon
  if(success)
  {
    dValue = token.left(token.size() - 1).toDouble() / 100.0;
    iValue = (int)dValue;
    lon = iValue + (dValue - iValue) * 100 / 60;

    if(token.endsWith("W"))
    {
      lon = -lon;
    }

    wpt.setLon(lon);
    success = tokenizer.getNextToken(line, ',', token);
  }

  // Elevation
  if(success)
  {
    if(token.endsWith("m"))
    {
      wpt.setAlt((int)token.left(token.size() - 1).toDouble());
    }

    success &= tokenizer.getNextToken(line, ',', token); // skip Style
    success &= tokenizer.getNextToken(line, ',', token); // skip Direction
    success &= tokenizer.getNextToken(line, ',', token); // skip Length
    success &= tokenizer.getNextToken(line, ',', token); // skip Frequency
    success &= tokenizer.getNextToken(line, '\n', token); // description
  }

  // description
  if(success)
  {
    wpt.setDescription(token);
  }

  return success;
}

bool WptFileParser::parseLineWpt(const QString &line, WayPoint &wpt)
{
  Tokenizer tokenizer;
  QString token;
  double lat;
  double lon;
  bool success;

  // W  AAR041 A 47.2455555556�N 7.7627777778�E 27-MAR-62 00:00:00 414.000000 Aarwangen Bruecke
  // B01188   32T   0478502   5168215   1880  B01188 RUERAS-MILEZ
  success = tokenizer.getFirstToken(line, "  ", token);

  if(success && token == "W")
  {
    success = tokenizer.getNextToken(line, ' ', token);
  }

  // name
  if(success)
  {
    wpt.setName(token);
    tokenizer.getNextToken(line, ' ', token); // skip A
    success = tokenizer.getNextToken(line, 0xba, token);
  }

  // lat
  if(success)
  {
    lat = token.toDouble();
    success = tokenizer.getNextToken(line, ' ', token);

    if(success)
    {
      if(token == "S")
      {
        lat = -lat;
      }

      wpt.setLat(lat);
      success = tokenizer.getNextToken(line, 0xba, token);
    }
  }

  // lon
  if(success)
  {
    lon = token.toDouble();
    success = tokenizer.getNextToken(line, ' ', token);

    if(success)
    {
      if(token == "W")
      {
        lon = -lon;
      }

      wpt.setLon(lon);
      tokenizer.getNextToken(line, ' ', token); // skip Date
      tokenizer.getNextToken(line, ' ', token); // skip Time
      success = tokenizer.getNextToken(line, ' ', token);
    }
  }

  // altitude
  if(success)
  {
    wpt.setAlt((int)token.toDouble());
  }

  // description
  if(success)
  {
    tokenizer.getNextToken(line, '\n', token);
    wpt.setDescription(token);
  }

  return success;
}
