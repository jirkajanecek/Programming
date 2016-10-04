/**
 * HexaViewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef HEXAVIEWERSELECTABLE_H
#define HEXAVIEWERSELECTABLE_H

#include "viewerselectable_v3.h"

class HexaViewerSelectable : public ViewerSelectable
{
	Q_OBJECT

public:
	
	HexaViewerSelectable(
		QString*		ptrLabelForm,
		QList<QString>*	ptrDataForm,
		QString			fName,
		QWidget*		ptrParent = 0);
	
	/**
	 * Destructor
	 **/
	~HexaViewerSelectable();	
	
protected:

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics(void);	
};

#endif // HEXAVIEWERSELECTABLE_H
