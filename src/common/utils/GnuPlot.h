////////////////////////////////////////////
//
// A C++ interface to gnuplot. 
//
// This is a direct translation from the C interface
// written by N. Devillard (which is available from
// http://ndevilla.free.fr/gnuplot/).
//
// As in the C interface this uses pipes and so wont
// run on a system that does'nt have POSIX pipe 
// support
//
// Rajarshi Guha
// <rajarshi@presidency.com>
//
// 07/03/03
//
// 26/01/04 - Gnuplot::cmd() was rewritten to accept a
// char* rather than a std::string, thus avoiding the
// va_start warning at compile time
// /////////////////////////////////////////

#ifndef GnuPlot_h
#define GnuPlot_h

#include <QVector>
#include <QString>
#include <QList>
#include <QDateTime>

class QTemporaryFile;
class QFile;

class GnuPlot
{
	public:
		typedef QVector<double> PlotVectorType;
		typedef QVector<QTime> TimeVectorType;
		
		GnuPlot();
		~GnuPlot();

		void execCmd(const QString &cmd);
		
		// plot setup
		void setStyle(const QString &style);
		void setLabelX(const QString &label);
		void setLabelY(const QString &label);
		void setLabelZ(const QString &label);
		void setMinMaxXYZ(double minX, double maxX, double minY=0, double maxY=0, double minZ=0, double maxZ=0);
		
		// Plots a linear equation (where you supply the
		// slope and intercept)
		void plotSlope(double a, double b, const QString &title);
		
		// Plot an equation which is supplied as a string
		void plotEquation(const QString &equation, const QString &title);
		
		// plot vectors
		void plotX(PlotVectorType &d, const QString &title);
		void plotXY(PlotVectorType &x, PlotVectorType &y, const QString &title);
		void plotXY(TimeVectorType &x, PlotVectorType &y, const QString &title);
		void plotXYZ(PlotVectorType &x, PlotVectorType &y, PlotVectorType &z, const QString &title);

		void clear();

		// select Output (x11, png, ps)
		void setOutput(const QString &name);
	
	private:
		typedef enum {Float, Time}AxisDataType;
		FILE *m_pGnuPipe;
		QString m_style;
		QList<QTemporaryFile*> m_filesToDel;
		int m_nplots;
    QString m_GnuplotBinary;
		
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
