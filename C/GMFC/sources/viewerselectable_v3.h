/**
 * Viewer ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef VIEWERSELECTABLE_H
#define VIEWERSELECTABLE_H

#include <QWidget>
#include <QLabel>
#include <QDragEnterEvent>
#include <QByteArray>
#include <QString>
#include <QPixmap>

#include <QTableWidgetSelectionRange>

#include <qcoreapplication.h>
#include <qmessagebox.h>
#include <qvariant.h>

#include <QDebug>

#include "selectablelayer.h"
#include "viewerStyles.h"
#include "viewer_v3.h"
#include "styles_v3.h"


class ViewerSelectable : public Viewer{

	Q_OBJECT

public:

	/**
	 * Explicit constructor
	 * @param data model
	 * @param Qt parent widget
	 **/
	ViewerSelectable(
		QString*		ptrLabelForm,
		QList<QString>*	ptrDataForm,
		QString			fName,
		QWidget*		ptrParent = 0);

	/**
	 * Destructor
	 **/
	~ViewerSelectable();

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics(void);

	void clearSelection(void);

	void selectedCellsInterval(signed int &startIndex, signed int &stopIndex);

	SelectableLayer* selectableLayer(void);

	void artificialResize(QString *ptrLabelForm, QList<QString> *ptrDataForm);	

protected:

	SelectableLayer* _ptrSelectableLayer;

	/**
	 * Handler accepts all key press events. It is due to selectable layer,
	 * which can not hold this event when selected cell reaches end of viewer
	 * space.
	 * @param key press event
	 * @return nothing
	 **/
	virtual void keyPressEvent(QKeyEvent *ptrEvent);
};

#endif // VIEWERSELECTABLE_H
