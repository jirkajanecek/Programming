#ifndef GMFC_H
#define GMFC_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QFileDialog>
#include <QMimeData>
#include <QKeyEvent>
#include <QFrame>
#include <QUrl>

#include "notImplemented.h"

#include "samplesViewer_v2.h"
#include "resultWidget.h"
#include "gmfcToolbar_v2.h"
#include "styles.h"
#include "infobar.h"

class GMFC : public QMainWindow
{
	Q_OBJECT

public:
	
	/**
	 * implicit constructor
	 **/
	GMFC(QWidget *parent = 0, Qt::WFlags flags = 0);
	
	/**
	 * implicit destructor
	 **/
	~GMFC();

	void configure(void);

	/**
	 * Method directly adjust size of widget by inputed QResizeEvent.
	 * This method should be used when size adjustion is requested and
	 * and programer doesn't able to guarantee spontaneous event. Event
	 * should be accepted in upper level. Method doesn't care if event was
	 * accepted or not. This should be used for force resize.
	 * @param received resize event.
	 * @return nothing
	 **/
	void artificialResize(QResizeEvent *pEv);

public slots:

	void slTBarActivated(QAction* act);

	void slDragAtAddress(QString addr);

	void slSuccRateUpdate(QString rate);

	void slSampleIsChanging(QString fName, QList<int> *ptrDataContent);

	void slSampleIsChangingSilent(QString fName, QList<int> *ptrDataContent);

signals:

	/**
	 * Signal is emited to resultWidget when sample is added.
	 * @param sample's file name
	 * @return nothing
	 **/
	void samplesAdded(QString fName);

	void sampleIsChanging( QString fName, QList<int>* dataContent );

	void sampleIsChangingSilent( QString fName, QList<int>* dataContent );

	/**
	 * Signal, which is emited when global shift is requested.
	 * @param slider's action
	 * @return nothing
	 **/
	void sgGlbSliderChanged( int act );

	/**
	 * Signal, which is emited when toolbar action is requested - globally
	 * @param toolbar action
	 * @return nothing
	 **/
	void sgGlbToolbarAction( Toolbar::ACTIONS act );

private:

	int				_appH;			/**< application height */
	int				_appW;			/**< application width */
	
	SamplesViewer	*_pSv;			/**< samples viewer */
	ResultWidget	*_pRw;			/**< result widget */
	GMFCToolbar		*_pTb;			/**< toolbar widget */
	InfoBar			*_ptrIb;		/**< info bar */

	int _resizeFloatBuffer;
	
	QFrame			*_pBFrame;		/**< base frame */
	std::bitset<Toolbar::SUM>	_tTbOpt;	/**< options comming from toolbar */

	/**
	 * Method places components to proper position in window.
	 * @return nothing
	 **/
	void placeComponents( void );

	/**
	 * Method connects components beetwen themselfs.
	 * @return nothing
	 **/
	void connectComponents( void );

	/**
	 * Method sets up application base frame
	 * @return nothing
	 **/
	void setupBaseFrame( void );

	/**
	 * Method adapts window to proper size
	 * @return nothing
	 **/
	void adaptWindowSize( void );

	/**
	 * Method processes dialog for file selection
	 * @param sample's file name
	 * @return returns 0 if sample was not selected, 1 if otherwise
	 **/
	void procDialogAddSample( QStringList &fNames );

	/**
	 * Method used for samples deployment into sample viewer and result
	 * widget.
	 * @param samples's path for deployment
	 * @return nothing
	 **/
	void deploySamples( QList<QString> fNames );

	void processInputArgs( void );

protected:

	virtual void dragEnterEvent( QDragEnterEvent* event );

	virtual void dragMoveEvent( QDragMoveEvent* event );

	virtual void dropEvent( QDropEvent* event );

	virtual void keyPressEvent( QKeyEvent *event );	

	virtual void resizeEvent( QResizeEvent *event );
};

#endif // GMFC_H
