#ifndef Device6015_h
#define Device6015_h

#include <qobject.h>
#include <qstring.h>
#include <qtimer.h>

class Device6015: public QObject
{
	public:
		Device6015();

		bool openDevice(const QString &dev, int baud);

		bool isOpen();

		void closeDevice();

		bool recieveTlg(int tout);

		const QString& getTlg();

		bool sendTlg(const QString &tlg);

	private:
		enum {MaxTlgSize = 255};

		QString m_tlg;
		int m_tout;
		int m_ttyFd;
		bool m_elapsed;

		bool getChar(char &ch);

		bool writeBuffer(const char *pBuff, int len);

		bool startTimer(int tout);

		bool isElapsed();
};
#endif
