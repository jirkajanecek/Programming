#include "addressbar.h"

AddressBar::AddressBar(int startAddr,
		int rows,
		QWidget *parent,
		PEbrowser* ptrPE,
		Qt::WFlags flags)
	: QTableWidget(parent)
{
	
	_addresses = new QList<int>();
	_shift = 0;
	_startAddr = startAddr;
	_addrVertical = rows;

	//PE file exists
	if( ptrPE != NULL ){
		
		_ptrPE = ptrPE;
		_baseAddrEn = false;
	}
	else{
		
		_ptrPE = NULL;
		_baseAddrEn = false;
	}

	//config file exists
	if( false ){
		_configExists = true;
	}
	else{
		_configExists = false;
	}

	this->setRowCount(_addrVertical);
	this->setColumnCount(1);
	
}

AddressBar::~AddressBar(){
	
	QTableWidgetItem *ptrItem = NULL;
		
	for( int i = 0; i < this->rowCount(); i++ )	{

		ptrItem = this->takeItem( i, 0);

		if( ptrItem != NULL ){
			delete ptrItem;
		}
	}
	
	delete _addresses;
}

void AddressBar::Output(QString msg){
	qDebug(msg.toAscii());
}

QString AddressBar::getAddressFromSeqIndex(int seqIdx){

	//check if sequential index is correct
	if((seqIdx >= ( _addrVertical * ADDR_STEP )) ||
		(seqIdx < 0)){
		return "";
	}

	//check if request for virtual address is enabled
	if( _baseAddrEn ){

		return this->numToHexString(this->trToImgAddr(_startAddr + seqIdx));
	}
	else{

		return this->numToHexString(_startAddr + seqIdx);
	}
}

void AddressBar::artificialResize(QResizeEvent* ptrEv, int startAddr){

	_startAddr = startAddr;

	//height difference
	int diffH = ptrEv->size().height() - ptrEv->oldSize().height();

	//cells differences
	int diffCells = abs(diffH)/_addrHeight;

	//no entire cell 
	if(diffCells == 0){
		return;
	}

	if(diffH < 0){
		_addrVertical -= diffCells;
	}
	else{
		_addrVertical += diffCells;
	}

	//reallocate address array
	this->reallocateAddresses();

	//refresh address bar
	this->refreshBar();

	this->resize(_appWidth, _appHeight);
	
	return;
}

void AddressBar::configure( void ){
	
	this->configureClassSpecifics();

	this->configureTWidget();
	
	this->resize(_appWidth,_appHeight);
	
	//this->setupBaseFrame();
	
	this->createAddresses();
	
	this->createBar();
	
	return;
}

void AddressBar::viewerChanged(int shift){
	
	_shift = (-1)*shift;
	
	this->propagateStep();
	
	return;
}

void AddressBar::viewerChangedAbs( int start ){
	
	_startAddr = start;
	
	this->refreshAddresses();
	
	this->refreshBar();
	
	return;
}

void AddressBar::slBaseImageAddress( void ){

	if( _baseAddrEn ){

		_baseAddrEn = false;
	}
	else{

		_baseAddrEn = true;
	}

	//refresh bar values
	this->refreshAddresses();

	//refresh 
	this->refreshBar();
	
	return;
}

void AddressBar::configureClassSpecifics(void){
	
	if(!_configExists){
		//config essential
		_addrWidth = 60;
		_addrHeight = CELL_H;		
	}
	
	_appWidth = _addrWidth;
	_appHeight = _addrVertical * _addrHeight;
	
	return;
}

void AddressBar::configureTWidget( void ){

	//table widget settings
	this->setShowGrid(false);
	this->setAcceptDrops(false);
	this->setCornerButtonEnabled(false);
	
	//this->resize( _appWidth + 2, _appHeight + 2);
	this->setFrameStyle(QFrame::NoFrame);
	this->resize( _appWidth, _appHeight);

	//scroll bar policy
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//hide headers
	(this->horizontalHeader())->hide();
	(this->verticalHeader())->hide();

	this->setSelectionMode(QAbstractItemView::NoSelection);	
	
	return;
}

void AddressBar::setupBaseFrame(void){
	_base = new QFrame(this);
	_base->resize(_appWidth, _appHeight);	
	_base->setFrameStyle(QFrame::Box | QFrame::Plain);
	_base->setLineWidth(1);	
	_base->setStyleSheet(FRAME_STYLE);
	return;
}

void AddressBar::createAddresses(void){
	
	for( int i = 0; i < _addrVertical; i++ ){
		
		if( _baseAddrEn ){

			_addresses->push_back( this->trToImgAddr(_startAddr + (i*ADDR_STEP)));
		}
		else{

			_addresses->push_back(_startAddr + (i*ADDR_STEP));
		}
	}
	return;
}

void AddressBar::refreshAddresses(void){
	
	for(int i = 0; i < _addresses->size(); i++){

		//if base image address enabled
		if( _baseAddrEn ){
			
			_addresses->replace(i, this->trToImgAddr(_startAddr + (i*ADDR_STEP)));
		}
		else{
			
			_addresses->replace(i,_startAddr + (i*ADDR_STEP));
		}
	}

	return;
}

void AddressBar::reallocateAddresses(void){

	int oldRowNb = this->rowCount();
	int diffRow = _addrVertical - oldRowNb;

	if(diffRow == 0){
		return;
	}

	//resetup new widget size variables
	_appHeight = _addrVertical * _addrHeight;
	this->setRowCount(_addrVertical);

	//smaller
	if(diffRow < 0){
		
		diffRow = abs(diffRow);
		QTableWidgetItem* ptrItem = NULL;

		//remove last N items from address array
		for(int i = 0; i < diffRow; i++){
			
			_addresses->removeLast();
			
			ptrItem = this->takeItem(oldRowNb - 1 - i, 1);
			if(ptrItem != NULL){
				delete ptrItem;
			}
		}
	}
	//bigger
	else if(diffRow > 0){

		//text's font
		QFont font;
		font.setFamily( CODE_FONT );
		font.setPixelSize(10);
		font.setBold(false);

		_addresses->clear();
		this->createAddresses();
		QTableWidgetItem* ptrItem = NULL;

		for(int i = 0; i < diffRow; i++){

			this->setRowHeight( oldRowNb + i, _addrHeight );
			
			ptrItem = new QTableWidgetItem();
			ptrItem->setFont(font);
			ptrItem->setFlags( Qt::ItemIsEnabled );
			ptrItem->setTextAlignment(Qt::AlignCenter);
			ptrItem->setBackground( QBrush(EMPTY_COLB));
			ptrItem->setForeground( QBrush(EMPTY_COLF));
			this->setItem(oldRowNb + i, 0, ptrItem);
		}
	}

	return;
}

void AddressBar::createBar( void ){
	
	QString label = "";
	QTableWidgetItem *ptrItem = NULL;

	//text's font
	QFont font;
	font.setFamily( CODE_FONT );
	font.setPixelSize(10);
	font.setBold(false);

	//set up address width
	this->setColumnWidth( 0, _addrWidth );	

	for( int i = 0; i < _addrVertical; i++ ){
				
		this->setRowHeight( i, _addrHeight );
		
		label = this->numToCellLabel(_addresses->at(i));	//cell label
		
		//_addresses->push_back(_addresses->at(i));			//store in address array
		
		//set up item properties
		ptrItem = new QTableWidgetItem( label );
		ptrItem->setFont(font);
		ptrItem->setFlags( Qt::ItemIsEnabled );
		ptrItem->setTextAlignment(Qt::AlignCenter);
		ptrItem->setBackground( QBrush(EMPTY_COLB));
		ptrItem->setForeground( QBrush(EMPTY_COLF));

		this->setItem( i, 0, ptrItem );
	}

	return;
}

void AddressBar::refreshBar( void ){
	
	QString label = "";
	QTableWidgetItem *ptrItem = NULL;
	
	for( int i = 0; i < _addrVertical; i++ ){
		
		label = this->numToCellLabel(_addresses->at(i));
		
		ptrItem = this->item( i, 0);
		ptrItem->setText( label );
	}

	return;
}

void AddressBar::propagateStep( void ){
	
	if( _shift == 0 ){
		return;
	}
	
	_startAddr = _startAddr + ((-1) * _shift);
	
	this->refreshAddresses();
	
	this->refreshBar();
	
	return;
}

QPoint* AddressBar::computePosition( int row ){
	return new QPoint(0, row * _addrHeight);	
}

QString AddressBar::numToCellLabel( unsigned int addr ){
	QString label = "";
	label = (label.setNum(addr,0x10)).toUpper();
	label = QString(ADDR_WIDTH - label.size(),'0') + label + ":";
	return label;
}

QString AddressBar::numToHexString( unsigned int addr ){
	
	QString label = "";
	label = (label.setNum(addr,0x10)).toUpper();
	label = QString(ADDR_WIDTH - label.size(),'0') + label;
	return "0x" + label;
}

unsigned long AddressBar::trToImgAddr( unsigned long physAddr ){

	//_ptrPE must be allocated [PE file only]
	unsigned long imgAddr = _ptrPE->OH_ImageBase() + physAddr;
	PEbrowser::Section_t sct;

	//section array check
	if( (_ptrPE->Sections()).size() == 0 ){
		return imgAddr;
	}

	//overlay - without anything
	if( physAddr >= _ptrPE->Overlay() ){
		return physAddr;
	}

	for( unsigned int i = 0; i < _ptrPE->Sections().size(); i++ ){
		
		sct = (_ptrPE->Sections())[i];
		
		if( sct.sizeOfRawData == 0 ){
			
			continue;
		}
		else if( (physAddr < sct.pointerToRawData) ){
			
			break;
		}
		else{
			
			imgAddr = 
				_ptrPE->OH_ImageBase() + sct.virtualAddress +
				(physAddr - sct.pointerToRawData);
		}
	}

	return imgAddr;
}