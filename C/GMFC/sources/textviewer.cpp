#include "textviewer.h"

TextViewer::TextViewer(QString filename, int rows, int cols, QWidget *parent, Qt::WFlags flags)
	: Viewer(filename, rows, cols, parent, flags)
{
	_substitutionalChar = ".";
}

TextViewer::~TextViewer(){
}

QString TextViewer::charToCellLabel(unsigned char byte){
	//translation char to hexa number
	QString character = "";
	bool transform = false;
	if( ((byte >= 0x00) && (byte < 0x20)) ||
		((byte >= 0x80) && (byte <= 0xA0))
	){
		transform = true;
	}	
	if(transform){
		character = _substitutionalChar;
	}
	else{
		character = byte;
	}
	return character;
}

void TextViewer::configureClassSpecifics( void ){
	
	//mime type
	_cellMimetype = "application/text-dragged-byte";
	_cellMimetype += _fName;
		
	if(!_configExists){
		//config essential
		_byteWidth = 9;
		_byteHeight = 11;
		_appWidth = _cellHorizontal * _byteWidth;
		_appHeight = _cellVertical * _byteHeight;
	}

	return;
}