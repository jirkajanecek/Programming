#include "hexaviewer_v3.h"

HexaViewer::HexaViewer(
		QString* ptrLabelForm,
		QList<QString>* ptrDataForm,
		QString fName,
		QWidget* ptrParent)
	: Viewer(ptrLabelForm, ptrDataForm, fName, ptrParent){}

HexaViewer::~HexaViewer(){}

void HexaViewer::configureClassSpecifics( void ){

	//mime type
	_cellMimetype = "application/hexa-dragged-byte";
	_cellMimetype += _v3_fName;

	_v2_cellWidth = CODE_FONT_W*HVIEWER_NUM_CELL;
	_v2_cellHeight = CELL_H;

	_v2_cols = INIT_COLS;
	_v2_rows = _v3_ptrDataForm->size()/_v2_cols;

	_appWidth = CODE_FONT_W*(_v2_cols*HVIEWER_NUM_CELL);
	_appHeight = CODE_FONT_H*_v2_rows;
	
	return;	
}