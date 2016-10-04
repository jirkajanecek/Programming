/**
 * HexaViewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include "viewer_v3.h"

class TextViewer : public Viewer
{
	Q_OBJECT

public:
	
	TextViewer(
		QString*		ptrLabelForm,
		QList<QString>*	ptrDataForm,
		QString			fName,
		QWidget*		ptrParent = 0);
	
	~TextViewer();

protected:

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics( void );	
};

#endif // TEXTVIEWER_H