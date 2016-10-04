#include <QtGui/QApplication>

#include "gmfc_v3.h"

int main(int argc, char *argv[])
{
	//application object
	QApplication a(argc, argv);
	
	//icon for application
	a.setWindowIcon(QIcon::fromTheme("application icon",
						QIcon(":/Resources/gmfc_v1.svg")));

	//GMFC object
	GMFC w;

	//configure object
	w.configure();
	
	//show object
	w.show();

	//execute application
	return a.exec();
}
