#include "hexaviewer_v2.h"

HexaViewer::HexaViewer(QStandardItemModel* ptrModel, QString fName, QWidget* ptrParent)
	: Viewer(ptrModel, fName, ptrParent){}

HexaViewer::~HexaViewer(){}

void HexaViewer::configureClassSpecifics( void ){

	//mime type
	_cellMimetype = "application/hexa-dragged-byte";
	_cellMimetype += _v2_fName;

	//config essential
	_v2_cellWidth = 18;
	_v2_cellHeight = CELL_H;
	
	return;	
}