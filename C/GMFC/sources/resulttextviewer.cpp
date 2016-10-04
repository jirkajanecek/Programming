#include "resulttextviewer.h"

ResultTextViewer::ResultTextViewer(int rows, int cols, QWidget *parent, Qt::WFlags flags)
	: ResultViewer(rows, cols, parent, flags){

	_substChar = ".";
}

ResultTextViewer::~ResultTextViewer(){}

QString ResultTextViewer::charToCellLabel( unsigned char byte ){

	QString character = "";
	bool transform = false;

	if( ((byte >= 0x00) && (byte < 0x20)) ||
		((byte >= 0x80) && (byte <= 0xA0))
	){
		transform = true;
	}	
	
	if(transform){
		character = _substChar;
	}
	else{
		character = byte;
	}
	return character;
}

void ResultTextViewer::configureClassSpecifics( void ){

	//config non-essential
	if(!_configExists)
	{
		//config essential
		_byteWidth = 9;
		_byteHeight = 11;
		_appWidth = _colNb * _byteWidth;
		_appHeight = _rowNb * _byteHeight;
	}

	this->confTWidget();
	
	return;
}

