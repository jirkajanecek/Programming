#include "addressbar_v2.h"

AddressBar::AddressBar(QStandardItemModel* ptrModel, QWidget* ptrParent)
	: QTableView(ptrParent){
	
	_v2_ptrModel = ptrModel;
	
	_v2_rows = ptrModel->rowCount();
	_v2_cols = ptrModel->columnCount();
}

AddressBar::~AddressBar(){}

void AddressBar::artificialResize(QStandardItemModel* ptrModel){

	int oldRows = _v2_rows;
	int oldCellsNum = oldRows * _v2_ptrModel->columnCount();

	_v2_rows = ptrModel->rowCount();

	//this->setModel(ptrModel);
	_v2_ptrModel = ptrModel;

	//component is bigger
	if(oldRows < ptrModel->rowCount()){
		for(int row = oldRows; row < ptrModel->rowCount(); row++){
			this->setRowHeight(row, CELL_H);
		}
	}

	this->resize(_v2_ptrModel->columnCount() * _v2_cellWidth,
					_v2_ptrModel->rowCount() * _v2_cellHeight);
	
	return;
}

void AddressBar::configure( void ){
	
	this->configureClassSpecifics();

	this->configureView();

	this->createView();
	
	this->resize(_v2_ptrModel->columnCount() * _v2_cellWidth,
					_v2_ptrModel->rowCount() * _v2_cellHeight);
	
	return;
}

void AddressBar::configureClassSpecifics(void){
	
	_v2_cellWidth = 60;
	_v2_cellHeight = CELL_H;		
		
	return;
}

void AddressBar::configureView(void){

	//view styles
	this->setShowGrid(false);	
	this->setFrameStyle(QFrame::NoFrame);

	//single selection mode
	this->setSelectionMode(QAbstractItemView::NoSelection);

	//setup drag and drop mode
	this->setDragEnabled(false);
	this->setAcceptDrops(false);

	//hide headers
	this->horizontalHeader()->hide();
	this->verticalHeader()->hide();
	
	//scroll bar policy
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//font policy
	QFont font;
	font.setFamily(CODE_FONT);
	font.setPixelSize(10);
	font.setBold(false);
	this->setFont(font);		
	
	return;
}

void AddressBar::createView(void){
	
	this->setModel(_v2_ptrModel);

	//create view
	for(int row = 0; row < _v2_ptrModel->rowCount(); row++){

		this->setRowHeight(row, _v2_cellHeight);
		
		if(row == 0){

			for(int col = 0; col < _v2_ptrModel->columnCount(); col++){
				this->setColumnWidth(col, _v2_cellWidth);
			}
		}
	}

	return;
}

void AddressBar::updateView(QStandardItemModel* ptrModel){
	
	_v2_ptrModel = ptrModel;
	//this->setModel(_v2_ptrModel);

	return;
}