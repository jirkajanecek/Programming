#include "error.h"

void Error::alert( QString title, QString desc ){

	//message box
	QMessageBox msgBox;

	//text set up
	msgBox.setWindowTitle( title );
	msgBox.setText( desc );
	
	//icon set up
	msgBox.setWindowIcon( QIcon::fromTheme("",
			QIcon(":/Global/Resources/error.svg")) );
	

	//message box execution
	msgBox.exec();
	
	return;
}