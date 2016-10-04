#include "gmfc.h"

GMFC::GMFC(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags){

	_pSv = new SamplesViewer(this);
	_pSv->configure();

	_pRw = new ResultWidget(this);
	_pRw->configure();

	_pTb = new GMFCToolbar( TOOL_HEIGHT, _pSv->height(), this);
	_pTb->configure( _tTbOpt );

	_ptrIb = new InfoBar(this);
	_ptrIb->configure( _pRw->width());
}

GMFC::~GMFC(){}

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

			emit this->sgGlbToolbarAction( Toolbar::OVERLAY );
			break;

		case Toolbar::ENTRY_POINT:

			emit this->sgGlbToolbarAction( Toolbar::ENTRY_POINT );
			break;

		case Toolbar::JMP_PAGE_D:

			emit this->sgGlbSliderChanged( (int)QAbstractSlider::SliderPageStepAdd );
			break;

		case Toolbar::JMP_PAGE_U:

			emit this->sgGlbSliderChanged( (int)QAbstractSlider::SliderPageStepSub );
			break;

		case Toolbar::JMP_ROW_D:

			emit this->sgGlbSliderChanged( (int)QAbstractSlider::SliderSingleStepAdd );
			break;

		case Toolbar::JMP_ROW_U:

			emit this->sgGlbSliderChanged( (int)QAbstractSlider::SliderSingleStepSub );
			break;

		case Toolbar::JMP_BEG:

			emit this->sgGlbSliderChanged( (int)QAbstractSlider::SliderToMinimum );
			break;

		case Toolbar::JMP_END:

			emit this->sgGlbSliderChanged( (int)QAbstractSlider::SliderToMaximum );
			break;

		default:

			//show "not implemented" dialog
			NotImplemented::alert();
			break;
	}
	
	return;
}

void GMFC::slDragAtAddress( QString addr ){

	_ptrIb->setDA(addr);

	return;
}

void GMFC::slSuccRateUpdate( QString rate ){

	_ptrIb->setSR(rate);
	
	return;
}

void GMFC::configure( void ){

	//place inputed components	
	this->placeComponents();

	//connect components
	this->connectComponents();

	//method sets up application base frame
	this->setupBaseFrame();

	//method adapts window to proper size
	this->adaptWindowSize();

	//method processes potential input arguments
	this->processInputArgs();
		
	this->setFocusPolicy(Qt::StrongFocus);

	return;
}

void GMFC::artificialResize( QResizeEvent *pEv ){

	//size differences
	int diffW = pEv->oldSize().width() - pEv->size().width();
	int diffH = pEv->oldSize().height() - pEv->size().height();	

	//no difference - just switch it off
	if((diffW == 0) && (diffH == 0)){
		return;
	}

	//components resize
	_pTb->artificialResize(pEv);
	_pRw->artificialResize(pEv);	//result viewer has to resized before sample viewer
	_pSv->artificialResize(pEv);

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
	QObject::connect( _pTb, SIGNAL(actionTriggered( QAction* )),
						this, SLOT(slTBarActivated( QAction* )));

	//content update sampleViewer -> resultWidget
	QObject::connect( _pSv, SIGNAL( sampleIsChanging( QString, QList<int>* )),
						_pRw, SLOT( updateCompare( QString, QList<int>* )));

	//content update sampleViewer -> resultWidget
	QObject::connect( _pSv, SIGNAL( sampleIsChangingSilent( QString, QList<int>* )),
						_pRw, SLOT( updateCompareSilent( QString, QList<int>* )));

	QObject::connect( _pSv, SIGNAL( sgFinalResizeRefresh()),
						_pRw, SLOT( slFinalResizeRefresh()));

	//sample removed sampleViewer -> resultWidget
	QObject::connect( _pSv, SIGNAL( sgSampleRemoved( QString )),
						_pRw, SLOT( slRemoveCompare( QString )));

	//result widget -> this
	QObject::connect( _pRw, SIGNAL( sgSuccRateUpdate( QString )),
						this, SLOT( slSuccRateUpdate( QString )));
	
	return;
}

void GMFC::setupBaseFrame( void ){

	this->setStyleSheet( "QMainWindow{ background-color:#dddddd; }" );

	return;
}

void GMFC::adaptWindowSize(void){

	this->resize( _pTb->width() + _pSv->width() + _pRw->width() + 20,
					_pTb->height() + 10 + CELL_H);

	return;
}

void GMFC::procDialogAddSample( QStringList &fNames ){

	//create file dialog object
	QFileDialog *pDialog = new QFileDialog(this, tr("Select file to compare"));

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

void GMFC::deploySamples( QList<QString> fNames ){

	FileWidget* ptrSmpl = NULL;
	
	for( int i = 0; i < fNames.count(); i++ ){

		//check if file even exists
		if( !QFile::exists(fNames[i]) ){
			continue;
		}
		
		ptrSmpl = _pSv->addSample( fNames[i] );

		//if new sample is added refresh result widget
		if( ptrSmpl != NULL ){

			//add new data content
			_pRw->addToCompare( ptrSmpl->FileName(), 
								ptrSmpl->DataContent(),
								ptrSmpl->MD5());
		}
	}

	return;
}

void GMFC::processInputArgs( void ){

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

void GMFC::dragEnterEvent( QDragEnterEvent* event ){	
	
	event->accept();
	return;
}

void GMFC::dragMoveEvent( QDragMoveEvent* event ){

	event->accept();	
	return;
}

void GMFC::dropEvent( QDropEvent* event ){

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
	this->deploySamples( fNames );

	return;
}

void GMFC::keyPressEvent( QKeyEvent *event ){

	switch( event->key() ){

		case Qt::Key_Escape:
			
			//accept event
			event->accept();
			
			//end application
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