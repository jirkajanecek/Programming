/**
 * HexaViewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include "viewer_v2.h"

class TextViewer : public Viewer
{
	Q_OBJECT

public:
	
	/**
	 * Explicit constructor
	 * @param data model
	 * @param Qt parent widget
	 **/
	TextViewer(QStandardItemModel* ptrModel, QString fName, QWidget *parent = 0);
	
	/**
	 * Destructor
	 **/
	~TextViewer();

protected:

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics( void );	
};

#endif // TEXTVIEWER_H