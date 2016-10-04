#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QtGui/QWidget>
#include <QApplication>
#include <QAbstractSlider>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRegExp>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QScrollArea>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QFileDialog>

#include "fileWidgetStyles.h"
#include "styles.h"

#include "hexaviewer.h"
#include "textviewer.h"
#include "addressbar.h"
#include "scrollbar.h"
#include "toolbar.h"
#include "notImplemented.h"
#include "saveparser.h"

#include "PEbrowser.h"

class FileWidget : public QWidget
{
	Q_OBJECT

public:

	typedef enum DIALOG_TYPES{
		NOT_SET,
		TARGET_ADDRESS,
		DESCRIPTION,
		BIN_SEARCH,
		DIALOG_SUM		
	}DIALOG_TYPES;
	
	//CONSTRUCTORS & DESTRUCTOR
	
	/**
	 * Explicit constructor
	 * @param file name
	 * @param parent widget
	 * @param flags
	 **/
	FileWidget(QString filename = "", 
				int rows = INIT_ROWS,
				int cols = INIT_COLS,
				QWidget *parent = 0,
				Qt::WFlags flags = 0);
	
	/**
	 * Implicit constructor
	 **/
	~FileWidget();

	//PUBLIC PROPERTIES
	
	/**
	 * Focus property
	 * @return true if widget has artificial set focus, false if otherwise
	 **/
	bool Focus( void );

	/**
	 * File name property
	 * @return sample's file name
	 **/
	QString FileName( void );

	/**
	 * File's md5
	 * @return sample's md5 hash
	 **/
	QString MD5( void );

	/**
	 * Buffer's index array property
	 * @return index array
	 **/
	QList<Viewer::IndexItem_t*>* Indexes( void );

	/**
	 * Viever data content array
	 * @return data array
	 **/
	QList<int>* DataContent( void );

	/**
	 * Active dialog object.
	 * @return object if 
	 **/
	QDialog* ActiveDialog( void );

	//PUBLIC METHODS

	/**
	 * This method configures file widget.
	 * @return nothing
	 **/
	void configure( void );

	/**
	 * Method sets "focus" on itself. It means, that widget change
	 * itself appearance.
	 * @param permanent flag - for permanent focus
	 * @return nothing
	 **/
	void setFocus( bool perm = false );

	/**
	 * Method unset widget's "focus appearance" and sets it back
	 * to normal.
	 * @param permanent flag - for permanent focus
	 * @return nothing
	 **/
	void setBlur( bool perm = false );

	/**
	 * Method set up save patterns intervals into fileWidget object
	 * @param save pattern intervals
	 * @return nothing
	 **/
	void setSaveIntervals(QList<SaveParser::saveInterval_t>* intrvs);

	/**
	 * Method returns current possible number of rows for actual sample 
	 * viewe [parent object] size.
	 * @param sample viewer's height
	 * @return number of rows suitable for this size of sampleViewer [parent object]
	 **/
	static int currentRows(int height);

signals:

	//SIGNALS

	/**
	 * Signal for propagation shift to the integrated components.
	 * @param byte shift
	 * @return nothing
	 **/
	void shiftComponents(int shift);

	/**
	 * Signal for setting up absolute [particular] value into integrated
	 * components.
	 * @param absolute value [address]
	 * @return nothing
	 **/
	void setComponents(int value);

	/**
	 * Signal for setting up absolute [particular] value into integrated
	 * components. This signal should be used when there is a need to set up
	 * all components (include scrollBar) from external source (like icons).
	 * @param absolute value [address]
	 * @return nothing
	 **/
	void setComponentsExternal( int value );

	/**
	 * Signal is emited when close button was clicked and widget 
	 * is ready to close.
	 * @param widget's file name
	 * @return nothing
	 **/
	void closing( QString fName );

	/**
	 * Signal is emited, when toolbar is used.
	 * @param sender object
	 * @return nothing
	 **/
	void focusReq( FileWidget* sender );

	void contentIsChanging( QString fName, QList<int> *dataContent );

	void contentIsChangingSilent( QString fName, QList<int> *dataContent );

	/**
	 * Signal is emited when drag in one of the viewers is executed. This signal
	 * holds string typed address of dragged byte.
	 **/
	void sgDragAtAddress( QString addr );

public slots:

	//PUBLIC SLOTS

	/**
	 * Method [slot], which responds when scroll bar's slider is changed. This method
	 * finds proper action based on inputed action.
	 * @param slider action
	 * @return nothing
	 **/
	void sliderChanged( int action );

	/**
	 * Method is activated when slider is pressed. It is due to focus establish.
	 * @return nothing
	 **/
	void sliderPressed( void );

	/**
	 * Method [slot], which respands when any icon is activated.
	 * @param toolbar's action
	 **/
	void slTBarActivated( QAction* action );

	/**
	 * Method [slot], which responds when any icon is activated externally
	 * out of FileWidget. So, this method allows 
	 **/
	void slTBarActivated( Toolbar::ACTIONS action );

	/** 
	 * Method process text dialog when it is submited.
	 * @param string value of input
	 * @return nothing
	 **/
	void processDialog( QString value );

	/**
	 * Method check text dialog when inputed is active, and it is filled
	 * by characters.
	 * @param string value  of input
	 * @return nothing
	 **/
	void checkDialog( QString value );

	void contentChanged( QString fName, QList<int> *dataContent );

	void contentChangedSilent( QString fName, QList<int> *dataContent );

	/**
	 * Slot is activated when drag action is in progress in one of viewers.
	 * @param sequence index
	 * @return nothing
	 **/
	void slDragInProgress( int seqIdx );

	/**
	 * Slot is activated when drop action is proceeded in one of viewers.
	 * @return nothing
	 **/
	void slDragIsFinished( void );

	void slArtificialResize(QResizeEvent* ptrEv);

private:

	//PRIVATE VARIABLE

	QFrame* _base;							/**< base frame */
	std::bitset<Toolbar::SUM> _tBarOpt;		/**< options comming from toolbar */
	DIALOG_TYPES _dType;					/**< current dialog action */

	//major active components
	HexaViewer	*_hexa;			/**< hexa viewer component */
	TextViewer	*_text;			/**< text viewer component */
	AddressBar	*_address;		/**< address bar component */
	ScrollBar	*_scroll;		/**< scroll bar component */
	Toolbar		*_tBar;			/**< toolbar component */
	QLabel		*_title;		/**< title for file widget */
	
	//scrollbar variables
	int _sbWidth;			/**< scroll bar width */
	int _sbHeight;			/**< scroll bar height */

	std::wstring _fileName;	/**< file path */
	
	//program internal variables
	PEbrowser	*_pe;				/**< pe browser object */
	QDialog		*_pActDialog;		/**< actual activated dialog */
	SaveParser	*_ptrSaveParser;	/**< save pattern parser */
	bool		_focus;				/**< focus flag */
	bool		_perm;				/**< permanent flag */

	int			_floatResBufferH;	/**< float resize height buffer */

	//PRIVATE METHODS
	
	/**
	 * Method configures base frame.
	 * @return nothing
	 **/
	void configureBaseFrame( void );

	/**
	 * Method places integrated components into their positions.
	 * @return nothing
	 **/
	void placeComponents( void );

	/**
	 * Method connects signals and slots from all integrated components.
	 * @return nothing
	 **/
	void connectComponents( void );

	/**
	 * Method, which processes internal programs scans etc. It is
	 * executed before gui is started.
	 * @return nothing
	 **/
	void executeProcessInternals( void );

	/**
	 * Method encapsulates peBrowser object. At the same time this method
	 * provides file scan.
	 * @return nothing, but internal PE object is ready to use after method
	 * execution.
	 **/
	void peScan( void );

	/**
	 * Method creates "target address" dialog and connects it to
	 * fileWidget. Also sets up actual dialog [target address dialog].
	 * After dialog is finished all dialog resources are released and 
	 * dialog is disconnected from fileWidget.
	 * @return nothing
	 **/
	void createTargetAddressDialog( void );

	/**
	 * Method creates "general description" dialog and connects it to
	 * fileWidget. Also sets up actual dialog [target address dialog].
	 * After dialog is finished all dialog resources are released and 
	 * dialog is disconnected from fileWidget.
	 * @return nothing
	 **/
	void createGeneralDescriptionDialog( void );

	void createBinarySearchDialog( bool &actFailed );

	void createSavePatternDialog( QList<QString> &fNames );

	/**
	 * Method corrects file name into right length.
	 * @return corrected file name
	 **/
	QString corFName( void );

	/**
	 * Method configures file widget's title
	 * @return nothing
	 **/
	void confTitle( int w, int h );

	/**
	 * Method filters events from scrollbar into filewidget.
	 * @param object, which generates event
	 * @param generated event
	 * @return true if event was handled, false if otherwise
	 **/
	bool eventFilter( QObject* obj, QEvent* ev );

	void execTBarAction( Toolbar::ACTIONS curAct ); 

public:

	/**
	 * Method handles wheel event.
	 * @param wheel event
	 * @param return nothing
	 **/
	void wheelEvent( QWheelEvent *event );

	/**
	 * Method handles keyboard event.
	 * @param keyboard event
	 * @return nothing
	 **/
	void keyPressEvent( QKeyEvent* event );
};

#endif // FILEWIDGET_H
