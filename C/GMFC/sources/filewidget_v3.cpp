#include "filewidget_v3.h"

FileWidget::FileWidgetException::FileWidgetException(QString msg) 
	: std::exception(msg.toStdString().c_str()){}

FileWidget::FileWidget(QString filename, int rows, int cols, QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	_dType = FileWidget::NOT_SET;
	_ptrPE = NULL;
	_fileName = filename.toStdWString();
	_fName = filename;
	_raw_file_path = this->parse_raw_file_path();
	
	_focusFlag = false;
	
	_pActDialog = NULL;

	_binarySearchSequence = "";
	_binarySearchSeek = 0;

	_textSearchSequence = "";
	_textSearchSeek = 0;

	_searchType = FileWidget::NOT_USED;
	
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
				
		_v2_ptrDataContent = new QList<int>();
		_v2_ptrSaveIntervals = new QList<SaveParser::saveInterval_t>();
		
		//program processing data load, PE scan, etc
		this->executeProcessInternals();

		int scrollMin = (_v2_rows*(-1)) +1;
		int pageStep = _v2_rows;
		int singleStep = 1;
		int scrollMax = _v2_fileSize/_v2_cols -1;
		if(_v2_fileSize % _v2_cols > 0){
			scrollMax++;
		}

		//base frame creation
		_base = new QFrame(this);	

		//address bar creation
		_address = new AddressViewer(
							&_v3_addressLabelForm,
							&_v3_addressDataForm,
							_fName,
							this);
		_address->configure();

		//hexa viewer allocation & configuration
		_hexa = new HexaViewer(
							&_v3_hexaLabelForm,
							&_v3_hexaDataForm,
							_fName,
							this);
		_hexa->configure();		
		
		//text viewer allocation & configuration
		_text = new TextViewer(
							&_v3_textLabelForm,
							&_v3_textDataForm,
							_fName,
							this);
		_text->configure();

		//scroll bar creation
		_scroll = new ScrollBar(_hexa->height(), this);
		_scroll->configure(
			scrollMin,			//min
			scrollMax,			//max
			_v2_scrollBarValue,	//value
			singleStep,			//single step
			pageStep);			//page step
			
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
	catch(std::bad_alloc){

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

	//disconnect scrollBar due to double changes
	QObject::disconnect(_scroll, SIGNAL(valueChanged(int)),
		this, SLOT(slScrollChanged(int)));

	_address->updateView(&_v3_addressLabelForm, &_v3_addressDataForm);
	_hexa->updateView(&_v3_hexaLabelForm, &_v3_hexaDataForm);
	_text->updateView(&_v3_textLabelForm, &_v3_textDataForm);
	_scroll->setValue(_v2_scrollBarValue);

	//reconnect scrollBar
	QObject::connect(_scroll, SIGNAL(valueChanged(int)),
		this, SLOT(slScrollChanged(int)));	

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

void FileWidget::artificialResize(int diffH){

	//no differences, no need to resize
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
	_v2_rows -= rowDiff;	
	_v2_cellsNum = _v2_rows*_v2_cols;

	//reallocate models for all components
	this->v2_reallocateModels();

	//resize other build-in components
	_hexa->artificialResize(&_v3_hexaLabelForm, &_v3_hexaDataForm);
	_text->artificialResize(&_v3_textLabelForm, &_v3_textDataForm);
	_address->artificialResize(&_v3_addressLabelForm, &_v3_addressDataForm);
	_scroll->artificialResize(_hexa->height(), _v2_scrollBarValue);

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

	//view changed can be used also for proper display of dragged 
	//address during mouse press event, so shift will be zero and
	//there is no need to proceed further
	if(shift == 0){
		return;
	}

	this->v2_propagateStep(shift);
	_v2_startAddress += shift;

	//update data models
	this->v2_updateModels();

	//disconnect scrollBar due to double changes
	QObject::disconnect(_scroll, SIGNAL(valueChanged(int)),
		this, SLOT(slScrollChanged(int)));

	//update other components
	_address->updateView(&_v3_addressLabelForm, &_v3_addressDataForm);
	_text->updateView(&_v3_textLabelForm, &_v3_textDataForm);
	_hexa->updateView(&_v3_hexaLabelForm, &_v3_hexaDataForm);	
	_scroll->setValue(_v2_scrollBarValue);

	//reconnect scrollBar
	QObject::connect(_scroll, SIGNAL(valueChanged(int)),
		this, SLOT(slScrollChanged(int)));	

	//send data content to upper level - noisy type
	if(drag != -1){
		this->contentChanged(_fName, _v2_ptrDataContent);
	}

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

	try
	{		
		_ptrPE = new PEbrowser(_fileName);
		_ptrPE->ScanFile();
	}

	catch( PEbrowser::Exc_FileOpen ){		
		_tBarOpt.set(Toolbar::PE, false);		
	}
	catch( PEbrowser::Exc_MZ ){
		_tBarOpt.set(Toolbar::PE, false);
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
	catch( PEbrowser::Exc_NoResources ){}
	catch( PEbrowser::Exc_NotImplemented ){}
	catch(PEbrowser::Exc_VersionInfo){}

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
	pDialog->setWindowTitle(tr("Local binary search"));
	pDialog->setWindowIcon(
		QIcon::fromTheme("Binary search",
							QIcon(":/Toolbar/Resources/search.svg")));	
	pDialog->setLabelText(tr("Insert binary sequence [1-32 bytes]: "));
	pDialog->setTextValue(_binarySearchSequence);

	//when binary dialog is opened seek position is automatically rewind to zero
	_binarySearchSeek = 0;
	
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

		_binarySearchSequence = pDialog->textValue();

		if(_binarySearchSequence.length() > 0){
			_searchType = FileWidget::BINARY_LOCAL;
		}
		
		//store input value
		int foundedAddress = this->v2_binarySearch(_binarySearchSequence);
		if(foundedAddress != -1){

			_binarySearchSeek = foundedAddress + 1;
			
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

void FileWidget::createTextSearchDialog(void){

	//variable init
	QInputDialog* pDialog = new QInputDialog(this);
	pDialog->setModal(true);
	pDialog->setFixedSize(QSize(350,150));
	pDialog->setInputMode(QInputDialog::TextInput);
	pDialog->setWindowTitle(tr("Local text search"));
	pDialog->setWindowIcon(
		QIcon::fromTheme("Text search",
							QIcon(":/Toolbar/Resources/search_text.svg")));	
	pDialog->setLabelText(tr("Insert text sequence [1-32 chars]: "));
	pDialog->setTextValue(_binarySearchSequence);
	
	//when binary dialog is opened seek position is automatically rewind to zero
	_textSearchSeek = 0;
	
	//insert validator
	QRegExpValidator *pVld = 
		new QRegExpValidator(QRegExp(".{1,64}"),pDialog);
	((QLineEdit*)(pDialog->children())[0])->setValidator( pVld );
	
	//set up proceeding dialog
	_pActDialog = pDialog;
	_dType = FileWidget::TXT_SEARCH;

	//connect input validator's function
	QObject::connect( pDialog, SIGNAL(textValueChanged(QString)),
						this, SLOT(slCheckDialog(QString)));
	
	if(pDialog->exec()){

		_textSearchSequence = pDialog->textValue();

		if(_textSearchSequence.length() > 0){
			_searchType = FileWidget::TEXT_LOCAL;
		}
		
		//store input value
		int foundedAddress = this->v2_textSearch(_textSearchSequence);
		if( foundedAddress != -1 ){

			_textSearchSeek = foundedAddress + 1;
			
			int shift = foundedAddress - _v2_startAddress;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
		}
		else{
			
			QMessageBox notFound;
			notFound.setWindowIcon(QIcon::fromTheme("Error Icon",
					QIcon(":/Global/Resources/error.svg")));
			notFound.setWindowTitle(tr("Text search"));
			notFound.setText(tr("Text pattern was not found in this sample"));
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

void FileWidget::createSavePatternDialog(void)
{
	

	QStringList fNames = QFileDialog::getOpenFileNames(
		NULL,
		"Select save pattern file",
		_raw_file_path);
	
	if(fNames.size() > 0)
	{
		for(int i = 0; i < fNames.size(); i++){
			_ptrSaveParser->addSavePattern(fNames[i],_ptrPE);
		}
		_v2_ptrSaveIntervals = _ptrSaveParser->Intervals();
		
		//refresh models 
		this->v2_updateModels();
		_hexa->updateView(&_v3_hexaLabelForm, &_v3_hexaDataForm);
		_text->updateView(&_v3_textLabelForm, &_v3_textDataForm);
	}

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
	if( len > 40 ){
		return "***" + fName.right(40);
	}

	return fName;
}

QString FileWidget::parse_raw_file_path(void)
{
	//no file name at all
	if(_fName.length() <= 0){
		return "";
	}

	//find position of the last slash in string 
	int slash_pos = -1;
	if((slash_pos = _fName.lastIndexOf(QRegExp("[\\\\/]"))) < 0){
		return "";
	}

	return _fName.left(slash_pos+1);
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

	switch(ptrEvent->key()){

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

		case Qt::Key_F7:

			switch(_searchType){

				case FileWidget::TEXT_LOCAL:
					this->createTextSearchDialog();
					break;

				case FileWidget::BINARY_LOCAL:
				case FileWidget::NOT_USED:
				default:
					this->createBinarySearchDialog();
					break;
			}

			break;

		case Qt::Key_F3:

			switch(_searchType){

				case FileWidget::TEXT_LOCAL:
					this->repeatedTextSearch();
					break;

				case FileWidget::BINARY_LOCAL:
					this->repeatedBinarySearch();
					break;

				case FileWidget::NOT_USED:
				default:					
					break;
			}
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
	QHash<QAction*, Toolbar::ACTIONS>* ptrActions = NULL;
	QAction* ptrAction = NULL;

	switch( curAct ){
		
		case Toolbar::CLOSE:

			emit this->closing(QString::fromStdWString(_fileName));
			
			//parent not exists [standalone run]
			if( this->parent() == NULL ){
				this->close();
			}			
			break;

		case Toolbar::DISABLE:

			ptrActions = _tBar->Actions();
			ptrAction = ptrActions->key(Toolbar::DISABLE);
			if(ptrAction->iconText() == "disable sample")
			{
				ptrAction->setIcon(QIcon(":/Toolbar/Resources/enable.svg"));
				ptrAction->setText("enable sample");

				_title->setText(this->corFName() + " - DISABLED");
				_title->setStyleSheet(TITLE_STYLE_DISABLED);

				emit this->sgDisableSample(QString::fromStdWString(_fileName));
			}
			else
			{
				ptrAction->setIcon(QIcon(":/Toolbar/Resources/disable.svg"));
				ptrAction->setText("disable sample");

				_title->setText(this->corFName());
				_title->setStyleSheet(TITLE_STYLE);

				emit this->sgEnableSample(_fName,_v2_ptrDataContent,_v2_md5);
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
			_address->updateView(&_v3_addressLabelForm, &_v3_addressDataForm);
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

		case Toolbar::JMP_LEFT:
			
			shift = 1;
			if(this->v2_validShift(shift)){
				this->slViewChanged(shift,-1);
			}
			break;

		case Toolbar::JMP_RIGHT:
			
			shift = -1;
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

		case Toolbar::TXT_SEARCH:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			//text search processing
			this->createTextSearchDialog();
			break;

		case Toolbar::SAVE_PATTERN:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}
			
			//process save pattern dialog
			this->createSavePatternDialog();			
			break;

		case Toolbar::SAVE_PATTERN_FORWARD:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			this->jumpToSaveArea(FileWidget::FORWARD);
			break;

		case Toolbar::SAVE_PATTERN_BACKWARD:

			//set up focus
			if(!_focusFlag && !silent){
				emit this->sgFocusTaken(this);
			}

			this->jumpToSaveArea(FileWidget::BACKWARD);
			break;

		default:
			//show "not implemented" dialog
			NotImplemented::alert();
			break;
	}

	return;
}

void FileWidget::repeatedBinarySearch(void){

	if((_binarySearchSequence == "") ||
		(_binarySearchSequence.size() % 2 != 0)){
		return;
	}

	//store input value
	int foundedAddress = this->v2_binarySearch(_binarySearchSequence);
	if( foundedAddress != -1 ){

		_binarySearchSeek = foundedAddress + 1;

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
		notFound.setText(tr("No other pattern was found in this sample"));
		notFound.setIcon( QMessageBox::Warning );
		notFound.exec();
	}

	return;
}

void FileWidget::repeatedTextSearch(void){

	if(_textSearchSequence == ""){
		return;
	}

	//store input value
	int foundedAddress = this->v2_textSearch(_textSearchSequence);
	if( foundedAddress != -1 ){

		_textSearchSeek = foundedAddress + 1;

		int shift = foundedAddress - _v2_startAddress;
		if(this->v2_validShift(shift)){
			this->slViewChanged(shift,-1);
		}
	}
	else{
			
		QMessageBox notFound;
		notFound.setWindowIcon(QIcon::fromTheme("Error Icon",
				QIcon(":/Global/Resources/error.svg")));
		notFound.setWindowTitle(tr("Text search"));
		notFound.setText(tr("No other pattern was found in this sample"));
		notFound.setIcon( QMessageBox::Warning );
		notFound.exec();
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

QString FileWidget::v2_charToCellLabelText(unsigned char input, bool richEnable){

	//translation char to hexa number
	QString character = "";
	bool transform = false;

	if(richEnable){

		///special cases
		switch(input){
		
			case 0x3c:
				character = "&lt;";
				break;

			case 0xa1:
				character = "&iexcl;";
				break;

			case 0xa2:
				character = "&cent;";
				break;

			case 0xa3:
				character = "&pound;";
				break;

			case 0xa4:
				character = "&curren;";
				break;

			case 0xa5:
				character = "&yen;";
				break;

			case 0xa7:
				character = "&sect;";
				break;

			case 0xa8:
				character = "&copy;";
				break;

			case 0xaa:
				character = "&ordf;";
				break;

			case 0xab:
				character = "&laquo;";
				break;

			case 0xac:
				character = "&not;";
				break;

			case 0xae:
				character = "&reg;";
				break;

			case 0xb0:
				character = "&deg;";
				break;

			case 0xb1:
				character = "&plusmn;";
				break;

			case 0xb5:
				character = "&micro;";
				break;

			case 0xb6:
				character = "&para;";
				break;

			case 0xba:
				character = "&ordm;";
				break;

			case 0xbb:
				character = "&raquo;";
				break;

			case 0xbc:
				character = "&frac14;";
				break;

			case 0xbd:
				character = "&frac12;";
				break;

			case 0xbe:
				character = "&frac34;";
				break;

			case 0xbf:
				character = "&iquest;";
				break;

			default:
				character = "";
		}
	}

	if(character.length() > 0){
		return character;
	}
	

	//transform border
	if((input <= 0x20) || (input > 0x7E) || (input == 0x3c)){
		transform = true;
	}

	//return proper value
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
	int cellLabelInt = -1;
	int currentAddress = _v2_startAddress, seqIdx = 0;	
	unsigned char cellLabelChar = '\0';
	QString cellLabel = "";
	bool inSavePattern = false;

	//save patterns check
	bool savePatternsEnable = false;
	if((_v2_ptrSaveIntervals != NULL) && (_v2_ptrSaveIntervals->count() > 0) &&
		(_ptrPE != NULL)){
		savePatternsEnable = true;
	}

	//scrollbar value
	_v2_scrollBarValue = (_v2_startAddress + _v2_scrollFloatBuffer) / _v2_cols;
	_v2_scrollFloatBuffer = (_v2_startAddress + _v2_scrollFloatBuffer) % _v2_cols;

	//data content has to empty
	_v2_ptrDataContent->clear();
	
	//data form clear
	_v3_addressDataForm.clear();
	_v3_hexaDataForm.clear();
	_v3_textDataForm.clear();

	//label form clear
	_v3_addressLabelForm = "";
	_v3_hexaLabelForm = "";
	_v3_textLabelForm = "";

	for(int row = 0; row < _v2_rows; row++){

		//image base requested
		if(_v2_imageAddressEnable){
			cellLabel = this->v2_charToCellLabelAddress(
				this->v2_physicalToImageAddress(currentAddress));
		}
		else{
			cellLabel = this->v2_charToCellLabelAddress(currentAddress);
		}

		_v3_addressDataForm.push_back(cellLabel);
		_v3_addressLabelForm += cellLabel + "<br/>";
		
		for(int col = 0; col < _v2_cols; col++){

			seqIdx = (row*_v2_cols) + col;

			//non empty cell
			if(_v2_ptrIdxs->at(seqIdx).bytes != -1){

				cellLabelChar = _v2_bytes.at(_v2_ptrIdxs->at(seqIdx).bytes);
				
				//data content
				_v2_ptrDataContent->push_back(cellLabelChar);

				//start of save pattern
				if(savePatternsEnable && !inSavePattern &&
					this->v2_savePatternAddressVisible(currentAddress)){

					inSavePattern = true;

					//hexa
					cellLabel = this->v2_charToCellLabelHexa(cellLabelChar);
					_v3_hexaDataForm.push_back(cellLabel);
					_v3_hexaLabelForm += LABEL_STYLE_OPEN(SAVE_STYLE_INLINE) + cellLabel;
						

					//text
					cellLabel = this->v2_charToCellLabelText(cellLabelChar);
					_v3_textDataForm.push_back(cellLabel);
					_v3_textLabelForm += LABEL_STYLE_OPEN(SAVE_STYLE_INLINE) + cellLabel;						
				}
				//end of save pattern
				else if(savePatternsEnable && inSavePattern &&
					!this->v2_savePatternAddressVisible(currentAddress)){

					inSavePattern = false;

					//hexa
					cellLabel = this->v2_charToCellLabelHexa(cellLabelChar);
					_v3_hexaDataForm.push_back(cellLabel);
					_v3_hexaLabelForm += LABEL_STYLE_CLOSE + cellLabel;

					//text
					cellLabel = this->v2_charToCellLabelText(cellLabelChar);
					_v3_textDataForm.push_back(cellLabel);
					_v3_textLabelForm += LABEL_STYLE_CLOSE + cellLabel;
				}
				//normal run
				else{

					//hexa
					cellLabel = this->v2_charToCellLabelHexa(cellLabelChar);
					_v3_hexaDataForm.push_back(cellLabel);
					_v3_hexaLabelForm += cellLabel;

					//text
					cellLabel = this->v2_charToCellLabelText(cellLabelChar);
					_v3_textDataForm.push_back(cellLabel);
					_v3_textLabelForm += cellLabel;
				}				
			}
			//empty cell
			else{

				//data content
				_v2_ptrDataContent->push_back(-1);

				//hexa
				_v3_hexaDataForm.push_back("");
				_v3_hexaLabelForm += "&nbsp;&nbsp;";

				//text
				_v3_textDataForm.push_back("");
				_v3_textLabelForm += "&nbsp;";
			}

			//spaces between characters
			if((col+1) != _v2_cols){
				_v3_hexaLabelForm += "&nbsp;";
				//_v3_textLabelForm += "&nbsp;";
			}	

			//new row -> new address
			currentAddress++;
		}

		//end of line
		_v3_hexaLabelForm += "<br/>";
		_v3_textLabelForm += "<br/>";
	}

	return;
}

void FileWidget::v2_updateModels(void){

	this->v2_createModels();
	return;
}

void FileWidget::v2_reallocateModels(void){	

	this->v2_createBytes(_v2_file);
	this->v2_createIndexes(_v2_bytes, _v2_offset, _v2_file);

	this->v2_createModels();

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
	
	int draggedAddress = _v2_startAddress + seqDrag;
	QString draggedAddressStr = "";

	if(_v2_imageAddressEnable){
		draggedAddressStr =  
			this->v2_charToCellLabelAddress(
				this->v2_physicalToImageAddress(draggedAddress),
				false);
	}
	else{
		draggedAddressStr = 
			this->v2_charToCellLabelAddress(
				draggedAddress,
				false);
	}

	return hexaPrefix + draggedAddressStr;
}

int FileWidget::v2_binarySearch(QString pattern){

	//size check
	if( (pattern.length() == 0) || (pattern.size() % 2) != 0 ){
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
	
	int seek = _binarySearchSeek, idx = 0;
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

int FileWidget::v2_textSearch(QString pattern){
	
	//size check
	if(pattern.length() == 0){
		return -1;
	}

	QByteArray patternByte = pattern.toAscii();

	int seek = _textSearchSeek, idx = 0;
	QByteArray read, flowWindow;

	_v2_ptrFile->seek(seek);
	
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

int FileWidget::binarySearchExternally(QString sequence){

	_searchType = FileWidget::BINARY_LOCAL;
	_binarySearchSeek = 0;

	int searchedAddress = this->v2_binarySearch(sequence);

	_binarySearchSequence = sequence;
	_binarySearchSeek = searchedAddress + 1;

	if(searchedAddress != -1){

		int shift = searchedAddress - _v2_startAddress;
		if(this->v2_validShift(shift)){
			this->slViewChanged(shift,-1);
		}
	}

	return searchedAddress;
}

int FileWidget::textSearchExternally(QString sequence){

	_searchType = FileWidget::TEXT_LOCAL;
	_textSearchSeek = 0;
	
	int searchedAddress = this->v2_textSearch(sequence);
	
	_textSearchSequence = sequence;
	_textSearchSeek = searchedAddress + 1;
	
	if(searchedAddress != -1){

		int shift = searchedAddress - _v2_startAddress;
		if(this->v2_validShift(shift)){
			this->slViewChanged(shift,-1);
		}
	}

	return searchedAddress;
}

int FileWidget::directAddressExternally(QString address){

	bool ok = false;
	unsigned int addressNumeric = address.toUInt(&ok, 0x10);

	if(ok && (addressNumeric >= 0) && (addressNumeric < _v2_fileSize)){
				
		int shift = addressNumeric - _v2_startAddress;
		if(this->v2_validShift(shift)){
			this->slViewChanged(shift,-1);
		}
	}
	else{
		addressNumeric = -1;
	}

	return addressNumeric; 
}

void FileWidget::jumpToSaveArea(FileWidget::DIRECTION direction){

	if(_v2_ptrSaveIntervals->size() <= 0){
		return;
	}

	int saveIndex = -1, saveDistance = this->_v2_fileSize;
	int currentIndexSaveDistance = 0;
	SaveParser::saveInterval_t saveInterval;

	for(int i = 0; i < _v2_ptrSaveIntervals->size(); i++){

		saveInterval = _v2_ptrSaveIntervals->at(i);
		currentIndexSaveDistance = abs(saveInterval.start - _v2_startAddress);

		switch(direction){

			case FileWidget::FORWARD:

				if(saveInterval.start > _v2_startAddress){
					if( currentIndexSaveDistance < saveDistance){
						saveIndex = i;
						saveDistance = currentIndexSaveDistance;
					}
				}
				break;

			case FileWidget::BACKWARD:

				if(saveInterval.start < _v2_startAddress){
					if( currentIndexSaveDistance < saveDistance){
						saveIndex = i;
						saveDistance = currentIndexSaveDistance;
					}
				}
				break;

			default:
				break;
		}
	}

	//nothing was found
	if(saveIndex < 0){
		
		QMessageBox notFound;
		notFound.setWindowIcon(QIcon::fromTheme("Error Icon",
				QIcon(":/Global/Resources/error.svg")));
		notFound.setWindowTitle(tr("Jump to save area"));
		notFound.setText(tr("No other save area in this direction."));
		notFound.setIcon( QMessageBox::Warning );
		notFound.exec();
		return;
	}

	int shift = _v2_ptrSaveIntervals->at(saveIndex).start - _v2_startAddress;
	if(this->v2_validShift(shift)){
		this->slViewChanged(shift,-1);
	}

	return;
}