#include "resultToolbar.h"

ResultToolbar::ResultToolbar( int w, int h, QWidget* parent )
	: Toolbar( w, h, parent ){}

ResultToolbar::~ResultToolbar(){}

void ResultToolbar::buildIcons( void ){

	QIcon icon;
	QAction* action = NULL;

	///XDEF CLIPboard icon

	//general icons
	icon = QIcon::fromTheme("xDEF to clipboard",
			QIcon(":/ResultWidget/Resources/clip.svg"));

	//create action from icon and title
	action = this->addAction(icon,"XDF to clipboard");

	//insert action into internal action array
	_actions->insert(action, Toolbar::RES_CLIP);

	///XDEF to file icon

	//general icons
	icon = QIcon::fromTheme("xDEF to file",
			QIcon(":/ResultWidget/Resources/xdefFile.svg"));

	//create action from icon and title
	action = this->addAction(icon,"XDF to file");

	//insert action into internal action array
	_actions->insert(action, Toolbar::XDEF_FILE);

	///XDEF CLIPboard icon - light version

	//general icons
	icon = QIcon::fromTheme("XDF to clipboard - light version",
			QIcon(":/ResultWidget/Resources/clip_light.svg"));

	//create action from icon and title
	action = this->addAction(icon,"XDF to clipboard - light version");

	//insert action into internal action array
	_actions->insert(action, Toolbar::RES_CLIP_LIGHT);

	///XDEF to file icon - light version

	//general icons
	icon = QIcon::fromTheme("XDF to file - light version",
			QIcon(":/ResultWidget/Resources/xdefFile_light.svg"));

	//create action from icon and title
	action = this->addAction(icon,"XDF to file - light version");

	//insert action into internal action array
	_actions->insert(action, Toolbar::XDEF_FILE_LIGHT);

	this->addSeparator();

	///*CHECK CODE icon

	//general icons
	icon = QIcon::fromTheme("check code",
			QIcon(":/ResultWidget/Resources/check.svg"));

	//create action from icon and title
	action = this->addAction(icon,"check code");

	//insert action into internal action array
	_actions->insert(action, Toolbar::RES_CHECK);

	///*SKIP icon

	//general icons
	icon = QIcon::fromTheme("skip",
			QIcon(":/ResultWidget/Resources/skip.svg"));

	//create action from icon and title
	action = this->addAction(icon,"skip");

	//insert action into internal action array
	_actions->insert(action, Toolbar::RES_SKIP);

	///*SEQ icon

	//general icons
	icon = QIcon::fromTheme("sequence",
			QIcon(":/ResultWidget/Resources/seq.svg"));

	//create action from icon and title
	action = this->addAction(icon,"sequence");

	//insert action into internal action array
	_actions->insert(action, Toolbar::RES_SEQ);

	this->addSeparator();

	///*DEFAULT icon

	//general icons
	icon = QIcon::fromTheme("rollback selection",
			QIcon(":/ResultWidget/Resources/default.svg"));

	//create action from icon and title
	action = this->addAction(icon,"rollback selection");

	//insert action into internal action array
	_actions->insert(action, Toolbar::RES_DEFAULT);

	///*DEFAULT icon - ALL

	//general icons
	icon = QIcon::fromTheme("rollback selections ALL",
			QIcon(":/ResultWidget/Resources/default_all.svg"));

	//create action from icon and title
	action = this->addAction(icon,"rollback selections ALL");

	//insert action into internal action array
	_actions->insert(action, Toolbar::RES_DEFAULT_ALL);

	this->addSeparator();
	
	return;
}

