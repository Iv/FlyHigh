#include <qapplication.h>
#include "MainWindow.h"
#include "IGPSDevice.h"

int main( int argc, char ** argv ) 
{
	QApplication appl(argc, argv);
	MainWindow* pMainWin = new MainWindow();
	int res;
	
	// only temporary here
	IGPSDevice::pInstance()->setDevice(IGPSDevice::Flytec5020);
	
	appl.setMainWidget(pMainWin);
	pMainWin->show();
	appl.connect(&appl, SIGNAL(lastWindowClosed()), &appl, SLOT(quit()));
	
	res = appl.exec();
	
	return res;
}
