/**
 * HexaViewer ver 1.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QtGui/QWidget>

#include "viewer.h"

class TextViewer : public Viewer
{
	Q_OBJECT

public:
	
	/**
	 * Explicit constructor
	 * @param file name for view
	 * @param Qt parent widget
	 * @param Qt flags
	 **/
	TextViewer(QString filename = "",
				int rows = INIT_ROWS,
				int cols = INIT_COLS,
				QWidget *parent = 0,
				Qt::WFlags flags = 0);
	
	/**
	 * Destructor
	 **/
	~TextViewer();
	
private:

	QString _substitutionalChar;	/**< substitutional char for unprintable characters */

protected:

	//PROPERTY functions

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics( void );
	
	//CONTENT functions
	
	/**
	 * Method used for translation from char into hexa number of inputed char.
	 * @param inputed char [byte]
	 * @return char hexa value in string type.
	 **/
	virtual QString charToCellLabel(unsigned char byte );	
};

#endif // TEXTVIEWER_H