#include "cell.h"

Cell::Cell( const QString &text,int width,int height,QWidget *parent,QPoint *pos) 
	: QLabel( text, parent )
{
	QFont font;
	font.setFamily( CODE_FONT );
	font.setPixelSize(10);
	font.setBold(false);
	if( pos == NULL )
	{
		this->resize(width, height);
	}
	else
	{
		this->setGeometry( pos->x(), pos->y(), width, height );
	}
	this->setFont(font);
	this->setLineWidth(1);
	this->setAlignment(Qt::AlignCenter);
	this->setStyleSheet(CELL_STYLE);
}

Cell::Cell(	int width,int height,QWidget *parent,QPoint *pos)
	: QLabel("", parent)
{
	QFont font;
	font.setFamily( CODE_FONT );
	font.setPixelSize(10);
	font.setBold(false);
	if( pos == NULL )
	{
		this->resize(width, height);
	}
	else
	{
		this->setGeometry( pos->x(), pos->y(), width, height );
	}
	this->setFont(font);
	this->setLineWidth(1);
	this->setAlignment(Qt::AlignCenter);
	this->setStyleSheet(CELL_STYLE);
}

void Cell::setText(QString text)
{
	QLabel::setText(text);
	return;
}

void Cell::colorize(QString color)
{
	this->setStyleSheet(
		"background-color: " + color + ";"
		"border-width:0px;");
	return;
}