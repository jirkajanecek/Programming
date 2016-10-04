#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QWidget>
#include <QHeaderView>

#include <qstandarditemmodel.h>
#include <qtableview.h>

//#include <qdebug.h>

#include "viewerStyles.h"
#include "styles_v3.h"

#include "viewer_v3.h"

class AddressViewer : public Viewer
{
	Q_OBJECT

public:

	AddressViewer(
		QString*		ptrLabelForm,
		QList<QString>*	ptrDataForm,
		QString			fName,
		QWidget*		ptrParent = 0);

	/** 
	 * implicit constructor
	 **/
	~AddressViewer();

protected:
	
	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics(void);
};

#endif // ADDRESSBAR_H
