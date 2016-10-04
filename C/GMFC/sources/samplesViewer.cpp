#include "samplesViewer.h"

SamplesViewer::SamplesViewer(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags){

	_floatResBufferH = 0;
		
	//focused sample init
	_pFocused = NULL;
		
	//widgets storage allocation
	_storWgts = new QHash<QString, FileWidget*>();

	//layout allocation
	_layout = new QHBoxLayout();
	this->confLayout();

	//base frame allocation
	//_base = new QFrame();
	_base = new QFrame();
	this->confBase();

	//scroll area allocation
	_scroll = new QScrollArea( this );
	this->confScroll();
}

SamplesViewer::~SamplesViewer(){

	//base frame deallocation
	if( _base != NULL ){
		delete _base;
	}
	
	//scroll area deallocation
	if( _scroll != NULL ){
		delete _scroll;
	}

	//storage deallocation
	if( _storWgts != NULL ){
		delete _storWgts;
	}

	
}

void SamplesViewer::configure(void){

	//sizes
	this->resize(SMPLS_W, SMPLS_H);
	this->setMinimumWidth(SMPL_W + 10);
	
	return;
}

FileWidget* SamplesViewer::addSample( QString fName ){

	FileWidget* pSmpl = NULL;
	FileWidget* retSmpl = NULL;
	
	if( _storWgts->contains( fName ) ){

		//load widget from storage
		pSmpl = _storWgts->value(fName);

		//set permanent focus
		this->setPermFocus( pSmpl );		
	}
	else{

		//create new file widget
		retSmpl = this->crNewSmpl( fName );		

		//set permanent focus
		this->setPermFocus( retSmpl );
	}

	return retSmpl;
}

void SamplesViewer::remSample( QString fName ){

	bool rmFoc = false;	/**< focused sample is removed */

	//file widget is not in storage
	if( !_storWgts->contains( fName ) ){
		return;
	}

	//emit sample removal to parent component
	emit this->sgSampleRemoved( fName );

	//download sample
	FileWidget* smpl = _storWgts->value(fName);

	//deleted sample is focused
	if( smpl == _pFocused ){

		rmFoc = true;
	}

	//remove event filter from samples viewer
	smpl->removeEventFilter(this);

	//disconnect signals from samplesViewer
	this->discSample(fName);

	//remove reconrd from widget storage
	_storWgts->remove(fName);
	
	//select closing sample from layout and remove it
	int idx = _layout->indexOf( smpl );
	QLayoutItem* pLayIt = _layout->itemAt( idx );
	_layout->removeItem(pLayIt);

	//deallocate closing sample
	if( pLayIt != NULL ){
		pLayIt->widget()->deleteLater();		
	}
	
	//refocused first possible sample
	if( _storWgts->count() <= 0 ){
		
		//set up focused sample to NULL [no other widget exists]
		_pFocused = NULL;

		//remove margin from layout
		_layout->setMargin(0);
	}
	else if( rmFoc ){

		//get the first sample from layout
		smpl = (FileWidget*)((_layout->itemAt(0))->widget());
		
		//set up focus
		this->setPermFocus( smpl );

		//set up new focused sample
		_pFocused = smpl;
	}

	//adjust size due to layout changes
	_base->adjustSize();

	return;
}

void SamplesViewer::confBase( void ){

	//set up proper size for base frame
	_base->resize(SMPLS_W-2, SMPL_H);
	
	//previous minimum size: _base->setMinimumWidth(SMPLS_W - 2);
	_base->setMinimumWidth(SMPL_W + 10 - 2);

	//border width
	_base->setFrameStyle(QFrame::NoFrame);
	_base->setLineWidth(0);
		
	//set up base frame's layout
	_base->setLayout(_layout);

	//set up styles for base frame
	_base->setStyleSheet("QFrame{background-color:transparent;}");
	
	return;
}

void SamplesViewer::confLayout( void ){

	//set up spacing for layout
	_layout->setSpacing(5);

	//set up margin 
	_layout->setMargin(0);

	//set up alignment for layout's widgets
	_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	//layout's size consraints due to proper size update in QFrame
	_layout->setSizeConstraint(QLayout::SetMinimumSize);
	//_layout->setSizeConstraint(QLayout::SetNoConstraint);
	
	return;
}

void SamplesViewer::confScroll( void ){

	//set up proper sizes
	_scroll->resize(SMPLS_W, SMPLS_H);
	//_scroll->setMinimumWidth(SMPLS_W);
	_scroll->setMinimumWidth(SMPL_W + 10);

	//set up background for base frame and border styles
	_scroll->setStyleSheet( "QScrollArea{background-color:#c2c2c2;" 
										"border: 1px solid #707070;}"
							"QScrollBar{}");
	_scroll->setMidLineWidth(0);

	//set up widget
	_scroll->setWidget(_base);

	//set up scroll bar policy
	_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	//accept drop event
	_scroll->setAcceptDrops(true);

	return;
}

FileWidget* SamplesViewer::crNewSmpl( QString fName ){

	//create fileWidget object and configures him
	//FileWidget* fWidget = new FileWidget(fName);
	FileWidget* fWidget = 
		new FileWidget(fName, FileWidget::currentRows(_base->height()), INIT_COLS);
	fWidget->configure();

	//install filter event filter
	fWidget->installEventFilter(this);

	//insert created file widget into layout
	_layout->addWidget( fWidget, 0, Qt::AlignCenter );

	//store widget
	_storWgts->insert(fName, fWidget);

	//connect sample's signals to proper slots
	this->connSample(fName);

	//set up proper margin with samples
	_layout->setSpacing(5);

	return fWidget;
}

void SamplesViewer::connSample( QString fName ){

	//check if sample exists
	if( !_storWgts->contains( fName) ){
		return;
	}

	//download sample
	FileWidget* smpl = _storWgts->value(fName);

	//connect to sampleViewer - closing
	QObject::connect(smpl, SIGNAL(closing(QString)),
						this, SLOT(remSample(QString)));

	//connect to sampleViewer - toolbar used
	QObject::connect(smpl, SIGNAL(focusReq(FileWidget*)),
						this, SLOT(setPermFocus(FileWidget*)));

	QObject::connect(smpl, SIGNAL( contentIsChanging( QString, QList<int>* ) ),
						this, SLOT( sampleChanged( QString, QList<int>* )));

	QObject::connect(smpl, SIGNAL( contentIsChangingSilent( QString, QList<int>* ) ),
						this, SLOT( sampleChangedSilent( QString, QList<int>* )));

	//connect sampleViewer to sample - resize
	QObject::connect(this, SIGNAL(sgArtificialResize(QResizeEvent*)),
						smpl, SLOT(slArtificialResize(QResizeEvent*)));

	if( this->parent() != NULL ){

		//gmfc -> sample [global shifts]		 
		QObject::connect( this->parent(), SIGNAL(sgGlbSliderChanged(int)),
							smpl,SLOT( sliderChanged(int)));

		//gmfc -> sample [global toolbar]
		QObject::connect( this->parent(), SIGNAL( sgGlbToolbarAction( Toolbar::ACTIONS )),
							smpl,SLOT( slTBarActivated( Toolbar::ACTIONS )));

		//sample -> gmfc [draged address]
		QObject::connect( smpl, SIGNAL(sgDragAtAddress( QString )),
							this->parent(), SLOT( slDragAtAddress( QString )));
	}
	
	return;
}

void SamplesViewer::discSample( QString fName ){
	
	//check if sample exists
	if( !_storWgts->contains(fName) ){
		return;
	}

	//download sample
	FileWidget* smpl = _storWgts->value(fName);

	//disconnect signals from sampleViewer - closing
	QObject::disconnect( smpl, SIGNAL(closing(QString)),
							this, SLOT(remSample(QString)));

	//connect to sampleViewer - toolbar used
	QObject::disconnect( smpl, SIGNAL(focusReq(FileWidget*)),
						this, SLOT(setPermFocus(FileWidget*)));

	QObject::disconnect( smpl, SIGNAL( contentIsChanging( QString, QList<int>* ) ),
						this, SLOT( sampleChanged( QString, QList<int>* )));

	if( this->parent() != NULL ){

		//gmfc -> sample [global shifts]		 
		QObject::disconnect( this->parent(), SIGNAL(sgGlbSliderChanged(int)),
							smpl,SLOT( sliderChanged(int)));

		//gmfc -> sample [global toolbar]
		QObject::disconnect( this->parent(), SIGNAL( sgGlbToolbarAction( Toolbar::ACTIONS )),
							smpl,SLOT( slTBarActivated( Toolbar::ACTIONS )));

		//sample -> gmfc [draged address]
		QObject::disconnect( smpl, SIGNAL(sgDragAtAddress( QString )),
							this->parent(), SLOT( slDragAtAddress( QString )));
	}

	return;
}

void SamplesViewer::resizeEvent( QResizeEvent* ){
	return;
}

bool SamplesViewer::eventFilter( QObject* obj, QEvent* ev ){

	switch( ev->type() ){

		//click on widget
		case QEvent::MouseButtonPress:

			//finish event calling
			ev->accept();

			//permanent colorization
			this->setPermFocus((FileWidget*)obj);

			break;

		case QEvent::Wheel:

			//finish event calling
			ev->accept();

			//is any widget focused
			if( _pFocused != NULL ){

				//send event to proper sample
				_pFocused->wheelEvent( (QWheelEvent*)ev );
			}

			break;		

		default:

			//send event filter to other instances
			return QObject::eventFilter( obj, ev);
	}

	return true;
}

void SamplesViewer::setPermFocus( FileWidget* smpl ){

	//already focused
	if( smpl->Focus() ){
		return;
	}

	//set blur to previous focused sample if exists
	if( _pFocused != NULL ){		
		
		_pFocused->setBlur( true );
	}

	//replace actual focused sample and set up focus
	_pFocused = smpl;
	_pFocused->setFocus( true );

	//empty event queue due to ensure widget's visibility
	qApp->processEvents();

	//ensure visibility for new sample
	_scroll->ensureWidgetVisible( smpl, X_VIS_MARGIN, Y_VIS_MARGIN );	

	return;
}

void SamplesViewer::sampleChanged( QString fName, QList<int>* dataContent ){

	emit this->sampleIsChanging( fName, dataContent );
	
	return;
}

void SamplesViewer::sampleChangedSilent( QString fName, QList<int>* dataContent ){

	emit this->sampleIsChangingSilent( fName, dataContent );
	
	return;
}

void SamplesViewer::keyPressEvent( QKeyEvent *event){
	
	switch( event->key() ){

		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_PageDown:
		case Qt::Key_PageUp:
		case Qt::Key_Home:
		case Qt::Key_End:

			//finish event calling
			event->accept();

			//focused sample exists
			if( _pFocused != NULL ){

				//activating keyboard eve
				_pFocused->keyPressEvent(event);
			}

			break;

		default:

			//ignore event - resend to parent widget
			event->ignore();
			break;
	}

	return;
}

void SamplesViewer::artificialResize(QResizeEvent* ptrEv){

	int diffH = ptrEv->oldSize().height() - ptrEv->size().height();
	int diffW = ptrEv->oldSize().width() - ptrEv->size().width();

	if((diffH == 0) && (diffW == 0)){
		return;
	}

	//rounded difference
	int roundedDiffH = (diffH + _floatResBufferH) / CELL_H;

	//temporary resize buffer
	int tmpFloatBufferH = (diffH + _floatResBufferH) % CELL_H;

	_floatResBufferH = tmpFloatBufferH;

	//no resize is needed - no rounded difference exists
	if((roundedDiffH == 0) && (diffW == 0)){
		return;
	}

	//create new resize event
	QSize roundedSize(ptrEv->size().width(),
						(ptrEv->oldSize().height() - (roundedDiffH * CELL_H)));
	QResizeEvent internalEv(roundedSize, ptrEv->oldSize());

	//signal to all subordinate samples
	emit this->sgArtificialResize(&internalEv);

	//resize scroll area + sample vieer itself
	_scroll->resize(
		_scroll->width() - diffW, _scroll->height() - (roundedDiffH * CELL_H));
	this->resize(_scroll->width(), _scroll->height());

	int scrollBarHeight = _scroll->horizontalScrollBar()->height();
	
	//properly resize sample viewer's base frame (holder of layout)
	if(_storWgts->count() == 0){		
		_base->setFixedSize(SMPL_W + 10 - 2, 
						_base->height() - (roundedDiffH * CELL_H));	
	}
	else{		
		_base->setFixedSize(_storWgts->count()*(SMPL_W + 5), 
						_base->height() - (roundedDiffH * CELL_H));
	}

	//emit resize signal to resultWidget
	emit this->sgFinalResizeRefresh();

	return;
}

