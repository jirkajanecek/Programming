#include "viewer.h"

//EXCEPTIONS
Viewer::ViewerException::ViewerException( void ) : std::exception(){}

Viewer::ViewerException::ViewerException( std::string msg ) : std::exception( msg.c_str()){}

Viewer::Viewer(QString filename, int rows, int cols, QWidget *parent, Qt::WFlags)
	: QTableWidget(parent)	
{
	//
	_shift = 0;
	_file = 0;
	_offset = 0;
	_drag = 0;
	_cellHorizontal = cols;
	_cellVertical = rows;
	_cellsNum = _cellHorizontal * _cellVertical;
	_fName = filename;
	_md5 = "";

	//default value for save patterns storage
	_ptrSaves = NULL;
	
	//init variables
	_indexes = new QList<Viewer::IndexItem_t*>();
	_dataCont = new QList<int>();
	
	//_ptrViewer = new QTableWidget( _cellVertical, _cellHorizontal, this );

	this->setRowCount(_cellVertical);
	this->setColumnCount(_cellHorizontal);

	//TODO: check size based of config file
	if( false ){
		
		//recompute sizes from config file
		_configExists = true;
	}
	else{		
		
		_configExists = false;
	}
}

Viewer::~Viewer(){
	
	//file handle cleaning
	if( (_contentFile != NULL) && (_contentFile->isOpen()) ){
		
		_contentFile->close();
		delete _contentFile;
	}
	
	//viewer content deallocation
	this->clearViewer();

	//viewer deallocation
	/*if(_ptrViewer != NULL){
		
		delete _ptrViewer;
	}*/

	//bytes deallocation
	if( _bytes != NULL ){
		
		delete _bytes;
	}

	//data content deallocation
	if( _dataCont != NULL ){
		
		delete _dataCont;
	}
}

int Viewer::FileSize( void ){
	return _contentFile->size();
}

QString Viewer::MD5( void ){
	return _md5;
}

QList<Viewer::IndexItem_t*>* Viewer::Indexes(void){

	return _indexes;
}

QList<int>* Viewer::DataContent( void ){

	return _dataCont;
}

int Viewer::CellsNumber( void ){
	return _cellsNum;
}

int Viewer::CellsHorizontal( void ){
	return _cellHorizontal;
}

int Viewer::CellsVertical( void ){
	return _cellVertical;
}

void Viewer::setSaveIntervals( QList<SaveParser::saveInterval_t>* intrvs){

	_ptrSaves = intrvs;
	return;
}

void Viewer::setPE( PEbrowser *ptrPE ){

	_ptrPE = ptrPE;
	return;
}

QString Viewer::charToCellLabel(unsigned char byte){
	//translation char to hexa number
	QString hexaNum = "";
	bool append = false;
	if( byte < 0x10 ){
		append = true;
	}
	hexaNum = (hexaNum.setNum(byte,0x10)).toUpper();
	if(append){
		hexaNum = "*" + hexaNum;
	}
	return "";
}

void Viewer::configure(void){
	
	this->openFile();

	this->computeMD5();

	this->configureClassSpecifics();

	this->configureTWidget();

	this->createBytes(_file);

	this->createIndexes(_bytes, _offset, _file);

	this->resize(_appWidth,_appHeight);	

	//this->setupBaseFrame();	
	
	this->createViewer(); //gui ready - deploying bytes
	
	return;
}

void Viewer::configureClassSpecifics( void ){
	
	//mime type
	_cellMimetype = "application/dragged-byte";	

	if(!_configExists){
		//config essential
		_byteWidth = 18;
		_byteHeight = 11;
		_appWidth = _cellHorizontal * _byteWidth;
		_appHeight = _cellVertical * _byteHeight;
	}
	else{
	}

	return;
}

void Viewer::configureTWidget( void ){

	//table widget settings
	this->setShowGrid(false);
	this->setAcceptDrops(true);
	this->setCornerButtonEnabled(false);
	//this->resize( _appWidth + 2, _appHeight + 2);
	this->setFrameStyle(QFrame::NoFrame);
	this->resize( _appWidth, _appHeight);	
	//this->setFixedSize( _appWidth + 2, _appHeight + 2);

	//single selection mode
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	
	//setup drag and drop mode
	this->setDragEnabled(true);
	/*this->viewport()->setAcceptDrops(true);
	this->setDragDropMode(QAbstractItemView::InternalMove);
	this->setDropIndicatorShown(true);*/
	
	//scroll bar policy
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//hide headers
	(this->horizontalHeader())->hide();
	(this->verticalHeader())->hide();

	return;
}

void Viewer::viewerChanged( int shift ){
	
	_shift = shift;
	
	this->propagateStep();
	return;
}

void Viewer::viewerChanged( int shift, int file, int offset ){
	
	_shift = shift;
	_file = file;
	_offset = offset;
		
	this->propagateStep();
	return;
}

void Viewer::viewerChangedAbs( int value ){
	
	//logic for setting up proper internal variables
	int seek = 0;
	
	if( value < 0 ){
		_offset = abs(value);
		seek = 0;
	}
	else{
		_offset = 0;
		seek = value;
	}
	
	_file = 0;
	this->clearBytes();
	this->createBytes(seek);
	this->clearIndexes();
	this->createIndexes(_bytes, _offset, seek);
	this->refreshViewer();
	return;
}

void Viewer::setupBaseFrame(void){
	_base = new QFrame(this);
	_base->resize(_appWidth, _appHeight);	
	_base->setFrameStyle(QFrame::Box | QFrame::Plain);
	_base->setLineWidth(1);	
	_base->setStyleSheet(FRAME_STYLE);
	return;
}

void Viewer::createIndexes(QByteArray* bytes, int viewerOffset, int fileOffset){
	
	Viewer::IndexItem_t *item = NULL;
	bool dataCopyEnable = false;

	this->clearIndexes();
	
	for( int i = 0; i < _cellsNum; i++ ){		
		
		//switch on/off 
		if( i == viewerOffset ){
			
			dataCopyEnable = true;
		}
		else if( (i - viewerOffset) >= bytes->count() ){
			
			dataCopyEnable = false;
		}		
		
		//fill up indexes
		item = new Viewer::IndexItem_t();
		item->viewer = i;
		
		if( dataCopyEnable ){
			
			item->file = fileOffset;
			item->bytes = (i - viewerOffset);
			fileOffset++;
		}
		else{
			
			item->file = -1;
			item->bytes = -1;
		}
		
		_indexes->push_back(item);
	}
	return;
}

void Viewer::clearIndexes(void){
	Viewer::IndexItem_t *item = NULL;
	for( int i = 0; i < _indexes->count(); i++  ){
		item = _indexes->at(i);
		delete item;
	}
	_indexes->clear();
	return;
}

void Viewer::validateBufferPosition( QPoint* point ){
	if( point->x() < 0 ){
		point->setX(0);
	}
	if( point->y() < 0 ){
		point->setY(0);
	}
	if( point->x() >= _appWidth){
		point->setX( _appWidth -1 );
	}
	if( point->y() >= _appHeight ){
		point->setY( _appHeight -1 );
	}
	return;
}

void Viewer::computeShift(QPoint pos){
	
	this->validateBufferPosition(&pos);
	
	int current = this->translatePointToSequence(pos);
	
	_shift = _drag - current;
	
	return;
}

bool Viewer::shiftRequested( QPoint pos ){

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

void Viewer::openFile(void){
		
	//content file handling
	if( _fName == "" ){
		
		//file name was not inputed
		throw Viewer::ViewerException("File name was not inputed.");
	}
	
	_contentFile = new QFile(_fName);
	if(!_contentFile->open(QIODevice::ReadOnly)){
		
		//file name not exists
		throw Viewer::ViewerException("File not exists.");
	}

	return;
}

void Viewer::computeMD5( void ){	
	return;
}

bool Viewer::addressVisible( int addr ){

	SaveParser::saveInterval_t sIntr;

	for( int i = 0; i < _ptrSaves->count(); i++){

		//load storage item into temporary struct
		sIntr = _ptrSaves->at(i);

		//check if address should be colorized
		if( (sIntr.type == SaveParser::PA) &&
			(addr >= sIntr.start) &&
			(addr <= sIntr.stop)){
				return true;
		}
	}

	return false;
}

void Viewer::createBytes( int seek ){
	
	char* buffer = NULL;
	
	_contentFile->seek( seek );
	_file = seek;	//set up start position in file
	
	buffer = (char*)malloc((_cellsNum));
	int readed = _contentFile->read(buffer, (_cellsNum));
	_bytes = new QByteArray( buffer, readed );
	
	free(buffer);
	return;
}

void Viewer::clearBytes( void ){
	if( _bytes != NULL){
		delete _bytes;
	}
	return;
}

void Viewer::createViewer(){
	
	this->clearViewer(); //cleaning entire area
	
	if( _indexes->count() != _cellsNum){
		//TODO: throw exception		
	}
	
	Viewer::IndexItem_t *info = NULL;
	QTableWidgetItem *item = NULL;
	QString hexaNb = "";

	//text's font
	QFont font;
	font.setFamily( CODE_FONT );
	font.setPixelSize(10);
	font.setBold(false);

	for( int rowIdx = 0; rowIdx < _cellVertical; rowIdx++ ){

		//row height
		this->setRowHeight( rowIdx, _byteHeight);		

		for( int colIdx = 0; colIdx < _cellHorizontal; colIdx++ ){
				
			//column width
			this->setColumnWidth( colIdx, _byteWidth );
		
			//load data from indexes array
			info = _indexes->at((rowIdx * _cellHorizontal) + colIdx);

			if( info->bytes == -1 ){
			
				//create and place new item
				item = new QTableWidgetItem("");		
			
				//push numeric data into data array 
				_dataCont->push_back(-1);
			}
			else{
			
				//translate byte value to string label
				hexaNb = this->charToCellLabel((unsigned char) _bytes->at(info->bytes));

				//create new item and place it to QTableWidget
				item = new QTableWidgetItem( hexaNb );			

				//push numeric data into data array
				_dataCont->push_back((unsigned char)_bytes->at(info->bytes));
			}

			//set up item properties
			item->setFont(font);
			item->setFlags( Qt::NoItemFlags );
			item->setTextAlignment(Qt::AlignCenter);
			item->setBackground( QBrush(EMPTY_COLB));
			item->setForeground( QBrush(EMPTY_COLF));
			
			//store into QTableWidget
			this->setItem( rowIdx, colIdx, item );
		}
	}
	return;
}

void Viewer::clearViewer(void){	
	
	QTableWidgetItem *item = NULL;
	int rowIdx = 0, colIdx = 0;	
	
	for( int i = 0; i < _cellsNum; i++ ){

		//rows and collumns computation
		rowIdx = i / _cellHorizontal;
		colIdx = i % _cellHorizontal;
		
		item = this->takeItem( rowIdx, colIdx );

		if( item != NULL ){
			delete item;
		}
	}
	
	return;
}

void Viewer::refreshViewer( bool silent ){
	
	if( (_indexes->size() != _cellsNum) ||
		(_dataCont->size() != _cellsNum)
	){
		return;
	}

	Viewer::IndexItem_t *index = NULL;
	QTableWidgetItem *item = NULL;
	QString hexaNum = "";
	int rowIdx = 0, colIdx = 0;

	//start address for computing virtual address
	int stAddr = _file;
	bool saveEn = false;

	//determines save pattern
	if( (_ptrSaves != NULL) && (_ptrSaves->count() > 0) && (_ptrPE != NULL) ){
		
		saveEn = true;
	}

	for( int i = 0; i < _indexes->size(); i++ ){
		
		//row and collumn computation
		rowIdx = i / _cellHorizontal;
		colIdx = i % _cellHorizontal;

		index = _indexes->at(i);

		//load item from TableWidget
		item = this->item( rowIdx, colIdx);

		//cell is empty
		if( index->bytes == -1 ){

			if( item->text() != "" ){
			
				//set up text label
				item->setText("");

				//set up data content item
				_dataCont->replace( i, -1);
			}

			item->setBackground( QBrush( EMPTY_COLB));
			item->setForeground( QBrush( EMPTY_COLF));			
		}
		//cell is not empty
		else{
			
			//translate char to hexa value
			hexaNum = charToCellLabel( _bytes->at(index->bytes) );

			if( item->text() != hexaNum ){
			
				//set up text label
				item->setText(hexaNum);

				//set up data content item
				_dataCont->replace( i, (unsigned char)_bytes->at(index->bytes) );
			}

			///SAVE PATTERN ENABLE
			//colorize cell based on save pattern
			if( saveEn ){

				//cell is in save pattern
				if( this->addressVisible(stAddr) ){

					item->setBackground( QBrush( SAVE_COLB));
					item->setForeground( QBrush( SAVE_COLF));
				}
				//cell is not in save pattern
				else{

					item->setBackground( QBrush( EMPTY_COLB));
					item->setForeground( QBrush( EMPTY_COLF));
				}

				//next address
				stAddr++;				
			}			
		}
	}

	if(!silent){
		//emit signal to parent object about viewer changes [to FileWidget]
		emit this->viewerIsChanging( _fName, _dataCont );
	}
	else{
		//emit silent signal for refresh
		emit this->viewerIsChangingSilent( _fName, _dataCont);
	}

	return;
}

void Viewer::reallocViewer(void){

	int oldCellNb = this->rowCount() * this->columnCount();
	int diff = _cellsNum - oldCellNb;
	
	if(diff == 0){
		return;
	}

	//application size changes
	_appWidth = _cellHorizontal * _byteWidth;
	_appHeight = _cellVertical * _byteHeight;	

	//resetup proper size for QTableidget
	this->setRowCount(_cellVertical);
	this->setColumnCount(_cellHorizontal);

	//deallocation needed
	if(diff < 0){

		int rowDiff = abs(diff) / _cellHorizontal; //difference in rows
				
		//not enought cells remain after resize
		//if(_offset > (_cellsNum - (rowDiff * _cellHorizontal))){
		if(_offset > (oldCellNb - (_cellHorizontal*rowDiff))){			
			_offset = _cellsNum - (_offset % _cellHorizontal);
		}

		//bytes array can be ignore because data are already read from file
		//there are not used all of them
		//create new index array
		this->clearIndexes();
		this->createIndexes(_bytes, _offset, _file);

		delete _dataCont;
		_dataCont = new QList<int>;
		
		for(int i = 0; i < _indexes->count(); i++){

			if(_indexes->at(i)->bytes == -1){
				_dataCont->push_back(-1);
			}
			else{
				_dataCont->push_back(
					(unsigned char)_bytes->at(_indexes->at(i)->bytes));
			}
		}		
	}
	//allocation needed
	else if(diff > 0){

		//recreate byte array [download data from file]
		this->clearBytes();
		this->createBytes(_file);

		//no offset resetup is needed - create indexes array
		this->clearIndexes();
		this->createIndexes(_bytes, _offset, _file);

		//font settings
		QFont font;
		font.setFamily( CODE_FONT );
		font.setPixelSize(10);
		font.setBold(false);

		Viewer::IndexItem_t *info = NULL;
		QTableWidgetItem* ptrIt = NULL;

		for(int i = oldCellNb; i < _cellsNum; i++ ){

			//setup row height
			if((i % _cellHorizontal) == 0){
				this->setRowHeight(i / _cellHorizontal, _byteHeight);
			}

			ptrIt = new QTableWidgetItem();
			ptrIt->setFlags(Qt::NoItemFlags);
			ptrIt->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
			ptrIt->setFont(font);
			ptrIt->setBackground(QBrush(EMPTY_COLB));
			ptrIt->setForeground(QBrush(EMPTY_COLF));

			info = _indexes->at(i);
			if(info->bytes == -1){
				
				ptrIt->setText("");
				_dataCont->push_back(-1);
			}
			else{

				ptrIt->setText(
						this->charToCellLabel((unsigned char) _bytes->at(info->bytes)));
				_dataCont->push_back((unsigned char)_bytes->at(info->bytes));
			}

			this->setItem(i / _cellHorizontal, i % _cellHorizontal, ptrIt);
		}	
	}

	return;
}

void Viewer::propagateStep( void ){
	
	//with no shift no need to refresh
	if( _shift == 0 ){
		return;
	}
	
	bool zeroOffset = false;	//true when offset is zero by computation
	int auxiliaryShift = 0;		//set number when offset is negative and 
								//any positive shift exists
	int offsetOld = _offset;	//backup old offset	
	
	//OFFSET computing
	_offset = _offset + (_shift * (-1));	//update offset
	if( _offset == 0 ){				//computed zero
		
		zeroOffset = true;
		//this->Output("#1");
	}
	else if( (_offset < 0) ||		//negative offset
		( _file >= abs(_shift) )	//file pointer bigger than proceeded shift
	){
		
		//QString str2; str2.setNum(_offset);
		//QString str3; str3.setNum(_file);
		//QString str4; str4.setNum(abs(_shift));
		//this->Output("Offset settings: offset " + str2 + " file "+ str3 + " abs shift "+ str4);
		//this->Output("#2");
		if( _offset < 0 ){
			auxiliaryShift = abs(_shift) + _offset;
		}
		_offset = 0;		
	}
	else if( (_offset >= _cellsNum) && (_file != 0)){
		_offset -= _file;
	}
	else if( _offset >= _cellsNum ){
		_offset = _cellsNum - 1;
		//this->Output("#3");
	}
	else{
		_offset -= _file;
		//this->Output("#4");
	}
	//SEEK computing
	int seek = _file + _shift - auxiliaryShift;	
	if( (_offset > 0) ||
		(zeroOffset == true)
	){
		seek = 0;
		//this->Output("#1a");
	}
	else if( seek < 0 ){
		seek = 0;
		//this->Output("#2a");
	}
	else if( seek >= _contentFile->size() ){
		seek = _shift - offsetOld;
		//this->Output("#3a");
	}
	else{
		//QString str; str.setNum(seek);
		//QString str1; str1.setNum(_offset);
		//this->Output("#4a " + str + " " + str1);
	}

	this->clearBytes();							//download data
	this->createBytes(seek);
	
	this->clearIndexes();						//create indexed
	this->createIndexes(_bytes,_offset,seek);	//render information
	
	this->refreshViewer();	
	
	//debug output
	//QString seekStr; seekStr.setNum(seek);
	//QString offsetStr; offsetStr.setNum(_offset);
	//QString fileStr; fileStr.setNum(_file);
	//QString shiftStr; shiftStr.setNum(_shift);
	//this->Output("seek: " + seekStr + " offset: " + offsetStr + 
	//	" file: " + fileStr + " shift: " + shiftStr);

	return;
}

QPoint* Viewer::translateSequenceToPoint( int seq ){
	if( seq < 0 ){
		return new QPoint(0,0);
	}
	if( seq >= _cellsNum ){
		return (new QPoint( (_cellHorizontal - 1) * _byteWidth, (_cellVertical - 1) * _byteHeight ));
	}
	int y = (seq / _cellHorizontal) * _byteHeight;
	int x = (seq % _cellHorizontal) * _byteWidth;
	QPoint *point = new QPoint(x,y);
	return point;	
}

int Viewer::translatePointToSequence( QPoint point ){
	int row = point.y() / _byteHeight;
	int collumn = point.x() / _byteWidth;
	return (row * _cellHorizontal) + collumn;
}

bool Viewer::isValidShift( int shift ){
	
	//zero shift
	if( shift == 0 ){
		
		return false;
	}
	//shift positive - top level
	else if((shift > 0) && ((_offset + (this->FileSize() - _file)) <= shift)){
		
		return false;		
	}
	//shift negative - bottom level
	else if((shift < 0) && 
			((_file == 0) && (_offset + abs(shift) >= _cellsNum))
	){
		
		return false;
	}
	//other ways
	else{
		
		return true;
	}
}

int Viewer::binarySearch( QString ){
	return -1;
}

void Viewer::forceRefresh( void ){

	this->refreshViewer();

	return;
}

void Viewer::artificialResize(QResizeEvent* ptrEv){

	//height difference
	int diffH = ptrEv->size().height() - ptrEv->oldSize().height();

	//cells differences
	int diffCells = abs(diffH)/_byteHeight;

	//no entire cell 
	if(diffCells == 0){
		return;
	}

	if(diffH < 0){
		_cellVertical -= diffCells;
	}
	else{
		_cellVertical += diffCells;
	}

	//recompute total cell number
	_cellsNum = _cellVertical * _cellHorizontal;

	//reallocate viewer content
	this->reallocViewer();

	//refresh viewer
	this->refreshViewer(true);

	//resize viewer
	this->resize(_appWidth, _appHeight);

	return;
}

int Viewer::absoluteTopValue(void){
	return _file - _offset;
}

void Viewer::dragEnterEvent(QDragEnterEvent *event ){	
	
	if( event->mimeData()->hasFormat(_cellMimetype) ){

		event->accept();		
	}
	else{
		
		//not my mimeType
		event->ignore();		
	}
	
	return;
}

void Viewer::dragMoveEvent( QDragMoveEvent *event ){	
	
	//accept event
	event->accept();

	//check if shift is requested
	if( this->shiftRequested(event->pos()) == false ){
		return;
	}

	//compute shift and change anctual [draged] index
	this->computeShift(event->pos());
	_drag = _drag + (_shift * (-1));

	//emit changes into other components
	emit this->viewerIsChanging(_shift, _file, _offset );	//emiting signal to textviewer
	emit this->viewerIsChanging(_shift);					//emiting signal to address bar
	
	//propagate step into viewer
	this->propagateStep();
	
	//emit changes into scroll bar
	emit this->viewerIsChangingAbs( _file - _offset );
	
	return;
}

void Viewer::dropEvent(QDropEvent *event ){
	
	//accept event
	event->accept();

	//drag action is finished
	emit this->sgDragIsFinished();

	//check if shift is requested
	if( this->shiftRequested(event->pos()) == false ){
		return;
	}

	//compute shift
	this->computeShift(event->pos());
	
	//emit changes to other components
	emit this->viewerIsChanging(_shift, _file, _offset );	//emiting signal to textviewer
	emit this->viewerIsChanging(_shift);					//emiting signal to address bar
	
	//propagate step into viewer
	this->propagateStep();
	
	//emit changes into scroll bar
	emit this->viewerIsChangingAbs( _file - _offset );

	return;
}

void Viewer::mousePressEvent(QMouseEvent *event){

	event->accept();
	
	//send message about mouse button press - send to upper level
	if( this->parent() != NULL ){		
		QCoreApplication::sendEvent( this->parent(), event);
	}

	//initialize _drag variable [internal array pointer]
	_drag = this->translatePointToSequence( event->pos() );

	//drag started
	emit this->sgDragInProgress( _drag );

	//take item from QTableWidget
	QTableWidgetItem *ptrItem = this->itemAt( event->pos() );

	//check if dragged item is valid
	if( (ptrItem == NULL) || (ptrItem->text() == "") ){
		return;
	}
	
	//create rectangel type object from QTableWidgetItem
	QRect ptrItemRct = this->visualItemRect( ptrItem );
	
	//create drag hotSpot
	QPoint hotSpot = event->pos() - ptrItemRct.topLeft();

	//create frame for pixmap rendering
	QLabel ptrItemLbl;
	ptrItemLbl.setStyleSheet(PIXMAP_STYLE);	
	ptrItemLbl.setText( ptrItem->text());
	ptrItemLbl.setAlignment( Qt::AlignCenter );
	ptrItemLbl.setFixedSize( _byteWidth, _byteHeight);
	
	//set up proper mimeData
	QMimeData *mData = new QMimeData();
	mData->setText( ptrItem->text() );
	mData->setData( _cellMimetype,
						QByteArray::number(hotSpot.x()) + " " +
						QByteArray::number(hotSpot.y()));
	
	//icon rendering
	QPixmap pixmap( ptrItemLbl.size());
	ptrItemLbl.render(&pixmap);	
	
	//create drag
	QDrag * drag = new QDrag(this);
	drag->setMimeData(mData);
	drag->setPixmap(pixmap);
	drag->setHotSpot(hotSpot);

	//engage drag
	Qt::DropAction dropAct = drag->exec(Qt::MoveAction);
	if( dropAct == Qt::MoveAction ){}

	return;
}