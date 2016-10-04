#include "resultwidget.h"

ResultWidget::ResultWidget(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags){

	//setup floating buffers
	_floatResBufferH = 0;
	_floatResBufferW = 0;

	//base frame
	_base = new QFrame(this);

	//result viewer object
	_viewer = new ResultViewer(INIT_ROWS, INIT_COLS, this);
	_viewer->configure();

	//result text viewer object
	_tViewer = new ResultTextViewer(INIT_ROWS, INIT_COLS, this);
	_tViewer->configure();

	//md5 list allocation
	_ptrMD5s = new QHash<QString,QString>();

	//toolbar object
	_tBar = new ResultToolbar( _viewer->width() + _tViewer->width() + 5,
								TOOL_HEIGHT, this);
	_tBar->configure( _tBarOpt );

	//resize widget
	this->resize( _viewer->width() +  _tViewer->width() + 15,
						_viewer->height() + _tBar->height() + 15);
	this->setupBaseFrame();
}

ResultWidget::~ResultWidget(){

	if(_viewer != NULL){
		delete _viewer;
	}
}

void ResultWidget::addToCompare( QString key, QList<int>* vCont, QString md5 ){

	//add new viewer to compore
	_viewer->addViewer( key, vCont );

	//add new viewer to compare
	_tViewer->addViewer( key, vCont );

	//add md5 sample's hash 
	_ptrMD5s->insert( key, md5 );

	//compute & emit percentage succes rate
	emit this->sgSuccRateUpdate(_viewer->getSuccessRate());

	return;
}

void ResultWidget::updateCompare( QString fName, QList<int>* dataContent ){

	//update content in hexa viewer
	_viewer->updateViewer( fName, dataContent );

	//update content in text viewer
	_tViewer->updateViewer( fName, dataContent );

	//compute & emit percentage succes rate
	emit this->sgSuccRateUpdate(_viewer->getSuccessRate());
	
	return;
}

void ResultWidget::updateCompareSilent(QString fName, QList<int>* ptrDataContent){

	_viewer->updateViewerSilent(fName, ptrDataContent);
	_tViewer->updateViewerSilent(fName, ptrDataContent);

	return;
}

void ResultWidget::slRemoveCompare( QString fName ){

	//remove sample from hexa viewer
	_viewer->removeViewer( fName );

	//remove sample from text viewer
	_tViewer->removeViewer( fName );

	//remove hash from compared list
	if( _ptrMD5s->contains(fName) ){
		_ptrMD5s->remove( fName);
	}

	//compute & emit percentage succes rate
	emit this->sgSuccRateUpdate(_viewer->getSuccessRate());
	
	return;
}

void ResultWidget::slFinalResizeRefresh(){

	_viewer->forceRefresh();
	_tViewer->forceRefresh();

	emit this->sgSuccRateUpdate(_viewer->getSuccessRate());

	return;
}

void ResultWidget::configure( void ){

	//place component to proper position
	this->placeComponents();

	//all integrated component's signal and slots are connected by this method
	this->connectComponents();

	return;
}

void ResultWidget::artificialResize(QResizeEvent *pEv){

	//difference in height
	int diffH = pEv->oldSize().height() - pEv->size().height();

	//zero difference
	if(diffH == 0){
		return;
	}

	//rounded difference -> will be put into child components [cell row]
	int roundedDiffH = (diffH + _floatResBufferH) / CELL_H;

	//remaining from resize action and previous errors [in pixels]
	int tmpFloatBufferH = (diffH + _floatResBufferH) % CELL_H;

	_floatResBufferH = tmpFloatBufferH;

	//no diff is possible
	if(roundedDiffH == 0){
		return;
	}

	//create new resize event - width is not important, height has to be recompute
	QSize roundedSize(pEv->size().width(), 
						(pEv->oldSize().height() - (roundedDiffH * CELL_H)));
	QResizeEvent internalEv(roundedSize, pEv->oldSize());

	//resize other integrated components
	_viewer->artificialResize(&internalEv);
	_tViewer->artificialResize(&internalEv);

	//resize widget to proper level
	_base->resize(_base->width(), _viewer->height() + 15 + _tBar->height());
	this->resize(_base->width(), _base->height());

	//emit success rate update
	emit this->sgSuccRateUpdate(_viewer->getSuccessRate());
		
	return;
}

void ResultWidget::setupBaseFrame( void ){

	//resize base frame
	_base->resize( this->width(), this->height() );

	//setup style for ResultWidget
	_base->setStyleSheet( RWIDGET_STYLE );

	return;
}

void ResultWidget::placeComponents( void ){
	
	//toolbar positioning
	_tBar->move(5, 5);

	//viewer positioning
	_viewer->move( 5, _tBar->height() + 10);

	//text viewer positioning
	_tViewer->move( _viewer->width() + 10, _tBar->height() + 10);
	
	return;
}

void ResultWidget::slTBarActivated( QAction* act ){

	QHash<QAction*, Toolbar::ACTIONS>* actTab = _tBar->Actions();
	Toolbar::ACTIONS curAct = actTab->value(act);
	
	//toolbar actions processing
	switch( curAct ){

		//clipboard processing
		case Toolbar::RES_CLIP:
			
			this->procResToClip();
			break;
		
		//xdf clipboard processing - light version
		case Toolbar::RES_CLIP_LIGHT:

			this->procResToClip( true );
			break;

		//check code block
		case Toolbar::RES_CHECK:
			
			emit this->sgColSelection( CC_COLB, CC_COLF );
			break;

		//skip block
		case Toolbar::RES_SKIP:
			
			emit this->sgColSelection( SK_COLB, SK_COLF );
			break;

		//sequence block
		case Toolbar::RES_SEQ:
			
			emit this->sgColSelection( SQ_COLB, SQ_COLF );
			break;

		//rollback selection
		case Toolbar::RES_DEFAULT:
			
			emit this->sgRollbackSelection();
			break;

		//rollback all
		case Toolbar::RES_DEFAULT_ALL:
			
			emit this->sgRollbackAll();
			break;

		//xdef to file
		case Toolbar::XDEF_FILE:		
			
			this->saveXdf();
			break;

		//xdef to file - light version
		case Toolbar::XDEF_FILE_LIGHT:

			this->saveXdf(true);
			break;

		default:
			break;
	}

	return;
}

void ResultWidget::connectComponents( void ){

	//toolbar -> resultWidget
	QObject::connect( _tBar, SIGNAL(actionTriggered( QAction* )),
						this, SLOT(slTBarActivated( QAction*)));

	//viewer -> text viewer
	QObject::connect( _viewer, SIGNAL(selectionChangedSG( QModelIndexList )),
						_tViewer, SLOT(selectionChangedSL( QModelIndexList )));

	//text viewer -> viewer
	QObject::connect( _tViewer, SIGNAL(selectionChangedSG( QModelIndexList )),
						_viewer, SLOT(selectionChangedSL( QModelIndexList )));

	//result widget -> table widget
	QObject::connect( this, SIGNAL( sgColSelection(QColor, QColor)),
						_tViewer, SLOT(colSelectionSL(QColor, QColor)));
	
	//result widget -> text widget
	QObject::connect( this, SIGNAL( sgColSelection(QColor, QColor)),
						_viewer, SLOT(colSelectionSL(QColor, QColor)));

	//result widget -> table widget
	QObject::connect( this, SIGNAL( sgRollbackSelection()),
						_tViewer, SLOT( slRollbackSelection()));

	//result widget -> text widget
	QObject::connect( this, SIGNAL( sgRollbackSelection()),
						_viewer, SLOT( slRollbackSelection()));

	//result widget -> table widget
	QObject::connect( this, SIGNAL( sgRollbackAll()),
						_tViewer, SLOT( slRollbackAll()));

	//result widget -> text widget
	QObject::connect( this, SIGNAL( sgRollbackAll()),
						_viewer, SLOT( slRollbackAll()));

	return;
}

void ResultWidget::procResToClip( bool lightVer ){

	QList<QString> tCont = _tViewer->TextContent();
	QList<QString> hCont = _viewer->TextContent();
	QList<ResultViewer::XDFblock_t> xdfs = _tViewer->XdfBlocks();
	QString def = "";

	//if light xdf version is requested
	if( !lightVer ){
		
		def = this->asmXdf( &tCont, &hCont, &xdfs);
	}
	else{

		def = this->asmXdfLight( &hCont, &xdfs );
	}

	//load clipboard object
	QClipboard* clip = QApplication::clipboard();

	if( clip != NULL){
		
		//set up new clipboard content
		clip->setText( def );
	}

	return;
}

QString ResultWidget::asmXdf( QList<QString> *tCont,
						QList<QString> *hCont,
						QList<ResultViewer::XDFblock_t> *xdfs ){
	
	int startIdx = 0, stopIdx = _tViewer->Columns() * _tViewer->Rows();
	bool rdStarted = false, frsBlock = false;
	
	//analyze gaps - setup xdfInterval
	for( int i = 0; i < xdfs->count(); i++ ){

		//first block - start index
		if( (xdfs->at(i) != ResultViewer::NS) &&
			(!frsBlock)){
			
			startIdx = i;
			frsBlock = true;
			rdStarted = true;
		}
		//end of non NS block
		else if( rdStarted &&
				(xdfs->at(i) == ResultViewer::NS)){
			
			stopIdx = i;
			rdStarted = false;
		}
		//start of another block - after gap
		else if( !rdStarted &&
				(xdfs->at(i) != ResultViewer::NS)){

			rdStarted = true;
		}
	}

	QString def = "", hTmp = "", tTmp = "", addr = "";
	ResultViewer::XDFblock_t curBlock = ResultViewer::NS;

	def += XDF_HEAD;

	//append md5 list
	def += XDF_MD5;

	//assembly md5 list
	QList<QString> keys = _ptrMD5s->keys();
	for( int i = 0; i < keys.size(); i++ ){

		def += ";" + _ptrMD5s->value( keys[i] ) + "\n";		
	}

	def += "\n";

	def += XDF_BEGIN;

	//check if any selectioin exists
	if( xdfs->contains(ResultViewer::CC) || 
		xdfs->contains(ResultViewer::SEQ) ||
		xdfs->contains(ResultViewer::SK)){

		//assembly xdf data
		for( int i = startIdx; i < stopIdx; i++ ){

			//change section
			if( curBlock != xdfs->at(i) ){
			
				this->publishRow(def, addr,i - startIdx, hTmp, tTmp );
			
				curBlock = xdfs->at(i);
			
				switch( curBlock ){

					case ResultViewer::CC:
						def += CC_TEXT + "\n";
						break;

					case ResultViewer::SEQ:
						def += SQ_TEXT + "\n";
						break;

					case ResultViewer::SK:
					case ResultViewer::NS:
					default:
						def += SK_TEXT + "\n";
						break;

				}
			}
			//start new line 
			else if( ( hTmp.size() % (_tViewer->Columns() * 2)) == 0 ){
			
				this->publishRow( def, addr, i - startIdx, hTmp, tTmp );
			}

			hTmp += hCont->at(i);
			tTmp += tCont->at(i);
		}
	
		//data into row - supplement with alignment
		if( hTmp.size() > 0 ){
				
			while( hTmp.size() <  (_tViewer->Columns() * 2) ){			
				hTmp.append(' ');
			}

			def += addr + " " + hTmp + "  " + tTmp + "'\n";
		}
	}

	def += XDF_END;
	
	return def;
}

QString ResultWidget::asmXdfLight( QList<QString> *hCont,
						QList<ResultViewer::XDFblock_t> *xdfs){
	
	QString def = "";
	int startIdx = 0, stopIdx = _tViewer->Columns() * _tViewer->Rows();
	bool rdStarted = false, frsBlock = false;
	
	//analyze gaps - setup xdfInterval
	for( int i = 0; i < xdfs->count(); i++ ){

		//first block - start index
		if( (xdfs->at(i) != ResultViewer::NS) &&
			(!frsBlock)){
			
			startIdx = i;
			frsBlock = true;
			rdStarted = true;
		}
		//end of non NS block
		else if( rdStarted &&
				(xdfs->at(i) == ResultViewer::NS)){
			
			stopIdx = i;
			rdStarted = false;
		}
		//start of another block - after gap
		else if( !rdStarted &&
				(xdfs->at(i) != ResultViewer::NS)){

			rdStarted = true;
		}
	}

	//check if any selectioin exists
	if( xdfs->contains(ResultViewer::CC) || 
		xdfs->contains(ResultViewer::SEQ) ||
		xdfs->contains(ResultViewer::SK)){

		int rowCnt = 0;

		//assembly xdf data
		for( int i = startIdx; i < stopIdx; i++, rowCnt++ ){

			//start new line 
			if( (( rowCnt % (_tViewer->Columns())) == 0) &&
				(rowCnt > 0)){
			
				def += "\n";
			}
			
			switch( xdfs->at(i) ){

				case ResultViewer::CC:						
				case ResultViewer::SEQ:
					
					def += hCont->at(i);
					break;

				case ResultViewer::SK:
				case ResultViewer::NS:
				default:

					def += "??";
					break;
			}
		}	
	}

	return def;
}

QString ResultWidget::trAddrToStr( int addr ){

	QString str = "";
	str.setNum( addr, 16);

	while( str.size() < XDF_ADDR_LEN ){
		
		str.insert( 0, "0" );
	}

	return str;

}

void ResultWidget::publishRow( QString &def,
								QString &addr,
								int idx,
								QString &hTmp,
								QString &tTmp ){

	//data into row
	if( hTmp.size() > 0 ){
				
		while( hTmp.size() <  (_tViewer->Columns() * 2) ){
			hTmp.append(' ');
		}
		
		def += addr + " " + hTmp + "  " + tTmp + "'\n";
	}

	//new address
	addr = this->trAddrToStr( idx );

	hTmp = "";

	tTmp = "'";

	return;
}

void ResultWidget::saveXdf( bool ligthVer ){

	QString fName = "", xdf = "";
	
	if(  false // configExists 
		// && prefere save path is exists
	){}
	else{

		this->procDialogSaveXdf(fName);
	}

	//fName input check
	if( fName.length() <= 0 ){		
		return;
	}

	//if full xdf version is requested
	if( !ligthVer ){

		//assembly xdef
		xdf = this->asmXdf( &_tViewer->TextContent(), 
							&_viewer->TextContent(),
							&_viewer->XdfBlocks());
	}
	else{
		
		//assembly xdef ligh version
		xdf = this->asmXdfLight( &_viewer->TextContent(),
							&_viewer->XdfBlocks());
	}

	//store xDEF to file
	QFile file( fName );
	
	if( !file.open( QIODevice::WriteOnly )){

		Error::alert( tr("Failed to write"), file.errorString());		
	}
	else{

		file.write( (xdf.toStdString()).c_str() );
		file.close();
	}

	return;
}

void ResultWidget::procDialogSaveXdf( QString &fName ){

	QFileDialog *pDialog = new QFileDialog( this, tr("Save xDEF file"));

	//setup dialog attributes
	pDialog->setModal( true );	
	pDialog->setAcceptMode( QFileDialog::AcceptSave );
	pDialog->setNameFilter("xDEF file (*.xdf)");
	pDialog->selectFile("result");

	//execute dialog
	if(pDialog->exec() == QDialog::Accepted){

		if( (pDialog->selectedFiles()).count() > 0){

			fName = (pDialog->selectedFiles())[0];
		}
	}

	delete pDialog;	
	
	return;
}

void ResultWidget::resizeEvent( QResizeEvent *pEv){

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