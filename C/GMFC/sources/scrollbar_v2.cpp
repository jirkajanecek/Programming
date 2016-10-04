#include "scrollbar_v2.h"

ScrollBar::ScrollBar(int barHeight, QWidget *parent, Qt::WFlags )
	: QScrollBar(Qt::Vertical, parent){
	
	this->resize(SCROLL_H,barHeight);
}

ScrollBar::~ScrollBar( void ){}

void ScrollBar::configure(int min, int max, int value, int sStep, int pStep){

	this->setMinimum(min);
	this->setMaximum(max);

	this->setValue(value);

	this->setSingleStep(sStep);
	this->setPageStep(pStep);

	this->setStyleSheet("QScrollBar{}");

	return;
}

void ScrollBar::artificialResize(int newHeight, int value){

	//NEED TO REWRITE	
	
	int pageStep = newHeight / CELL_H * this->singleStep();
	
	this->setPageStep(pageStep);
	this->setMinimum(1 - pageStep);
	//maximum stay on default

	this->setValue(value);	

	this->resize(SCROLL_H, newHeight);

	return;
}

bool ScrollBar::event(QEvent* ptrEvent){
	
	switch(ptrEvent->type()){

		case QEvent::Wheel:
			ptrEvent->ignore();
			return true;

		case QEvent::MouseButtonPress:
		
			//for focus request - result is not important
			if(this->parent() != NULL){
				QCoreApplication::sendEvent(this->parent(), ptrEvent);
			}
			return QScrollBar::event(ptrEvent);

		default:
			return QScrollBar::event(ptrEvent);

	}
}