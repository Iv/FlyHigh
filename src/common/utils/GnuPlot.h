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

#ifndef GnuPlot_h
#define GnuPlot_h

#include <QList>
#include <QString>
#include <QTime>
#include <QVector>

class QTemporaryFile;
class QFile;

class GnuPlot
{
	public:
		typedef QVector<double> PlotVectorType;
		typedef QVector<QTime> TimeVectorType;

		GnuPlot();

		~GnuPlot();

void begin();

void end();

		void execCmd(const QString &cmd);

		// plot setup
		void setMultiplot(int rows, int cols, const QString &title);

		void unsetMultiplot();

		void setStyle(const QString &style);

		void setLabelX(const QString &label);

		void setLabelY(const QString &label);

		void setLabelZ(const QString &label);

		void setMinMaxXYZ(double minX, double maxX, double minY=0, double maxY=0, double minZ=0, double maxZ=0);

		/**
      Plots a linear equation (where you supply the slope and intercept)
		*/
		void plotSlope(double a, double b, const QString &title);

		/**
      Plot an equation which is supplied as a string
    */
		void plotEquation(const QString &equation, const QString &title);

		// plot vectors
		void plotX(PlotVectorType &d, const QString &title);

		void plotXY(PlotVectorType &x, PlotVectorType &y, const QString &title);

		void plotXY(TimeVectorType &x, PlotVectorType &y, const QString &title);

		void plotXYZ(PlotVectorType &x, PlotVectorType &y, PlotVectorType &z, const QString &title);

    // multiple plots in one figure
		void multiplotXY(TimeVectorType &x, PlotVectorType &y);

		void multiplotXY(TimeVectorType &x, PlotVectorType &y1, const QString &title1,
                     PlotVectorType &y2, const QString &title2);

		void clear();

		/**
      selects output (x11, qt, png, ps)
    */
		void setOutput(const QString &name);

	private:
		typedef enum {Float, Time}AxisDataType;
		typedef QList<QTemporaryFile*> TempFileList;

		QString m_style;
		TempFileList m_filesToDel;
		int m_nplots;
    QString m_GnuplotBinary;
    QFile *m_pLogFile;

		QFile* getOpenTmpFile();

		void setAxisData(const char axis, AxisDataType axisData);

    /**
     * look for the gnuplot executable
     * the platform depending executable is searched in all
     * PATH environment variable directories
     * @return true if found
     */
    bool findGnuplot();
};

#endif
