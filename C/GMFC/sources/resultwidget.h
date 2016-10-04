#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QClipboard>
#include <QApplication>
#include <QFileDialog>

#include "notImplemented.h"
#include "error.h"

#include "resultviewer.h"
#include "resulttextviewer.h"
#include "resultToolbar.h"
#include "styles.h"

class ResultWidget : public QWidget
{
	Q_OBJECT

public:
	
	//CONSTRUCTORS & DESTRUCTOR
	
	/**
	 * Implicit constructor
	 **/
	ResultWidget(QWidget *parent = 0, Qt::WFlags flags = 0);

	/**
	 * Implicit destructor
	 **/
	~ResultWidget();

	//PUBLIC PROPERTY

	//PUBLIC METHODS
	
	/**
	 * Method adds new array to comparison. Key should be filename with file path.
	 * Method invokes viewers (table and text viewer) refresh.
	 * @param key for identification
	 * @param content for comparison
	 * @return nothing
	 **/
	void addToCompare( QString key, QList<int>* vCont, QString md5  );	

	/**
	 * Method configures widget (cocretely it configures base frame). 
	 * @return nothing
	 **/
	void configure( void );

	/**
	 * Method directly adjust size of widget by inputed QResizeEvent.
	 * This method should be used when size adjustion is requested and
	 * and programer doesn't able to guarantee spontaneous event. Event
	 * should be accepted in upper level. Method doesn't care if event was
	 * accepted or not. This should be used for force resize.
	 * @param received resize event.
	 * @return nothing
	 **/
	void artificialResize( QResizeEvent *pEv );

signals:

	/**
	 * Method colorizes selected items by inputed color. Method can colorize
	 * background and foreground.
	 * @param background color
	 * @param foreground color [font color]
	 * @return nothing
	 **/
	void sgColSelection( QColor colB, QColor colF );

	/**
	 * Method [signal] reverts colorization of selected items.
	 * @return nothing
	 **/
	void sgRollbackSelection( void );

	/**
	 * Method [signal] reverts colorization of all items.
	 * @return nothing
	 **/
	void sgRollbackAll( void );

	/**
	 * Signal is emited when viewer was changed.
	 * @param string typed succes rate
	 * @return nothing
	 **/
	void sgSuccRateUpdate( QString rate );

public slots:

	/**
	 * Method contains functionality when toolbar is activated. Generally this
	 * method runs proper method base on acepted action from toolbar.
	 * @param action defines from toolbar
	 * @return nothing
	 **/
	void slTBarActivated( QAction* act );

	void updateCompare( QString fName, QList<int>* dataContent );

	void updateCompareSilent(QString fName, QList<int>* ptrDataContent);

	void slRemoveCompare( QString fName );

	void slFinalResizeRefresh();

private:
	
	ResultViewer*					_viewer;	/**< result viewer object */
	ResultTextViewer*				_tViewer;	/**< result text viewer object */
	QFrame*							_base;		/**< base frame */
	ResultToolbar*					_tBar;		/**< result viewer toolbar */
	std::bitset<Toolbar::SUM>		_tBarOpt;	/**< options comming from toolbar */
	QHash<QString,QString>*			_ptrMD5s;		/**< md5 hashes storage [fileName, md5] */

	int	_floatResBufferH;	/**< floating resize buffer for height */
	int	_floatResBufferW;	/**< floating resize buffer for width */

	/**
	 * Method configures base frame.
	 * @return nothing
	 **/
	void setupBaseFrame( void );

	/**
	 * Method places components of widget into proper position.
	 * @return nothing
	 **/
	void placeComponents( void );	

	/**
	 * Method connects component's signal and slots.
	 * @return nothings
	 **/
	void connectComponents( void );

	/**
	 * Method process selected [colorize] input into xDEF. Generally it is
	 * envelope for more specific methods. Also method copies assemblied xDEF 
	 * into application clipboard.
	 * @return nothing
	 **/
	void procResToClip( bool lightVer = false );

	/**
	 * Method assemblies xDEF from inputed parameters.
	 * @param text content list
	 * @param hexa content list
	 * @param xDef blocks list
	 * @return asseblied xDEF
	 **/
	QString asmXdf( 
			QList<QString> *tCont,
			QList<QString> *hCont,
			QList<ResultViewer::XDFblock_t> *xdfs );

	/**
	 * Method assemblies xDEF from inputed parameters.
	 * @param hexa content list
	 * @param xDef blocks list
	 * @return asseblied xDEF
	 **/
	QString asmXdfLight(
			QList<QString> *hCont,
			QList<ResultViewer::XDFblock_t> *xdfs );

	/**
	 * Method translates numeric address into string form. Also method aligns
	 * legnth of address to predefined size [XDF_ADDR_LEN].
	 * @param numeric address
	 * @return string address
	 **/
	QString trAddrToStr( int addr );

	/**
	 * Method appends one row from xDEF into def parameter.
	 * @param xDEF definition
	 * @param string form of address
	 * @param numeric index for next address [it is forward generated]
	 * @param row of hexa data
	 * @param row of text data
	 * @return nothing, because new row is appended into def parameter.
	 **/
	void publishRow( QString &def,
						QString &addr,
						int idx,
						QString &hTmp,
						QString &tTmp );

	void saveXdf( bool lightVer = false );

	void procDialogSaveXdf( QString &fName );

protected:

	virtual void resizeEvent( QResizeEvent *pEv );
};

#endif // RESULTWIDGET_H
