#include "selectablelayer.h"

SelectableLayer::SelectableLayer(QWidget* ptrParent)
	: QTableWidget(ptrParent){}

SelectableLayer::~SelectableLayer(){}

void SelectableLayer::configure(int rows, int cols, int cellWidth, int cellHeight){

	_rows = rows; _cols = cols; _cellWidth = cellWidth; _cellHeight = cellHeight;

	this->setRowCount(_rows);
	this->setColumnCount(_cols);

	_cellWidth;
	for(int row = 0; row < _rows; row++){

		//columns
		for(int col = 0; col < _cols; col++){
			
			if(row == 0){
				this->setColumnWidth(col, _cellWidth);
			}
			this->setItem(row, col, new QTableWidgetItem());
			this->item(row,col)->setFlags(
				Qt::ItemIsSelectable |
				Qt::ItemIsEnabled);
		}

		//rows
		this->setRowHeight(row, _cellHeight);
	}	
	
	this->setAcceptDrops(false);
	this->setCornerButtonEnabled(false);
	this->setShowGrid(false);

	//scroll bar policy
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//hide headers
	(this->horizontalHeader())->hide();
	(this->verticalHeader())->hide();

	this->setSelectionMode(QAbstractItemView::ExtendedSelection);
	this->setSelectionBehavior(QAbstractItemView::SelectItems);

	this->setStyleSheet(SELECTABLE_LAYER_STYLE(SELECTION_COLOR));
	
	//table widget settings
	this->setFrameStyle(QFrame::NoFrame);
	this->resize(_cellWidth*_cols, _cellHeight*_rows);

	QObject::connect(
		this, SIGNAL(itemSelectionChanged()),
		this, SLOT(slItemSelectionChanged()));

	return;
}

void SelectableLayer::eraseAllColors(void){
	this->setStyleSheet(SELECTABLE_LAYER_STYLE(SELECTION_COLOR));
	return;
}

/*void SelectableLayer::keyPressEvent(QKeyEvent *ptrEvent){

	switch(ptrEvent->key()){

		case Qt::Key_Tab:

			if(this->parent() != NULL){
				ptrEvent->accept();
				QCoreApplication::sendEvent(this->parent(),ptrEvent);				
			}
			else{
				ptrEvent->ignore();
			}			
			break;

		default:
			ptrEvent->ignore();
			break;
	}
	
	return;
}*/

QModelIndexList SelectableLayer::selectedIndexes(void){
	return QTableWidget::selectedIndexes();
}

void SelectableLayer::slItemSelectionChanged(void){
	emit this->sgSelectionChangedExternaly(this->selectedIndexes());	
	return;
}

void SelectableLayer::slSelectionChangedExternaly(QModelIndexList indexes){

	QObject::disconnect(
		this, SIGNAL(itemSelectionChanged()),
		this, SLOT(slItemSelectionChanged()));
	
	this->clearSelection();	
	for(int i = 0; i < indexes.size(); i++){
		this->item(indexes[i].row(), indexes[i].column())->setSelected(true);
	}

	QObject::connect(
		this, SIGNAL(itemSelectionChanged()),
		this, SLOT(slItemSelectionChanged()));

	return;
}

void SelectableLayer::reallocateLayer(int rows){

	int diffRows = rows - _rows;
	this->setRowCount(rows);

	//smaller
	if(diffRows < 0){

		for(int row = _rows-1; row > rows; --row){
			
			for(int col = 0; col < _cols; col++){
				delete this->takeItem(row, col);
			}
			this->removeRow(row);
		}
	}
	//bigger
	else{
		
		for(int row = _rows; row <= rows; row++){
			
			for(int col = 0; col < _cols; col++){
				this->setItem(row, col, new QTableWidgetItem(*this->item(0,0)));
			}
			this->setRowHeight(row,_cellHeight);
		}
	}
	
	_rows = rows;
	this->setStyleSheet(SELECTABLE_LAYER_STYLE(SELECTION_COLOR));
	this->resize(_cellWidth*_cols, _cellHeight*_rows);
	
	return;
}