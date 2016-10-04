#include "samplesViewer_v3.h"

SamplesViewer::SamplesViewer(QWidget *parent)
	: QScrollArea(parent){

	//_floatResBufferH = 0;	
	_ptrFocusedSample = NULL;
	_ptrActiveDialog = NULL;
	
	_searchedBinarySequence = "";
	_searchedTextSequence = "";
	_searchType = FileWidget::NOT_USED;
	
	try{
		_ptrWidgetStorage = new QHash<QString, FileWidget*>();

		_ptrLayout = new QHBoxLayout();
		this->configureLayout();

		_ptrBase = new QFrame();
		this->configureBase();

		this->configureScroll();
	}
	catch(std::bad_alloc){

		QMessageBox msgBox;
		msgBox.setWindowIcon(
			QIcon::fromTheme("Error Icon",
			QIcon(":/Global/Resources/error.svg")));
		msgBox.setWindowTitle(tr("Fatal Error"));
		msgBox.setText(tr("Application can not start due to memory allocation error."));
		msgBox.setIcon( QMessageBox::Critical );
		msgBox.exec();
	}
}

SamplesViewer::~SamplesViewer(){

	//base frame deallocation
	if(_ptrBase != NULL){
		delete _ptrBase;
	}
	
	//storage deallocation
	if(_ptrWidgetStorage != NULL ){
		delete _ptrWidgetStorage;
	}	
}

void SamplesViewer::configure(void){
	return;
}

FileWidget* SamplesViewer::addSample(QString fName){

	FileWidget* ptrExistingSample = NULL;
	FileWidget* ptrCreatedSample = NULL;
	
	//sample exists in storage
	if(_ptrWidgetStorage->contains(fName)){
		
		ptrExistingSample = _ptrWidgetStorage->value(fName);

		if(_ptrFocusedSample != NULL){
			_ptrFocusedSample->setFocus(false);
		}
		_ptrFocusedSample = ptrExistingSample;
		_ptrFocusedSample->setFocus(true);
	}
	//create new sample
	else{

		//create sample
		ptrCreatedSample = this->crNewSmpl(fName);

		//remove focus and resetup for the created sample
		if(_ptrFocusedSample != NULL){
			_ptrFocusedSample->setFocus(false);
		}
		_ptrFocusedSample = ptrCreatedSample;
		_ptrFocusedSample->setFocus(true);

		int sampleCnt = _ptrWidgetStorage->count();
		_ptrBase->resize(
			(sampleCnt*SMPL_W) + ((sampleCnt-1)*5),
			_ptrBase->height());
	}

	return ptrCreatedSample;
}

void SamplesViewer::slRemoveSample(QString fName){

	//file widget is not in storage
	if(!_ptrWidgetStorage->contains(fName)){
		return;
	}

	bool removedSampleHasFocus = false;	//flag for 'focused sample is removed'
	FileWidget* ptrRemovedSample = _ptrWidgetStorage->value(fName);

	//deleted sample is focused
	if(ptrRemovedSample == _ptrFocusedSample){
		removedSampleHasFocus = true;
	}

	emit this->sgSampleRemoved(fName);
	
	ptrRemovedSample->removeEventFilter(this);		
	this->disconnectSample(fName);

	//remove sample from sample's storage
	_ptrWidgetStorage->remove(fName);	
	
	//remove sample from storage
	int layoutIndex = _ptrLayout->indexOf(ptrRemovedSample);
	QLayoutItem* ptrLayoutItem = _ptrLayout->itemAt(layoutIndex);
	_ptrLayout->removeItem(ptrLayoutItem);

	//destroy sample's resources
	if( ptrLayoutItem != NULL ){
		ptrLayoutItem->widget()->deleteLater();
	}

	//no sample remains in storage
	if(_ptrWidgetStorage->count() <= 0)
	{
		_ptrFocusedSample = NULL;
		_ptrLayout->setMargin(0);		
	}
	//removed sample has focus
	else if(removedSampleHasFocus)
	{		
		FileWidget* ptrSample = (FileWidget*)((_ptrLayout->itemAt(0))->widget());		
		ptrSample->setFocus(true);		
		_ptrFocusedSample = ptrSample;
		this->ensureVisibility(_ptrFocusedSample);
	}
	else{
		this->ensureVisibility(_ptrFocusedSample);		
	}

	//adjust size due to layout changes
	//properly resize sample viewer's base frame (holder of layout)
	int sampleCnt = _ptrWidgetStorage->count();
	if(sampleCnt == 0)
	{		
		_ptrBase->resize(
			this->width() - 2, 
			_ptrBase->height());
	}
	else{		
		_ptrBase->resize(
			(sampleCnt*SMPL_W) + ((sampleCnt-1)*5), 
			_ptrBase->height());
	}

	return;
}

void SamplesViewer::slRemoveSamples(QStringList fNames)
{
	for(int i = 0; i < fNames.size(); i++)
	{
		//file widget is not in storage
			if(!_ptrWidgetStorage->contains(fNames[i])){
			return;
		}

		bool removedSampleHasFocus = false;	//flag for 'focused sample is removed'
		FileWidget* ptrRemovedSample = _ptrWidgetStorage->value(fNames[i]);

		//deleted sample is focused
		if(ptrRemovedSample == _ptrFocusedSample){
			removedSampleHasFocus = true;
		}

		emit this->sgSampleRemoved(fNames[i],true);
	
		ptrRemovedSample->removeEventFilter(this);		
		this->disconnectSample(fNames[i]);

		//remove sample from sample's storage
		_ptrWidgetStorage->remove(fNames[i]);
	
		//remove sample from storage
		int layoutIndex = _ptrLayout->indexOf(ptrRemovedSample);
		QLayoutItem* ptrLayoutItem = _ptrLayout->itemAt(layoutIndex);
		_ptrLayout->removeItem(ptrLayoutItem);

		//destroy sample's resources
		if( ptrLayoutItem != NULL ){
			ptrLayoutItem->widget()->deleteLater();
		}

		//no sample remains in storage
		if(_ptrWidgetStorage->count() <= 0)
		{
			_ptrFocusedSample = NULL;
			_ptrLayout->setMargin(0);		
		}
		//removed sample has focus
		else if(removedSampleHasFocus)
		{		
			FileWidget* ptrSample = (FileWidget*)((_ptrLayout->itemAt(0))->widget());		
			ptrSample->setFocus(true);		
			_ptrFocusedSample = ptrSample;			
		}
	}

	emit this->sgFinalResizeRefresh();

	//adjust size due to layout changes
	//properly resize sample viewer's base frame (holder of layout)
	int sampleCnt = _ptrWidgetStorage->count();
	if(sampleCnt == 0)
	{
		QApplication::processEvents();
		_ptrBase->setFixedSize(
			this->width() - 2, 
			_ptrBase->height());			
	}
	else
	{
		this->ensureVisibility(_ptrFocusedSample);
		_ptrBase->setFixedSize(
			(sampleCnt*SMPL_W) + ((sampleCnt-1)*5), 
			_ptrBase->height());			
	}

	return;
}

void SamplesViewer::slDisableSample(QString fName)
{
	//check if sample exists and get it
	if( !_ptrWidgetStorage->contains(fName) ){
		return;
	}	
	FileWidget* ptrSample = _ptrWidgetStorage->value(fName);

	//disconnect content changes into result widget
	QObject* ptrParent = NULL;
	if((ptrParent = this->parent()) != NULL){

		QObject::disconnect(
			ptrSample, SIGNAL(sgContentIsChanging(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChanging(QString, QList<int>*)));

		QObject::disconnect(
			ptrSample, SIGNAL(sgContentIsChangingSilent(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChangingSilent(QString, QList<int>*)));

		QObject::disconnect(
			ptrSample, SIGNAL(sgDragAtAddress(QString)),
			ptrParent, SLOT(slDragAtAddress(QString)));
	}

	//remove data content from result widget
	emit this->sgSampleRemoved(fName);
	return;
}

void SamplesViewer::slEnableSample(
		QString fName,
		QList<int>* ptrDataContent,
		QString md5)
{
	//check if sample exists and get it
	if( !_ptrWidgetStorage->contains(fName) ){
		return;
	}	
	FileWidget* ptrSample = _ptrWidgetStorage->value(fName);

	//disconnect content changes into result widget
	QObject* ptrParent = NULL;
	if((ptrParent = this->parent()) != NULL){

		QObject::connect(
			ptrSample, SIGNAL(sgContentIsChanging(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChanging(QString, QList<int>*)));

		QObject::connect(
			ptrSample, SIGNAL(sgContentIsChangingSilent(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChangingSilent(QString, QList<int>*)));

		QObject::connect(
			ptrSample, SIGNAL(sgDragAtAddress(QString)),
			ptrParent, SLOT(slDragAtAddress(QString)));
	}

	emit this->sgSampleAdded(fName, ptrDataContent, md5);
	return;
}

void SamplesViewer::slFocusTaken(FileWidget* ptrSample){

	if(ptrSample != _ptrFocusedSample){

		_ptrFocusedSample->setFocus(false);
		_ptrFocusedSample = ptrSample;
		_ptrFocusedSample->setFocus(true);
		this->ensureVisibility(_ptrFocusedSample);		
	}

	return;
}

void SamplesViewer::configureBase(void){

	_ptrBase->resize(SMPLS_W, SMPL_H);
	_ptrBase->setMinimumWidth(SMPL_W + 5);

	_ptrBase->setLineWidth(0);
	_ptrBase->setFrameStyle(QFrame::NoFrame);
	_ptrBase->setStyleSheet("QFrame{background-color:transparent;}");
	
	_ptrBase->setLayout(_ptrLayout);	
	return;
}

void SamplesViewer::configureLayout(void){

	_ptrLayout->setMargin(0);
	_ptrLayout->setSpacing(5);
	_ptrLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	_ptrLayout->setSizeConstraint(QLayout::SetMinimumSize);
	
	return;
}

void SamplesViewer::configureScroll(void){

	//set up proper sizes
	this->resize(SMPLS_W + 2, SMPLS_H);
	this->setMinimumWidth(SMPL_W + 5 + 2);

	//set up background for base frame and border styles
	this->setMidLineWidth(0);
	this->setStyleSheet(
		"QScrollArea{background-color:#c2c2c2;border: 1px solid #707070;}"
		"QScrollBar{}");

	//set up widget
	this->setWidget(_ptrBase);
	
	//set up scroll bar policy
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//filtering evnts from scroll area to sample viewer [to focused sample]
	///?? reaaly on place ??
	this->installEventFilter(this);
	
	//accept drop event
	this->setAcceptDrops(true);

	//scrollbar settings
	QScrollBar* ptrScrollBar = this->horizontalScrollBar();
	ptrScrollBar->setPageStep(SMPL_W+5);
	ptrScrollBar->setSingleStep(SMPL_W+5);
	ptrScrollBar->setMinimum(0);
	
	//ptrScrollBar->setTracking(false);
	
	return;
}

FileWidget* SamplesViewer::crNewSmpl(QString fName){

	//create fileWidget object and configures him
	FileWidget* ptrNewSample = 
		new FileWidget(fName, FileWidget::currentRows(_ptrBase->height()), INIT_COLS);
	ptrNewSample->configure();
	ptrNewSample->setObjectName("FileWidget");

	//install filter event filter
	ptrNewSample->installEventFilter(this);

	//insert new sample to layout & storage
	_ptrLayout->addWidget(ptrNewSample, 0, Qt::AlignCenter);
	_ptrWidgetStorage->insert(fName, ptrNewSample);

	//connect sample's signals to proper slots
	this->connectSample(fName);

	return ptrNewSample;
}

void SamplesViewer::connectSample(QString fName){

	//check if sample exists and get it
	if( !_ptrWidgetStorage->contains(fName) ){
		return;
	}	
	FileWidget* ptrSample = _ptrWidgetStorage->value(fName);

	//closing
	QObject::connect(
		ptrSample, SIGNAL(closing(QString)),
		this, SLOT(slRemoveSample(QString)));

	//focus
	QObject::connect(
		ptrSample, SIGNAL(sgFocusTaken(FileWidget*)),
		this, SLOT(slFocusTaken(FileWidget*)));

	//disable
	QObject::connect(
		ptrSample, SIGNAL(sgDisableSample(QString)),
		this, SLOT(slDisableSample(QString)));

	//enable
	QObject::connect(
		ptrSample, SIGNAL(sgEnableSample(QString, QList<int>*, QString)),
		this, SLOT(slEnableSample(QString, QList<int>*, QString)));

	QObject* ptrParent = NULL;
	if((ptrParent = this->parent()) != NULL){

		QObject::connect(
			ptrSample, SIGNAL(sgContentIsChanging(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChanging(QString, QList<int>*)));

		QObject::connect(
			ptrSample, SIGNAL(sgContentIsChangingSilent(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChangingSilent(QString, QList<int>*)));

		QObject::connect(
			ptrSample, SIGNAL(sgDragAtAddress(QString)),
			ptrParent, SLOT(slDragAtAddress(QString)));
	}

	//TODO: rewrite to direct call ******
	//global toolbar, global jumps
	return;
}

void SamplesViewer::disconnectSample(QString fName){
	
	//check if sample exists and get it
	if( !_ptrWidgetStorage->contains(fName) ){
		return;
	}	
	FileWidget* ptrSample = _ptrWidgetStorage->value(fName);

	///closing
	QObject::disconnect(
		ptrSample, SIGNAL(closing(QString)),
		this, SLOT(slRemoveSample(QString)));
	
	///focus
	QObject::disconnect(
		ptrSample, SIGNAL(sgFocusTaken(FileWidget*)),
		this, SLOT(slFocusTaken(FileWidget*)));

	//disable
	QObject::connect(
		ptrSample, SIGNAL(sgDisableSample(QString)),
		this, SLOT(slDisableSample(QString)));

	//enable
	QObject::connect(
		ptrSample, SIGNAL(sgEnableSample(QString, QList<int>*, QString)),
		this, SLOT(slEnableSample(QString, QList<int>*, QString)));

	QObject* ptrParent = NULL;
	if((ptrParent = this->parent()) != NULL){

		QObject::disconnect(
			ptrSample, SIGNAL(sgContentIsChanging(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChanging(QString, QList<int>*)));

		QObject::disconnect(
			ptrSample, SIGNAL(sgContentIsChangingSilent(QString, QList<int>*)),
			ptrParent, SLOT(slSampleIsChangingSilent(QString, QList<int>*)));

		QObject::disconnect(
			ptrSample, SIGNAL(sgDragAtAddress(QString)),
			ptrParent, SLOT(slDragAtAddress(QString)));
	}

	//TODO: rewrite to direct call ******
	//global toolbar, global jumps
	return;
}

bool SamplesViewer::eventFilter(QObject* ptrObj, QEvent* ptrEvent){

	switch(ptrEvent->type()){

		//click on widget
		case QEvent::MouseButtonPress:

			//finish event calling
			ptrEvent->accept();
			if((ptrObj->objectName() == "FileWidget") &&
				(ptrObj != _ptrFocusedSample)){

				_ptrFocusedSample->setFocus(false);
				_ptrFocusedSample = ((FileWidget*)ptrObj);
				_ptrFocusedSample->setFocus(true);
				this->ensureVisibility(_ptrFocusedSample);
			}
			break;

		case QEvent::Wheel:

			//finish event calling
			ptrEvent->accept();
			if(_ptrFocusedSample != NULL){				
				_ptrFocusedSample->wheelEvent((QWheelEvent*)ptrEvent);
			}
			break;	

		/*case QEvent::KeyPress:			
						
			if(_ptrFocusedSample != NULL){
				ptrEvent->accept();
				_ptrFocusedSample->keyPressEvent((QKeyEvent*)ptrEvent);
			}
			else{
				ptrEvent->ignore();
			}
			break;*/

		default:

			//send event filter to other instances
			return QObject::eventFilter(ptrObj, ptrEvent);
	}

	return true;
}

void SamplesViewer::keyPressEvent(QKeyEvent *ptrEvent){

	QStringList keys;
	FileWidget *ptrSample = NULL;
	FileWidget::SEARCH_TYPES focusedSampleSearchType = FileWidget::NOT_USED;
	bool localSearchEnabled = false;

	switch( ptrEvent->key() ){

		///repeat search
		case Qt::Key_F3:

			ptrEvent->accept();
			if(_ptrFocusedSample != NULL){
				_ptrFocusedSample->keyPressEvent((QKeyEvent*)ptrEvent);
			}						
			break;

		///new search dialog
		case Qt::Key_F7:
				
			if(_ptrFocusedSample != NULL){
				_ptrFocusedSample->keyPressEvent((QKeyEvent*)ptrEvent);
			}
			break;

		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_PageDown:
		case Qt::Key_PageUp:
		case Qt::Key_Home:
		case Qt::Key_End:		

			ptrEvent->accept();
			if(_ptrFocusedSample != NULL){				
				_ptrFocusedSample->keyPressEvent((QKeyEvent*)ptrEvent);
			}
			break;

		/*case Qt::Key_Tab:
			ptrEvent->accept();
			break;*/

		case Qt::Key_Escape:
			ptrEvent->ignore();
			break;

		default:
			ptrEvent->ignore();
			break;
	}
	return;
}

void SamplesViewer::artificialResize(int diffW, int diffH){

	if((diffH == 0) && (diffW == 0)){
		return;
	}

	//signal to all subordinate samples to resize
	QList<QString> keys = _ptrWidgetStorage->keys();
	for(int i = 0; i < keys.size(); i++){
		_ptrWidgetStorage->value(keys[i])->artificialResize(diffH);
	}

	int sampleCnt = _ptrWidgetStorage->count();
	if(sampleCnt == 0){
		
		_ptrBase->setFixedSize(
			this->width() - 2 - diffW, 
			_ptrBase->height() - diffH);
	}
	else{

		_ptrBase->setFixedSize(
			(sampleCnt*SMPL_W) + ((sampleCnt-1)*5), 
			_ptrBase->height() - diffH);
	}

	//resize scroll area + sample viewer itself
	this->resize(
		this->width() - diffW, 
		this->height() - diffH);

	//emit resize signal to resultWidget
	emit this->sgFinalResizeRefresh();

	return;
}

FileWidget* SamplesViewer::focusedSample(void){
	return _ptrFocusedSample;
}

void SamplesViewer::ensureVisibility(FileWidget* ptrSample){

	QApplication::processEvents();
	this->ensureWidgetVisible(ptrSample,5,0);
	return;
}

void SamplesViewer::globalToolbarActivated(Toolbar::ACTIONS action){

	FileWidget* ptrSample = NULL;
	QStringList keys = _ptrWidgetStorage->keys();

	QString binarySequence = "", textSequence = "", addressSequence = "";
	QList<QString> nonProcessibleOperationFiles;

	bool silentRun = false;

	bool eraseFlag = false;

	//dialogs
	switch(action){

		case Toolbar::BIN_SEARCH_GLOBAL:

			binarySequence = this->createGlobalBinarySearchDialog(eraseFlag);
			if(binarySequence == ""){
				return;
			}		
			break;

		case Toolbar::TXT_SEARCH_GLOBAL:
			
			textSequence = this->createGlobalTextSearchDialog(eraseFlag);
			if(textSequence == ""){
				return;
			}		
			break;

		case Toolbar::DIRECT_ADDRESS_GLOBAL:

			addressSequence = this->createGlobalTargetAddressDialog();
			if(addressSequence == ""){
				return;
			}			
			break;

		default:
			break;
	}	

	//execution
	for(int i = 0; i < keys.size(); i++){
		ptrSample = _ptrWidgetStorage->value(keys[i]);

		switch(action){

			case Toolbar::BIN_SEARCH_GLOBAL:

				if(ptrSample->binarySearchExternally(binarySequence) == -1){					
					nonProcessibleOperationFiles.push_back(ptrSample->FileName());					
				}
				break;

			case Toolbar::TXT_SEARCH_GLOBAL:

				if(ptrSample->textSearchExternally(textSequence) == -1){
					nonProcessibleOperationFiles.push_back(ptrSample->FileName());
				}
				break;

			case Toolbar::DIRECT_ADDRESS_GLOBAL:
				
				if(ptrSample->directAddressExternally(addressSequence) == -1){
					nonProcessibleOperationFiles.push_back(ptrSample->FileName());
				}
				break;

			default:
				ptrSample->slTBarActivated(action);
				break;
		}
	}

	/*if(eraseFlag){
		//this->ensureVisibility(_ptrFocusedSample);
		QApplication::processEvents();
		int sampleCnt = _ptrWidgetStorage->count();
		if(sampleCnt == 0){
		
			_ptrBase->resize(
				this->width() - 2, 
				_ptrBase->height());
		}
		else{		
		
			_ptrBase->resize(
				(sampleCnt*SMPL_W) + ((sampleCnt-1)*5), 
				_ptrBase->height());
		}	
	}*/

	emit this->sgFinalResizeRefresh();

	//report on the end
	if(nonProcessibleOperationFiles.size() != 0){

		if(eraseFlag){
			this->slRemoveSamples(nonProcessibleOperationFiles);
		}
		else
		{
			QString messageBoxText = "Files, where operation can not be proceeded:\n";

			for(int i = 0; i < nonProcessibleOperationFiles.size(); i++){
				messageBoxText += nonProcessibleOperationFiles[i] + "\n";
			}

			QMessageBox notFound;
			notFound.setWindowIcon(QIcon::fromTheme("Error Icon",
				QIcon(":/Global/Resources/error.svg")));
			notFound.setWindowTitle(tr("Binary search"));
			notFound.setText(messageBoxText);
			notFound.setIcon(QMessageBox::Warning);
			notFound.exec();
		}
	}

	return;
}

void SamplesViewer::focusNextSample(void){

	///any sample exists in viewer & there is focused sample
	int layoutSampleCount = _ptrLayout->count();
	if((layoutSampleCount <= 1) || (_ptrFocusedSample == NULL)){
		return;
	}

	int focusedSampleIndex = _ptrLayout->indexOf(_ptrFocusedSample);
	int nextFocusedSampleIndex = focusedSampleIndex + 1;
	if(focusedSampleIndex + 1 == layoutSampleCount){
		nextFocusedSampleIndex = 0;
	}

	FileWidget* ptrNextFocusedSample = 
		(FileWidget*)_ptrLayout->itemAt(nextFocusedSampleIndex)->widget();
	
	if(ptrNextFocusedSample != NULL){
		this->slFocusTaken(ptrNextFocusedSample);
	}
	return;
}

QString SamplesViewer::createGlobalBinarySearchDialog(bool &eraseFlag){

	QString returnedSequence = "";

	QDialog *ptrDialog = new QDialog(this);
	ptrDialog->setModal(true);
	ptrDialog->setWindowTitle(tr("Global binary search"));
	ptrDialog->setWindowIcon(
		QIcon::fromTheme("Binary search",
							QIcon(":/Toolbar/Resources/search.svg")));
	ptrDialog->resize(370,115);
	
	///label
	QLabel *ptrLabel = new QLabel(ptrDialog);
	ptrLabel->setText(tr("Insert binary sequence [1-32 bytes]: "));
	ptrLabel->move(12,15);

	///text input
	QLineEdit *ptrLine = new QLineEdit(ptrDialog);
	ptrLine->setText(_searchedBinarySequence);
	ptrLine->resize(350,20);
	ptrLine->move(12,35);

	QRegExpValidator *ptrValidator = 
		new QRegExpValidator(QRegExp("[0-9a-fA-F]{1,64}"),ptrDialog);
	ptrLine->setValidator(ptrValidator);

	///checkbox
	QCheckBox *ptrCheckbox = new QCheckBox(
		"remove samples, where pattern is missing",
		ptrDialog);
	ptrCheckbox->resize(235,ptrCheckbox->height());
	ptrCheckbox->move(12,55);

	///button box
	QDialogButtonBox *ptrButtonBox = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal,
		ptrDialog);
	ptrButtonBox->move(205,85);

	//connect button box with dialog
	QObject::connect(ptrButtonBox,SIGNAL(accepted()),ptrDialog,SLOT(accept()));
	QObject::connect(ptrButtonBox,SIGNAL(rejected()),ptrDialog,SLOT(reject()));
	
	//set up proceeding dialog
	_dType = FileWidget::BIN_SEARCH;
	_ptrActiveDialog = ptrDialog;

	//connect input validator's function
	QObject::connect(
		ptrLine, SIGNAL(textChanged(QString)),
		this, SLOT(slCheckDialog(QString)));
	
	if(ptrDialog->exec()){
		returnedSequence =  ptrLine->text();
		if(ptrCheckbox->isChecked()){
			eraseFlag = true;
		}
		else{
			eraseFlag = false;
		}
	}
	else{
		returnedSequence = "";
		eraseFlag = false;
	}

	//diconnection
	QObject::disconnect(
		ptrDialog, SIGNAL(textChanged(QString)),
		this, SLOT(slCheckDialog(QString)));
		
	_dType = FileWidget::NOT_SET;
	_ptrActiveDialog = NULL;

	delete ptrDialog;

	_searchedBinarySequence = returnedSequence;
	return returnedSequence;
}

QString SamplesViewer::createGlobalTextSearchDialog(bool &eraseFlag){
	
	QString returnedSequence = "";

	QDialog *ptrDialog = new QDialog(this);
	ptrDialog->setModal(true);
	ptrDialog->setWindowTitle(tr("Global text search"));
	ptrDialog->setWindowIcon(
		QIcon::fromTheme("Text search",
							QIcon(":/Toolbar/Resources/search_text.svg")));	
	ptrDialog->resize(370,115);
	
	///label
	QLabel *ptrLabel = new QLabel(ptrDialog);
	ptrLabel->setText(tr("Insert text sequence [1-32 chars]: "));
	ptrLabel->move(12,15);

	///text input
	QLineEdit *ptrLine = new QLineEdit(ptrDialog);
	ptrLine->setText(_searchedTextSequence);
	ptrLine->resize(350,20);
	ptrLine->move(12,35);

	QRegExpValidator *ptrValidator = 
		new QRegExpValidator(QRegExp(".{1,32}"), ptrDialog);
	ptrLine->setValidator(ptrValidator);

	///checkbox
	QCheckBox *ptrCheckbox = new QCheckBox(
		"remove samples, where pattern is missing",
		ptrDialog);
	ptrCheckbox->resize(235,ptrCheckbox->height());
	ptrCheckbox->move(12,55);

	///button box
	QDialogButtonBox *ptrButtonBox = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal,
		ptrDialog);
	ptrButtonBox->move(205,85);

	//connect button box with dialog
	QObject::connect(ptrButtonBox,SIGNAL(accepted()),ptrDialog,SLOT(accept()));
	QObject::connect(ptrButtonBox,SIGNAL(rejected()),ptrDialog,SLOT(reject()));

	//set up proceeding dialog
	_dType = FileWidget::TXT_SEARCH;
	_ptrActiveDialog = ptrDialog;

	//connect input validator's function
	QObject::connect(
		ptrLine, SIGNAL(textChanged(QString)),
		this, SLOT(slCheckDialog(QString)));
	
	if(ptrDialog->exec()){
		returnedSequence =  ptrLine->text();
		if(ptrCheckbox->isChecked()){
			eraseFlag = true;
		}
		else{
			eraseFlag = false;
		}
	}
	else{
		returnedSequence = "";
		eraseFlag = false;
	}

	//diconnection
	QObject::disconnect(
		ptrLine, SIGNAL(textChanged(QString)),
		this, SLOT(slCheckDialog(QString)));
		
	_dType = FileWidget::NOT_SET;
	_ptrActiveDialog = NULL;

	//delete ptrValidator;
	delete ptrDialog;

	_searchedTextSequence = returnedSequence;
	return returnedSequence;
}

QString SamplesViewer::createGlobalTargetAddressDialog(void){

	QString returnedAddress = "";

	//variable init
	QInputDialog* ptrDialog = new QInputDialog(this);
	ptrDialog->setModal(true);
	ptrDialog->setFixedSize(QSize(350,150));
	ptrDialog->setInputMode(QInputDialog::TextInput);
	ptrDialog->setWindowTitle(tr("Target address navigation"));
	ptrDialog->setWindowIcon(
		QIcon::fromTheme(
			"concrete address",
			QIcon(":/Toolbar/Resources/targetAddress.svg")));	
	ptrDialog->setLabelText(tr("Set address:"));
	ptrDialog->setTextValue("0");	

	//insert validator
	QRegExpValidator *pVld = 
		new QRegExpValidator(QRegExp("[0-9a-fA-F]{1,8}"),ptrDialog);
	((QLineEdit*)(ptrDialog->children())[0])->setValidator(pVld);
	
	//set up actual activated dialog
	_dType = FileWidget::TARGET_ADDRESS;
	_ptrActiveDialog = ptrDialog;

	//text input validator connection
	QObject::connect( ptrDialog, SIGNAL(textValueChanged(QString)),
		this, SLOT(slCheckDialog(QString)));
	
	//dialog is accepted
	if(ptrDialog->exec()){		
		returnedAddress = ptrDialog->textValue();
	}
	else{
		returnedAddress = "";
	}

	//diconnection
	QObject::disconnect( ptrDialog, SIGNAL(textValueChanged( QString )),
		this, SLOT(slCheckDialog(QString)));
		
	//unset actual activated dialog
	_ptrActiveDialog = NULL;	
	_dType = FileWidget::NOT_SET;

	delete pVld;
	delete ptrDialog;
	
	return returnedAddress;
}

void SamplesViewer::slCheckDialog(QString value){

	bool ok;
	unsigned int hex = 0;
	QLabel *pNotif = NULL;
	QDialogButtonBox *pButBox = NULL;
	QObjectList children;
	QPushButton* pOK = NULL;

	switch(_dType){

		case FileWidget::TARGET_ADDRESS:
			
			children = _ptrActiveDialog->children();
			pButBox = (QDialogButtonBox*)children[3];			
			pOK = pButBox->button(QDialogButtonBox::Ok);
			
			hex = value.toUInt(&ok, 16);
			if(!ok){
				pOK->setEnabled(false);
			}
			else{
				pOK->setEnabled(true);
			}
			break;

		//binary search dialog check
		case FileWidget::BIN_SEARCH:

			children = _ptrActiveDialog->children();
			pButBox = (QDialogButtonBox*)children[4];
			pOK = pButBox->button(QDialogButtonBox::Ok);

			if(value.size() % 2 != 0){				
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