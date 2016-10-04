#include "resultwidget_v3.h"

ResultWidget::ResultWidget(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags){

	_v3_rows = INIT_ROWS;
	_v3_cols = INIT_COLS;
	_v3_cellsNum = _v3_rows*_v3_cols;

	_v3_successRate = 0.0;
	_v3_equalCells = 0;
	_v3_notEqualCells = 0;
	_v3_notEmptyCells = 0;

	_v3_hexaLabelForm = "";
	_v3_textLabelForm = "";

	for(int i = 0; i < _v3_cellsNum; i++){
		_v3_hexaDataForm.push_back("");
		_v3_textDataForm.push_back("");
	}

	_referenceSampleKey = "";

	try{

		_v3_ptrXdfs = new QList<ResultWidget::xdfBlock_t>();
		_v3_ptrEquals = new QList<ResultWidget::equalState_t>();
		_v3_ptrSampleStorage = new QHash<QString, QList<int>*>();

		//base frame
		_base = new QFrame(this);

		//hexa viewer
		_v3_ptrHexaViewer = new HexaViewerSelectable(
			&_v3_hexaLabelForm,
			&_v3_hexaDataForm,
			"",
			this);
		_v3_ptrHexaViewer->configure();

		//text viewer
		_v3_ptrTextViewer = new TextViewerSelectable(
			&_v3_textLabelForm,
			&_v3_textDataForm,
			"",
			this);
		_v3_ptrTextViewer->configure();

		//md5 list allocation
		_ptrMD5s = new QHash<QString,QString>();

		//toolbar object
		_tBar = new ResultToolbar(
			_v3_ptrHexaViewer->width() + _v3_ptrTextViewer->width() + 5,
			TOOL_HEIGHT,
			this);
		_tBar->configure(_tBarOpt);

		//resize widget
		this->resize( 
			_v3_ptrHexaViewer->width() + _v3_ptrTextViewer->width() + 15,
			_v3_ptrHexaViewer->height() + _tBar->height() + 15);
		this->setupBaseFrame();
	}
	catch(std::bad_alloc){

		QMessageBox msgBox;
		msgBox.setWindowIcon(QIcon::fromTheme("Error Icon",
			QIcon(":/Global/Resources/error.svg")));
		msgBox.setWindowTitle(tr("Fatal Error"));
		msgBox.setText(tr("Application can start due to memory allocation error."));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.exec();
	}
}

ResultWidget::~ResultWidget(){

	if(_v3_ptrHexaViewer != NULL){
		delete _v3_ptrHexaViewer;
	}

	if(_v3_ptrTextViewer != NULL){
		delete _v3_ptrTextViewer;
	}

	if(_base != NULL){
		delete _base;
	}

	if(_ptrMD5s != NULL){
		delete _ptrMD5s;
	}

	if(_tBar != NULL){
		delete _tBar;
	}
}

void ResultWidget::slAddToCompare(QString key, QList<int>* ptrDataContent, QString md5){

	_v3_ptrSampleStorage->insert(key, ptrDataContent);
	_ptrMD5s->insert(key, md5);

	//erase colorization
	if(_v3_ptrXdfs->contains(ResultWidget::CC) || 
		_v3_ptrXdfs->contains(ResultWidget::SEQ) ||
		_v3_ptrXdfs->contains(ResultWidget::SK)){
			this->v3_eraseXdfSelection();
	}

	//recreate models
	this->v3_createModels();
	
	//update hexa viewer
	_v3_ptrHexaViewer->clearSelection();
	_v3_ptrHexaViewer->updateView(&_v3_hexaLabelForm, &_v3_hexaDataForm);

	//update text viewer
	_v3_ptrTextViewer->clearSelection();	
	_v3_ptrTextViewer->updateView(&_v3_textLabelForm, &_v3_textDataForm);

	//compute & emit percentage succes rate
	emit this->sgSuccRateUpdate(this->v3_successRate());

	return;
}

void ResultWidget::updateCompare(QString key, QList<int>* ptrDataContent){

	_v3_ptrSampleStorage->insert(key, ptrDataContent);

	//erase colorization
	if(_v3_ptrXdfs->contains(ResultWidget::CC) || 
		_v3_ptrXdfs->contains(ResultWidget::SEQ) ||
		_v3_ptrXdfs->contains(ResultWidget::SK)){
			this->v3_eraseXdfSelection();
	}
	
	//recreate models
	this->v3_createModels();
	
	//update hexa viewer
	_v3_ptrHexaViewer->clearSelection();
	_v3_ptrHexaViewer->updateView(&_v3_hexaLabelForm, &_v3_hexaDataForm);

	//update text viewer
	_v3_ptrTextViewer->clearSelection();	
	_v3_ptrTextViewer->updateView(&_v3_textLabelForm, &_v3_textDataForm);

	//compute & emit percentage succes rate
	emit this->sgSuccRateUpdate(this->v3_successRate());
	
	return;
}

void ResultWidget::updateCompareSilent(QString key, QList<int>* ptrDataContent){
	
	_v3_ptrSampleStorage->insert(key, ptrDataContent);
	return;
}

void ResultWidget::slRemoveCompare(QString key, bool silent){

	if(_v3_ptrSampleStorage->contains(key)){
		_v3_ptrSampleStorage->remove(key);
	}
	
	if(_ptrMD5s->contains(key)){
		_ptrMD5s->remove(key);
	}

	//silent version - do not refresh
	if(silent){
		return;
	}

	//erase colorization
	if(_v3_ptrXdfs->contains(ResultWidget::CC) || 
		_v3_ptrXdfs->contains(ResultWidget::SEQ) ||
		_v3_ptrXdfs->contains(ResultWidget::SK)){
			this->v3_eraseXdfSelection();
	}

	//recreate models
	this->v3_createModels();
	
	//update hexa viewer
	_v3_ptrHexaViewer->clearSelection();
	_v3_ptrHexaViewer->updateView(&_v3_hexaLabelForm, &_v3_hexaDataForm);

	//update text viewer
	_v3_ptrTextViewer->clearSelection();	
	_v3_ptrTextViewer->updateView(&_v3_textLabelForm, &_v3_textDataForm);

	//compute & emit percentage succes rate
	emit this->sgSuccRateUpdate(this->v3_successRate());
	
	return;
}

void ResultWidget::slFinalResizeRefresh(){

	//erase colorization
	if(_v3_ptrXdfs->contains(ResultWidget::CC) || 
		_v3_ptrXdfs->contains(ResultWidget::SEQ) ||
		_v3_ptrXdfs->contains(ResultWidget::SK)){
			this->v3_eraseXdfSelection();
	}

	//recreate models
	this->v3_createModels();
	
	//update hexa viewer
	_v3_ptrHexaViewer->clearSelection();
	_v3_ptrHexaViewer->artificialResize(&_v3_hexaLabelForm, &_v3_hexaDataForm);

	//update text viewer
	_v3_ptrTextViewer->clearSelection();	
	_v3_ptrTextViewer->artificialResize(&_v3_textLabelForm, &_v3_textDataForm);

	this->resize( 
		_v3_ptrHexaViewer->width() + _v3_ptrTextViewer->width() + 15,
		_v3_ptrHexaViewer->height() + _tBar->height() + 15);
	_base->resize(this->width(), this->height());

	//compute & emit percentage succes rate
	emit this->sgSuccRateUpdate(this->v3_successRate());

	return;
}

void ResultWidget::configure( void ){

	//place component to proper position
	this->placeComponents();

	//all integrated component's signal and slots are connected by this method
	this->connectComponents();

	return;
}

void ResultWidget::artificialResize(int diffH){

	//zero difference
	if(diffH == 0){
		return;
	}

	//row difference
	int rowDiff = diffH/CELL_H;

	//no diff is possible
	if(rowDiff == 0){
		return;
	}

	//recompute rows and cells number
	_v3_rows -= rowDiff;
	_v3_cellsNum = _v3_rows*_v3_cols;

	return;
}

void ResultWidget::copySelection(void)
{
	//get sequence indexes from hexa widget
	signed int start = -1, stop = -1;
	_v3_ptrHexaViewer->selectedCellsInterval(start,stop);

	if((start == -1) || (stop == -1)){
		return;
	}

	QString clipboardContent = "";

	for(signed int i = start; i <= stop; i++){
		clipboardContent += this->_v3_hexaDataForm[i];
	}

	//load clipboard object
	QClipboard* ptrClipboard = QApplication::clipboard();
	if(ptrClipboard != NULL){		
		ptrClipboard->setText(clipboardContent);
	}

	return;
}

void ResultWidget::setupBaseFrame( void ){

	_base->resize(this->width(), this->height());
	_base->setStyleSheet(RWIDGET_STYLE);
	return;
}

void ResultWidget::placeComponents( void ){
	
	_tBar->move(5,5);
	_v3_ptrHexaViewer->move(5, _tBar->height() + 10);
	_v3_ptrTextViewer->move(_v3_ptrHexaViewer->width() + 10, _tBar->height() + 10);
	return;
}

void ResultWidget::slTBarActivated(QAction* act){

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
			this->procResToClip(true);
			break;

		//check code block
		case Toolbar::RES_CHECK:			
			this->v3_colorizeSelection(ResultWidget::CC);
			break;

		//skip block
		case Toolbar::RES_SKIP:			
			this->v3_colorizeSelection(ResultWidget::SK);
			break;

		//sequence block
		case Toolbar::RES_SEQ:			
			this->v3_colorizeSelection(ResultWidget::SEQ);
			break;

		//rollback selection
		case Toolbar::RES_DEFAULT:			
			this->v3_colorizeSelection(ResultWidget::NS);
			break;

		//rollback all
		case Toolbar::RES_DEFAULT_ALL:
			this->v3_eraseXdfSelection();
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
	QObject::connect( 
		_tBar, SIGNAL(actionTriggered(QAction*)),
		this, SLOT(slTBarActivated(QAction*)));

	QObject::connect(
		_v3_ptrHexaViewer->selectableLayer(), 
		SIGNAL(sgSelectionChangedExternaly(QModelIndexList)),
		_v3_ptrTextViewer->selectableLayer(), 
		SLOT(slSelectionChangedExternaly(QModelIndexList)));

	QObject::connect(
		_v3_ptrTextViewer->selectableLayer(), 
		SIGNAL(sgSelectionChangedExternaly(QModelIndexList)),
		_v3_ptrHexaViewer->selectableLayer(), 
		SLOT(slSelectionChangedExternaly(QModelIndexList)));

	return;
}

void ResultWidget::procResToClip(bool lightVer){

	QString definition = "";

	//if light xdf version is requested
	if(!lightVer){
		definition = this->asmXdf(&_v3_textDataForm, &_v3_hexaDataForm, _v3_ptrXdfs);
	}
	else{
		definition = this->asmXdfLight(&_v3_hexaDataForm, _v3_ptrXdfs);
	}

	//load clipboard object
	QClipboard* ptrClipboard = QApplication::clipboard();
	if(ptrClipboard != NULL){		
		ptrClipboard->setText(definition);
	}

	return;
}

QString ResultWidget::asmXdf( QList<QString>* ptrTextContent,
						QList<QString>* ptrHexaContent,
						QList<ResultWidget::xdfBlock_t>* ptrXdfs ){
	
	int startIdx = 0, stopIdx = _v3_cols*_v3_rows;
	bool rdStarted = false, frsBlock = false;
	
	//analyze gaps - setup xdfInterval
	for(int i = 0; i < ptrXdfs->count(); i++){

		//first block - start index
		if( (ptrXdfs->at(i) != ResultWidget::NS) && (!frsBlock)){			
			startIdx = i;
			frsBlock = true;
			rdStarted = true;
		}
		//end of non NS block
		else if(rdStarted && (ptrXdfs->at(i) == ResultWidget::NS)){			
			stopIdx = i;
			rdStarted = false;
		}
		//start of another block - after gap
		else if( !rdStarted && (ptrXdfs->at(i) != ResultWidget::NS)){
			rdStarted = true;
		}
	}

	QString def = "", hTmp = "", tTmp = "", addr = "";
	ResultWidget::xdfBlock_t curBlock = ResultWidget::NS;

	def += XDF_HEAD;
	def += XDF_MD5;

	//MD5 list
	QList<QString> keys = _ptrMD5s->keys();
	for( int i = 0; i < keys.size(); i++ ){
		def += ";" + _ptrMD5s->value( keys[i] ) + "\n";
	}

	def += "\n";
	def += XDF_BEGIN;

	//check if any selectioin exists
	if(ptrXdfs->contains(ResultWidget::CC) || 
		ptrXdfs->contains(ResultWidget::SEQ) ||
		ptrXdfs->contains(ResultWidget::SK)){

		//assembly xdf data
		for( int i = startIdx; i < stopIdx; i++ ){

			//change section
			if( curBlock != ptrXdfs->at(i) ){
			
				this->publishRow(def, addr,i - startIdx, hTmp, tTmp );			
				curBlock = ptrXdfs->at(i);
			
				switch( curBlock ){

					case ResultWidget::CC:
						def += CC_TEXT + "\n";
						break;

					case ResultWidget::SEQ:
						def += SQ_TEXT + "\n";
						break;

					case ResultWidget::SK:
					case ResultWidget::NS:
					default:
						def += SK_TEXT + "\n";
						break;
				}
			}
			//start new line 
			else if((hTmp.size()%(_v3_cols*2)) == 0){			
				this->publishRow( def, addr, i - startIdx, hTmp, tTmp );
			}

			hTmp += ptrHexaContent->at(i);
			tTmp += ptrTextContent->at(i);
		}
	
		//data into row - supplement with alignment
		if(hTmp.size() > 0){
				
			while(hTmp.size() < (_v3_cols*2)){			
				hTmp.append(' ');
			}
			def += addr + " " + hTmp + "  " + tTmp + "'\n";
		}
	}

	def += XDF_END;
	return def;
}

QString ResultWidget::asmXdfLight(
						QList<QString>* ptrHexaContent,
						QList<ResultWidget::xdfBlock_t>* ptrXdfs){
	
	QString def = "";
	int startIdx = 0, stopIdx = _v3_cols*_v3_rows;
	bool rdStarted = false, frsBlock = false;
	
	//analyze gaps - setup xdfInterval
	for(int i = 0; i < ptrXdfs->count(); i++){

		//first block - start index
		if((ptrXdfs->at(i) != ResultWidget::NS) && (!frsBlock)){			
			startIdx = i;
			frsBlock = true;
			rdStarted = true;
		}
		//end of non NS block
		else if(rdStarted && (ptrXdfs->at(i) == ResultWidget::NS)){			
			stopIdx = i;
			rdStarted = false;
		}
		//start of another block - after gap
		else if( !rdStarted && (ptrXdfs->at(i) != ResultWidget::NS)){
			rdStarted = true;
		}
	}

	//check if any selectioin exists
	if( ptrXdfs->contains(ResultWidget::CC) || 
		ptrXdfs->contains(ResultWidget::SEQ) ||
		ptrXdfs->contains(ResultWidget::SK)){

		int rowCnt = 0;

		//assembly xdf data
		for(int i = startIdx; i < stopIdx; i++, rowCnt++){

			//start new line 
			if(((rowCnt%_v3_cols) == 0) && (rowCnt > 0)){			
				def += "\n";
			}
			
			switch(ptrXdfs->at(i)){

				case ResultWidget::CC:						
				case ResultWidget::SEQ:					
					def += ptrHexaContent->at(i);
					break;

				case ResultWidget::SK:
				case ResultWidget::NS:
				default:
					def += "??";
					break;
			}
		}	
	}

	return def;
}

QString ResultWidget::trAddrToStr(int addr){

	QString str = ""; str.setNum(addr, 16);
	while(str.size() < XDF_ADDR_LEN ){
		str.insert( 0, "0" );
	}

	return str;
}

void ResultWidget::publishRow(
						QString &definition,
						QString &address,
						int idx,
						QString &hexaTmp,
						QString &textTmp){

	//data into row
	if(hexaTmp.size() > 0){
				
		while(hexaTmp.size() < (_v3_cols* 2)){
			hexaTmp.append(' ');
		}		
		definition += address + " " + hexaTmp + "  " + textTmp + "'\n";
	}

	//new address
	address = this->trAddrToStr(idx);
	hexaTmp = "";
	textTmp = "'";

	return;
}

void ResultWidget::saveXdf( bool ligthVer ){

	QString fName = "", xdf = "";
	
	fName = QFileDialog::getSaveFileName(NULL,"save XDF","result.xdf","xDEF file (*.xdf)");	
	
	//fName input check
	if(fName.length() <= 0){		
		return;
	}

	int position = fName.indexOf(
		XDF_TERMINATION,
		fName.length() - ((QString)XDF_TERMINATION).length());

	//filename extension
	if(position == -1){
		fName += XDF_TERMINATION;
	}

	//if full xdf version is requested
	if(!ligthVer){		
		xdf = this->asmXdf( &_v3_textDataForm, &_v3_hexaDataForm, _v3_ptrXdfs);
	}
	//assembly xdef ligh version
	else{		
		xdf = this->asmXdfLight(&_v3_hexaDataForm, _v3_ptrXdfs);
	}

	//store xDEF to file
	QFile file(fName);	
	if(!file.open( QIODevice::WriteOnly)){
		Error::alert( tr("Failed to write"), file.errorString());		
	}
	else{
		file.write((xdf.toStdString()).c_str());
		file.close();
	}

	return;
}

void ResultWidget::v3_createModels(void){
	
	//erase all data [label & data array]
	_v3_hexaLabelForm = "";
	_v3_textLabelForm = "";
	_v3_hexaDataForm.clear();
	_v3_textDataForm.clear();
	_v3_ptrXdfs->clear();
	_v3_ptrEquals->clear();	

	//success rate clear
	_v3_equalCells = 0;
	_v3_notEqualCells = 0;
	_v3_notEmptyCells = 0;

	bool empty = true;
	QString cellLabel = "";
	int sequentialIndex = 0;
	unsigned char byte = '\0';
	bool inEmpty = true, inEqual = true, inNotEqual = true;	//section flags

	for(int row = 0; row < _v3_rows; row++){

		for(int col = 0; col < _v3_cols; col++){

			sequentialIndex = (row*_v3_cols) + col;

			//empty cell
			if(_v3_ptrSampleStorage->size() == 0){

				//hexa
				_v3_hexaDataForm.push_back("");
				_v3_hexaLabelForm += "&nbsp;&nbsp;";

				//text
				_v3_textDataForm.push_back("");
				_v3_textLabelForm += "&nbsp;";

				_v3_ptrEquals->push_back(ResultWidget::EMPTY);
				_v3_ptrXdfs->push_back(ResultWidget::NS);
			}
			//equal cell
			else if(this->v3_isEqual(sequentialIndex, &byte, &empty)){

				//equal & empty
				if(empty){

					//set up section flags
					if(inNotEqual){
						
						inNotEqual = false;
						inEqual = true;

						//end of old style
						_v3_hexaLabelForm += LABEL_STYLE_CLOSE;
						_v3_textLabelForm += LABEL_STYLE_CLOSE;

						_v3_hexaLabelForm += LABEL_STYLE_OPEN(EMPTY_STYLE_INLINE);
						_v3_textLabelForm += LABEL_STYLE_OPEN(EMPTY_STYLE_INLINE);
					}

					//hexa
					_v3_hexaDataForm.push_back("");
					_v3_hexaLabelForm += "&nbsp;&nbsp;";

					//text
					_v3_textDataForm.push_back("");
					_v3_textLabelForm += "&nbsp;";

					_v3_ptrEquals->push_back(ResultWidget::EMPTY);
					_v3_ptrXdfs->push_back(ResultWidget::NS);

				}
				//equal & !empty
				else{

					//set up section flags
					if(inNotEqual){
						
						inNotEqual = false;
						inEqual = true;

						//end of old style
						_v3_hexaLabelForm += LABEL_STYLE_CLOSE;
						_v3_textLabelForm += LABEL_STYLE_CLOSE;

						_v3_hexaLabelForm += LABEL_STYLE_OPEN(EQ_STYLE_INLINE);
						_v3_textLabelForm += LABEL_STYLE_OPEN(EQ_STYLE_INLINE);
					}

					//hexa
					cellLabel = FileWidget::v2_charToCellLabelHexa(byte);
					_v3_hexaDataForm.push_back(cellLabel);
					_v3_hexaLabelForm += cellLabel;

					//text
					_v3_textDataForm.push_back(FileWidget::v2_charToCellLabelText(byte,false));
					_v3_textLabelForm += FileWidget::v2_charToCellLabelText(byte);

					_v3_ptrEquals->push_back(ResultWidget::EQ);
					_v3_ptrXdfs->push_back(ResultWidget::NS);

					_v3_equalCells++;
					_v3_notEmptyCells++;
				}
			}
			//not equal cell
			else{

				//set up section flags
				if(inEqual){

					inNotEqual = true;
					inEqual = false;

					//end of old style
					_v3_hexaLabelForm += LABEL_STYLE_CLOSE;
					_v3_textLabelForm += LABEL_STYLE_CLOSE;

					_v3_hexaLabelForm += LABEL_STYLE_OPEN(NEQ_STYLE_INLINE);
					_v3_textLabelForm += LABEL_STYLE_OPEN(NEQ_STYLE_INLINE);
				}

				//hexa
				cellLabel = FileWidget::v2_charToCellLabelHexa(byte);
				_v3_hexaDataForm.push_back(cellLabel);
				_v3_hexaLabelForm += cellLabel;

				//text
				_v3_textDataForm.push_back(FileWidget::v2_charToCellLabelText(byte,false));
					_v3_textLabelForm += FileWidget::v2_charToCellLabelText(byte);

				_v3_ptrEquals->push_back(ResultWidget::NEQ);
				_v3_ptrXdfs->push_back(ResultWidget::NS);

				_v3_notEqualCells++;
				_v3_notEmptyCells++;
			}

			//spaces between characters
			if((col+1) != _v3_cols){
				_v3_hexaLabelForm += "&nbsp;";
			}
		}

		//end of line
		_v3_hexaLabelForm += "<br/>";
		_v3_textLabelForm += "<br/>";
	}
	
	return;
}

QString ResultWidget::v3_successRate(void){

	if(_v3_notEmptyCells <= 0){
		return "";
	}

	float rate = _v3_equalCells/((float)_v3_notEmptyCells);
	rate = ((int)(rate *10000))/100.0;

	QString strRate;
	return strRate.setNum(rate,'f',2);
}

bool ResultWidget::v3_isEqual(int index, unsigned char* ptrByte, bool* ptrEmpty){

	QList<QString> keys = _v3_ptrSampleStorage->keys();
	int tVal = 0, referenceSampleIndex = 0;

	//translate string "shown sample" to array index
	if(keys.contains(_referenceSampleKey)){		
		referenceSampleIndex = keys.indexOf(_referenceSampleKey);
	}
	else{
		referenceSampleIndex = 0;
		_referenceSampleKey = keys[0];
	}
	
	//cycle through all samples
	for(int i = 0 ; i < keys.count(); i++){

		if( i == 0 ){			
			tVal = (_v3_ptrSampleStorage->value(keys[i]))->at(index);
		}
		else{
			
			//if samples are different
			if((_v3_ptrSampleStorage->value(keys[i]))->at(index) != tVal){

				//current cell value is suitable for show [not empty]
				if(_v3_ptrSampleStorage->value(keys[referenceSampleIndex])->at(index) != -1){
					
					//use value from previosly shown file
					tVal = _v3_ptrSampleStorage->value(keys[referenceSampleIndex])->at(index);
					*ptrByte = (unsigned char)tVal;
				}
				else{
				
					//find non empty byte when bytes are not equal
					for( int j = 0; j < keys.count(); j++ ){
					
						//if examined cell is different from empty space
						if((_v3_ptrSampleStorage->value(keys[j]))->at(index) != -1){
						
							referenceSampleIndex = j;
							_referenceSampleKey = keys[j];
							
							tVal = _v3_ptrSampleStorage->value(keys[j])->at(index);
							*ptrByte = (unsigned char)tVal;							
							break; //break the cycle - first unempty was found
						}					
					}
				}
				
				return false;
			}
			
			tVal = _v3_ptrSampleStorage->value(keys[i])->at(index);
		}
	}

	//store equal byte
	if((tVal == -1)){		
		*ptrEmpty = true;
	}
	else{		
		*ptrEmpty = false;
		*ptrByte = (unsigned char)tVal;
	}

	return true;
}

void ResultWidget::v3_colorizeSelection(ResultWidget::xdfBlock_t colorType){

	if((_v3_ptrXdfs->size() == 0) || (_v3_ptrEquals->size() == 0)){
		return;
	}

	QColor color;
	switch(colorType){
		case ResultWidget::CC:
			color = CC_COLB_TRANS;
			break;
		case ResultWidget::SEQ:
			color = SQ_COLB_TRANS;
			break;
		case ResultWidget::SK:
			color = SK_COLB_TRANS;
			break;
		case ResultWidget::NS:
			color = NS_COLB_TRANS;
			break;
		default:
			break;
	}

	QModelIndexList indexes = 
		_v3_ptrHexaViewer->selectableLayer()->selectedIndexes();
	QAbstractItemModel* ptrHexaModel = 
		_v3_ptrHexaViewer->selectableLayer()->model();
	QAbstractItemModel* ptrTextModel = 
		_v3_ptrTextViewer->selectableLayer()->model();
	int sequentialIndex = 0;

	for(int i = 0; i < indexes.size(); i++){
		
		sequentialIndex = (indexes[i].row()*_v3_cols) + indexes[i].column();

		if(_v3_ptrEquals->at(sequentialIndex) != ResultWidget::EMPTY){

			_v3_ptrXdfs->replace(sequentialIndex, colorType);
			ptrHexaModel->setData(
				indexes[i],
				QVariant(QBrush(color)),
				Qt::BackgroundColorRole);
			ptrTextModel->setData(
				indexes[i],
				QVariant(QBrush(color)),
				Qt::BackgroundColorRole);
		}
	}

	//clear selection
	_v3_ptrHexaViewer->selectableLayer()->clearSelection();
	_v3_ptrTextViewer->selectableLayer()->clearSelection();

	return;
}

void ResultWidget::v3_eraseXdfSelection(void){

	int sequentialIndex = 0;
	QAbstractItemModel *ptrHexaModel = _v3_ptrHexaViewer->selectableLayer()->model();
	QAbstractItemModel *ptrTextModel = _v3_ptrTextViewer->selectableLayer()->model();
	QModelIndex modelIndex;

	for(int row = 0; row < _v3_rows; row++){
		for(int col = 0; col < _v3_cols; col++){

			sequentialIndex = (row*_v3_cols) + col;
		
			if((_v3_ptrXdfs->at(sequentialIndex) != ResultWidget::NS) &&
				(_v3_ptrEquals->at(sequentialIndex) != ResultWidget::EMPTY)){
				
				//xdf array
				_v3_ptrXdfs->replace(sequentialIndex,ResultWidget::NS);

				//hexa viewer
				modelIndex = ptrHexaModel->index(row, col);
				ptrHexaModel->setData(
					modelIndex,
					QVariant(QBrush(NS_COLB_TRANS)),
					Qt::BackgroundColorRole);

				//text viewer
				modelIndex = ptrTextModel->index(row, col);
				ptrTextModel->setData(
					modelIndex,
					QVariant(QBrush(NS_COLB_TRANS)),
					Qt::BackgroundColorRole);
			}			
		}
	}

	_v3_ptrHexaViewer->clearSelection();
	_v3_ptrTextViewer->clearSelection();

	return;
}