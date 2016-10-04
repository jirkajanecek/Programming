#ifndef SAMPLESVIEWER_H
#define SAMPLESVIEWER_H

#include <QWidget>
#include <QBoxLayout>
#include <QSizePolicy>

#include <QDropEvent>
#include <QDragEnterEvent>

#include <qdebug.h>

#include "styles.h"
#include "filewidget.h"

class SamplesViewer : public QWidget
{
	Q_OBJECT

public:
	
	/**
	 * implicit constructor [only one]
	 **/
	SamplesViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	
	/**
	 * implicit destructor
	 **/
	~SamplesViewer();

	/**
	 * Method configures SamplesViewer component.
	 * @return nothing
	 **/
	void configure( void );

	/**
	 * Method adds new sample into sample viewer.
	 * @param file name of the new inputed sample
	 * @return added sample object
	 **/
	FileWidget* addSample( QString fName );

	void artificialResize(QResizeEvent* ptrEv);

signals:

	void sampleIsChanging(QString fName, QList<int>* dataContent);

	void sampleIsChangingSilent(QString fName, QList<int>* dataContent);

	void sgSampleRemoved(QString fName);

	void sgArtificialResize(QResizeEvent* ptrEv);

	void sgFinalResizeRefresh(void);

public slots:

	/**
	 * Method removes samples defined by file name from sample viewer.
	 * @param sample's file name
	 * @return nothing
	 **/
	void remSample( QString fName );

	/**
	 * Method sets permanent focus (proper coloring) for selected
	 * sample. Also method erases color from previous widget.
	 * @param selected sample
	 * @param true if sample already exists or false if sample was created now
	 * @return nothing
	 **/
	void setPermFocus( FileWidget* smpl );

	/**
	 * Method handles changes (shifts) in sample.
	 * @param file name
	 * @param data array
	 * @return nothing
	 **/
	void sampleChanged( QString fName, QList<int>* dataContent );

	/**
	 * Method handles changes (shifts) in sample. Silent version
	 * @param file name
	 * @param data array
	 * @return nothing
	 **/
	void sampleChangedSilent( QString fName, QList<int>* dataContent );

private:

	QFrame*			_base;			/**< base frame */
	QScrollArea*	_scroll;		/**< scroll area */
	QHBoxLayout*	_layout;		/**< layout for file widgets */

	QHash<QString, FileWidget*>*	_storWgts;		/**< storage for created file widgets */

	FileWidget*		_pFocused;		/**< Focused sample */

	int _floatResBufferH;
	
	/**
	 * Method configures base frame.
	 * @return nothing
	 **/
	void confBase( void );

	/**
	 * Method configures layout for file widgets.
	 * @return nothing
	 **/
	void confLayout( void );

	/**
	 * Method configures scroll area for potential file widget.
	 * @return nothing
	 **/
	void confScroll( void );

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
	void connSample(QString fName);

	/**
	 * Method disconnects sample's signals to sampleViewer or other
	 * components, which are required.
	 * @param sample's file name.
	 * @return nothing
	 **/
	void discSample(QString fName);

	/**
	 * Method updates layout's samples. Provides complete rebuild of
	 * layout by new positioning.
	 * @param index to layout
	 * @return nothing
	 **/
	void updLayout(void);

	/**
	 * Resize event handler. Method resizes samples viewer.
	 * @param resize event
	 * @return nothing
	 **/	
	void resizeEvent(QResizeEvent *re);

	/**
	 * Event filter installed in 
	 **/
	bool eventFilter(QObject* obj, QEvent* ev);

protected:

	void keyPressEvent(QKeyEvent* event);
};

#endif // SAMPLESVIEWER_H
