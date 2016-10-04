#include "addressviewer_v3.h"

AddressViewer::AddressViewer(
		QString* ptrLabelForm,
		QList<QString>* ptrDataForm,
		QString fName,
		QWidget* ptrParent)
	: Viewer(ptrLabelForm, ptrDataForm, fName, ptrParent){}

AddressViewer::~AddressViewer(){}

void AddressViewer::configureClassSpecifics(void){
	
	this->setAcceptDrops(false);
	
	_v2_cellWidth = CODE_FONT_W*AVIEWER_NUM_CELL;
	_v2_cellHeight = CELL_H;

	_v2_cols = AVIEWER_COLS;
	_v2_rows = _v3_ptrDataForm->size()/_v2_cols;

	_appWidth = CODE_FONT_W * (_v2_cols * AVIEWER_NUM_CELL);
	_appHeight = CODE_FONT_H * _v2_rows;
		
	return;
}