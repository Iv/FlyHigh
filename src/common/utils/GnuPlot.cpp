/***************************************************************************
 *   Copyright (C) 2005 by Alex Graf                                       *
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

#include <QTemporaryFile>
#include <QProcess>
#include <QStringList>
#include <QRegExp>
#include <QDir>
#include <QProcess>
#include "IFlyHighRC.h"
#include "Error.h"
#include "GnuPlot.h"

// command to start gnuplot
#define GNUPLOT_BIN_NAME "gnuplot"

GnuPlot::GnuPlot(void)
{
  m_nplots = 0;
  m_pLogFile = NULL;
  setStyle("lines");
}

GnuPlot::~GnuPlot()
{
  // loop through running gnuplot processes an end them
  for(ProcessList::Iterator it=m_Processes.begin();it!=m_Processes.end();++it) {
    if((*it)->state()!=QProcess::NotRunning) {
      // Terminating process politely
      (*it)->terminate();
      if(!(*it)->waitForFinished(3000)) {
        // Process did not terminate within 3 secs. Try killing
        (*it)->kill();
      }
    }
    delete *it;
  }
  m_Processes.clear();
  clear();
}

void GnuPlot::begin()
{
  clear();
  m_pLogFile = getOpenTmpFile();
}

void GnuPlot::end()
{
  // do not terminate gnuplot process until window is closed
  execCmd("pause mouse close");

  m_pLogFile->close();

  QProcess* proc = new QProcess();
  // unify stdout and stderr
  proc->setProcessChannelMode(QProcess::MergedChannels);
  // start gnuplot with command file as argument
  proc->start(GNUPLOT_BIN_NAME, QStringList() << m_pLogFile->fileName(), QIODevice::ReadOnly);
  m_Processes.append(proc);
  // we do not communicate with the process
  proc->closeReadChannel(QProcess::StandardOutput);
  proc->closeReadChannel(QProcess::StandardError);
  proc->closeWriteChannel();
}

void GnuPlot::clear()
{
  // delete temporary files
  for(TempFileList::Iterator it=m_filesToDel.begin(); it!=m_filesToDel.end(); it++)
  {
    delete *it;
  }
  m_filesToDel.clear();

  m_nplots = 0;
}

void GnuPlot::setMultiplot(int rows, int cols, const QString &title)
{
  QString cmdstr = QString("set multiplot layout %1, %2 title \"%3\"").arg(rows).arg(cols).arg(title);
  execCmd(cmdstr);
}

void GnuPlot::unsetMultiplot()
{
  execCmd("unset multiplot");
}

void GnuPlot::setStyle(const QString &style)
{
  if(style != "lines" &&
    style != "points" &&
    style != "linespoints" &&
    style != "impulses" &&
    style != "dots" &&
    style != "steps" &&
    style != "errorbars" &&
    style != "boxes" &&
    style != "boxerrorbars")
  {
    m_style = "points";
  }
  else
  {
    m_style = style;
  }
}

void GnuPlot::execCmd(const QString &cmd)
{
  QString locCmd;

  if(m_pLogFile != NULL)
  {
    locCmd = cmd;
    locCmd += "\n";
    m_pLogFile->write(locCmd.toLatin1(), locCmd.length());
  }
}

void GnuPlot::setLabelX(const QString &label)
{
  QString cmdstr = QString("set xlabel \"%1\"").arg(label);
  execCmd(cmdstr);
}

void GnuPlot::setLabelY(const QString &label)
{
  QString cmdstr = QString("set ylabel \"%1\"").arg(label);
  execCmd(cmdstr);
}

void GnuPlot::setLabelZ(const QString &label)
{
  QString cmdstr = QString("set zlabel \"%1\"").arg(label);
  execCmd(cmdstr);
}

void GnuPlot::showGrid(bool show)
{
  QString cmdstr;

  if(show)
    cmdstr = "set grid back lw 1 lc rgb \"#bbbbbb\"";
  else
    cmdstr = "unset grid";

  execCmd(cmdstr);
}

//
// Plots a linear equation (where you supply the
// slope and intercept)
//
void GnuPlot::plotSlope(double a, double b, const QString &title)
{
  QString cmdstr;

  if(m_nplots > 0)
  {
    cmdstr = QString("replot %1 * x + %2 title \"%3\" with %4")
                      .arg(a).arg(b).arg(title).arg(m_style);
  }
  else
  {
    cmdstr = QString("plot %1 * x + %2 title \"%3\" with %4")
                      .arg(a).arg(b).arg(title).arg(m_style);
  }

  setAxisData('x', Float);
  execCmd(cmdstr);
  m_nplots++;
}

//
// Plot an equation which is supplied as a string
//
void GnuPlot::plotEquation(const QString &equation, const QString &title)
{
  QString cmdstr;

  if(m_nplots > 0)
  {
    cmdstr = QString("replot %1 title \"%2\" with %3").arg(equation).arg(title).arg(m_style);
  }
  else
  {
    cmdstr = QString("plot %1 title \"%2\" with %3").arg(equation).arg(title).arg(m_style);
  }

  setAxisData('x', Float);
  execCmd(cmdstr);
  m_nplots++;
}

void GnuPlot::plotX(PlotVectorType &x, const QString &title)
{
  QString cmdstr;
  QString line;
  int valueNr;

  QFile* pFile = getOpenTmpFile();

  if(pFile)
  {
    for(valueNr=0; valueNr<x.size(); valueNr++)
    {
      line.sprintf("%f\n", x[valueNr]);
      pFile->write(line.toLatin1(), line.length());
    }

    pFile->close();

    if(m_nplots > 0)
    {
      cmdstr = QString("replot \"%1\" with %2").arg(pFile->fileName()).arg(m_style);
    }
    else
    {
      cmdstr = QString("plot \"%1\" title \"%2\" with %3").arg(pFile->fileName()).arg(title).arg(m_style);
    }

    setAxisData('x', Float);
    execCmd(cmdstr);
    m_nplots++;
  }
}

void GnuPlot::plotXY(PlotVectorType &x, PlotVectorType &y, const QString &title)
{
  QString cmdstr;
  QString line;
  int valueNr;
  QFile* pFile = getOpenTmpFile();

  if(pFile)
  {
    for(valueNr=0; valueNr<x.size(); valueNr++)
    {
      line.sprintf("%f %f\n", x[valueNr], y[valueNr]);
      pFile->write(line.toLatin1(), line.length());
    }

    pFile->close();

    if(m_nplots > 0)
    {
      cmdstr = QString("replot \"%1\" with %2").arg(pFile->fileName()).arg(m_style);
    }
    else
    {
      cmdstr = QString("plot \"%1\" title \"%2\" with %3").arg(pFile->fileName()).arg(title).arg(m_style);
    }

    setAxisData('x', Float);
    execCmd(cmdstr);
    m_nplots++;
  }
}

void GnuPlot::plotXY(TimeVectorType &x, PlotVectorType &y, const QString &title)
{
  QString cmdstr;
  QString line;
  int valueNr;
  QFile* pFile = getOpenTmpFile();

  if(pFile)
  {
    for(valueNr=0; valueNr<x.size(); valueNr++)
    {
      line = QString("%1 %2\n").arg(x[valueNr].toString(Qt::ISODate)).arg(y[valueNr]);
      pFile->write(line.toLatin1(), line.length());
    }
    pFile->close();

    if(m_nplots > 0)
    {
      cmdstr = QString("replot \"%1\" using 1:2 with %2").arg(pFile->fileName()).arg(m_style);
    }
    else
    {
      cmdstr = QString("plot \"%1\" using 1:2 title \"%2\" with %3").arg(pFile->fileName()).arg(title).arg(m_style);
    }

    setAxisData('x', Time);
    execCmd(cmdstr);
    m_nplots++;
  }
}

void GnuPlot::plotXYZ(PlotVectorType &x, PlotVectorType &y, PlotVectorType &z, const QString  &title)
{
  QString cmdstr;
  QString line;
  int valueNr;
  QFile* pFile = getOpenTmpFile();

  if(pFile)
  {
    for(valueNr=0; valueNr<x.size(); valueNr++)
    {
      line.sprintf("%f %f %f\n", x[valueNr], y[valueNr], z[valueNr]);
      pFile->write(line.toLatin1(), line.length());
    }

    pFile->close();

    if(m_nplots > 0)
    {
      cmdstr = QString("replot \"%1\" title \"%2\" with %3").arg(pFile->fileName()).arg(title).arg(m_style);
    }
    else
    {
      cmdstr = QString("splot \"%1\" title \"%2\" with %3").arg(pFile->fileName()).arg(title).arg(m_style);
    }

    setAxisData('x', Float);
    execCmd(cmdstr);
    m_nplots++;
  }
}

void GnuPlot::multiplotXY(TimeVectorType &x, PlotVectorType &y)
{
  QString cmdstr;
  QString line;
  int valueNr;
  QFile* pFile = getOpenTmpFile();

  if(pFile)
  {
    for(valueNr=0; valueNr<x.size(); valueNr++)
    {
      line = QString("%1 %2\n").arg(x[valueNr].toString(Qt::ISODate)).arg(y[valueNr]);
      pFile->write(line.toLatin1(), line.length());
    }

    pFile->close();
    setAxisData('x', Time);
    cmdstr = QString("plot \"%1\" using 1:2 title \"\" with %2")
                    .arg(pFile->fileName()).arg(m_style);
    execCmd(cmdstr);
  }
}

void GnuPlot::multiplotXY(TimeVectorType &x, PlotVectorType &y1, const QString &title1,
                          PlotVectorType &y2, const QString &title2)
{
  QString cmdstr;
  QString line;
  int valueNr;
  QFile* pFile = getOpenTmpFile();

  if(pFile)
  {
    for(valueNr=0; valueNr<x.size(); valueNr++)
    {
      line = QString("%1 %2 %3\n").arg(x[valueNr].toString(Qt::ISODate))
                                  .arg(y1[valueNr]).arg(y2[valueNr]);
      pFile->write(line.toLatin1(), line.length());
    }

    pFile->close();
    setAxisData('x', Time);
    cmdstr = QString("plot \"%1\" using 1:2 title \"%2\" with %3,"
                     "\"%4\" using 1:3 title \"%5\" with %6")
                    .arg(pFile->fileName()).arg(title1).arg(m_style)
                    .arg(pFile->fileName()).arg(title2).arg(m_style);
    execCmd(cmdstr);
  }
}

void GnuPlot::setMinMaxXYZ(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
  QString cmdstr;

  cmdstr.sprintf("set xrange [%f:%f]\n", minX, maxX);
  execCmd(cmdstr);

  cmdstr.sprintf("set yrange [%f:%f]\n", minY, maxY);
  execCmd(cmdstr);

  cmdstr.sprintf("set zrange [%f:%f]\n", minZ, maxZ);
  execCmd(cmdstr);
}

QFile* GnuPlot::getOpenTmpFile()
{
  QTemporaryFile* pTemp = NULL;

  // create temporary files for output
  pTemp = new QTemporaryFile();

  if(pTemp->open())
  {
    m_filesToDel.push_back(pTemp);
  }
  else
  {
    delete pTemp;
    pTemp = NULL;
  }

  return pTemp;
}

void GnuPlot::setAxisData(const char axis, AxisDataType axisData)
{
  QString cmd;

  switch(axisData)
  {
    case Time:
      cmd.sprintf("set %cdata time", axis);
      execCmd(cmd);
      execCmd("set timefmt \"%H:%M:%S\"");
      cmd.sprintf("set format %c ", axis);
      cmd += "\"%H:%M:%S\"";
      execCmd(cmd);
    break;
    case Float:
      cmd.sprintf("set %cdata", axis);
      execCmd(cmd);
      cmd.sprintf("set format %c ", axis);
      cmd += "\"%g\"";
      execCmd(cmd);
    break;
  }
}
