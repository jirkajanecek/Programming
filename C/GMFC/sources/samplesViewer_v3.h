#ifndef SAMPLESVIEWER_H
#define SAMPLESVIEWER_H

#include <QWidget>
#include <QBoxLayout>
#include <QSizePolicy>
#include <qhash.h>
#include <qstring.h>

#include <QDropEvent>
#include <QDragEnterEvent>

#include <qdebug.h>

#include "styles_v3.h"
#include "filewidget_v3.h"

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

	void focusNextSample(void);

signals:

	void sgSampleRemoved(QString fName, bool silent = false);

	void sgSampleAdded(QString fName, QList<int>* ptrDataContent, QString md5);

	void sgFinalResizeRefresh(void);

public slots:

	/**
	 * Method removes samples defined by file name from sample viewer.
	 * @param sample's file name
	 * @return nothing
	 **/
	void slRemoveSample(QString fName);

	void slRemoveSamples(QStringList fNames);

	void slDisableSample(QString fName);

	void slEnableSample(QString fName, QList<int>* ptrDataContent, QString md5);

	void slFocusTaken(FileWidget* ptrSample);

	void slCheckDialog(QString value);

private:

	QFrame*			_ptrBase;			/**< base frame */
	QHBoxLayout*	_ptrLayout;			/**< layout for file widgets */

	FileWidget*		_ptrFocusedSample;	/**< Focused sample */

	FileWidget::DIALOG_TYPES	_dType;	/**< Dialog type */
	QDialog*		_ptrActiveDialog;	/**< Dialog active right now */
	
	QHash<QString,FileWidget*>*	_ptrWidgetStorage;	/**< storage for created file widgets */

	QString _searchedBinarySequence;		/**< repeated binary search used also as input history */
	QString	_searchedTextSequence;			/// repeated text search, also used as input history

	FileWidget::SEARCH_TYPES _searchType;	///search type, when global search type 
	
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

	QString createGlobalBinarySearchDialog(bool &eraseFlag);

	QString createGlobalTextSearchDialog(bool &eraseFlag);

	QString createGlobalTargetAddressDialog(void);

protected:

	virtual void keyPressEvent(QKeyEvent* ptrEvent);	
};

#endif // SAMPLESVIEWER_H
