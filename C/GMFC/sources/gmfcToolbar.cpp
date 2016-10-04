#include "gmfcToolbar.h"

GMFCToolbar::GMFCToolbar( int w, int h, QWidget *parent ) : Toolbar(w, h, parent){	

	this->setOrientation( Qt::Vertical );

	_floatResBufferH = 0;
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

	///JUMP OPTIONALLY
	
	//general icons
	icon = QIcon::fromTheme("Jump optionally",
			QIcon(":/GMFC/Resources/jmp_opt.svg"));

	//create action from icon and title
	action = this->addAction(icon,"Jump optionally");

	//insert action into internal action array
	_actions->insert(action, Toolbar::JMP_OPT);

	return;
}

//void GMFCToolbar::artificialResize(QResizeEvent* ptrEv){
void GMFCToolbar::artificialResize(QResizeEvent* ptrEv){

	//size difference
	int diffH = ptrEv->oldSize().height() - ptrEv->size().height();
	
	if(diffH == 0){
		return;
	}

	//rounded difference
	int roundedDiffH = (diffH + _floatResBufferH) / CELL_H;

	//temporary resize buffer
	int tmpFloatBufferH = (diffH + _floatResBufferH) % CELL_H;

	_floatResBufferH = tmpFloatBufferH;

	//no resize is needed - no rounded difference exists
	if(roundedDiffH == 0){
		return;
	}

	_height -= (roundedDiffH * CELL_H);
		
	//resize toolbar
	_base->resize(_width, _height);
	this->resize(_width, _height);

	return;
}