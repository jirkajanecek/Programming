#include "gmfc_v3.h"

GMFC::GMFC(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags){

	_resizeFloatBuffer = 0;
	_ptrShortcuts = new QList<QShortcut*>();

	_pSv = new SamplesViewer(this);
	_pSv->configure();

	_pRw = new ResultWidget(this);
	_pRw->configure();

	_pTb = new GMFCToolbar( TOOL_HEIGHT, _pSv->height(), this);
	_pTb->configure( _tTbOpt );

	_ptrIb = new InfoBar(this);
	_ptrIb->configure(_pRw->width());
}

GMFC::~GMFC(){

	delete _ptrIb;
	delete _pTb;
	delete _pRw;
	delete _pSv;

	for(int i = 0; i < _ptrShortcuts->size(); i++ ){
		delete _ptrShortcuts->at(i);
	}
	delete _ptrShortcuts;
}

void GMFC::slTBarActivated( QAction* act ){

	QHash<QAction*, Toolbar::ACTIONS>* actTab = _pTb->Actions();
	Toolbar::ACTIONS curAct = actTab->value(act);	

	QStringList fNames;

	switch( curAct ){

		case Toolbar::GMFC_ADD:

			//process add sample dialog
			this->procDialogAddSample(fNames);

			//deploy samples into both views [sample,result]
			this->deploySamples( fNames );

			break;
			
		case Toolbar::GMFC_FILELIST:
		case Toolbar::JMP_OPT:

			//show "not implemented" dialog
			NotImplemented::alert();
			break;

		case Toolbar::OVERLAY:
		case Toolbar::ENTRY_POINT:
		case Toolbar::JMP_PAGE_D:
		case Toolbar::JMP_PAGE_U:
		case Toolbar::JMP_ROW_D:
		case Toolbar::JMP_ROW_U:
		case Toolbar::JMP_BEG:
		case Toolbar::JMP_END:
		case Toolbar::BIN_SEARCH_GLOBAL:
		case Toolbar::TXT_SEARCH_GLOBAL:
		case Toolbar::DIRECT_ADDRESS_GLOBAL:

			_pSv->globalToolbarActivated(curAct);
			break;

		default:

			//show "not implemented" dialog
			NotImplemented::alert();
			break;
	}
	
	return;
}

void GMFC::slDragAtAddress(QString addr){
	_ptrIb->setDA(addr);
	return;
}

void GMFC::slSuccRateUpdate(QString rate){
	_ptrIb->setSR(rate);	
	return;
}

void GMFC::slSampleIsChanging(QString fName, QList<int>* ptrDataContent){
	_pRw->updateCompare(fName, ptrDataContent);
	return;
}

void GMFC::slSampleIsChangingSilent(QString fName, QList<int>* ptrDataContent){
	_pRw->updateCompareSilent(fName, ptrDataContent);
	return;
}

void GMFC::slUp(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_ROW_U);
	return;
}

void GMFC::slDown(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_ROW_D);
	return;
}

void GMFC::slLeft(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_LEFT);
	return;
}

void GMFC::slRight(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_RIGHT);
	return;
}

void GMFC::slPageUp(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_PAGE_U);
	return;
}

void GMFC::slPageDown(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_PAGE_D);
	return;
}

void GMFC::slHome(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_BEG);
	return;
}

void GMFC::slEnd(void){
	_pSv->globalToolbarActivated(Toolbar::JMP_END);
	return;
}

void GMFC::slTab(void){
	_pSv->focusNextSample();
	return;
}

void GMFC::slCopy(void){
	_pRw->copySelection();
	return;
}

void GMFC::configure(void){

	//place inputed components	
	this->placeComponents();

	//connect components
	this->connectComponents();

	this->manageShortcuts();

	//method sets up application base frame
	this->setupBaseFrame();

	//method adapts window to proper size
	this->adaptWindowSize();

	//method processes potential input arguments
	this->processInputArgs();
		
	this->setFocusPolicy(Qt::StrongFocus);

	return;
}

void GMFC::artificialResize(QResizeEvent *pEv){

	//size differences
	int diffW = pEv->oldSize().width() - pEv->size().width();
	int diffH = pEv->oldSize().height() - pEv->size().height();	

	//no difference - just switch it off
	if((diffW == 0) && (diffH == 0)){
		return;
	}

	int roundedDiffH = ((_resizeFloatBuffer + diffH)/CELL_H)*CELL_H;
	_resizeFloatBuffer = (_resizeFloatBuffer + diffH)%CELL_H;

	_pTb->artificialResize(roundedDiffH);
	_pRw->artificialResize(roundedDiffH);
	_pSv->artificialResize(diffW, roundedDiffH);

	return;
}

void GMFC::placeComponents(void){

	//toolbar placement
	_pTb->move(5, 5);

	//sample viewer placement
	_pSv->move( _pTb->width() + 10, 5);

	//result viewer placement
	_pRw->move( _pSv->x() + _pSv->width() + 5, 31);

	//info bar placement
	_ptrIb->move( _pRw->x(), _pRw->y() - _ptrIb->height() - 4);

	return;
}

void GMFC::connectComponents(void){

	//toolbar -> this
	QObject::connect(
		_pTb, SIGNAL(actionTriggered( QAction* )),
		this, SLOT(slTBarActivated( QAction* )));

	QObject::connect( 
		_pSv, SIGNAL( sgFinalResizeRefresh()),
		_pRw, SLOT( slFinalResizeRefresh()));

	//sample removed sampleViewer -> resultWidget
	QObject::connect(
		_pSv, SIGNAL(sgSampleRemoved(QString, bool)),
		_pRw, SLOT(slRemoveCompare(QString, bool)));

	//sample added sampleViewer -> resultWidget
	QObject::connect(
		_pSv, SIGNAL(sgSampleAdded(QString, QList<int>*, QString)),
		_pRw, SLOT(slAddToCompare(QString, QList<int>*, QString)));

	//result widget -> this
	QObject::connect( 
		_pRw, SIGNAL( sgSuccRateUpdate( QString )),
		this, SLOT( slSuccRateUpdate( QString )));
	
	return;
}

void GMFC::setupBaseFrame( void ){

	this->setStyleSheet("QMainWindow{ background-color:#dddddd; }");

	return;
}

void GMFC::adaptWindowSize(void){

	_appW = _pTb->width() + _pSv->width() + _pRw->width() + 20;
	_appH = _pTb->height() + 10 + CELL_H;

	this->resize( 
		_pTb->width() + _pSv->width() + _pRw->width() + 20,
		_pTb->height() + 10 + CELL_H);

	//get host screen size
	QDesktopWidget *desktopWidget = QApplication::desktop();
	QRect availableGeometry = desktopWidget->availableGeometry();
	
	qDebug() << "app W: " << _appW << availableGeometry.width() <<
		availableGeometry.height();

	//default application size is too big for the used screen
	int newWidth = this->width(), newHeight = this->height();
	bool sizeCorrectionNeeded = false;

	if(_appW > (availableGeometry.width() - 30)){
		newWidth = availableGeometry.width() - 30;
		sizeCorrectionNeeded = true;
	}

	if(_appH > (availableGeometry.height() - 40)){
		newHeight = availableGeometry.height()-40;
		sizeCorrectionNeeded = true;
	}

	if(sizeCorrectionNeeded){
		
		//resize to proper size for this screen
		QSize newSize(newWidth, newHeight);
		QResizeEvent resizeEvent(newSize,this->size());
		this->artificialResize(&resizeEvent);
		this->placeComponents();

		this->resize( 
			_pTb->width() + _pSv->width() + _pRw->width() + 20,
			_pTb->height() + 10 + CELL_H);
	}
	
	return;
}

void GMFC::procDialogAddSample( QStringList &fNames ){

	fNames = QFileDialog::getOpenFileNames(NULL,"Select samples to compare");
	return;
}

void GMFC::deploySamples(QList<QString> fNames){

	FileWidget* ptrSmpl = NULL;
	
	for( int i = 0; i < fNames.count(); i++ ){

		//check if file even exists
		if( !QFile::exists(fNames[i]) ){
			continue;
		}
		
		ptrSmpl = _pSv->addSample(fNames[i]);

		//if new sample is added refresh result widget
		if( ptrSmpl != NULL ){

			//add new data content
			_pRw->slAddToCompare( 
				ptrSmpl->FileName(), 
				ptrSmpl->DataContent(),
				ptrSmpl->MD5());
		}
	}

	_pSv->ensureVisibility(ptrSmpl);
	return;
}

void GMFC::processInputArgs(void){

	int argc = QApplication::argc();
	char **argv = QApplication::argv();

	if( argc <= 1 ){
		return;
	}

	int argCnt = 1;
	QList<QString> fNames;	

	while( argCnt < argc ){

		fNames.append( QString(argv[argCnt]) );

		argCnt++;
	}

	if( fNames.count() > 0 ){

		this->deploySamples( fNames );
	}	

	return;
}

void GMFC::manageShortcuts(void){

	QShortcut *ptrShortcut = NULL;

	///ctrl + up
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slUp()));

	///ctrl + down
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slDown()));

	///ctrl + left
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slLeft()));

	///ctrl + right
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slRight()));

	///ctrl + page up
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageUp),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slPageUp()));

	///ctrl + page down
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageDown),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slPageDown()));

	///ctrl + home
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Home),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slHome()));

	///ctrl + end
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_End),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slEnd()));

	///ctrl + tab
	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab),this);
	_ptrShortcuts->push_back(ptrShortcut);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slTab()));

	ptrShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C),this);
	QObject::connect(
		ptrShortcut, SIGNAL(activated()),
		this,SLOT(slCopy()));
}

void GMFC::dragEnterEvent( QDragEnterEvent* event ){	
	
	event->accept();
	return;
}

void GMFC::dragMoveEvent( QDragMoveEvent* event ){

	event->accept();	
	return;
}

void GMFC::dropEvent(QDropEvent* event){

	//accept all events
	event->accept();
	
	//get mime data information
	const QMimeData* ptrMD = event->mimeData();

	//get all urls
	QList<QUrl> urls = ptrMD->urls();

	//drop when cell is draged
	if( (urls.size() <= 0) &&
		(_ptrIb->DA() != IB_DA_VAL_DFT)){		
		
		_ptrIb->setDA();
	}
	//no url is drop here
	else if( urls.size() <= 0){
		
		return;
	}

	//parse file names from draged files
	QList<QString> fNames;
	for( int i = 0 ; i < urls.size(); i++ ){
		
		if( urls[i].isLocalFile() ){
			
			fNames.append( urls[i].toLocalFile() );
		}
	}

	//deploy samples into sample viewer
	this->deploySamples(fNames);

	return;
}

void GMFC::keyPressEvent(QKeyEvent *event){
	
	switch(event->key()){

		case Qt::Key_Escape:			
			event->accept();			
			this->close();
			break;

		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_PageDown:
		case Qt::Key_PageUp:
		case Qt::Key_Home:
		case Qt::Key_End:
		case Qt::Key_F3:
		case Qt::Key_F7:
		//case Qt::Key_Tab:

			event->accept();
			QCoreApplication::sendEvent( _pSv, event);
			break;

		default:
			event->ignore();
			break;
	}

	return;
}

void GMFC::resizeEvent( QResizeEvent *pEv){

	//user based event
	if( pEv->spontaneous() ){
		
		pEv->accept();

		//setup minimal height
		QSize tbUsableSize = _pTb->usableSize();
		this->setMinimumHeight(tbUsableSize.height() + 10 + CELL_H);
		this->setMinimumWidth(_pTb->width() + 20 + _pRw->width() + SMPL_W + 10);
		
		//activate artificial resize
		this->artificialResize(pEv);

		//place newly resized components to proper possitions
		this->placeComponents();
		return;
	}
	//system event
	else{
		
		pEv->ignore();
		return;
	}
}