/**
 * FileWidget ver 2.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#define ADDR_LABEL_WIDTH 8
#define DATA_BLOCK 4096

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

#include "hexaviewer_v2.h"
#include "textviewer_v2.h"
#include "addressbar_v2.h"
#include "scrollbar_v2.h"

#include "toolbar.h"
#include "notImplemented.h"
#include "saveparser.h"

#include "md5.h"
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
		SAVE_PATTERN,
		DIALOG_SUM		
	}DIALOG_TYPES;

	/**
	 * Viewer::IndexItem_t structure defines indexes into viewer, file and
	 * bytes array. 
	 */
	typedef struct{
		int viewer;			/**< index into viewer */
		int file;			/**< index into file */
		int bytes;			/**< index into bytes */
	}FileWidget::t_IdxsIt;

	/**
	 * FileWidget exception.
	 **/
	class FileWidgetException : public std::exception{

	public:
		
		FileWidgetException(QString msg);
	};
	
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
	 * Viever data content array
	 * @return data array
	 **/
	QList<int>* DataContent( void );

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
	void setFocus(bool flag);
	bool focus(void);

	/**
	 * Method returns current possible number of rows for actual sample 
	 * viewe [parent object] size.
	 * @param sample viewer's height
	 * @return number of rows suitable for this size of sampleViewer [parent object]
	 **/
	static int currentRows(int height);

	/**
	 * [v2]
	 **/
	void artificialResize(int diffH);

signals:

	/**
	 * Signal is emited when close button was clicked and widget 
	 * is ready to close.
	 * @param widget's file name
	 * @return nothing
	 **/
	void closing(QString fName);

	/**
	 * Signal is emited, when toolbar is used.
	 * @param sender object
	 * @return nothing
	 **/
	void sgFocusTaken(FileWidget* sender);

	void sgContentIsChanging(QString fName, QList<int> *dataContent);

	void sgContentIsChangingSilent(QString fName, QList<int> *dataContent);

	/**
	 * [v2] Signal is emited when drag in one of the viewers is executed. This signal
	 * holds string typed address of dragged byte.
	 **/
	void sgDragAtAddress(QString addr);

public slots:

	/**
	 * Method [slot], which responds when any icon is activated.
	 * @param toolbar's action
	 **/
	void slTBarActivated(QAction* action);

	/**
	 * Method [slot], which responds when any icon is activated externally
	 * out of FileWidget. So, this method allows 
	 **/
	void slTBarActivated(Toolbar::ACTIONS action);

	/**
	 * Method check text dialog when inputed is active, and it is filled
	 * by characters.
	 * @param string value  of input
	 * @return nothing
	 **/
	void slCheckDialog(QString value);

	/**
	 * [v2] Slot is activated when drag action is in progress in one of viewers.
	 * @param sequence index
	 * @return nothing
	 **/
	void slDragInProgress(int seqIdx);	

	/**
	 * [v2] Slot is used when view components changed their state and model refresh
	 * is requested.
	 * @param shift in components
	 * @return nothing
	 **/
	void slViewChanged(int shift, int drag);

	/**
	 * [v2] Method [slot], which responds when scroll bar's slider is changed. This method
	 * finds proper action based on inputed action.
	 * @param slider action
	 * @return nothing
	 **/
	void slScrollChanged(int value);

private:

	QFrame* _base;							/**< base frame */
	std::bitset<Toolbar::SUM> _tBarOpt;		/**< options comming from toolbar */
	
	//dialogs variables
	DIALOG_TYPES	_dType;				/**< current dialog action */
	QDialog			*_pActDialog;		/**< actual activated dialog */

	//internal view components
	HexaViewer	*_hexa;			/**< hexa viewer component */
	TextViewer	*_text;			/**< text viewer component */
	AddressBar	*_address;		/**< address bar component */
	ScrollBar	*_scroll;		/**< scroll bar component */
	Toolbar		*_tBar;			/**< toolbar component */
	QLabel		*_title;		/**< title for file widget */
	
	std::wstring _fileName;	/**< file path */
	QString	_fName;
	
	//program internal variables
	PEbrowser*	_ptrPE;				/**< pe browser object */	
	SaveParser	*_ptrSaveParser;	/**< save pattern parser */	
	
	bool	_focusFlag;		/**< focus flag */	
	
	QFile*	_v2_ptrFile;	/**< file handler */
	qint64	_v2_fileSize;	/**< file size */

	bool	_v2_imageAddressEnable;	/**< flag for image address view */

	bool	_v2_draggedAddressSent;

	QString	_v2_md5;			/**< sample's MD5 hash */
	int		_v2_startAddress;	/**< adress bar start address */
	
	int		_v2_scrollBarValue;		/**< current scroll bar value */
	int		_v2_scrollFloatBuffer;	/**< remain after divide operation */
	
	//data models
	QStandardItemModel*	_v2_ptrHexaModel;		/**< hexaviewer's model */
	QStandardItemModel* _v2_ptrTextModel;		/**< textviewer's model */
	QStandardItemModel* _v2_ptrAddressModel;	/**< address's model */

	QList<int>*	_v2_ptrDataContent;
	QList<SaveParser::saveInterval_t>* _v2_ptrSaveIntervals;

	//model sizes
	int _v2_rows;		/**< number of rows in models */
	int _v2_cols;		/**< number of columns in models */
	int _v2_cellsNum;	/**< number of all rows */

	//file internal pointers
	//QByteArray*						_v2_bytes;	/**< file data array */
	QByteArray						_v2_bytes;	/**< file data array */
	QList<FileWidget::t_IdxsIt>*	_v2_ptrIdxs;	/**< indexes array - holder for bytes, viewer and file array */
	int								_v2_file;		/**< sequence index to file, where data block in ptrBytes starts */
	int								_v2_offset;		/**< sequence index to model, where data statrs (forst non-empty cell) */

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
	void createGeneralDescriptionDialog(void);

	void createBinarySearchDialog(void);

	void createSavePatternDialog(void);

	/**
	 * Method corrects file name into right length.
	 * @return corrected file name
	 **/
	QString corFName( void );

	/**
	 * Method configures file widget's title
	 * @return nothing
	 **/
	void confTitle(int w, int h);

	void execTBarAction(Toolbar::ACTIONS curAct, bool silent = false); 

	void contentChanged(QString fName, QList<int> *dataContent, bool silent = false);

	void v2_openFile(void);

	void v2_computeMD5(void);

	QString v2_charToCellLabelHexa(unsigned char input);

	QString v2_charToCellLabelText(unsigned char input);

	QString v2_charToCellLabelAddress(unsigned int input, bool view = true);

	void v2_createBytes(int seek);

	void v2_createIndexes(QByteArray ptrBytes, int offset, int file);

	void v2_clearIndexes(void);

	void v2_createModels(void);

	void v2_updateModels(void);

	void v2_reallocateModels(void);

	bool v2_savePatternAddressVisible(int address);

	unsigned long v2_physicalToImageAddress(int physicalAddress);

	QString v2_draggedAddress(int seqDrag);

	int v2_binarySearch(QString pattern);

	void v2_propagateStep(int shift);

	bool v2_validShift(int shift);

public:

	/**
	 * Method handles wheel event.
	 * @param wheel event
	 * @param return nothing
	 **/
	void wheelEvent(QWheelEvent *event);

	/**
	 * Method handles keyboard event.
	 * @param keyboard event
	 * @return nothing
	 **/
	void keyPressEvent(QKeyEvent* event);	
};

#endif // FILEWIDGET_H
