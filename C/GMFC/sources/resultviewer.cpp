#include "resultviewer.h"

//EXCEPTIONS
ResultViewer::ResultViewerException::ResultViewerException( void ) 
	: std::exception(){}

ResultViewer::ResultViewerException::ResultViewerException( std::string msg )
	: std::exception( msg.c_str() ){}

ResultViewer::ResultViewer(int rows, int cols, QWidget *parent, Qt::WFlags flags)
	: QTableWidget(parent){
	
	_viewers = new QHash<QString, QList<int>*>();

	_eqFlags = new QList<ResultViewer::t_eqState>();

	_xdfFlags = new QList<ResultViewer::XDFblock_t>();

	_configExists = false;
	_idxShSmplStr = "";
	
	_colNb = cols;
	_rowNb = rows;
	_cellsNum = _rowNb * _colNb;

	this->setColumnCount(_colNb);
	this->setRowCount(_rowNb);

	_succRate = 0.0;	
}

ResultViewer::~ResultViewer(){

	//deallocate items in table widget
	QTableWidgetItem* item = NULL;

	for( int i = 0; i < _rowNb; i++ ){

		for( int j = 0; j < _colNb; j++ ){

			item = this->takeItem(i, j);

			if( item != NULL ){
				delete item;
			}
		}
	}
	
	//deallocate viewer storage
	if(_viewers != NULL){
		delete _viewers;
	}

	//deallocate quality flags
	if( _eqFlags != NULL ){
		delete _eqFlags;
	}

	//deallocate xdf flags
	if( _xdfFlags != NULL ){
		delete _xdfFlags;
	}
}

/*** PROPERTIES ***/

QList<QTableWidgetItem*> ResultViewer::SelectedItems( void ){

	return this->selectedItems();
}

QList<QString> ResultViewer::TextContent( void ){

	QList<QString> texts;

	for( int i = 0 ; i < _rowNb; i++ ){

		for( int j = 0; j < _colNb; j++ ){

			texts.push_back(this->item( i, j)->text());
		}
	}

	return texts;
}

QList<ResultViewer::XDFblock_t> ResultViewer::XdfBlocks( void ){

	return *_xdfFlags;
}

int ResultViewer::Rows( void ){
	return _rowNb;
}

int ResultViewer::Columns( void ){
	return _colNb;
}

/*** METHODS ***/

void ResultViewer::addViewer( QString key, QList<int>* viewer ){
	
	//check compatibility size
	if( viewer->size() != _cellsNum ){		
		throw ResultViewer::ResultViewerException("Not Equal size of array");		
	}

	//viewer insertion
	_viewers->insert(key, viewer);

	//refresh byte content
	this->refreshViewer();

	return;
}

void ResultViewer::updateViewer( QString key, QList<int>* viewer ){

	//existence chceck
	if( !_viewers->contains(key) ){

		return;
	}
	
	//add new viewer - if viewer already exists, it wil rewrite it
	this->addViewer( key, viewer);
	
	//refresh GUI viewer
	//this->refreshViewer();

	return;
}

void ResultViewer::updateViewerSilent(QString key, QList<int>* ptrViewer){

	//existence check
	if(!_viewers->contains(key)){
		return;
	}

	//check compatibility size
	if( ptrViewer->size() != _cellsNum ){		
		throw ResultViewer::ResultViewerException("Not Equal size of array");		
	}

	//viewer insertion
	_viewers->insert(key, ptrViewer);

	return;
}

void ResultViewer::removeViewer( QString key ){

	//if viewer exists in viewers list
	if( !_viewers->contains(key) ){

		return;
	}

	//remove viewer
	_viewers->remove(key);

	//refresh viewer view
	this->refreshViewer();

	return;
}

bool ResultViewer::checkViewer( QString key ){
	
	return _viewers->contains(key);
}

void ResultViewer::configure(void){
	
	//configure class specifics
	this->configureClassSpecifics();

	//connect internal components
	this->connectComps();

	//setup base frame
	this->setupBaseFrame();

	//create viewer array
	this->createViewer();

	qDebug() << "cols: " << this->columnCount() << " rows: " << this->rowCount();

	return;
}

void ResultViewer::Output( QString msg ){

	qDebug(msg.toAscii());

	return;
}

QString ResultViewer::getSuccessRate( void ){

	if( _nemNb <= 0 ){
		return "";
	}

	float rate = _eqNb / ((float)_nemNb);
	rate = ((int)(rate *10000)) / 100.0;	//executed by QString::setNum below

	QString strRate;

	return strRate.setNum( rate, 'f', 2);
}

void ResultViewer::artificialResize( QResizeEvent *pEv ){

	//difference between old and new size in whole cells
	int diffHeight = 
			pEv->size().height() - pEv->oldSize().height();
	
	int diffCells = abs(diffHeight)/_byteHeight;

	//no entire cell
	if( diffCells == 0 ){
		return;
	}

	//recompute row number
	if( diffHeight < 0 ){
		_rowNb -= diffCells;
	}
	else{
		_rowNb += diffCells;
	}	

	//recompute total cell number
	_cellsNum = _rowNb * _colNb;

	//realloc viewer
	this->reallocViewer();

	return;
}

void ResultViewer::forceRefresh(){
	
	this->refreshViewer();
	return;
}

void ResultViewer::selectionChangedSL( QModelIndexList idxs ){

	//DISCONNECTION due to endless loop
	QObject::disconnect( this, SIGNAL( itemSelectionChanged()),
						this, SLOT( selectionChanged()));
	
	//load selected items
	QList<QTableWidgetItem*> sItems = this->selectedItems();

	//clear selection
	for( int i = 0 ; i < sItems.count(); i++ ){

		(sItems[i])->setSelected(false);
	}
	
	//zero content check
	if( idxs.count() <= 0 ){
		return;
	}

	QTableWidgetItem* item = NULL;

	//new selection
	for( int i = 0; i < idxs.count(); i++ ){

		item = this->item( idxs[i].row(), idxs[i].column() );

		if( item != NULL ){
			item->setSelected(true);
		}
	}

	//RE-CONNECTION
	QObject::connect( this, SIGNAL( itemSelectionChanged()),
						this, SLOT( selectionChanged()));

	return;
}

void ResultViewer::colSelectionSL( QColor colB, QColor colF ){

	//download selected items
	QList<QTableWidgetItem*> items = this->selectedItems();

	//index to equality array
	int idx = -1;

	//sugested colors
	QColor sColB, sColF;

	//disconnect changes
	QObject::disconnect( this, SIGNAL( itemSelectionChanged()),
						this, SLOT( selectionChanged()));

	//colorization and deselection of items
	for( int i = 0; i < items.count(); i++ ){

		//index computation
		idx = items[i]->row() * _colNb + items[i]->column();

		//xdfFlags udpate - due to clipboard read
		_xdfFlags->replace( idx, this->colToXdf(colB) );
		
		//condition for colorizing
		if( this->colPermited( _eqFlags->at(idx), colB, &sColB, &sColF) ){
			
			//colorize background
			items[i]->setBackground( QBrush(colB) );

			//colorize foreground
			items[i]->setForeground( QBrush(colF) );
		}
		else{

			//colorize background
			items[i]->setBackground( QBrush(sColB) );

			//colorize foreground
			items[i]->setForeground( QBrush(sColF) );
		}

		items[i]->setSelected(false);
	}

	//this->setRangeSelected( QTableWidgetSelectionRange(0,0,_rowNb, _colNb),
	//								false);

	/*//debug 
	QString s, s1;
	for( int i = 0; i < _rowNb * _colNb; i++ ){
		s.setNum( _xdfFlags->at(i), 10);
		s1.setNum( i );
		this->Output( s1 + " - " + s );
	}*/

	//connect again
	QObject::connect( this, SIGNAL( itemSelectionChanged()),
						this, SLOT( selectionChanged()));
	
	return;
}

void ResultViewer::slRollbackSelection( void ){

	QList<QTableWidgetItem*> items = this->selectedItems();

	//disconnect changes
	QObject::disconnect( this, SIGNAL( itemSelectionChanged()),
						this, SLOT( selectionChanged()));

	int idx = 0;
	
	//rollback
	for( int i = 0; i < items.count(); i++ ){

		//compute index into eqFlags
		idx = (items[i]->row() * _colNb) + items[i]->column();

		//set up previous state
		switch( _eqFlags->at(idx) ){

			case ResultViewer::EMPTY:
				items[i]->setBackground( QBrush( EMPTY_COLB ) );
				items[i]->setForeground( QBrush( EMPTY_COLF ) );
				break;

			case ResultViewer::EQ:
				items[i]->setBackground( QBrush( EQ_COLB ) );
				items[i]->setForeground( QBrush( EQ_COLF ) );
				break;

			case ResultViewer::NEQ:
				items[i]->setBackground( QBrush( NEQ_COLB ) );
				items[i]->setForeground( QBrush( NEQ_COLF ) );
				break;
		}

		//deselect items
		items[i]->setSelected(false);

		//clean previous xdfState and set up to NS
		_xdfFlags->replace( idx, ResultViewer::NS );
	}

	//connect again
	QObject::connect( this, SIGNAL( itemSelectionChanged()),
						this, SLOT( selectionChanged()));

	return;
}

void ResultViewer::slRollbackAll( void ){

	QTableWidgetItem* item = NULL;
	
	//disconnect changes
	//QObject::disconnect( this, SIGNAL( itemSelectionChanged()),
	//					this, SLOT( selectionChanged()));

	int idx = 0;
	
	//rollback
	for( int i = 0; i < _colNb * _rowNb; i++ ){

		//compute index into eqFlags
		idx = i;

		//select item from QTableWidget
		item = this->item( i / _colNb, i % _colNb );

		//set up previous state
		switch( _eqFlags->at(idx) ){

			case ResultViewer::EMPTY:
				item->setBackground( QBrush( EMPTY_COLB ) );
				item->setForeground( QBrush( EMPTY_COLF ) );
				break;

			case ResultViewer::EQ:
				item->setBackground( QBrush( EQ_COLB ) );
				item->setForeground( QBrush( EQ_COLF ) );
				break;

			case ResultViewer::NEQ:
				item->setBackground( QBrush( NEQ_COLB ) );
				item->setForeground( QBrush( NEQ_COLF ) );
				break;
		}

		//clean previous xdfState and set up to NS
		_xdfFlags->replace( idx, ResultViewer::NS );
	}

	//connect again
	//QObject::connect( this, SIGNAL( itemSelectionChanged()),
	//					this, SLOT( selectionChanged()));
	
	return;
}

void ResultViewer::selectionChanged( void ){

	//download model
	QItemSelectionModel* model = this->selectionModel();

	//download selected indexes
	QModelIndexList idxs = model->selectedIndexes();

	/*int minIdx = -1;
	int maxIdx = -1;

	if( idxs.count() == 1 ){

		_frsIdx = ((model->currentIndex()).row() * this->columnCount()) + 
						(model->currentIndex()).column();
		_lstIdx = _frsIdx;

		this->Output("frst");
	}
	else{
		
		_lstIdx = ((model->currentIndex()).row() * this->columnCount()) + 
						(model->currentIndex()).column();

		this->Output("last");
	}

	//determines border values
	if( _frsIdx < _lstIdx ){
		
		minIdx = _frsIdx;
		maxIdx = _lstIdx;
	}
	else{

		minIdx = _lstIdx;
		maxIdx = _frsIdx;
	}

	QString s1, s2;
	s1.setNum(minIdx);
	s2.setNum(maxIdx);
	this->Output( s1 + " -> " + s2 );
	

	//DISCONNECTION due to endless loop
	QObject::disconnect( this, SIGNAL( itemSelectionChanged()),
					this, SLOT( selectionChanged()));

	QTableWidgetSelectionRange range(0,0,
									this->rowCount(),
									this->columnCount());
	this->setRangeSelected(range, false);

	//additional selection
	for( int i = minIdx; i <= maxIdx; i++ ){
		
		this->item( 
			i / this->columnCount(), 
			i % this->columnCount())->setSelected(true);		
	}	
		
	model = this->selectionModel();

	idxs = model->selectedIndexes();

	//RE-CONNECTION
	QObject::connect( this, SIGNAL( itemSelectionChanged()),
					this, SLOT( selectionChanged()));*/

	//emit proper signal for selection refresh
	emit this->selectionChangedSG( idxs );

	return;
}

void ResultViewer::configureClassSpecifics( void ){

	//config non-essential
	if(!_configExists)
	{
		//config essential
		_byteWidth = 18;
		_byteHeight = CELL_H;
		_appWidth = _colNb * _byteWidth;
		_appHeight = _rowNb * _byteHeight;
	}

	this->confTWidget();

	return;
}

void ResultViewer::confTWidget(void){

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

	//table widget settings
	//this->resize( _appWidth + 2, _appHeight + 2);
	this->setFrameStyle(QFrame::NoFrame);
	this->resize( _appWidth, _appHeight);

	return;

}

void ResultViewer::setupBaseFrame(void){
	
	//this->resize(this->width(), this->height());

	return;
}

void ResultViewer::createViewer(void){
	
	QTableWidgetItem* item = NULL;
	unsigned char byte = '\0';
	bool empty = true;
	
	QFont font;
	font.setFamily( CODE_FONT );
	font.setPixelSize(10);
	font.setBold(false);
	
	for( int i = 0; i < _rowNb; i++ ){

		this->setRowHeight( i, _byteHeight);		

		for( int j = 0; j < _colNb; j++ ){

			this->setColumnWidth( j, _byteWidth );
			
			item = new QTableWidgetItem();
			item->setFont(font);			
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);									
			item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);

			if( _viewers->count() <= 0 ){
				
				//set up cell's background color
				item->setBackground( QBrush( EMPTY_COLB));
				
				//set up cell's foreground color
				item->setForeground( QBrush( EMPTY_COLF));
				
				//set up equality flag into storage
				_eqFlags->push_back(ResultViewer::EMPTY);
			}
			//is equal
			else if( isEq((i * _colNb + j), &byte, &empty) ){

				//empty
				if( empty ){
					
					//set up cell's background color
					item->setBackground( QBrush( EMPTY_COLB));

					//set up cell's foreground color
					item->setForeground( QBrush( EMPTY_COLF));

					//set up equality flag into storage
					_eqFlags->push_back(ResultViewer::EMPTY);
				}
				else{
					
					//set up cell's background color 
					item->setBackground( QBrush( EQ_COLB));

					//set up cell's foreground color
					item->setForeground( QBrush( EQ_COLF));

					//set up cell's text label
					item->setText( this->charToCellLabel( byte));					

					//set up equality flag into storage
					_eqFlags->push_back( ResultViewer::EQ );
				}			
			}
			//is not equal
			else{
				
				//set up cell's background color
				item->setBackground( QBrush( NEQ_COLB));

				//set up cell's foreground color
				item->setForeground( QBrush( NEQ_COLF));

				//set up cell's text label
				item->setText( this->charToCellLabel(byte) );

				//set up equality  flag into storage
				_eqFlags->push_back(ResultViewer::NEQ);
			}

			//place QTableWidgetItem
			this->setItem( i, j, item);

			//place NS xdfFlag
			_xdfFlags->push_back(ResultViewer::NS);
		}
	}

	return;
}

void ResultViewer::refreshViewer( void ){

	QTableWidgetItem* item = NULL;
	unsigned char byte = '\0';
	QString hexNb = "";
	bool empty = true;
	int idx = 0;
	QString curText = "", newLabel = "";

	//success rate reset values
	_eqNb = 0; _neqNb = 0; _nemNb = 0;

	for( int i = 0; i < _rowNb; i++ ){

		for( int j = 0; j < _colNb; j++ ){

			//table item download
			//item = this->takeItem( i, j );
			item = this->item( i, j );

			//index computation
			idx = (i * _colNb) + j;

			//take current label text
			curText = item->text();
			
			if( _viewers->count() <= 0 ){
				
				//set up cell's background color
				item->setBackground( QBrush(EMPTY_COLB));

				//set up cell's foreground color
				item->setForeground( QBrush( EMPTY_COLF));

				if(curText != ""){				
					//emty cell = no text
					item->setText("");
				}

				//set up equality 
				_eqFlags->replace( idx, ResultViewer::EMPTY );							
			}
			//is equal
			else if( isEq( (i * _colNb) + j, &byte, &empty) ){

				if( empty ){

					item->setBackground( QBrush(EMPTY_COLB));

					//set up cell's foreground color
					item->setForeground( QBrush( EMPTY_COLF));

					if(curText != ""){
						//empty cell = no text
						item->setText("");
					}

					//set up equality 
					_eqFlags->replace( idx, ResultViewer::EMPTY);
				}
				else{
					
					item->setBackground( QBrush(EQ_COLB));

					//set up cell's foreground color
					item->setForeground( QBrush( EMPTY_COLF));

					newLabel = this->charToCellLabel( byte);

					if(curText != newLabel){
						item->setText(newLabel);
					}

					//set up equality 
					_eqFlags->replace( idx, ResultViewer::EQ );

					//success rate
					_eqNb++; _nemNb++;
				}
			}
			//is not equal
			else{
				
				item->setBackground( QBrush( NEQ_COLB));

				//set up cell's foreground color
				item->setForeground( QBrush( EMPTY_COLF));
				
				newLabel = this->charToCellLabel( byte);

				if(curText != newLabel){
					item->setText(newLabel);
				}

				//set up equality 
				_eqFlags->replace( idx, ResultViewer::NEQ );

				//success rate
				_neqNb++; _nemNb++;
			}

			//place NS xdfState - entire array is set to default state
			_xdfFlags->replace( idx, ResultViewer::NS );			
		}
	}

	return;
}

void ResultViewer::reallocViewer( void ){

	int oldCellsNum = this->rowCount() * this->columnCount();
	int diff = _cellsNum - oldCellsNum;
	
	//application size changes
	_appWidth = _colNb * _byteWidth;
	_appHeight = _rowNb * _byteHeight;		

	this->setRowCount(_rowNb);
	this->setColumnCount(_colNb);

	//no diff exists
	if(diff == 0){
		return;
	}
		
	//deallocation needed
	if( diff < 0 ){

		QTableWidgetItem *pItem = NULL;

		//deallocates items in QTableWidget
		for( int i = _cellsNum; i < oldCellsNum; i++ ){

			//QTableWidget delete 
			pItem = this->takeItem( i / _colNb, i %_colNb);
			delete pItem;
		}

		//deallocates items in other featerues array
		for( int i = oldCellsNum - 1; i >= _cellsNum; i-- ){

			_eqFlags->removeAt(i);
			_xdfFlags->removeAt(i);
		}
	}
	//allocation needed - more cells needed
	else if( diff > 0 ){

		//font settings
		QFont font;
		font.setFamily( CODE_FONT );
		font.setPixelSize(10);
		font.setBold(false);

		QTableWidgetItem *pItem = NULL;

		for( int i = oldCellsNum; i < _cellsNum; i++ ){

			//setup row height
			if( i % _colNb == 0 ){
				this->setRowHeight( i / _colNb, _byteHeight);				
			}

			//create new table widget item and setup its properties
			pItem = new QTableWidgetItem();
			pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);									
			pItem->setFont(font);
			pItem->setTextAlignment(Qt::AlignCenter);			
			
			//set up cell's background color
			pItem->setBackground( QBrush( EMPTY_COLB));
				
			//set up cell's foreground color
			pItem->setForeground( QBrush( EMPTY_COLF));
			
			//put into QtableWidget
			this->setItem( i / _colNb, i % _colNb, pItem);

			//create new item in equal array
			_eqFlags->push_back(ResultViewer::EMPTY);

			//create new item in xdf array
			_xdfFlags->push_back(ResultViewer::NS);
		}		
	}
	
	//objects resizing
	this->resize( _appWidth, _appHeight);

	return;
}

bool ResultViewer::isEq(int idx, unsigned char* byte, bool *empty ){

	QList<QString> keys = _viewers->keys();
	int tVal = 0;
	int idxShSmpl = 0;

	//translate string "shown sample" to array index
	if( keys.contains(_idxShSmplStr) ){
		
		idxShSmpl = keys.indexOf( _idxShSmplStr );
	}
	else{

		idxShSmpl = 0;
		_idxShSmplStr = keys[0];
	}
	
	//cycle through all samples
	for(int i = 0 ; i < keys.count(); i++){

		if( i == 0 ){
			
			tVal = (_viewers->value(keys[i]))->at(idx);
		}
		else{
			
			//if samples are different
			if( (_viewers->value(keys[i]))->at(idx) != tVal){				

				//current cell value is suitable for show [not empty]
				if( (_viewers->value(keys[idxShSmpl]))->at(idx) != -1 ){

					//use value from previosly shown file
					tVal = (_viewers->value(keys[idxShSmpl]))->at(idx);
					*byte = (unsigned char)tVal;
				}
				else{
				
					//find non empty byte when bytes are not equal
					for( int j = 0; j < keys.count(); j++ ){
					
						//if examined cell is different from empty space
						if( (_viewers->value(keys[j]))->at(idx) != -1 ){
						
							idxShSmpl = j;
							_idxShSmplStr = keys[j];
							
							tVal = (_viewers->value(keys[j]))->at(idx);
							*byte = (unsigned char)tVal;

							//break the cycle - first unempty was found
							break;
						}					
					}
				}
				
				return false;
			}
			
			tVal = (_viewers->value(keys[i]))->at(idx);
		}
	}

	//store equal byte
	if( (tVal == -1) ){
		
		*empty = true;
	}
	else{
		
		*empty = false;
		*byte = (unsigned char)tVal;
	}

	return true;
}

bool ResultViewer::colPermited( ResultViewer::t_eqState cState,
								QColor col,
								QColor *colB,
								QColor *colF ){

	switch( cState ){

		case ResultViewer::EQ:

			*colB = EQ_COLB;
			*colF = EQ_COLF;

			return true;

		case ResultViewer::NEQ:
			
			*colB = NEQ_COLB;
			*colF = NEQ_COLF;
			
			//check code
			if( col == CC_COLB ){
				return false;
			}
			//skip 
			else if( col == SK_COLB ){
				return true;
			}
			//seq
			else{
				return true;
			}
			break;			

		case ResultViewer::EMPTY:
		default:

			*colB = EMPTY_COLB;
			*colF = EMPTY_COLF;

			return false;			
	}
}
	
QString ResultViewer::charToCellLabel(unsigned char byte ){
	
	//translation char to hexa number
	QString hexaNum = "";
	bool append = false;
	
	if( byte < 0x10 ){
		append = true;
	}

	hexaNum = (hexaNum.setNum(byte,0x10)).toUpper();
	
	if(append){
		hexaNum = "0" + hexaNum;
	}

	return hexaNum;
}

void ResultViewer::connectComps( void ){

	//tWidget selection -> this object
	QObject::connect( this, SIGNAL( itemSelectionChanged()),
						this, SLOT( selectionChanged()));
	
	return;
}

bool ResultViewer::sortFunction( QModelIndex i1, QModelIndex i2 ){
	
	if( i1.row() == i2.row() ){

		return i1.column() < i2.column();
	}
	else{

		return i1.row() < i2.row();
	}

}

ResultViewer::XDFblock_t ResultViewer::colToXdf( QColor col ){

	//SEQ
	if( col == SQ_COLB ){
		return ResultViewer::SEQ;
	}
	//CHECK CODE
	else if( col == CC_COLB ){
		return ResultViewer::CC;
	}
	//SKIP
	else if( col == SK_COLB ){
		return ResultViewer::SK;
	}
	//NS
	else{
		return ResultViewer::NS;
	}
}

void ResultViewer::rollback( QList<QTableWidgetItem*> items ){

	

	return;
}

void ResultViewer::resizeEvent( QResizeEvent *pEv ){

	if( pEv->spontaneous() ){
		
		pEv->accept();
		this->artificialResize(pEv);
	}
	else{

		pEv->ignore();
		return;
	}	

	return;
}