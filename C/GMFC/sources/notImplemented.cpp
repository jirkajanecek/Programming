#include "notImplemented.h"

void NotImplemented::alert( void ){

	//message box
	QMessageBox msgBox;

	//text set up
	msgBox.setWindowTitle("Not ready");
	msgBox.setText("Functionality is not implemented yet");
	
	//icon set up
	msgBox.setWindowIcon( QIcon::fromTheme("",
			QIcon(":/Global/Resources/error.svg")) );

	msgBox.setIcon(QMessageBox::Warning);

	//message box execution
	msgBox.exec();
	
	return;
}