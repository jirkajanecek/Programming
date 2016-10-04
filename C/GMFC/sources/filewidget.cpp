#include "filewidget.h"

FileWidget::FileWidget(QString filename, int rows, int cols, QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	_dType = FileWidget::NOT_SET;
	_pe = NULL;
	_fileName = filename.toStdWString();
	
	_focus = false;
	_perm = false;

	_pActDialog = NULL;

	_floatResBufferH = 0;

	try{
	
		//save pattern parser object allocation
		_ptrSaveParser = new SaveParser();
		
		//program processing for file name
		this->executeProcessInternals();

		//base frame creation
		_base = new QFrame(this);	

		//address bar creation
		_address = new AddressBar(0, rows, this, _pe);
		_address->configure();

		//hexa viewer allocation & configuration
		_hexa = new HexaViewer(filename, rows, cols, this);
		_hexa->configure();
		_hexa->setPE(_pe);
		
		//text viewer allocation & configuration
		_text = new TextViewer(filename, rows, cols, this);
		_text->configure();
		_text->setPE( _pe );
		
		//scroll bar creation
		_scroll = new ScrollBar(_hexa->height(), this);
		_scroll->configure(((-1)*_hexa->CellsNumber())+1,_hexa->FileSize() -1, 0, 
			_hexa->CellsHorizontal(), _hexa->CellsNumber());

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

	if(_pe != NULL){
		delete _pe;
	}

}

bool FileWidget::Focus( void ){
	return _focus;
}

QString FileWidget::FileName( void ){

	return QString::fromStdWString(_fileName);
}

QString FileWidget::MD5( void ){
	return _hexa->MD5();
}

QList<Viewer::IndexItem_t*>* FileWidget::Indexes( void ){

	return _hexa->Indexes();
}

QList<int>* FileWidget::DataContent( void ){

	return _hexa->DataContent();
}

QDialog* FileWidget::ActiveDialog( void ){

	return _pActDialog;
}

void FileWidget::configure( void ){
	
	//install event filter for scrollBar
	_scroll->installEventFilter(this);

	//set up save patterns for hexa and text viewers
	this->setSaveIntervals( _ptrSaveParser->Intervals());

	this->placeComponents();

	this->connectComponents();

	return;
}

void FileWidget::setFocus( bool perm ){

	_base->setStyleSheet(FWGT_FOC_COL );

	_focus = true;

	if( perm ){
		
		_perm = true;
	}
	
	return;
}

void FileWidget::setBlur( bool perm ){

	//switch off permanent focus
	if( perm ){

		_perm = false;

		_focus = false;

		_base->setStyleSheet(FWGT_BLUR_COL);
	}

	//switch off focus when widget is not permanently focused
	if( !_perm ){

		_focus = false;

		_base->setStyleSheet(FWGT_BLUR_COL);
	}

	return;
}

void FileWidget::setSaveIntervals( QList<SaveParser::saveInterval_t>* intrvs){

	_hexa->setSaveIntervals( intrvs );

	_text->setSaveIntervals( intrvs );

	return;
}

int FileWidget::currentRows(int height){

	return (height - (TITLE_HEIGHT + TOOL_HEIGHT + 20)) / CELL_H;
}

void FileWidget::sliderChanged(int action){

	int sVal = 0;
	
	if( !this->Focus() ){
		
		//emit focus request
		emit this->focusReq(this);
	}

	switch( action ){

		//Home
		case QAbstractSlider::SliderToMinimum:

			//this->Output("#0");
			emit this->setComponents(0);

			//artificially set up slider
			_scroll->setValue(0);

			break;

		//End
		case QAbstractSlider::SliderToMaximum:

			//this->Output("#1");
			emit this->setComponents(_hexa->FileSize() - _hexa->CellsNumber());

			//artificially set up slider
			_scroll->setValue( _hexa->FileSize() - _hexa->CellsNumber() );

			break;

		//No Action
		case QAbstractSlider::SliderNoAction:

			break;

		//Down Arrow [keyboard & click]
		case QAbstractSlider::SliderSingleStepAdd:
			
			if( _hexa->isValidShift( _hexa->CellsHorizontal() )){
				
				//this->Output("#2+");
				emit this->shiftComponents( _hexa->CellsHorizontal() );

				//manually set up scrollbar value
				_scroll->setValue( _scroll->value() + _hexa->CellsHorizontal() );
			}
			else{
				
				//this->Output("#2-");
				_scroll->setSliderPosition(_scroll->value());
			}
			break;

		//Up Arrow [keyboard & click]
		case QAbstractSlider::SliderSingleStepSub:
			
			if( _hexa->isValidShift( _hexa->CellsHorizontal()*(-1) )){
				
				//this->Output("#3+");
				emit this->shiftComponents( _hexa->CellsHorizontal()*(-1) );

				//manually set up scrollbar value
				_scroll->setValue( _scroll->value() - _hexa->CellsHorizontal() );
			}
			else{
				
				//this->Output("#3-");
				_scroll->setSliderPosition(_scroll->value());
			}
			break;

		//Page Down
		case QAbstractSlider::SliderPageStepAdd:
			
			if( _hexa->isValidShift( _hexa->CellsNumber() )){
				
				//this->Output("#4+");
				emit this->shiftComponents( _hexa->CellsNumber() );

				//manually set up scrollbar value
				_scroll->setValue( _scroll->value() + _hexa->CellsNumber() );
			}
			else{
				
				//this->Output("#4-");
				_scroll->setSliderPosition(_scroll->value());
			}
			break;

		//Page Up
		case QAbstractSlider::SliderPageStepSub:
			
			if( _hexa->isValidShift( _hexa->CellsNumber()*(-1) )){
				
				//this->Output("#5+");
				emit this->shiftComponents( _hexa->CellsNumber()*(-1) );

				//manually set up scrollbar value
				_scroll->setValue( _scroll->value() - _hexa->CellsNumber() );
			}
			else{
				
				//this->Output("#5-");
				_scroll->setSliderPosition(_scroll->value());
			}
			break;

		//Scroll event, Drag
		case QAbstractSlider::SliderMove:

			//get value of scroll bar
			sVal = _scroll->value();

			//emit signal to other components
			emit this->setComponents(_scroll->value());

			break;

		default:
			break;
	}
	
	return;
}

void FileWidget::sliderPressed( void ){

	if( !this->Focus() ){
	
		//emit focus request
		emit this->focusReq(this);
	}

	return;
}

void FileWidget::slTBarActivated( QAction* action ){
	
	QHash<QAction*, Toolbar::ACTIONS>* actTab = _tBar->Actions();
	Toolbar::ACTIONS curAct = actTab->value(action);

	this->execTBarAction( curAct );

	return;
}

void FileWidget::slTBarActivated( Toolbar::ACTIONS action){

	this->execTBarAction( action );
	
	return;
}

void FileWidget::processDialog( QString value ){

	bool ok;
	int hex = 0;

	switch( _dType ){

		//jump to target address
		case FileWidget::TARGET_ADDRESS:
			
			//store input value
			hex = value.toInt(&ok, 16);

			//border check
			if( ok && (hex >= 0) && (hex <= _hexa->FileSize()-1) ){
				
				//send signal to all fileWidget's commponents
				emit this->setComponentsExternal( hex );
			}

			break;

		default:
			break;
	}

	return;
}

void FileWidget::checkDialog( QString value ){

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

			if( !ok || (hex < 0) || (hex > _hexa->FileSize()-1) ){				
				
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

void FileWidget::contentChanged( QString fName, QList<int> *dataContent){

	emit this->contentIsChanging( fName, dataContent );
	
	return;
}

void FileWidget::contentChangedSilent( QString fName, QList<int> *dataContent){

	emit this->contentIsChangingSilent( fName, dataContent );
	
	return;
}

void FileWidget::slDragInProgress( int seqIdx ){

	//get address from address bar
	QString addr = _address->getAddressFromSeqIndex(seqIdx);

	//emit address to upper level
	emit this->sgDragAtAddress(addr);

	return;
}

void FileWidget::slDragIsFinished( void ){

	//emit address to upper level
	emit this->sgDragAtAddress("");

	return;
}

void FileWidget::slArtificialResize(QResizeEvent* ptrEv){

	//difference in height
	int diffH = ptrEv->oldSize().height() - ptrEv->size().height();

	//no differences, no need to resize
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
	QSize roundedSize(ptrEv->size().width(), 
						(ptrEv->oldSize().height() - (roundedDiffH * CELL_H)));
	QResizeEvent internalEv(roundedSize, ptrEv->oldSize());

	qDebug() << "old Hexa H: " << _hexa->height() << " Hexa W: " << _hexa->width();

	//resize other build-in components
	_hexa->artificialResize(&internalEv);
	_text->artificialResize(&internalEv);
	_address->artificialResize(&internalEv, _hexa->absoluteTopValue());
	_scroll->artificialResize(&internalEv, _hexa->absoluteTopValue());

	qDebug() << "Hexa H: " << _hexa->height() << " Hexa W: " << _hexa->width();

	//send the content array to result widget [without refreshing]

	//resize widget itself
	_base->resize(_base->width(),
					_hexa->height() + 20 + _title->height() + _tBar->height());
	this->setFixedSize(_base->width(), _base->height());
	
	return;
}

void FileWidget::configureBaseFrame( void ){
	
	_base->resize(_address->width() + _hexa->width() + _text->width() + _scroll->width() + 25,
			_hexa->height() + _tBar->height() + _title->height() + 20);

	_base->setStyleSheet(FWIDGET_STYLE);

	return;
}

void FileWidget::placeComponents( void ){
	
	_title->move(5,5);
	_tBar->move(5, 30);	
	_address->move(5,63);
	_hexa->move(_address->x() + _address->width() + 5, _address->y());
	_text->move(_hexa->x() + _hexa->width() + 5, _hexa->y());
	_scroll->move(_text->x() + _text->width() + 5, _text->y());	

	return;
}

void FileWidget::connectComponents( void ){
	
	//scroll bar -> file widget - action happend
	QObject::connect(_scroll, SIGNAL(actionTriggered(int)),
						this, SLOT(sliderChanged(int)));

	//scroll bar -> file widget - just pressed
	QObject::connect(_scroll, SIGNAL(sliderPressed()),
						this, SLOT(sliderPressed()));

	//hexa widget -> text viewer
	QObject::connect(_hexa, SIGNAL(viewerIsChanging(int, int, int)),
						_text, SLOT(viewerChanged(int, int, int)));

	//hexa viewer -> address viewer
	QObject::connect(_hexa, SIGNAL(viewerIsChanging(int)),
						_address, SLOT(viewerChanged(int)));

	//hexa viewer -> scroll bar
	QObject::connect(_hexa, SIGNAL(viewerIsChangingAbs(int)),
						_scroll, SLOT(viewerChanged(int)));

	//text widget -> text viewer
	QObject::connect(_text, SIGNAL(viewerIsChanging(int, int, int)),
						_hexa, SLOT(viewerChanged(int, int, int)));

	//text viewer -> address viewer
	QObject::connect(_text, SIGNAL(viewerIsChanging(int)),
						_address, SLOT(viewerChanged(int)));

	//text viewer -> scroll bar
	QObject::connect(_text, SIGNAL(viewerIsChangingAbs(int)),
						_scroll, SLOT(viewerChanged(int)));

	//file widget [scroll bar] -> hexa viewer
	QObject::connect(this, SIGNAL(shiftComponents(int)),
						_hexa, SLOT(viewerChanged(int)));

	//file widget [scroll bar] -> text viewer
	QObject::connect(this, SIGNAL(shiftComponents(int)),
						_text, SLOT(viewerChanged(int)));

	//file widget [scroll bar] -> address viewer
	QObject::connect(this, SIGNAL(shiftComponents(int)),
						_address, SLOT(viewerChanged(int)));

	//file widget [scroll bar] -> hexa viewer
	QObject::connect(this, SIGNAL(setComponents(int)),
						_hexa, SLOT(viewerChangedAbs(int)));

	//file widget [scroll bar] -> text viewer
	QObject::connect(this, SIGNAL(setComponents(int)),
						_text, SLOT(viewerChangedAbs(int)));

	//file widget [scroll bar] -> address
	QObject::connect(this, SIGNAL(setComponents(int)),
						_address, SLOT(viewerChangedAbs(int)));

	///SET COMPONENTS EXTERNALY [FROM ICONS]
	//external -> scrollBar
	QObject::connect( this, SIGNAL(setComponentsExternal(int)),
						_scroll, SLOT( viewerChanged(int)));

	//file widget [scroll bar] -> hexa viewer
	QObject::connect(this, SIGNAL(setComponentsExternal(int)),
						_hexa, SLOT(viewerChangedAbs(int)));

	//file widget [scroll bar] -> text viewer
	QObject::connect(this, SIGNAL(setComponentsExternal(int)),
						_text, SLOT(viewerChangedAbs(int)));

	//file widget [scroll bar] -> address
	QObject::connect(this, SIGNAL(setComponentsExternal(int)),
						_address, SLOT(viewerChangedAbs(int)));

	//toolbar -> fileWidget
	QObject::connect(_tBar, SIGNAL(actionTriggered( QAction* )),
						this, SLOT(slTBarActivated( QAction* )));

	//hexa viewer -> fileWidget - content changes
	QObject::connect( _hexa, SIGNAL( viewerIsChanging( QString, QList<int>* ) ),
						this, SLOT(contentChanged( QString, QList<int>* )));

	//hexa viewer -> fileWidget - content changes SILENT
	QObject::connect( _hexa, SIGNAL( viewerIsChangingSilent( QString, QList<int>* ) ),
						this, SLOT(contentChangedSilent( QString, QList<int>* )));

	///DRAG & DROP - DRAGED ADDRESS
	//hexa viewer -> fileWidget
	QObject::connect( _hexa, SIGNAL( sgDragInProgress( int )),
						this, SLOT( slDragInProgress( int )));

	//text viewer -> fileWidget
	QObject::connect( _text, SIGNAL( sgDragInProgress( int )),
						this, SLOT( slDragInProgress( int )));

	//hexa viewer -> fileWidget
	QObject::connect( _hexa, SIGNAL( sgDragIsFinished( void )),
						this, SLOT( slDragIsFinished( void )));

	//text viewer -> fileWidget
	QObject::connect( _text, SIGNAL( sgDragIsFinished( void )),
						this, SLOT( slDragIsFinished( void )));

	return;
}

void FileWidget::executeProcessInternals( void ){
	
	this->peScan();	//PE scan
	
	return;
}

void FileWidget::peScan( void ){

	_tBarOpt.set(Toolbar::PE);
	_tBarOpt.set(Toolbar::PE_OVERLAY, false);	/**< clear PE_OVERLAY in default */

	//option for scanning
	std::bitset<PEbrowser::OPTION_SIZE> peOpt;
	peOpt.set(PEbrowser::SECTION_HEADERS);
	peOpt.set(PEbrowser::REAL_VALUES);

	try{
		
		_pe = new PEbrowser(_fileName);
		_pe->ScanFile();
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
		
		delete _pe;
		_pe = NULL;
		
		return;
	}

	//check if overlay exists
	if( _pe->Overlay() != (unsigned long)(-1) ){
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
	tmp.setNum(_hexa->FileSize() -1,16);
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

	//connection
	QObject::connect( pDialog, SIGNAL(textValueSelected( QString )),
						this, SLOT(processDialog(QString)));
	QObject::connect( pDialog, SIGNAL(textValueChanged(QString)),
						this, SLOT(checkDialog(QString)));
	
	pDialog->exec();

	//diconnection
	QObject::disconnect( pDialog, SIGNAL(textValueSelected( QString )),
							this, SLOT(processDialog(QString)));
	QObject::disconnect( pDialog, SIGNAL(textValueChanged( QString )),
							this, SLOT(checkDialog(QString)));
	
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
	
	//variables declaration
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
	/*for(int i = 0; i < 15; i++){

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
	}*/

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
	delete pDialog;
	
	return;
}

void FileWidget::createBinarySearchDialog( bool &actFailed ){

	//variable init
	QInputDialog* pDialog = new QInputDialog(this);
	_dType = FileWidget::BIN_SEARCH;
	
	//setting up dialog
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
	QRegExpValidator *pVld = new QRegExpValidator(
									QRegExp("[0-9a-fA-F]{1,64}"),pDialog);
	((QLineEdit*)(pDialog->children())[0])->setValidator( pVld );
	
	//set up actual activated dialog
	_pActDialog = pDialog;

	//connection
	QObject::connect( pDialog, SIGNAL(textValueChanged(QString)),
						this, SLOT(checkDialog(QString)));
	
	if( pDialog->exec() == QDialog::Accepted){

		//store input value
		int hex = _hexa->binarySearch( pDialog->textValue() );
		
		//check if address is proper
		if( hex != -1 ){

			//send signal to all fileWidget's commponents
			emit this->setComponentsExternal(hex);
		}
		else{

			//action failed flag for upper level
			actFailed = true;
		}
	}

	//diconnection
	QObject::disconnect( pDialog, SIGNAL(textValueChanged(QString)),
						this, SLOT(checkDialog(QString)));
		
	//unset actual activated dialog
	_pActDialog = NULL;
	
	//cleaning
	_dType = FileWidget::NOT_SET;
	delete pVld;
	delete pDialog;
	
	return;
}

void FileWidget::createSavePatternDialog( QList<QString> &fNames ){

	//create file dialog object
	QFileDialog *pDialog = new QFileDialog(this, tr("Select save pattern file"));

	//set up modalily
	pDialog->setModal(true);

	//file mode to any file [everythink can be compared]
	pDialog->setFileMode( QFileDialog::ExistingFiles );	

	//execute dialog
	if(pDialog->exec() == QDialog::Accepted){

		fNames = pDialog->selectedFiles();
	}

	//clean up dialog object
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
	
	//size settings
	_title->resize(w,h);

	//style settingss
	_title->setStyleSheet(TITLE_STYLE);
	_title->setIndent(5);
	
	return;
}

void FileWidget::wheelEvent( QWheelEvent* event ){

	//no futher event escalation
	event->accept();

	//direction UP
	if( event->delta() > 0 ){
			
		this->sliderChanged( QAbstractSlider::SliderSingleStepSub );
	}
	//direction DOWN
	else{

		this->sliderChanged( QAbstractSlider::SliderSingleStepAdd );
	}
	
	return;
}

void FileWidget::keyPressEvent( QKeyEvent* event ){

	if( _pActDialog != NULL ){

		switch( _dType ){

			case FileWidget::TARGET_ADDRESS:
				
				//send event into input 
				qApp->sendEvent( (_pActDialog->children())[0], event );

				return;

			case FileWidget::DESCRIPTION:
				return;

			default:
				break;
		}		
	}

	//slider escalation
	bool sldEsc = false;

	//default value for slider action
	QAbstractSlider::SliderAction sldAct = QAbstractSlider::SliderNoAction;

	switch( event->key() ){

		case Qt::Key_Home:

			//set up action
			sldAct = QAbstractSlider::SliderToMinimum;

			//escalation to slider permitted
			sldEsc = true;

			// no futher event escalation
			event->accept();

			break;

		case Qt::Key_End:

			//set up action
			sldAct = QAbstractSlider::SliderToMaximum;

			//escalation to slider permitted
			sldEsc = true;

			// no futher event escalation
			event->accept();

			break;

		case Qt::Key_Left:

			//check if requested shift is valid
			if( _hexa->isValidShift(1) ){

				//emit signal to other components
				emit this->shiftComponents(1);

				//manually set up slider
				_scroll->setValue( _scroll->value() + 1 );
			}
			else{

				_scroll->setSliderPosition( _scroll->value() );
			}

			// no futher event escalation
			event->accept();

			break;

		case Qt::Key_Right:

			//check if requested shift is valid
			if( _hexa->isValidShift(-1) ){

				//emit signal to other components
				emit this->shiftComponents(-1);

				//manually set up slider
				_scroll->setSliderPosition( _scroll->value() -1 );
			}
			else{

				_scroll->setSliderPosition( _scroll->value() );
			}

			// no futher event escalation
			event->accept();

			break;

		case Qt::Key_Up:

			//set up action
			sldAct = QAbstractSlider::SliderSingleStepSub;

			//escalation to slider permitted
			sldEsc = true;

			// no futher event escalation
			event->accept();

			break;

		case Qt::Key_Down:

			//set up action
			sldAct = QAbstractSlider::SliderSingleStepAdd;

			//escalation to slider permitted
			sldEsc = true;

			// no futher event escalation
			event->accept();

			break;

		case Qt::Key_PageUp:

			//set up action
			sldAct = QAbstractSlider::SliderPageStepSub;

			//escalation to slider permitted
			sldEsc = true;

			// no futher event escalation
			event->accept();

			break;

		case Qt::Key_PageDown:

			//set up action
			sldAct = QAbstractSlider::SliderPageStepAdd;

			//escalation to slider permitted
			sldEsc = true;

			// no futher event escalation
			event->accept();

			break;

		default:

			//escalate event to upper level
			event->ignore();

			break;
	}

	if( sldEsc ){

		//escalate action to slider - there is oalredyimplemented behaviour
		this->sliderChanged(sldAct);
	}

	return;
}

bool FileWidget::eventFilter( QObject* obj, QEvent* ev){

	//switch for events
	switch( ev->type() ){

		//wheel event
		case QEvent::Wheel:

			//accept event - no futher escalation
			ev->accept();

			//event from scrollBar
			if( obj == _scroll ){
			
				//send wheel event into another handler in this object
				QCoreApplication::sendEvent( this, ev );
			}

			break;

		//keyboard event
		case QEvent::KeyPress:

			//accept event - no futher escalation
			ev->accept();

			if( obj == _scroll ){

				//send keyboard event into another handler in this object
				QCoreApplication::sendEvent( this, ev );
			}

			break;

		//other events are escalating futher
		default:

			//send event filter to other instances
			return QObject::eventFilter(obj, ev);
	}

	return true;
}

void FileWidget::execTBarAction( Toolbar::ACTIONS curAct ){

	long long ePt = -1;
	long long overlay = -1;
	long long peAddr = -1;

	bool actFailed = false;

	QMessageBox error; QList<QString> fNames;

	switch( curAct ){
		
		case Toolbar::CLOSE:
			
			emit this->closing(QString::fromStdWString(_fileName));

			//parent not exists [standalone run]
			if( this->parent() == NULL ){
				this->close();
			}
			
			break;

		case Toolbar::GEN_DESC:
			
			/*if( !this->Focus() ){
				
				//emit focus request
				emit this->focusReq(this);
			}
			
			this->createGeneralDescriptionDialog();*/

			NotImplemented::alert();

			break;

		case Toolbar::DIRECT_ADDRES:
			
			if( !this->Focus() ){
				
				//emit focus request
				emit this->focusReq(this);
			}

			this->createTargetAddressDialog();

			break;

		case Toolbar::IMAGE_BASE:

			//only for PE file - for global signalization
			if( !_tBarOpt.test(Toolbar::PE )){
				return;
			}
			
			if( !this->Focus() ){
				
				//emit focus request
				emit this->focusReq(this);
			}

			//switch addresses between zero and image base offset
			_address->slBaseImageAddress();

			break;

		case Toolbar::ENTRY_POINT:

			//only for PE file - for global signalization
			if( !_tBarOpt.test(Toolbar::PE )){
				return;
			}
			
			if( !this->Focus() ){
				
				//emit focus request
				emit this->focusReq(this);
			}

			ePt = (long long)_pe->EntryPoint();

			if( ePt < 0 ){

				error.setWindowIcon(QIcon::fromTheme("Error Icon",
									QIcon(":/Global/Resources/error.svg")));
				error.setWindowTitle(tr("Entry point problem"));
				error.setText(tr("Entry point was not computed"));
				error.setIcon( QMessageBox::Critical);
				error.exec();
			}
			else{
				
				emit setComponentsExternal((int)ePt);			
			}
			
			break;

		case Toolbar::OVERLAY:

			//only for PE file - for global signalization
			if( !_tBarOpt.test(Toolbar::PE )){
				return;
			}
			
			if( !this->Focus() ){
				
				//emit focus request
				emit this->focusReq(this);
			}

			overlay = (long long)_pe->Overlay();

			if( overlay < 0 ){

				error.setWindowIcon(QIcon::fromTheme("Error Icon",
									QIcon(":/Global/Resources/error.svg")));
				error.setWindowTitle(tr("Overlay problem"));
				error.setText(tr("Overlay was not computed"));
				error.setIcon( QMessageBox::Critical);
				error.exec();
			}
			else{
				
				emit setComponentsExternal((int)overlay);			
			}

			break;

		case Toolbar::PE_SECTION:

			//only for PE file - for global signalization
			if( !_tBarOpt.test(Toolbar::PE )){
				return;
			}
			
			if( !this->Focus() ){
				
				//emit focus request
				emit this->focusReq(this);
			}

			peAddr = (long long)_pe->PE_Address();

			if( peAddr < 0 ){
				
				error.setWindowIcon(QIcon::fromTheme("Error Icon",
									QIcon(":/Global/Resources/error.svg")));
				error.setWindowTitle(tr("PE address problem"));
				error.setText(tr("PE address was not computed"));
				error.setIcon( QMessageBox::Critical);
				error.exec();
			}
			else{
				
				emit this->setComponentsExternal((int)peAddr);
			}

			break;

		case Toolbar::JMP_BEG:

			this->sliderChanged( (int)QAbstractSlider::SliderToMinimum );
			
			break;

		case Toolbar::JMP_END:

			this->sliderChanged( (int)QAbstractSlider::SliderToMaximum );
			
			break;

		case Toolbar::BIN_SEARCH:

			if( !this->Focus() ){

				//emit focus request
				emit this->focusReq(this);
			}
			
			this->createBinarySearchDialog( actFailed );

			if( actFailed ){
				
				error.setWindowIcon(QIcon::fromTheme("Error Icon",
						QIcon(":/Global/Resources/error.svg")));
				error.setWindowTitle(tr("Binary search"));
				error.setText(tr("Pattern not exists"));
				error.setIcon( QMessageBox::Warning );
				error.exec();
			}
			
			break;

		case Toolbar::SAVE_PATTERN:

			this->createSavePatternDialog( fNames );

			for( int i = 0; i < fNames.size(); i++ ) {

				_ptrSaveParser->addSavePattern( fNames[i], _pe );
			}

			//refresh both viewers
			_hexa->forceRefresh();
			_text->forceRefresh();

			break;

		default:

			//show "not implemented" dialog
			NotImplemented::alert();

			break;
	}

	return;
}