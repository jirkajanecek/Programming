#include "scrollbar.h"

ScrollBar::ScrollBar(int barHeight, QWidget *parent, Qt::WFlags )
	: QScrollBar(Qt::Vertical, parent){
	
	this->resize(15,barHeight);
}

ScrollBar::~ScrollBar( void ){
}

void ScrollBar::configure(int min, int max, int sPos, int sStep, int pStep){

	this->setMinimum(min);

	this->setMaximum(max);

	this->setValue(sPos);

	this->setSingleStep(sStep);

	this->setPageStep(pStep);

	this->setStyleSheet("QScrollBar{}");

	this->setFocusPolicy(Qt::WheelFocus);

	return;
}

void ScrollBar::artificialResize(QResizeEvent* ptrEv, int value){

	//height difference in pixels
	int diffH = ptrEv->oldSize().height() - ptrEv->size().height();

	//setup new page step
	int newPageStep = this->pageStep() - ((diffH / CELL_H)*this->singleStep());
	this->setPageStep(newPageStep);
	
	//scroll bar internal values
	this->setMinimum(1 - newPageStep);
	this->setValue(value);	

	//resize scroll bar itself
	this->resize(this->width(), this->height() - diffH);

	return;
}

void ScrollBar::viewerChanged( int value ){
	
	this->setValue( value );

	return;
}