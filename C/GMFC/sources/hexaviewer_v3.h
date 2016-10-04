/**
 * HexaViewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef HEXAVIEWER_H
#define HEXAVIEWER_H

#include "viewer_v3.h"

class HexaViewer : public Viewer
{
	Q_OBJECT

public:
	
	HexaViewer(
		QString*		ptrLabelForm,
		QList<QString>*	ptrDataForm,
		QString			fName,
		QWidget*		ptrParent = 0);
	
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
