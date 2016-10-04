#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QtGui/QWidget>
#include <QScrollBar>
#include <QWheelEvent>
#include <QResizeEvent>

#include <qcoreapplication.h>

#include <qdebug.h>

#include "styles.h"

class ScrollBar : public QScrollBar
{
	Q_OBJECT

public:
	
	/**
	 * Constructor
	 * @param scroll bar height
	 * @param parent widget
	 * @param flags
	 **/	 
	ScrollBar(int barHeight = 0,QWidget *parent = 0,Qt::WFlags flags = 0);

	/**
	 * Destructor
	 **/
	~ScrollBar();

	/**
	 * Method configures scroll bar object.
	 * @param minimum value of file
	 * @param maximum value of file
	 * @param initial file position
	 * @param size of single step
	 * @param size of page step
	 **/
	void configure( int min, int max, int value, int sStep, int pStep);

	void artificialResize(int newHeight, int value);

protected:

	virtual bool event(QEvent* ptrEvent);
};

#endif // SCROLLBAR_H
