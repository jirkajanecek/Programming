#include "toolbar.h"

Toolbar::Toolbar(
		int w,
		int h,
		QWidget *parent): QToolBar(TOOL_TITLE, parent)
{
	_height = h;
	_width = w;
	_base = new QFrame(this);
	_actions = new QHash<QAction*, Toolbar::ACTIONS>();
}

Toolbar::~Toolbar(){
	delete _base;
	delete _actions;
}

QHash<QAction*, Toolbar::ACTIONS>* Toolbar::Actions( void ){
	return _actions;
}

void Toolbar::configure( std::bitset<Toolbar::SUM> options ){
	
	this->setMovable(false);
	this->resize(_width, _height);
	
	//commented due to resizing acceptance
	/*this->setMinimumWidth(_width);
	this->setMaximumWidth(_width);
	this->setMaximumHeight(_height);
	this->setMinimumHeight(_height);*/

	//this->setStyle( &QWindowsVistaStyle() );
	this->setStyleSheet("QIcon{padding-left:0px;padding-right:0px;margin-left:0px;margin-right:0px;}");
	this->setIconSize( QSize(TOOL_ICON_WIDTH, TOOL_ICON_HEIGHT));

	_options = options;
	
	this->setupBaseFrame();	
	this->buildIcons();

	return;
}

void Toolbar::setupBaseFrame( void ){	
	
	_base->resize(_width, _height);
	_base->setFrameStyle(QFrame::Box | QFrame::Plain);
	_base->setLineWidth(0);
	_base->setStyleSheet("background-color:#707070;"
							"border-width:1px;");
	return;
}

void Toolbar::buildIcons( void ){
	
	QIcon icon;	
	QAction* action = NULL;

	///concrete address
	icon = QIcon::fromTheme(
		"concrete address",
		QIcon(":/Toolbar/Resources/targetAddress.svg"));	//target address
	action = this->addAction(icon,"jump to concrete address");
	_actions->insert(action, Toolbar::DIRECT_ADDRES);

	///jump to begin
	icon = QIcon::fromTheme(
		"Jump to file begin",
		QIcon(":/Toolbar/Resources/jmp_beg.svg"));
	action = this->addAction(icon,"Jump to file begin");
	_actions->insert(action, Toolbar::JMP_BEG);

	///jump to end
	icon = QIcon::fromTheme(
		"Jump to file end",
		QIcon(":/Toolbar/Resources/jmp_end.svg"));
	action = this->addAction(icon,"Jump to file end");
	_actions->insert(action, Toolbar::JMP_END);

	this->addSeparator();	//separator

	///binary search
	icon = QIcon::fromTheme(
		"Binary search",
		QIcon(":/Toolbar/Resources/search.svg"));
	action = this->addAction(icon,"Binary search");
	_actions->insert(action, Toolbar::BIN_SEARCH);

	///text serch
	icon = QIcon::fromTheme(
		"Text search",
		QIcon(":/Toolbar/Resources/search_text.svg"));
	action = this->addAction(icon,"Text search");
	_actions->insert(action, Toolbar::TXT_SEARCH);

	this->addSeparator();	//separator

	///general description
	icon = QIcon::fromTheme(
		"description",
		QIcon(":/Toolbar/Resources/desc.svg"));	//general description
	action = this->addAction(icon, tr("show file description"));
	_actions->insert(action, Toolbar::GEN_DESC);

	this->addSeparator();	//separator

	///PE files
	if(_options.test(Toolbar::PE)){

		///image base addresses
		icon = QIcon::fromTheme(
			"show addresses with image base offset", 
			QIcon(":/Toolbar/Resources/image_base.svg"));
		action = this->addAction(icon,"show addresses with image base offset");
		_actions->insert(action, Toolbar::IMAGE_BASE);
		
		///entry point
		icon = QIcon::fromTheme(
			"entry point", 
			QIcon(":/Toolbar/Resources/entryPoint.svg"));
		action = this->addAction(icon, "jump to entry point");
		_actions->insert(action, Toolbar::ENTRY_POINT);

		//overlay
		if( _options.test(Toolbar::PE_OVERLAY) ){

			icon = QIcon::fromTheme(
				"overlay", 
				QIcon(":/Toolbar/Resources/overlay.svg"));
			action = this->addAction(icon, "jump to overlay");
			_actions->insert(action, Toolbar::OVERLAY);
		}
		
		//pe section
		icon = QIcon::fromTheme(
			"PE header",
			QIcon(":/Toolbar/Resources/pe.svg"));
		action = this->addAction(icon, "jump to PE header");
		_actions->insert(action, Toolbar::PE_SECTION);

		this->addSeparator();	//separator

		///save pattern
		icon = QIcon::fromTheme(
			"Add save pattern",
			QIcon(":/Toolbar/Resources/save_pattern.svg"));
		action = this->addAction(icon, "Add save pattern");
		_actions->insert(action, Toolbar::SAVE_PATTERN);

		///save pattern fwd
		icon = QIcon::fromTheme(
			"Next save area",
			QIcon(":/Toolbar/Resources/save_pattern_fwd.svg"));
		action = this->addAction(icon, "Next save area");
		_actions->insert(action, Toolbar::SAVE_PATTERN_FORWARD);

		//save pattern bck
		icon = QIcon::fromTheme(
			"Previous save area",
			QIcon(":/Toolbar/Resources/save_pattern_bck.svg"));
		action = this->addAction(icon, "Previous save area");
		_actions->insert(action, Toolbar::SAVE_PATTERN_BACKWARD);

		this->addSeparator();	//separator
	}

	icon = QIcon::fromTheme(
		"disable sample",
		QIcon(":/Toolbar/Resources/disable.svg"));
	action = this->addAction(icon,"disable sample");
	_actions->insert(action, Toolbar::DISABLE);

	//visual icons
	icon = QIcon::fromTheme(
		"close file",
		QIcon(":/Toolbar/Resources/close.svg"));
	action = this->addAction(icon,"close file");
	_actions->insert(action, Toolbar::CLOSE);

	return;
}

QSize Toolbar::usableSize(void){

	//usable width & height
	int usWidth = 0;
	int usHeight = 0;

	Qt::Orientation orient = this->orientation();
	QList<QAction*> actions = _actions->keys();

	//default value based on orientation type
	if(orient == Qt::Horizontal){
		usHeight = this->height();
	}
	else{
		usWidth = this->width();
	}

	//icons round
	for(int i = 0; i < actions.count(); i++ ){
		
		if(orient == Qt::Horizontal){
			usWidth += this->widgetForAction(actions[i])->width();
		}
		else{
			usHeight += this->widgetForAction(actions[i])->height();
		}
	}

	//separators round
	for(int i = 0; i < _separators.count(); i++){

		if(orient == Qt::Horizontal){
			usWidth += this->widgetForAction(_separators[i])->width();
		}
		else{
			usHeight += this->widgetForAction(_separators[i])->height();
		}
	}

	//end stuff
	if(((actions.count() > 0) ||
		(_separators.count() > 0)) &&
		(orient == Qt::Horizontal)){
		
		usWidth += USABLE_END_STUFF;
	}
	else{
		usHeight += USABLE_END_STUFF;
	}

	return QSize( usWidth, usHeight);
}