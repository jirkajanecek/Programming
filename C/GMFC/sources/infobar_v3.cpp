#include "infobar_v3.h"

InfoBar::InfoBar(QWidget *parent, Qt::WFlags flags)
	: QFrame(parent, flags){

	//assembly label
	QString daLabel = IB_DA_TEXT; daLabel += IB_DA_VAL_DFT;
	QString srLabel = IB_SR_TEXT; srLabel += IB_SR_VAL_DFT;

	//draged byte's address
	_ptrDA = new QLabel( daLabel,this);

	//success rate
	_ptrSR = new QLabel( srLabel,this);
}

InfoBar::~InfoBar(){
	
	delete _ptrDA;
	delete _ptrSR;
}

void InfoBar::setDA( QString addr ){

	//DA label
	QString daLabel = IB_DA_TEXT;

	//check on address size
	if( addr.size() > 0 ){
		
		daLabel += addr;		
	}
	else{

		daLabel += IB_DA_VAL_DFT;		
	}

	//setup DA text
	_ptrDA->setText( daLabel );

	return;
}

QString InfoBar::DA( void ){

	return _ptrDA->text().remove(IB_DA_TEXT);	
}

void InfoBar::setSR( QString rate ){

	QString srLabel = IB_SR_TEXT;

	//check on rate size
	if( rate.size() > 0 ){
		
		srLabel += rate + " %";
	}
	else{

		srLabel += IB_SR_VAL_DFT;
	}

	//setup SR text
	_ptrSR->setText( srLabel );

	return;
}

QString InfoBar::SR( void ){

	return _ptrSR->text().remove(IB_SR_TEXT).remove(" %");
}

void InfoBar::configure( int w ){

	//setup proper size to bar
	this->resize( w, BAR_HEIGHT );
	this->setStyleSheet( BAR_STYLE );
	
	//object's font
	QFont font;
	font.setFamily( IB_FONT );
	font.setPixelSize(11);
	font.setBold(true);
	
	//setup draged address
	_ptrDA->resize( IB_DA_WIDTH, IB_DA_HEIGHT );
	_ptrDA->setStyleSheet(IB_CELL_STYLE);
	_ptrDA->move(5,3);
	_ptrDA->setFont(font);
	_ptrDA->setIndent(5);

	//setup success rate
	_ptrSR->resize( IB_SR_WIDTH, IB_SR_HEIGHT );
	_ptrSR->setStyleSheet(IB_CELL_STYLE);
	_ptrSR->move(_ptrDA->width() + 5 + 5, 3);
	_ptrSR->setFont(font);
	_ptrSR->setIndent(5);

	return;
}