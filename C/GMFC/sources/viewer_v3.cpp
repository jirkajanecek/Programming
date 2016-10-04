#include "viewer_v3.h"

//EXCEPTIONS
Viewer::ViewerException::ViewerException( void ) : std::exception(){}

Viewer::ViewerException::ViewerException( std::string msg ) : std::exception( msg.c_str()){}

Viewer::Viewer(QString* ptrLabelForm, QList<QString>* ptrDataForm, QString fName, QWidget* ptrParent)
	: QLabel(ptrParent){

	_shift = 0;
	_drag = 0;

	_v3_fName = fName;
	_v3_ptrDataForm = ptrDataForm;
	_v3_ptrLabelForm = ptrLabelForm;
}

Viewer::~Viewer(){}

void Viewer::configure(void){
	
	this->configureClassSpecifics();	
	this->configureView();
	this->createView();

	this->resize(_appWidth, _appHeight);	
	return;
}

void Viewer::configureClassSpecifics( void ){
	
	//mimetype
	_cellMimetype = "application/dragged-byte";	

	//cell settings
	_v2_cellWidth = CODE_FONT_W*HVIEWER_NUM_CELL;
	_v2_cellHeight = CELL_H;

	_v2_cols = INIT_COLS;
	_v2_rows = _v3_ptrDataForm->size()/_v2_cols;

	_appWidth = CODE_FONT_W * (_v2_cols * HVIEWER_NUM_CELL);
	_appHeight = CODE_FONT_H * _v2_rows;
	
	return;
}

void Viewer::configureView(void){

	//view styles
	this->setFrameStyle(QFrame::NoFrame);
	this->setAcceptDrops(true);	
	this->setTextFormat(Qt::RichText);
	this->setStyleSheet(VIEWER_STYLE);
	this->setAlignment(Qt::AlignCenter);
	this->setContentsMargins(0,0,0,0);	
	
	//font policy
	QFont font;
	font.setFamily(CODE_FONT);
	font.setPixelSize(CODE_FONT_SIZE);
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
	
	this->setText(*_v3_ptrLabelForm);
	return;
}

void Viewer::updateView(QString* ptrLabelForm, QList<QString>* ptrDataForm){
	
	_v3_ptrLabelForm = ptrLabelForm;
	_v3_ptrDataForm = ptrDataForm;

	this->setText(*_v3_ptrLabelForm);
	return;
}

int Viewer::translatePointToSequence(QPoint point){
	
	int row = point.y() / _v2_cellHeight;
	int collumn = point.x() / _v2_cellWidth;
	int seq = (row * _v2_cols) + collumn;
	
	return seq;
}

void Viewer::artificialResize(QString* ptrLabelForm, QList<QString>* ptrDataForm){

	_v3_ptrDataForm = ptrDataForm;
	_v3_ptrLabelForm = ptrLabelForm;

	_v2_rows = _v3_ptrDataForm->size()/_v2_cols;
	_appHeight = CODE_FONT_H * _v2_rows;

	this->resize(_appWidth, _appHeight);
	this->setText(*_v3_ptrLabelForm);

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

	QString strData = _v3_ptrDataForm->at(_drag);
	if(strData == ""){
		return;
	}

	//emit dragged start to upper level over view changed slot
	//no shift is really requested
	emit this->sgViewChanged(0,_drag);

	//create rectangel type object from QTableWidgetItem
	QRect cellRect =
		QRect((_drag%_v2_cols) * _v2_cellWidth,
		(_drag/_v2_cols) * _v2_cellHeight, _v2_cellWidth, _v2_cellHeight);
	
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
	emit this->sgViewChanged(0,-2);

	return;
}