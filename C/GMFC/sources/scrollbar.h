#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QtGui/QWidget>
#include <QScrollBar>
#include <QWheelEvent>
#include <QResizeEvent>

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
	void configure( int min, int max, int sPos, int sStep, int pStep);

	void artificialResize(QResizeEvent *ptrEv, int value);	
	
public slots:

	/**
	 * Method [slot], which responds to viewer changes - shift version.
	 * @param cell shift
	 * @param position in file
	 * @param viewer offset
	 **/
	void viewerChanged( int value );

};

#endif // SCROLLBAR_H
