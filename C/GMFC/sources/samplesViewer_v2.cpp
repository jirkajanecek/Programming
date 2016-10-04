#include "samplesViewer_v2.h"

SamplesViewer::SamplesViewer(QWidget *parent)
	: QScrollArea(parent){

	//_floatResBufferH = 0;	
	_ptrFocusedSample = NULL;
	_scrollFloatBuffer = 0;

	try{
		_ptrWidgetStorage = new QHash<QString, FileWidget*>();

		_ptrLayout = new QHBoxLayout();
		this->configureLayout();

		_ptrBase = new QFrame();
		this->configureBase();

		//_ptrScroll = new QScrollArea( this );
		this->configureScroll();
	}
	catch(std::bad_alloc){

		QMessageBox msgBox;
		msgBox.setWindowIcon(QIcon::fromTheme("Error Icon",
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
	
	//scroll area deallocation
	//if(_ptrScroll != NULL){
	//	delete _ptrScroll;
	//}

	//storage deallocation
	if(_ptrWidgetStorage != NULL ){
		delete _ptrWidgetStorage;
	}	
}

void SamplesViewer::configure(void){

	//sizes
	//this->resize(SMPLS_W, SMPLS_H);
	//this->setMinimumWidth(SMPL_W + 5);

	return;
}

FileWidget* SamplesViewer::addSample(QString fName){

	FileWidget* ptrExistingSample = NULL;
	FileWidget* ptrCreatedSample = NULL;
	
	//sample exists in storage
	if(_ptrWidgetStorage->contains(fName)){
		
		ptrExistingSample = _ptrWidgetStorage->value(fName);
		ptrExistingSample->setFocus(true);
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

		_ptrBase->setFixedSize(
			_ptrWidgetStorage->count()*(SMPL_W + 5), 
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

	//signal to resultWidget & disconnect samples's signals
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
	if(_ptrWidgetStorage->count() <= 0){	
		
		_ptrFocusedSample = NULL;
		_ptrLayout->setMargin(0);		
	}
	//removed sample has focus
	else if(removedSampleHasFocus){
		
		FileWidget* ptrSample = (FileWidget*)((_ptrLayout->itemAt(0))->widget());		
		ptrSample->setFocus(true);		
		_ptrFocusedSample = ptrSample;
		this->ensureVisibility(ptrSample);
	}

	//adjust size due to layout changes
	//properly resize sample viewer's base frame (holder of layout)
	if(_ptrWidgetStorage->count() == 0){
		
		_ptrBase->resize(
			this->width() - 2, 
			_ptrBase->height());
	}
	else{		
		
		_ptrBase->resize(
			_ptrWidgetStorage->count()*(SMPL_W + 5), 
			_ptrBase->height());	
	}

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
	this->setStyleSheet("QScrollArea{background-color:#c2c2c2;" 
										"border: 1px solid #707070;}"
						"QScrollBar{}");

	//set up widget
	this->setWidget(_ptrBase);
	
	//set up scroll bar policy
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//filtering evnts from scroll area to sample viewer [to focused sample]
	this->installEventFilter(this);
	
	//accept drop event
	this->setAcceptDrops(true);

	//scrollbar settings
	QScrollBar* ptrScrollBar = this->horizontalScrollBar();
	ptrScrollBar->setPageStep(SMPL_W+5);
	ptrScrollBar->setSingleStep(SMPL_W+5);
	ptrScrollBar->setMinimum(0);
	
	ptrScrollBar->setTracking(false);
	
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

	QObject::connect(
		ptrSample, SIGNAL(closing(QString)),
		this, SLOT(slRemoveSample(QString)));

	QObject::connect(
		ptrSample, SIGNAL(sgFocusTaken(FileWidget*)),
		this, SLOT(slFocusTaken(FileWidget*)));

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

	QObject::disconnect(
		ptrSample, SIGNAL(closing(QString)),
		this, SLOT(slRemoveSample(QString)));

	QObject::disconnect(
		ptrSample, SIGNAL(sgFocusTaken(FileWidget*)),
		this, SLOT(slFocusTaken(FileWidget*)));

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

		case QEvent::KeyPress:
			
			ptrEvent->accept();
			if(_ptrFocusedSample != NULL){				
				_ptrFocusedSample->keyPressEvent((QKeyEvent*)ptrEvent);
			}		

		default:

			//send event filter to other instances
			return QObject::eventFilter(ptrObj, ptrEvent);
	}

	return true;
}

void SamplesViewer::keyPressEvent(QKeyEvent *ptrEvent){
	
	ptrEvent->accept();
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

	//properly resize sample viewer's base frame (holder of layout)
	if(_ptrWidgetStorage->count() == 0){
		
		_ptrBase->setFixedSize(
			_ptrBase->width() - diffW,
			_ptrBase->height() - diffH);	
	}
	else{		
		
		_ptrBase->setFixedSize(
			_ptrWidgetStorage->count()*(SMPL_W + 5), 
			_ptrBase->height() - diffH);
	}

	//resize scroll area + sample vieer itself
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
	this->ensureWidgetVisible(ptrSample,10,0);
	return;
}

void SamplesViewer::globalToolbarActivated(Toolbar::ACTIONS action){

	FileWidget* ptrSample = NULL;
	QStringList keys = _ptrWidgetStorage->keys();

	for(int i = 0; i < keys.size(); i++){

		ptrSample = _ptrWidgetStorage->value(keys[i]);
		ptrSample->slTBarActivated(action);
	}

	emit this->sgFinalResizeRefresh();

	return;
}