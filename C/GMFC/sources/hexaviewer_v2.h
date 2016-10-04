/**
 * HexaViewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef HEXAVIEWER_H
#define HEXAVIEWER_H

#include "viewer_v2.h"

class HexaViewer : public Viewer
{
	Q_OBJECT

public:
	
	/**
	 * Explicit constructor
	 * @param file name for view
	 * @param Qt parent widget
	 * @param Qt flags
	 **/
	HexaViewer(QStandardItemModel* ptrModel, QString fName, QWidget *parent = 0);
	
	/**
	 * Destructor
	 **/
	~HexaViewer();	
	
protected:

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics(void);
};

#endif // HEXAVIEWER_H
