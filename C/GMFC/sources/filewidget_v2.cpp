#include "filewidget_v2.h"

FileWidget::FileWidgetException::FileWidgetException(QString msg) 
	: std::exception(msg.toStdString().c_str()){}

FileWidget::FileWidget(QString filename, int rows, int cols, QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	_dType = FileWidget::NOT_SET;
	_ptrPE = NULL;
	_fileName = filename.toStdWString();
	_fName = filename;
	
	_focusFlag = false;
	
	_pActDialog = NULL;
	
	_v2_rows = rows;
	_v2_cols = cols;
	_v2_cellsNum = _v2_rows * _v2_cols;

	_v2_bytes = NULL;
	_v2_ptrIdxs = NULL;
	_v2_file = 0;
	_v2_offset = 0;

	_v2_md5 = "";
	_v2_startAddress = 0;
	_v2_scrollBarValue = 0;
	_v2_scrollFloatBuffer = 0;
	_v2_imageAddressEnable = false;
	_v2_draggedAddressSent = false;	

	try{
	
		//save pattern parser object allocation
		_ptrSaveParser = new SaveParser();
		_v2_ptrIdxs = new QList<FileWidget::t_IdxsIt>();
				
		//models initialiazing
		_v2_ptrHexaModel = new QStandardItemModel(rows, cols);
		_v2_ptrTextModel = new QStandardItemModel(rows, cols);
		_v2_ptrAddressModel = new QStandardItemModel(rows, 1);

		_v2_ptrDataContent = new QList<int>();
		_v2_ptrSaveIntervals = new QList<SaveParser::saveInterval_t>();
		
		//program processing data load, PE scan, etc
		this->executeProcessInternals();

		//base frame creation
		_base = new QFrame(this);	

		//address bar creation
		_address = new AddressBar(_v2_ptrAddressModel, this);
		_address->configure();

		//hexa viewer allocation & configuration
		_hexa = new HexaViewer(_v2_ptrHexaModel, _fName, this);
		_hexa->configure();		
		
		//text viewer allocation & configuration
		_text = new TextViewer(_v2_ptrTextModel, _fName, this);
		_text->configure();
				
		//scroll bar creation
		_scroll = new ScrollBar(_hexa->height(), this);
		_scroll->configure((_v2_rows*(-1)) +1,				//min
							_v2_fileSize/_v2_cols -1,		//max
							_v2_scrollBarValue,				//value
							1,								//single step
							_v2_rows);						//page step
			
		//toolbar creation
		_tBar = new Toolbar(
			_address->width() + _hexa->width() + _text->width() + _scroll->width() + 15,
			TOOL_HEIGHT,
			this);
		_tBar->configure(_tBarOpt);

		//title creation
		_title = new QLabel( this->corFName(), this );
		this->confTitle(
			_address->width() + _hexa->width() + _text->width() + _scroll->width() + 15,
			TITLE_HEIGHT);

		//widget size settings and configuration
		this->configureBaseFrame();
		this->setFixedSize(_base->width(), _base->height());		
	}
	catch( std::bad_alloc ){

		QMessageBox msgBox;
		msgBox.setWindowIcon(QIcon::fromTheme("Error Icon",
						QIcon(":/Global/Resources/error.svg")));
		msgBox.setWindowTitle(tr("Fatal Error"));
		msgBox.setText(tr("Application can start due to memory allocation error."));
		msgBox.setIcon( QMessageBox::Critical );
		msgBox.exec();
	}
}

FileWidget::~FileWidget(){
	
	if( _hexa != NULL ){
		delete _hexa;
	}

	if( _text != NULL ){
		delete _text;
	}

	if( _address != NULL ){
		delete _address;
	}

	if( _tBar != NULL ){
		delete _tBar;
	}

	delete _scroll;
	delete _base;

	if(_ptrPE != NULL){
		delete _ptrPE;
	}

	//file handler closing
	if(_v2_ptrFile != NULL){

		if(_v2_ptrFile->isOpen()){
			_v2_ptrFile->close();
		}

		delete _v2_ptrFile;
	}

	//data content
	delete _v2_ptrDataContent;
	
	//save intervals
	delete _v2_ptrSaveIntervals;	
}

QString FileWidget::FileName( void ){

	return _fName;
}

QString FileWidget::MD5( void ){
	return _v2_md5;
}

QList<int>* FileWidget::DataContent( void ){

	return _v2_ptrDataContent;
}

void FileWidget::configure( void ){
	
	//install event filter for scrollBar
	_scroll->installEventFilter(this);

	this->placeComponents();

	this->connectComponents();

	return;
}

void FileWidget::setFocus(bool flag){	

	//already has focus
	if((flag && _focusFlag) || (!flag && !_focusFlag)){
		return;
	}

	//request for focus and not have it already
	if(flag && !_focusFlag){
		_base->setStyleSheet(FWGT_FOC_COL);
		_focusFlag = true;
	}

	//request for blur and not have it already
	if((!flag && _focusFlag)){
		_base->setStyleSheet(FWGT_BLUR_COL);
		_focusFlag = false;
	}	
	
	return;
}

bool FileWidget::focus(void){
	return _focusFlag;
}

int FileWidget::currentRows(int height){

	return (height - (TITLE_HEIGHT + TOOL_HEIGHT + 20)) / CELL_H;
}

void FileWidget::slScrollChanged(int value){

	int currentShift = (value*_v2_cols) - (_v2_scrollBarValue*_v2_cols);

	this->v2_propagateStep(currentShift);
	_v2_startAddress += (currentShift);

	this->v2_updateModels();
	
	//_hexa->updateView(_v2_ptrHexaModel);
	//_text->updateView(_v2_ptrTextModel);
	//_address->updateView(_v2_ptrAddressModel);
	_scroll->setValue(_v2_scrollBarValue);

	this->contentChanged(_fName, _v2_ptrDataContent);

	return;
}

void FileWidget::slTBarActivated(QAction* action){
	
	QHash<QAction*, Toolbar::ACTIONS>* actTab = _tBar->Actions();
	Toolbar::ACTIONS toolbarAction = actTab->value(action);

	this->execTBarAction(toolbarAction);
	this->contentChanged(_fName, _v2_ptrDataContent);
	return;
}

void FileWidget::slTBarActivated(Toolbar::ACTIONS action){

	this->execTBarAction(action,true);
	this->contentChanged(_fName, _v2_ptrDataContent, true);
	return;
}

void FileWidget::slCheckDialog(QString value){

	bool ok;
	int hex = 0;
	QLabel *pNotif = NULL;
	QDialogButtonBox *pButBox = NULL;
	QObjectList children;
	QPushButton* pOK = NULL;

	switch( _dType ){

		//target address dialog check
		case FileWidget::TARGET_ADDRESS:	
			
			hex = value.toInt(&ok, 16);			
			children = _pActDialog->children();			
			pNotif = (QLabel*)children[2];			
			pButBox = (QDialogButtonBox*)children[4];			
			pOK = pButBox->button(QDialogButtonBox::Ok);

			if( !ok || (hex < 0) || (hex > _v2_fileSize - 1) ){				
				
				pNotif->setText("Address is not valid.");				
				pOK->setEnabled(false);				
				_pActDialog->setWindowIcon(QIcon::fromTheme("Error Icon",
					QIcon(":/Global/Resources/error.svg")));
			}
			else{
				
				pNotif->setText("");				
				pOK->setEnabled(true);				
				_pActDialog->setWindowIcon(QIcon::fromTheme("Error Icon",
					QIcon(":/Toolbar/Resources/targetAddress.svg")));
			}
			break;

		//binary search dialog check
		case FileWidget::BIN_SEARCH:

			children = _pActDialog->children();			
			pButBox = (QDialogButtonBox*)children[3];			
			pOK = pButBox->button(QDialogButtonBox::Ok);

			if( value.size() % 2 != 0 ){				
				pOK->setEnabled(false);
			}
			else{
				pOK->setEnabled(true);
			}
			break;
			
		default:
			break;
	}

	return;
}

void FileWidget::contentChanged(QString fName, QList<int> *dataContent, bool silent){

	if(silent){
		emit this->sgContentIsChangingSilent(fName, dataContent);
	}
	else{
		emit this->sgContentIsChanging(fName, dataContent);
	}	
	return;
}

void FileWidget::slDragInProgress(int seqIdx){

	//get address from address bar
	QString addr = this->v2_draggedAddress(seqIdx);

	//emit address to upper level
	emit this->sgDragAtAddress(addr);

	return;
}

void FileWidget::artificialResize(int diffH){

	//no differences, no need to resize
	if(diffH == 0){
		return;
	}

	//row difference
	int rowDiff = diffH / CELL_H;

	//no diff is possible
	if(rowDiff == 0){
		return;
	}

	//recompute rows and cells number
	_v2_rows -= rowDiff;	
	_v2_cellsNum = _v2_rows*_v2_cols;

	//reallocate models for all components
	this->v2_reallocateModels();

	//resize other build-in components
	_hexa->artificialResize(_v2_ptrHexaModel);
	_text->artificialResize(_v2_ptrTextModel);
	_address->artificialResize(_v2_ptrAddressModel);
	_scroll->artificialResize(_hexa->height(), _v2_scrollBarValue);

	//TODO: send the content array to result widget [without refreshing]
	qDebug() << _v2_ptrDataContent->size();
	this->contentChanged(_fName, _v2_ptrDataContent, true);

	//resize widget itself
	_base->resize(_base->width(),
		_hexa->height() + 20 + _title->height() + _tBar->height());
	this->setFixedSize(_base->width(), _base->height());
	
	return;
}

void FileWidget::slViewChanged(int shift, int drag){

	//drag started
	if((!_v2_draggedAddressSent) && (drag >= 0)){
		_v2_draggedAddressSent = true;
		emit this->sgDragAtAddress(this->v2_draggedAddress(drag));
	}

	//drag finished
	if(drag == -2){
		_v2_draggedAddressSent = false;
		emit this->sgDragAtAddress("");
	}

	this->v2_propagateStep(shift);
	_v2_startAddress += shift;

	//update data models
	this->v2_updateModels();

	//send data content to upper level - noisy type
	if(drag != -1){
		this->contentChanged(_fName, _v2_ptrDataContent);
	}

	//disconnect scrollBar due to double changes
	QObject::disconnect(_scroll, SIGNAL(valueChanged(int)),
		this, SLOT(slScrollChanged(int)));

	//_address->updateView(_v2_ptrAddressModel);
	//_hexa->updateView(_v2_ptrHexaModel);
	//_text->updateView(_v2_ptrTextModel);
	_scroll->setValue(_v2_scrollBarValue);

	//reconnect scrollBar
	QObject::connect(_scroll, SIGNAL(valueChanged(int)),
		this, SLOT(slScrollChanged(int)));	

	return;
}

void FileWidget::configureBaseFrame(void){
	
	_base->resize(_address->width() + _hexa->width() + _text->width() + _scroll->width() + 25,
			_hexa->height() + _tBar->height() + _title->height() + 20);

	_base->setStyleSheet(FWIDGET_STYLE);

	return;
}

void FileWidget::placeComponents(void){
	
	_title->move(5,5);
	_tBar->move(5, 30);	
	_address->move(5,63);
	_hexa->move(_address->x() + _address->width() + 5, _address->y());
	_text->move(_hexa->x() + _hexa->width() + 5, _hexa->y());
	_scroll->move(_text->x() + _text->width() + 5, _text->y());	

	return;
}

void FileWidget::connectComponents(void){
	
	//hexa -> fileWidget
	QObject::connect(_hexa, SIGNAL(sgViewChanged(int,int)),
						this, SLOT(slViewChanged(int,int)));
	//text -> fileWidget
	QObject::connect(_text, SIGNAL(sgViewChanged(int,int)),
						this, SLOT(slViewChanged(int,int)));
	//scroll -> fileWidget
	QObject::connect(_scroll, SIGNAL(valueChanged(int)),
						this, SLOT(slScrollChanged(int)));
	//toolbar -> fileWidget
	QObject::connect(_tBar, SIGNAL(actionTriggered(QAction*)),
						this, SLOT(slTBarActivated(QAction*)));

	return;
}

void FileWidget::executeProcessInternals(void){
	
	this->peScan();	//PE scan

	//file operations
	this->v2_openFile();
	this->v2_computeMD5();

	//create models from inputed data
	this->v2_createBytes(_v2_file);
	this->v2_createIndexes(_v2_bytes,_v2_offset,_v2_file);
	this->v2_createModels();

	this->contentChanged(_fName,_v2_ptrDataContent);

	return;
}

void FileWidget::peScan(void){

	_tBarOpt.set(Toolbar::PE);
	_tBarOpt.set(Toolbar::PE_OVERLAY, false);	/**< clear PE_OVERLAY in default */

	//option for scanning
	std::bitset<PEbrowser::OPTION_SIZE> peOpt;
	peOpt.set(PEbrowser::SECTION_HEADERS);
	peOpt.set(PEbrowser::REAL_VALUES);

	try{
		
		_ptrPE = new PEbrowser(_fileName);
		_ptrPE->ScanFile();
	}

	catch( PEbrowser::Exc_FileOpen ){
		
		_tBarOpt.set(Toolbar::PE, false);		
	}

	catch( PEbrowser::Exc_MZ ){
		_tBarOpt.set(Toolbar::PE, false);
	}

	catch( PEbrowser::Exc_NoResources ){
	}

	catch( PEbrowser::Exc_NotImplemented ){
	}

	catch( PEbrowser::Exc_PE ){
		_tBarOpt.set(Toolbar::PE, false);
	}

	catch( PEbrowser::Exc_Section ){
		_tBarOpt.set(Toolbar::PE, false);
	}

	catch( PEbrowser::Exc_Size ){
		_tBarOpt.set(Toolbar::PE, false);
	}

	catch( PEbrowser::Exc_Stat ){
		_tBarOpt.set(Toolbar::PE, false);
	}

	catch( PEbrowser::Exc_Uninit ){
		_tBarOpt.set(Toolbar::PE, false);
	}

	//not PE file delete PEbrowser object and resetup to NULL
	if( !_tBarOpt.test(Toolbar::PE) ){
		
		delete _ptrPE;
		_ptrPE = NULL;
		
		return;
	}

	//check if overlay exists
	if( _ptrPE->Overlay() != (unsigned long)(-1) ){
		_tBarOpt.set(Toolbar::PE_OVERLAY);
	}

	return;
}

void FileWidget::createTargetAddressDialog( void ){
	
	//variable init
	QInputDialog* pDialog = new QInputDialog(this);
	_dType = FileWidget::TARGET_ADDRESS;
	
	//label prepare
	QString tmp; QString l_tmp;
	tmp.setNum(_v2_fileSize - 1,16);
	l_tmp.setNum(tmp.length());

	//setting up dialog
	pDialog->setModal(true);
	pDialog->setFixedSize(QSize(350,150));
	pDialog->setInputMode(QInputDialog::TextInput);
	pDialog->setWindowTitle(tr("Target address navigation"));
	pDialog->setWindowIcon(
		QIcon::fromTheme("concrete address",
							QIcon(":/Toolbar/Resources/targetAddress.svg")));	
	pDialog->setLabelText(tr("Set address from 0 to %1 [hex]:").arg(tmp));
	pDialog->setTextValue("0");
	
	//inserting notification area
	QLabel *pNotif = new QLabel(pDialog);
	pNotif->setText("");
	pNotif->move(13, 50);
	pNotif->setStyleSheet("color:#ff0000;");	

	//insert validator
	QRegExpValidator *pVld = new QRegExpValidator(
									QRegExp("[0-9a-fA-F]{1," + l_tmp + "}"),pDialog);
	((QLineEdit*)(pDialog->children())[0])->setValidator( pVld );
	
	//set up actual activated dialog
	_pActDialog = pDialog;

	//text input validator connection
	QObject::connect( pDialog, SIGNAL(textValueChanged(QString)),
		this, SLOT(slCheckDialog(QString)));
	
	//dialog is accepted
	if(pDialog->exec()){
		
		//inputed address
		QString inputedAddress = pDialog->textValue();

		//inputed address not empty
		if(inputedAddress.size() > 0){

			bool ok = false;
			int inputedAddressNum = inputedAddress.toInt(&ok, 0x10);

			if(ok && (inputedAddressNum >= 0) && (inputedAddressNum < _v2_fileSize)){
				
				//compute and propagate shift into data models
				int shift = inputedAddressNum - _v2_startAddress;
				if(this->v2_validShift(shift)){
					this->slViewChanged(shift,-1);
				}
			}
			else{

				QMessageBox notFound;
				notFound.setWindowIcon(QIcon::fromTheme("Error Icon",
					QIcon(":/Global/Resources/error.svg")));
				notFound.setWindowTitle(tr("Jum to address"));
				notFound.setText(tr("Address is not correct for this sample"));
				notFound.setIcon(QMessageBox::Warning);
				notFound.exec();
			}
		}
	}

	//diconnection
	QObject::disconnect( pDialog, SIGNAL(textValueChanged( QString )),
		this, SLOT(slCheckDialog(QString)));
		
	//unset actual activated dialog
	_pActDialog = NULL;
	
	//cleaning
	_dType = FileWidget::NOT_SET;
	delete pNotif;
	delete pVld;
	delete pDialog;
	
	return;
}

void FileWidget::createGeneralDescriptionDialog( void ){
	
	/*//variables declaration
	QDialog *pDialog = new QDialog(this);	
	QDialogButtonBox *pButBox 
		= new QDialogButtonBox( QDialogButtonBox::Ok, Qt::Horizontal, pDialog);
	
	//dialog settings
	pDialog->setFixedSize(450, 290);	
	pDialog->setModal( false );	
	pDialog->setWindowTitle(tr("File description"));	
	pDialog->setWindowIcon(
		QIcon::fromTheme("file description",
			QIcon(":/Toolbar/Resources/desc.svg")));
	
	//button box
	pButBox->move( 366, 250 );

	//grid layout
	QGridLayout *pGrid = new QGridLayout();
	pGrid->setSizeConstraint(QLayout::SetMinAndMaxSize);	
	pGrid->setHorizontalSpacing(0);	
	pGrid->setVerticalSpacing(3);	
	pGrid->setMargin(0);
	
	//label
	pGrid->setColumnStretch(0,0);	
	pGrid->setColumnMinimumWidth(0,170);

	//data
	pGrid->setColumnStretch(1,0);	
	pGrid->setColumnMinimumWidth(1,240);

	//layout content
	//QLabel *pLabel = NULL;
	
	//TODO: connect data with hash table containing labels, and data
	for(int i = 0; i < 15; i++){

		//row settings
		pGrid->setRowStretch(i,0);
		
		pGrid->setRowMinimumHeight(i,20);
		
		//label
		pLabel = new QLabel("Info Label:");
		
		pLabel->setStyleSheet("background-color:#a0a0a0;");
		
		pGrid->addWidget( pLabel, i, 0);
		
		//data
		pLabel = new QLabel("Data");
		
		pLabel->setStyleSheet("background-color:#a0a0a0;");
		
		pGrid->addWidget( pLabel, i, 1);
	}

	//base frame
	QFrame *pBase = new QFrame();	
	pBase->setLayout(pGrid);

	//scroll area
	QScrollArea *pScroll = new QScrollArea(pDialog);	
	pScroll->move(10,20);	
	pScroll->setFixedSize(430,220);	
	pScroll->adjustSize();	
	pScroll->setStyleSheet("QScrollBar{}");	
	pScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	pScroll->setWidget(pBase);

	//set actual activated dialog
	_pActDialog = pDialog;

	//connect buttons to dialog
	QObject::connect(pButBox, SIGNAL(accepted()),pDialog,SLOT(accept()));

	pDialog->exec();

	//disconnect buttons to dialog
	QObject::disconnect(pButBox, SIGNAL(accepted()),pDialog,SLOT(accept()));

	//unset actual activated dialog
	_pActDialog = NULL;

	delete pScroll;
	//delete pBase;
	//delete pGrid;	
	delete pButBox;
	delete pDialog;*/
	
	return;
}

void FileWidget::createBinarySearchDialog(void){

	//variable init
	QInputDialog* pDialog = new QInputDialog(this);
	pDialog->setModal(true);
	pDialog->setFixedSize(QSize(350,150));
	pDialog->setInputMode(QInputDialog::TextInput);
	pDialog->setWindowTitle(tr("Binary search"));
	pDialog->setWindowIcon(
		QIcon::fromTheme("Binary search",
							QIcon(":/Toolbar/Resources/search.svg")));	
	pDialog->setLabelText(tr("Insert binary sequence [1-32 bytes]: "));
	pDialog->setTextValue("");	
	
	//insert validator
	QRegExpValidator *pVld = 
		new QRegExpValidator(QRegExp("[0-9a-fA-F]{1,64}"),pDialog);
	((QLineEdit*)(pDialog->children())[0])->setValidator( pVld );
	
	//set up proceeding dialog
	_pActDialog = pDialog;
	_dType = FileWidget::BIN_SEARCH;

	//connect input validator's function
	QObject::connect( pDialog, SIGNAL(textValueChanged(QString)),
						this, SLOT(slCheckDialog(QString)));
	
	if(pDialog->exec()){

		//store input value
		int foundedAddress = this->v2_binarySearch(pDialog->textValue());		
		if( foundedAddress != -1 ){

			int shift = foundedAddress - _v2_startAddress;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
		}
		else{
			
			QMessageBox notFound;
			notFound.setWindowIcon(QIcon::fromTheme("Error Icon",
					QIcon(":/Global/Resources/error.svg")));
			notFound.setWindowTitle(tr("Binary search"));
			notFound.setText(tr("Binary pattern was not found in this sample"));
			notFound.setIcon( QMessageBox::Warning );
			notFound.exec();
		}
	}

	//diconnection
	QObject::disconnect( pDialog, SIGNAL(textValueChanged(QString)),
						this, SLOT(slCheckDialog(QString)));
		
	//set up no proceeding dialog
	_pActDialog = NULL;
	_dType = FileWidget::NOT_SET;

	delete pVld;
	delete pDialog;	
	return;
}

void FileWidget::createSavePatternDialog(void){

	//create file dialog object
	QFileDialog *pDialog = new QFileDialog(this, tr("Select save pattern file"));
	pDialog->setModal(true);
	pDialog->setFileMode(QFileDialog::ExistingFiles);

	//set up proceeding dialog
	_dType = FileWidget::SAVE_PATTERN;
	_pActDialog = pDialog;

	//execute dialog
	if(pDialog->exec()){
		QStringList fNames = pDialog->selectedFiles();
		for(int i = 0; i < fNames.size(); i++){
			_ptrSaveParser->addSavePattern( fNames[i], _ptrPE );
		}
		_v2_ptrSaveIntervals = _ptrSaveParser->Intervals();

		//refresh models 
		this->v2_updateModels();
		_hexa->updateView(_v2_ptrHexaModel);
		_text->updateView(_v2_ptrTextModel);
	}

	//set up no proceeding dialog
	_dType = FileWidget::NOT_SET;
	_pActDialog = NULL;
		
	delete pDialog;
	return;
}

QString FileWidget::corFName(void){

	int len = 0;

	//no string
	if( (len = _fileName.length()) <= 0){
		return "No title";
	}

	//convertion
	QString fName = QString::fromStdWString(_fileName);
	
	//string is too long
	if( len > 50 ){
		return "***" + fName.right(50);
	}

	return fName;
}

void FileWidget::confTitle(int w, int h){

	//font settings
	QFont font;
	font.setFamily( CODE_FONT );
	font.setPixelSize(11);
	font.setBold(true);
	_title->setFont(font);
	
	//size & styles settings
	_title->resize(w,h);
	_title->setStyleSheet(TITLE_STYLE);
	_title->setIndent(5);
	
	return;
}

void FileWidget::wheelEvent(QWheelEvent* event){

	//no futher event escalation
	event->accept();

	int requestedShift = 0;

	//direction UP
	if( event->delta() > 0 ){			
		requestedShift = -1;
	}
	//direction DOWN
	else{
		requestedShift = 1;
	}

	//check if shift is correct, then run shift method
	int signedShift = requestedShift*_v2_cols;
	if(this->v2_validShift(signedShift)){
		this->slViewChanged(signedShift, -1);
	}

	//data content has been changed
	this->contentChanged(_fName, _v2_ptrDataContent);
	
	return;
}

void FileWidget::keyPressEvent(QKeyEvent* ptrEvent){

	//dialog is active
	if( _pActDialog != NULL ){
		ptrEvent->ignore();
		return;
	}

	if(!_focusFlag){
		ptrEvent->ignore();
		return;
	}

	int shift = 0;

	switch( ptrEvent->key() ){

		case Qt::Key_Home:
			shift = (0 - _v2_startAddress);
			break;

		case Qt::Key_End:
			shift = (_v2_fileSize - _v2_cellsNum) - _v2_startAddress;
			break;

		case Qt::Key_Left:
			shift = 1;
			break;

		case Qt::Key_Right:
			shift = -1;
			break;

		case Qt::Key_Up:
			shift = (-1)*_v2_cols;
			break;

		case Qt::Key_Down:
			shift = _v2_cols;
			break;

		case Qt::Key_PageUp:
			shift = (-1)*_v2_cols*_v2_rows;
			break;

		case Qt::Key_PageDown:			
			shift = _v2_cols*_v2_rows;
			break;

		default:
			//escalate event to upper level
			ptrEvent->ignore();
			return;
	}

	//update model and deply new values
	if(this->v2_validShift(shift)){
		this->slViewChanged(shift,-1);
	}

	this->contentChanged(_fName, _v2_ptrDataContent);

	return;
}

void FileWidget::execTBarAction(Toolbar::ACTIONS curAct, bool silent){

	int entryPointAddress = -1, overlayAddress = -1, peAddress = -1;
	bool actFailed = false;
	QMessageBox error; QList<QString> fNames;
	int shift = 0;

	switch( curAct ){
		
		case Toolbar::CLOSE:

			emit this->closing(QString::fromStdWString(_fileName));
			
			//parent not exists [standalone run]
			if( this->parent() == NULL ){
				this->close();
			}			
			break;

		case Toolbar::GEN_DESC:
			
			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}
			
			//this->createGeneralDescriptionDialog();
			NotImplemented::alert();
			break;

		case Toolbar::DIRECT_ADDRES:
			
			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			this->createTargetAddressDialog();
			break;

		case Toolbar::IMAGE_BASE:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}
			
			//only for PE file - for global signalization
			if( !_tBarOpt.test(Toolbar::PE )){
				return;
			}

			//switch addresses between zero and image base offset
			if(_v2_imageAddressEnable){
				_v2_imageAddressEnable = false;
			}
			else{
				_v2_imageAddressEnable = true;
			}

			//update and propagate model
			this->v2_updateModels();
			_address->updateView(_v2_ptrAddressModel);
			break;

		case Toolbar::ENTRY_POINT:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			//only for PE file - for global signalization
			if(!_tBarOpt.test(Toolbar::PE)){
				return;
			}			

			entryPointAddress = (int)_ptrPE->EntryPoint();
			if(entryPointAddress < 0){

				if(silent){
					return;
				}
				
				error.setWindowIcon(QIcon::fromTheme("Error Icon",
					QIcon(":/Global/Resources/error.svg")));
				error.setWindowTitle(tr("Entry point"));
				error.setText(tr("Jump to entry point can not be proceed"));
				error.setIcon(QMessageBox::Critical);
				error.exec();
			}
			else{
				shift = entryPointAddress - _v2_startAddress;
				this->slViewChanged(shift, -1);
			}			
			break;

		case Toolbar::OVERLAY:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			//only for PE file - for global signalization
			if( !_tBarOpt.test(Toolbar::PE )){
				return;
			}

			overlayAddress = (int)_ptrPE->Overlay();
			if(overlayAddress < 0){
				
				if(silent){
					return;
				}
				
				error.setWindowIcon(QIcon::fromTheme("Error Icon",
					QIcon(":/Global/Resources/error.svg")));
				error.setWindowTitle(tr("Overlay"));
				error.setText(tr("Jump to overlay can not be proceed"));
				error.setIcon( QMessageBox::Critical);
				error.exec();
			}
			else{				
				shift = overlayAddress - _v2_startAddress;
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::PE_SECTION:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			//only for PE file - for global signalization
			if( !_tBarOpt.test(Toolbar::PE )){
				return;
			}
			
			peAddress = (int)_ptrPE->PE_Address();
			if(peAddress < 0){

				if(silent){
					return;
				}

				error.setWindowIcon(QIcon::fromTheme("Error Icon",
									QIcon(":/Global/Resources/error.svg")));
				error.setWindowTitle(tr("PE address"));
				error.setText(tr("Jump to PE address can not be proceed"));
				error.setIcon( QMessageBox::Critical);
				error.exec();
			}
			else{				
				shift = peAddress - _v2_startAddress;
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::JMP_BEG:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}
			
			shift = 0 - _v2_startAddress;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::JMP_END:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			shift = (_v2_fileSize - _v2_cellsNum) - _v2_startAddress;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::JMP_PAGE_D:

			shift = _v2_cellsNum;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::JMP_PAGE_U:

			shift = 0 - _v2_cellsNum;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::JMP_ROW_D:

			shift = _v2_cols;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::JMP_ROW_U:

			shift = 0 - _v2_cols;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::BIN_SEARCH:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}
			
			//binary search processing
			this->createBinarySearchDialog();
			break;

		case Toolbar::SAVE_PATTERN:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}
			
			//process save pattern dialog
			this->createSavePatternDialog();			
			break;

		default:
			//show "not implemented" dialog
			NotImplemented::alert();
			break;
	}

	return;
}

void FileWidget::v2_openFile(void){

	//file name not exists
	if(_fName == ""){
		throw FileWidget::FileWidgetException("File name was not inputed");
	}

	_v2_ptrFile = new QFile(_fName);

	if(!_v2_ptrFile->open(QIODevice::ReadOnly)){
		throw FileWidget::FileWidgetException("File not exists.");
	}

	_v2_fileSize = _v2_ptrFile->size();

	return;
}

void FileWidget::v2_computeMD5(void){

	//md5 variables
	MD5_CTX md5ctx;
	unsigned char md5bts[16] = "\0";
	
	//buffer variables
	unsigned int read = 0;
	const unsigned int bufferLng = 4096;
	unsigned char buffer[bufferLng] = "\0";	

	QString tmpByte = "";

	//clear md5 hash
	_v2_md5 = "";

	//opened file exists
	if( !_v2_ptrFile->isOpen() ){
		return;
	}

	//initialize md5 structure
	MD5Init(&md5ctx);

	//update/compute md5
	while( !_v2_ptrFile->atEnd() ){		
		read = _v2_ptrFile->read( (char*)buffer, bufferLng);
		MD5Update( &md5ctx, buffer, read);
	}

	//convert context to md5 "byte array"
	MD5Final( md5bts, &md5ctx);

	//translation into string
	for( int i = 0 ; i < 16; i++ ){
		//convertion uses method for label text creation
		_v2_md5.append(this->v2_charToCellLabelHexa(md5bts[i]));
	}

	//seek file descriptor to begin
	_v2_ptrFile->seek(0);

	return;
}

QString FileWidget::v2_charToCellLabelHexa(unsigned char input){

	//translation char to hexa number
	QString hexaNum = "";
	bool append = false;

	if(input < 0x10){
		append = true;
	}

	hexaNum = (hexaNum.setNum(input, 0x10)).toUpper();

	if(append){
		hexaNum = "0" + hexaNum;
	}

	return hexaNum;
}

QString FileWidget::v2_charToCellLabelText(unsigned char input){

	//translation char to hexa number
	QString character = "";
	bool transform = false;

	if( ((input >= 0x00) && (input < 0x20)) ||
		((input >= 0x80) && (input <= 0xA0))
	){
		transform = true;
	}

	if(transform){
		character = ".";
	}
	else{
		character = input;
	}

	return character;
}

QString FileWidget::v2_charToCellLabelAddress(unsigned int input, bool view){

	QString hexaAddress = "";
	hexaAddress = hexaAddress.setNum(input, 0x10).toUpper();
	hexaAddress = 
		QString(ADDR_LABEL_WIDTH - hexaAddress.size(),'0') + hexaAddress;

	if(view){
		hexaAddress += ":";
	}

	return hexaAddress;
}

void FileWidget::v2_createBytes(int seek){

	_v2_bytes.clear();

	_v2_ptrFile->seek(seek);
	_v2_file = seek;	//set up start up position in file

	//ptrBuffer = (char*)malloc(_v2_cellsNum * sizeof(unsigned char));
	_v2_bytes = _v2_ptrFile->read(_v2_cellsNum);

	if((_v2_bytes.count() == 0) && (!_v2_ptrFile->atEnd())){
		throw FileWidgetException("Error during read from file");
	}

	return;
}

void FileWidget::v2_createIndexes(QByteArray bytes, int offset, int file){

	FileWidget::t_IdxsIt indexItem;
	bool nonEmptyCell = false;

	//delete previous content
	_v2_ptrIdxs->clear();

	for( int i = 0; i < _v2_cellsNum; i++ ){
		
		//switch on/off 
		if(i == offset){
			nonEmptyCell = true;
		}
		else if( (i - offset) >= bytes.count() ){			
			nonEmptyCell = false;
		}
		
		//fill up indexes
		indexItem.viewer = i;
		
		if(nonEmptyCell){			
			indexItem.file = file;
			indexItem.bytes = (i - offset);
			file++;
		}
		else{			
			indexItem.file = -1;
			indexItem.bytes = -1;
		}
		
		_v2_ptrIdxs->push_back(indexItem);
	}

	return;
}

void FileWidget::v2_createModels(void){

	//indexes count is different from viewed cells
	if(_v2_ptrIdxs->count() != _v2_cellsNum){
		throw FileWidget::FileWidgetException(
			"Indexes items count is different from cells number.");
	}

	FileWidget::t_IdxsIt indexItem;
	QModelIndex cellIndex;
	int cellLabelInt = -1;
	int currentAddress = _v2_startAddress, seqIdx = 0;	
	unsigned char cellLabelChar = '\0';
	QString cellLabel = "";
	QStandardItemModel* ptrItemModel;

	//scrollbar value
	_v2_scrollBarValue = (_v2_startAddress + _v2_scrollFloatBuffer) / _v2_cols;
	_v2_scrollFloatBuffer = (_v2_startAddress + _v2_scrollFloatBuffer) % _v2_cols;

	//cell setttings
	QMap<int, QVariant> cellSet;
	cellSet.insert(Qt::TextAlignmentRole, QVariant(Qt::AlignHCenter | Qt::AlignVCenter));
	cellSet.insert(Qt::BackgroundRole, QVariant(QBrush(EMPTY_COLB)));
	cellSet.insert(Qt::ForegroundRole, QVariant(QBrush(EMPTY_COLF)));	

	//data content has to empty
	_v2_ptrDataContent->clear();	

	for(int row = 0; row < _v2_rows; row++){

		//ADDRESS
		cellSet.insert(Qt::DisplayRole, 
			QVariant(this->v2_charToCellLabelAddress(currentAddress)));
		cellIndex = _v2_ptrAddressModel->index(row, 0);
		_v2_ptrAddressModel->setItemData(cellIndex, cellSet);
		_v2_ptrAddressModel->itemFromIndex(cellIndex)->setFlags(Qt::NoItemFlags);

		for(int col = 0; col < _v2_cols; col++){

			int seqIdx = (row*_v2_cols) + col;

			//non empty cell
			if(_v2_ptrIdxs->at(seqIdx).bytes != -1){

				cellLabelChar = _v2_bytes.at(_v2_ptrIdxs->at(seqIdx).bytes);
				
				//DATA CONTENT
				_v2_ptrDataContent->push_back(cellLabelChar);

				//HEXA
				cellSet.insert(Qt::DisplayRole,
					QVariant(this->v2_charToCellLabelHexa(cellLabelChar)));
				cellIndex = _v2_ptrHexaModel->index(row, col);
				_v2_ptrHexaModel->setItemData(cellIndex, cellSet);
				_v2_ptrHexaModel->itemFromIndex(cellIndex)->setFlags(Qt::NoItemFlags);

				//TEXT
				cellSet.insert(Qt::DisplayRole,
					QVariant(this->v2_charToCellLabelText(cellLabelChar)));
				cellIndex = _v2_ptrTextModel->index(row, col);
				_v2_ptrTextModel->setItemData(cellIndex, cellSet);
				_v2_ptrTextModel->itemFromIndex(cellIndex)->setFlags(Qt::NoItemFlags);
			}
			else{

				//DATA CONTENT
				_v2_ptrDataContent->push_back(-1);

				//HEXA
				cellSet.insert(Qt::DisplayRole, QVariant(""));
				cellIndex = _v2_ptrHexaModel->index(row,col);
				_v2_ptrHexaModel->setItemData(cellIndex, cellSet);
				_v2_ptrHexaModel->itemFromIndex(cellIndex)->setFlags(Qt::NoItemFlags);

				//TEXT
				cellSet.insert(Qt::DisplayRole, QVariant(""));
				cellIndex = _v2_ptrTextModel->index(row,col);
				_v2_ptrTextModel->setItemData(cellIndex, cellSet);
				_v2_ptrTextModel->itemFromIndex(cellIndex)->setFlags(Qt::NoItemFlags);
			}

			//new row -> new address
			currentAddress++;
		}
	}

	return;
}

void FileWidget::v2_updateModels(void){

	qDebug() << "update model address: " << _v2_startAddress;

	//indexes count is different from viewed cells
	if(_v2_ptrIdxs->count() != _v2_cellsNum){
		throw FileWidget::FileWidgetException(
			"Indexes items count is different from cells number.");
	}

	FileWidget::t_IdxsIt indexItem;
	QModelIndex cellIndex, hexaIndex, textIndex;
	int currentAddress = _v2_startAddress, seqIdx = 0;
	
	_v2_scrollBarValue = (_v2_startAddress + _v2_scrollFloatBuffer) / _v2_cols;
	_v2_scrollFloatBuffer = (_v2_startAddress + _v2_scrollFloatBuffer) % _v2_cols;
	
	//save patterns check
	bool savePatternsEnable = false;
	if((_v2_ptrSaveIntervals != NULL) && (_v2_ptrSaveIntervals->count() > 0) &&
		(_ptrPE != NULL)){
		savePatternsEnable = true;
	}
	
	for(int row = 0; row < _v2_rows; row++){

		//ADDRESS
		cellIndex = _v2_ptrAddressModel->index(row, 0);

		//virtual address view
		if(_v2_imageAddressEnable){
			
			//address model label
			_v2_ptrAddressModel->setData(cellIndex,
				QVariant(this->v2_charToCellLabelAddress(
					this->v2_physicalToImageAddress(currentAddress))),
				Qt::DisplayRole);

		}
		else{

			//address model label
			_v2_ptrAddressModel->setData(cellIndex,
				QVariant(this->v2_charToCellLabelAddress(currentAddress)),
				Qt::DisplayRole);
		}

		for(int col = 0; col < _v2_cols; col++){

			int seqIdx = (row*_v2_cols) + col;
			hexaIndex = _v2_ptrHexaModel->index(row,col);
			textIndex = _v2_ptrTextModel->index(row,col);

			//non empty cell
			if(_v2_ptrIdxs->at(seqIdx).bytes != -1){
				
				//cell coloring based on save pattern vissibility existence
				if(savePatternsEnable){

					if(this->v2_savePatternAddressVisible(currentAddress)){						
						
						//hexa model coloring
						_v2_ptrHexaModel->setData(hexaIndex,
							QVariant(QBrush(SAVE_COLB)), Qt::BackgroundRole);
						_v2_ptrHexaModel->setData(hexaIndex,
							QVariant(QBrush(SAVE_COLF)), Qt::ForegroundRole);

						//text model coloring
						_v2_ptrTextModel->setData(textIndex,
							QVariant(QBrush(SAVE_COLB)), Qt::BackgroundRole);
						_v2_ptrTextModel->setData(textIndex,
							QVariant(QBrush(SAVE_COLF)), Qt::ForegroundRole);
					}
					else{

						//hexa model coloring
						_v2_ptrHexaModel->setData(hexaIndex,
							QVariant(QBrush(EMPTY_COLB)), Qt::BackgroundRole);
						_v2_ptrHexaModel->setData(hexaIndex,
							QVariant(QBrush(EMPTY_COLF)), Qt::ForegroundRole);

						//text model coloring
						_v2_ptrTextModel->setData(textIndex,
							QVariant(QBrush(EMPTY_COLB)), Qt::BackgroundRole);
						_v2_ptrTextModel->setData(textIndex,
							QVariant(QBrush(EMPTY_COLF)), Qt::ForegroundRole);
					}
				}
				else{
					
					//hexa model coloring
					_v2_ptrHexaModel->setData(hexaIndex,
						QVariant(QBrush(EMPTY_COLB)), Qt::BackgroundRole);
					_v2_ptrHexaModel->setData(hexaIndex,
						QVariant(QBrush(EMPTY_COLF)), Qt::ForegroundRole);

					//text model coloring
					_v2_ptrTextModel->setData(textIndex,
						QVariant(QBrush(EMPTY_COLB)), Qt::BackgroundRole);
					_v2_ptrTextModel->setData(textIndex,
						QVariant(QBrush(EMPTY_COLF)), Qt::ForegroundRole);
				}
				
				unsigned char cellLabelChar =
					_v2_bytes.at(_v2_ptrIdxs->at(seqIdx).bytes);
				
				//DATA CONTENT
				_v2_ptrDataContent->replace(seqIdx,cellLabelChar);

				//hexa model label
				_v2_ptrHexaModel->setData(hexaIndex, 
					QVariant(this->v2_charToCellLabelHexa(cellLabelChar)),
					Qt::DisplayRole);

				//text model label
				_v2_ptrTextModel->setData(textIndex, 
					QVariant(this->v2_charToCellLabelText(cellLabelChar)),
					Qt::DisplayRole);
			}
			else{

				//hexa model coloring
				_v2_ptrHexaModel->setData(hexaIndex,
					QVariant(QBrush(EMPTY_COLB)), Qt::BackgroundRole);
				_v2_ptrHexaModel->setData(hexaIndex,
					QVariant(QBrush(EMPTY_COLF)), Qt::ForegroundRole);

				//text model coloring
				_v2_ptrTextModel->setData(textIndex,
					QVariant(QBrush(EMPTY_COLB)), Qt::BackgroundRole);
				_v2_ptrTextModel->setData(textIndex,
					QVariant(QBrush(EMPTY_COLF)), Qt::ForegroundRole);
				
				//DATA CONTENT
				_v2_ptrDataContent->replace(seqIdx,-1);				

				//hexa model label
				_v2_ptrHexaModel->setData(hexaIndex, 
					QVariant(""),
					Qt::DisplayRole);

				//text model label
				_v2_ptrTextModel->setData(textIndex, 
					QVariant(""),
					Qt::DisplayRole);
			}

			//new row -> new address
			currentAddress++;
		}
	}

	return;
}

void FileWidget::v2_reallocateModels(void){

	int oldRows = _v2_ptrHexaModel->rowCount();
	int oldCellsNum = _v2_cols * oldRows;

	int diffCells = _v2_cellsNum - oldCellsNum;
	int	diffRows = abs(_v2_rows - oldRows);

	qDebug() << "df: " << diffRows << " ol: " << oldRows <<
		" v2: " << _v2_rows << " diffCells: " << diffCells;

	if(diffCells == 0){
		return;
	}

	//smaller - dealocate
	if(diffCells < 0){

		//recompute offset if resize is to big
		if(_v2_offset > (oldCellsNum - (_v2_cols*diffRows))){
			_v2_offset = _v2_cellsNum - (_v2_offset % _v2_cols);
		}

		//remove rows in all models
		_v2_ptrHexaModel->removeRows(_v2_rows - diffRows, diffRows);
		_v2_ptrTextModel->removeRows(_v2_rows - diffRows, diffRows);
		_v2_ptrAddressModel->removeRows(_v2_rows - diffRows, diffRows);

		qDebug() << "new rows!! " << _v2_ptrHexaModel;

		for(int cell = _v2_rows*_v2_cols; cell < oldRows*_v2_cols; cell++){
			_v2_ptrDataContent->removeLast();
		}
	}
	//bigger
	else if(diffCells > 0){

		//inserts row in all models
		_v2_ptrHexaModel->insertRows(oldRows, diffRows);
		_v2_ptrTextModel->insertRows(oldRows, diffRows);
		_v2_ptrAddressModel->insertRows(oldRows, diffRows);

		//cell setttings
		QMap<int, QVariant> cellSet;
		cellSet.insert(Qt::TextAlignmentRole, QVariant(Qt::AlignHCenter | Qt::AlignVCenter));
		cellSet.insert(Qt::BackgroundRole, QVariant(QBrush(EMPTY_COLB)));
		cellSet.insert(Qt::ForegroundRole, QVariant(QBrush(EMPTY_COLF)));

		QModelIndex addressIndex, hexaIndex, textIndex;

		//TODO: set up new rows
		for(int row = oldRows; row < _v2_rows; row++){

			addressIndex = _v2_ptrAddressModel->index(row, 0);
			_v2_ptrAddressModel->setItemData(addressIndex, cellSet);

			for(int col = 0; col < _v2_cols; col++){

				hexaIndex = _v2_ptrHexaModel->index(row, col);
				textIndex = _v2_ptrTextModel->index(row, col);

				_v2_ptrHexaModel->setItemData(hexaIndex, cellSet);
				_v2_ptrTextModel->setItemData(textIndex, cellSet);

				_v2_ptrDataContent->push_back(-1);
			}
		}		
	}

	qDebug() << "data content: " << _v2_ptrDataContent->size();

	this->v2_createBytes(_v2_file);
	this->v2_createIndexes(_v2_bytes, _v2_offset, _v2_file);	

	this->v2_updateModels();

	return;
}

bool FileWidget::v2_savePatternAddressVisible(int address){

	SaveParser::saveInterval_t sIntr;

	for( int i = 0; i < _v2_ptrSaveIntervals->count(); i++){

		//load storage item into temporary struct
		sIntr = _v2_ptrSaveIntervals->at(i);

		//check if address should be colorized
		if((sIntr.type == SaveParser::PA) && (address >= sIntr.start) &&
			(address <= sIntr.stop)){
				return true;
		}
	}

	return false;
}

unsigned long FileWidget::v2_physicalToImageAddress(int physicalAddress){

	//chack PE validity
	if((_ptrPE == NULL) && (_ptrPE->OH_FileType() == PEbrowser::UNKNOWN)){
		throw FileWidget::FileWidgetException(
					"Not PE file, this operation can not be proceed.");
	}

	unsigned long imageAddress = _ptrPE->OH_ImageBase() + physicalAddress;
	PEbrowser::Section_t section;
	std::vector<PEbrowser::Section_t> sections;

	//no section exists
	if((sections = _ptrPE->Sections()).size() == 0){
		return imageAddress;
	}

	//in overlay
	if(physicalAddress >= _ptrPE->Overlay()){
		return physicalAddress;
	}

	//select proper address from section
	for(int secIter = 0; secIter < sections.size(); secIter++){

		section = sections[secIter];

		//zero size of raw data
		if(section.sizeOfRawData == 0){
			continue;
		}
		else if(physicalAddress < section.pointerToRawData){
			break;
		}
		else{
			imageAddress =
				_ptrPE->OH_ImageBase() + section.virtualAddress + 
				(physicalAddress - section.pointerToRawData);
		}
	}

	return imageAddress;
}

QString FileWidget::v2_draggedAddress(int seqDrag){

	QString hexaPrefix = "0x";
	
	int draggedAddress = _v2_startAddress + (seqDrag - _v2_offset);
	QString draggedAddressStr = "";

	if(_v2_imageAddressEnable){
		draggedAddressStr =  this->v2_charToCellLabelAddress(
								this->v2_physicalToImageAddress(draggedAddress), false);
	}
	else{
		draggedAddressStr =  this->v2_charToCellLabelAddress(draggedAddress, false);
	}

	return hexaPrefix + draggedAddressStr;
}

int FileWidget::v2_binarySearch(QString pattern){

	//size check
	if( (pattern.size() % 2) != 0 ){
		return -1;
	}

	unsigned char byte = '\0';
	QString byteCouple = "";
	QByteArray patternByte;
	bool conversionCorrent = false;

	for( int i = 0; i < pattern.size(); i+=2 ){

		byteCouple = pattern[i];
		byteCouple.append( pattern[i+1]);

		byte = (char)byteCouple.toInt(&conversionCorrent, 0x10);

		//conversion failed
		if(!conversionCorrent){
			return -1;
		}

		patternByte.append(byte);
	}
	
	int seek = 0, idx = 0;
	_v2_ptrFile->seek(seek);
	QByteArray read, flowWindow;
	
	//go through file
	while(!_v2_ptrFile->atEnd()){
		
		//read from file
		read = _v2_ptrFile->read(DATA_BLOCK);

		//empty buffer
		if(read.size() <= 0){
			return -1;
		}

		//append data into flow window
		flowWindow.append(read);

		//cut begin of flow window
		if(flowWindow.size() > (2*DATA_BLOCK)){
			flowWindow.remove(0, DATA_BLOCK);
		}		

		//check if pattern is contained
		if((idx = flowWindow.indexOf(patternByte)) >= 0){

			//begin of file
			if(seek == 0){
				return idx;
			}
			//second data block from flowing window
			else if(idx > DATA_BLOCK){
				return seek - DATA_BLOCK + idx;
			}
			//first data block from flowinf window
			else{				
				return seek + idx;				
			}			
		}

		//setup new seek point
		seek += read.size();
	}

	//reset to previous seeked position
	_v2_ptrFile->seek(_v2_file);
	
	return -1;
}

void FileWidget::v2_propagateStep(int shift){

	//with no shift no need to refresh
	if(shift == 0){
		return;
	}

	bool zeroOffset = false;	//true when offset is zero by computation
	int auxiliaryShift = 0;		//set number when offset is negative and 
								//any positive shift exists
	int offsetOld = _v2_offset;	//backup old offset	
	
	//OFFSET computing
	_v2_offset = _v2_offset + (shift * (-1));	//update offset

	if( _v2_offset == 0 ){						//computed zero		
		zeroOffset = true;		
	}
	// zero offset or file pointer bigger than proceeded shift
	else if((_v2_offset < 0) || (_v2_file >= abs(shift))){
		
		if( _v2_offset < 0 ){
			auxiliaryShift = abs(shift) + _v2_offset;
		}
		_v2_offset = 0;
	}
	else if((_v2_offset >= _v2_cellsNum) && (_v2_file != 0)){
		_v2_offset -= _v2_file;
	}
	else if(_v2_offset >= _v2_cellsNum){
		_v2_offset = _v2_cellsNum - 1;		
	}
	else{
		_v2_offset -= _v2_file;
	}

	//SEEK computing
	int seek = _v2_file + shift - auxiliaryShift;	
	
	if((_v2_offset > 0) || (zeroOffset == true)){
		seek = 0;		
	}
	else if(seek < 0){
		seek = 0;		
	}
	else if(seek >= _v2_fileSize){
		seek = shift - offsetOld;		
	}
	else{}

	this->v2_createBytes(seek);
	this->v2_createIndexes(_v2_bytes,_v2_offset,seek);

	return;
}

bool FileWidget::v2_validShift(int shift){

	//zero shift
	if(shift == 0){		
		return false;
	}
	//shift positive - top level
	else if((shift > 0) && ((_v2_offset + _v2_fileSize - _v2_file) <= shift)){		
		return false;		
	}
	//shift negative - bottom level
	else if((shift < 0) && ((_v2_file == 0) && (_v2_offset + abs(shift) >= _v2_cellsNum))){
		return false;
	}
	//other ways
	else{		
		return true;
	}
}