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

#include <unistd.h>
#include <QTemporaryFile>
#include <QProcess>
#include <QStringList>
#include <QRegExp>
#include <QDir>
#include "IFlyHighRC.h"
#include "Error.h"
#include "GnuPlot.h"

// qnuplot executable name depends on OS
// PATH separator char as well
#ifdef Q_OS_WIN
#define GNUPLOT_BIN_NAME "gnuplot.exe"
#define PATH_SEP_CHAR ";"
#else
#define GNUPLOT_BIN_NAME "gnuplot"
#define PATH_SEP_CHAR ":"
#endif

GnuPlot::GnuPlot(void)
{
  m_nplots = 0;
  m_pLogFile = NULL;
  setStyle("lines");
  findGnuplot();
}

GnuPlot::~GnuPlot()
{
	clear();
}

void GnuPlot::begin()
{
  clear();
  m_pLogFile = getOpenTmpFile();
}

void GnuPlot::end()
{
  QString cmd;

	m_pLogFile->close();
  cmd = m_GnuplotBinary;
  cmd += " -p ";
  cmd += m_pLogFile->fileName();
	system(cmd.toStdString().c_str());
}

void GnuPlot::clear()
{
  TempFileList::iterator it;

	if(m_filesToDel.size() > 0)
	{
		// delete temporary files
		for(it=m_filesToDel.begin(); it!=m_filesToDel.end(); it++)
		{
			delete *it;
		}

		m_filesToDel.clear();
	}

	m_nplots = 0;
}

void GnuPlot::setMultiplot(int rows, int cols, const QString &title)
{
	QString cmdstr = QString("set multiplot layout %1, %2 title \"%3\"")
                          .arg(rows).arg(cols).arg(title);
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
	QFile file;
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

void GnuPlot::setOutput(const QString &name)
{
	int pos;
	QString cmdstr;
	QString term;

	// terminal by extension
  pos = name.lastIndexOf(".");
	term = name.right(pos+1);

	if(term == "png")
	{
		execCmd("set terminal png");
    cmdstr = QString("set output \"%1\"").arg(name);
		execCmd(cmdstr);
	}
	else if(term == "ps")
	{
		execCmd("set terminal ps");
		cmdstr = QString("set output \"%1\"").arg(name);
		execCmd(cmdstr);
	}
	else
	{
		execCmd("set terminal \"x11\"");
	}
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

bool GnuPlot::findGnuplot()
{
  m_GnuplotBinary.clear();

  // fetch environment variables
  QStringList env = QProcess::systemEnvironment();

  // Starting from Qt 4.6, reading the PATH environment variable
  // may be shortened to
  // QString path = QProcessEnvironment::systemEnvironment().value("PATH");
  // Since a lot of distros still provide older Qt versions
  // we stick to this rather clumsy way

  // find PATH members
  QRegExp rx("PATH=*", Qt::CaseInsensitive, QRegExp::Wildcard);
  int idx = env.indexOf(rx);

  if(idx!=-1)
  {
    // got PATH
    QString path(env.at(idx));
    // strip the prefix "PATH="
    path.remove("PATH=");
    // tokenize
    QStringList parts = path.split(PATH_SEP_CHAR);
    // iterate through PATH elements
    QStringListIterator iter(parts);
    while(iter.hasNext())
    {
      QDir dir(iter.next());
      if(dir.exists(GNUPLOT_BIN_NAME))
      {
        m_GnuplotBinary = dir.filePath(GNUPLOT_BIN_NAME);
        // there may be spaces in the path: surround with "
        m_GnuplotBinary.prepend('"').append('"');
        break;
      }
    }
  }

  return !m_GnuplotBinary.isEmpty();
}
