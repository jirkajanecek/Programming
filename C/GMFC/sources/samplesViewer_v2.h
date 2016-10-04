#ifndef SAMPLESVIEWER_H
#define SAMPLESVIEWER_H

#include <QWidget>
#include <QBoxLayout>
#include <QSizePolicy>
#include <qhash.h>

#include <QDropEvent>
#include <QDragEnterEvent>

#include <qdebug.h>
#include <qgraphicseffect.h>

#include "styles.h"
#include "filewidget_v2.h"

class SamplesViewer : public QScrollArea
{
	Q_OBJECT

public:
	
	/**
	 * implicit constructor [only one]
	 **/
	SamplesViewer(QWidget *parent = 0);
	
	/**
	 * implicit destructor
	 **/
	~SamplesViewer();

	/**
	 * Method configures SamplesViewer component.
	 * @return nothing
	 **/
	void configure(void);

	/**
	 * Method adds new sample into sample viewer.
	 * @param file name of the new inputed sample
	 * @return added sample object
	 **/
	FileWidget* addSample(QString fName);

	void artificialResize(int diffW, int diffH);

	FileWidget* focusedSample(void);

	void ensureVisibility(FileWidget* sample);

	void globalToolbarActivated(Toolbar::ACTIONS action);

signals:

	void sgSampleRemoved(QString fName);	

	void sgFinalResizeRefresh(void);

public slots:

	/**
	 * Method removes samples defined by file name from sample viewer.
	 * @param sample's file name
	 * @return nothing
	 **/
	void slRemoveSample(QString fName);

	void slFocusTaken(FileWidget* ptrSample);

private:

	QFrame*			_ptrBase;			/**< base frame */
	//QScrollArea*	_ptrScroll;			/**< scroll area */
	QHBoxLayout*	_ptrLayout;			/**< layout for file widgets */

	FileWidget*		_ptrFocusedSample;	/**< Focused sample */	
	
	int _scrollFloatBuffer;

	QHash<QString,FileWidget*>*	_ptrWidgetStorage;	/**< storage for created file widgets */

	/**
	 * Method configures base frame.
	 * @return nothing
	 **/
	void configureBase(void);

	/**
	 * Method configures layout for file widgets.
	 * @return nothing
	 **/
	void configureLayout(void);

	/**
	 * Method configures scroll area for potential file widget.
	 * @return nothing
	 **/
	void configureScroll(void);

	/**
	 * Method creates new sample and stores it into internal storage.
	 * @param sample's file name
	 * @return pointer to created widget
	 **/
	FileWidget* crNewSmpl(QString fName);

	/**
	 * Method connects sample's signals to sampleViewer or other
	 * components, which are required.
	 * @param sample's file name.
	 * @return nothing
	 **/
	void connectSample(QString fName);

	/**
	 * Method disconnects sample's signals to sampleViewer or other
	 * components, which are required.
	 * @param sample's file name.
	 * @return nothing
	 **/
	void disconnectSample(QString fName);

	/**
	 * Event filter installed in 
	 **/
	bool eventFilter(QObject* obj, QEvent* ev);

protected:

	virtual void keyPressEvent(QKeyEvent* ptrEvent);	
};

#endif // SAMPLESVIEWER_H
