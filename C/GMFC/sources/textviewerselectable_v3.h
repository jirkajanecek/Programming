/**
 * HexaViewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef TEXTVIEWERSELECTABLE_H
#define TEXTVIEWERSELECTABLE_H

#include "viewerselectable_v3.h"

class TextViewerSelectable : public ViewerSelectable
{
	Q_OBJECT

public:
	
	TextViewerSelectable(
		QString*		ptrLabelForm,
		QList<QString>*	ptrDataForm,
		QString			fName,
		QWidget*		ptrParent = 0);
	
	~TextViewerSelectable();

protected:

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics(void);
};

#endif // TEXTVIEWERSELECTABLE_H