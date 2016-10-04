#include "viewer_v2.h"

//EXCEPTIONS
Viewer::ViewerException::ViewerException( void ) : std::exception(){}

Viewer::ViewerException::ViewerException( std::string msg ) : std::exception( msg.c_str()){}

Viewer::Viewer(QStandardItemModel* ptrModel, QString fName, QWidget* ptrParent)
	: QTableView(ptrParent){

	_shift = 0;
	_drag = 0;

	_v2_ptrModel = ptrModel;
	_v2_fName = fName;

	_v2_rows = ptrModel->rowCount();
	_v2_cols = ptrModel->columnCount();
}

Viewer::~Viewer(){}

void Viewer::configure(void){
	
	this->configureClassSpecifics();
	
	this->configureView();

	this->createView();

	this->resize(_v2_ptrModel->columnCount() * _v2_cellWidth,
					_v2_ptrModel->rowCount() * _v2_cellHeight);
	
	return;
}

void Viewer::configureClassSpecifics( void ){
	
	//mimetype
	_cellMimetype = "application/dragged-byte";	

	//cell settings
	_v2_cellWidth = 18;
	_v2_cellHeight = CELL_H;
	
	return;
}

void Viewer::configureView(void){

	//view styles
	this->setShowGrid(false);	
	this->setFrameStyle(QFrame::NoFrame);
		
	//single selection mode
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	
	//setup drag and drop mode
	this->setDragEnabled(true);
	this->setAcceptDrops(true);	
		
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

void Viewer::computeShift(QPoint pos){
	
	int current = this->translatePointToSequence(pos);
	_shift = _drag - current;
	return;
}

bool Viewer::shiftRequested(QPoint pos){

	//compute current index	
	int current = this->translatePointToSequence(pos);

	//compare with dragged index
	if( _drag != current ){
		return true;
	}
	else{
		return false;
	}
}

void Viewer::createView(void){
	
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

void Viewer::updateView(QStandardItemModel* ptrModel){
	
	_v2_ptrModel = ptrModel;
	//this->setModel(_v2_ptrModel);

	return;
}

int Viewer::translatePointToSequence(QPoint point){
	
	int row = point.y() / _v2_cellHeight;
	int collumn = point.x() / _v2_cellWidth;
	int seq = (row * _v2_ptrModel->columnCount()) + collumn;
	
	return seq;
}

void Viewer::artificialResize(QStandardItemModel* ptrModel){

	int oldRows = _v2_rows;
	int oldCellsNum = oldRows * _v2_ptrModel->columnCount();

	_v2_rows = _v2_ptrModel->rowCount();
	_v2_cols = _v2_ptrModel->columnCount();

	qDebug() << "oldRows: " << oldRows << " newRows: " << ptrModel->rowCount();

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

void Viewer::dragEnterEvent(QDragEnterEvent *event ){	
	
	if( event->mimeData()->hasFormat(_cellMimetype) ){
		event->accept();		
	}
	else{	
		event->ignore();		
	}	
	return;
}

void Viewer::dragMoveEvent(QDragMoveEvent *event){	
	
	//accept event
	event->accept();

	//check if shift is requested
	if(this->shiftRequested(event->pos()) == false){
		return;
	}

	//compute shift and change anctual [draged] index
	this->computeShift(event->pos());
	_drag = _drag + (_shift * (-1));

	emit sgViewChanged(_shift, _drag);
		
	return;
}

void Viewer::dropEvent(QDropEvent *event){
	
	//accept event
	event->accept();

	//check if shift is requested
	if( this->shiftRequested(event->pos()) == false ){		
		emit this->sgViewChanged(0, -2);
		return;
	}
	//compute shift
	else{		
		this->computeShift(event->pos());	
		emit this->sgViewChanged(_shift, -2);
		return;
	}
}

void Viewer::mousePressEvent(QMouseEvent *event){

	event->accept();

	//send request for focus to upper level [sampleViewer due to eventFilter]
	if(this->parent() != NULL){
		QCoreApplication::sendEvent(this->parent(), event);
		QCoreApplication::processEvents();
	}
	
	//initialize _drag variable [internal array pointer]
	_drag = this->translatePointToSequence( event->pos() );

	//take item from QTableWidget
	QModelIndex index = this->indexAt(event->pos());

	QString strData = "";
	if((strData = index.data(Qt::DisplayRole).toString()) == ""){
		return;
	}

	//create rectangel type object from QTableWidgetItem
	QRect cellRect =
		QRect(index.column() * _v2_cellWidth,
		index.row() * _v2_cellHeight, _v2_cellWidth, _v2_cellHeight);
	
	//create drag hotSpot
	QPoint hotSpot = event->pos() - cellRect.topLeft();

	//create frame for pixmap rendering
	QLabel ptrItemLbl;
	ptrItemLbl.setStyleSheet(PIXMAP_STYLE);	
	ptrItemLbl.setText(strData);
	ptrItemLbl.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
	ptrItemLbl.setFixedSize( _v2_cellWidth, _v2_cellHeight);

	QFont font;
	font.setFamily(CODE_FONT);
	font.setPixelSize(10);
	font.setBold(false);
	ptrItemLbl.setFont(font);
	
	//set up proper mimeData
	QMimeData *ptrMData = new QMimeData();
	ptrMData->setText(strData);
	ptrMData->setData(
		_cellMimetype,
		QByteArray::number(hotSpot.x()) + " " +
		QByteArray::number(hotSpot.y()));
	
	//icon rendering
	QPixmap pixmap( ptrItemLbl.size());
	ptrItemLbl.render(&pixmap);	
	
	//create drag
	QDrag* ptrDrag = new QDrag(this);
	ptrDrag->setMimeData(ptrMData);
	ptrDrag->setPixmap(pixmap);
	ptrDrag->setHotSpot(hotSpot);

	//engage drag
	Qt::DropAction dropAct = ptrDrag->exec(Qt::MoveAction);
	if( dropAct == Qt::MoveAction ){}	

	return;
}