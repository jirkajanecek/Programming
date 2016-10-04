#include "textviewer_v3.h"

TextViewer::TextViewer(
		QString* ptrLabelForm,
		QList<QString>* ptrDataForm,
		QString fName,
		QWidget* ptrParent)
	: Viewer(ptrLabelForm, ptrDataForm, fName, ptrParent){}

TextViewer::~TextViewer(){}

void TextViewer::configureClassSpecifics(void){
	
	//mime type
	_cellMimetype = "application/text-dragged-byte";
	_cellMimetype += _v3_fName;

	_v2_cellWidth = CODE_FONT_W*TVIEWER_NUM_CELL;
	_v2_cellHeight = CELL_H;

	_v2_cols = TVIEWER_COLS;
	_v2_rows = _v3_ptrDataForm->size()/_v2_cols;

	_appWidth = CODE_FONT_W * (_v2_cols * TVIEWER_NUM_CELL);
	_appHeight = CODE_FONT_H * _v2_rows;
		
	return;
}