#include "hexaviewerselectable_v3.h"

HexaViewerSelectable::HexaViewerSelectable(
		QString* ptrLabelForm,
		QList<QString>* ptrDataForm,
		QString fName,
		QWidget* ptrParent)
	: ViewerSelectable(ptrLabelForm, ptrDataForm, fName, ptrParent){}

HexaViewerSelectable::~HexaViewerSelectable(){}

void HexaViewerSelectable::configureClassSpecifics( void ){

	//ViewerSelectable::configureClassSpecifics();
	this->setTextInteractionFlags(Qt::TextSelectableByMouse);

	_v2_cellWidth = CODE_FONT_W*HVIEWER_NUM_CELL;
	_v2_cellHeight = CELL_H;

	_v2_cols = INIT_COLS;
	_v2_rows = _v3_ptrDataForm->size()/_v2_cols;

	_appWidth = CODE_FONT_W*(_v2_cols*HVIEWER_NUM_CELL);
	_appHeight = CODE_FONT_H*_v2_rows;

	_ptrSelectableLayer->configure(
		_v2_rows,
		_v2_cols,
		_v2_cellWidth,
		_v2_cellHeight);

	return;
}
