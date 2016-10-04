#include "gmfcToolbar_v2.h"

GMFCToolbar::GMFCToolbar( int w, int h, QWidget *parent ) : Toolbar(w, h, parent){	

	this->setOrientation(Qt::Vertical);
}

GMFCToolbar::~GMFCToolbar(){}

void GMFCToolbar::buildIcons( void ){

	QIcon icon;
	QAction* action = NULL;

	///sample's action

	//ADD NEW SAMPLE
	//general icons
	icon = QIcon::fromTheme("Add new sample",
			QIcon(":/GMFC/Resources/addSmpl.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Add new sample");

	//insert action into internal action array
	_actions->insert(action, Toolbar::GMFC_ADD);

	action = this->addSeparator();
	_separators.push_back(action);

	//SHOW OPEN FILES
	icon = QIcon::fromTheme("Show opened files",
			QIcon(":/GMFC/Resources/fileList.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Show opened files");

	//insert action into internal action array
	_actions->insert(action, Toolbar::GMFC_FILELIST);

	action = this->addSeparator();
	_separators.push_back(action);

	///* SEARCH

	//search binary sequence
	icon = QIcon::fromTheme("Binary search",
			QIcon(":/Toolbar/Resources/search.svg"));
	action = this->addAction(icon, "Binary search");
	_actions->insert(action, Toolbar::BIN_SEARCH_GLOBAL);

	//search text sequence
	icon = QIcon::fromTheme("Text search",
			QIcon(":/Toolbar/Resources/search_text.svg"));
	action = this->addAction(icon, "text search");
	_actions->insert(action, Toolbar::TXT_SEARCH_GLOBAL);

	action = this->addSeparator();
	_separators.push_back(action);

	///* JUMPS

	///JUMP TO ENTRY POINT

	//general icons
	icon = QIcon::fromTheme("Jump entry point",
			QIcon(":/GMFC/Resources/entryPoint.svg"));

	//create action from icon and title
	action = this->addAction(icon,"jump to entry point");

	//insert action into internal action array
	_actions->insert(action, Toolbar::ENTRY_POINT);

	///JUMP TO OVERLAY

	//entry point
	icon = QIcon::fromTheme("Jump to overlay", 
			QIcon(":/GMFC/Resources/overlay.svg"));

	action = this->addAction(icon, "jump to overlay");

	_actions->insert(action, Toolbar::OVERLAY);

	///JUMP TO DIRECT ADDRESS
	icon = QIcon::fromTheme("Concrete address",
			QIcon(":/Toolbar/Resources/targetAddress.svg"));
	action = this->addAction(icon, "Concrete address");
	_actions->insert(action, Toolbar::DIRECT_ADDRESS_GLOBAL);
	
	///JUMP TO PREVIOUS ROW
	
	//general icons
	icon = QIcon::fromTheme("Jump to one row up",
			QIcon(":/GMFC/Resources/jmp_up_row.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Jump to one row up");

	//insert action into internal action array
	_actions->insert(action, Toolbar::JMP_ROW_U);

	///JUMP TO NEXT ROW

	//general icons
	icon = QIcon::fromTheme("Jump to one row down",
			QIcon(":/GMFC/Resources/jmp_down_row.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Jump to one row down");

	//insert action into internal action array
	_actions->insert(action, Toolbar::JMP_ROW_D);

	///JUMP TO NEXT PAGE
	
	//general icons
	icon = QIcon::fromTheme("Jump to one page up",
			QIcon(":/GMFC/Resources/jmp_up_page.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Jump to one page up");

	//insert action into internal action array
	_actions->insert(action, Toolbar::JMP_PAGE_U);

	///JUMP TO PREVIOUS PAGE
	
	//general icons
	icon = QIcon::fromTheme("Jump to one page down",
			QIcon(":/GMFC/Resources/jmp_down_page.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Jump to one page down");

	//insert action into internal action array
	_actions->insert(action, Toolbar::JMP_PAGE_D);

	///JUMP TO BEGIN
	
	//general icons
	icon = QIcon::fromTheme("Jump to file begin",
			QIcon(":/GMFC/Resources/jmp_beg.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Jump to file begin");

	//insert action into internal action array
	_actions->insert(action, Toolbar::JMP_BEG);

	///JUMP TO END
	
	//general icons
	icon = QIcon::fromTheme("Jump to file end",
			QIcon(":/GMFC/Resources/jmp_end.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Jump to file end");

	//insert action into internal action array
	_actions->insert(action, Toolbar::JMP_END);	

	return;
}

//void GMFCToolbar::artificialResize(QResizeEvent* ptrEv){
void GMFCToolbar::artificialResize(int diffH){

	_height -= diffH;
		
	//resize toolbar
	_base->resize(_width, _height);
	this->resize(_width, _height);

	return;
}