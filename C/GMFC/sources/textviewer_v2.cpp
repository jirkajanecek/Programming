#include "textviewer_v2.h"

TextViewer::TextViewer(QStandardItemModel* ptrModel, QString fName, QWidget* ptrParent)
	: Viewer(ptrModel, fName, ptrParent){}

TextViewer::~TextViewer(){}

void TextViewer::configureClassSpecifics(void){
	
	//mime type
	_cellMimetype = "application/text-dragged-byte";
	_cellMimetype += _v2_fName;
		
	//config essential
	_v2_cellWidth = 11;
	_v2_cellHeight = CELL_H;
	
	return;
}