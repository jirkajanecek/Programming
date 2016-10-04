/**
 * Viewer ver 1.0
 * component of GMFC 
 * Autor: Jiri Janecek
 **/

#ifndef VIEWER_H
#define VIEWER_H

#include <iostream>
#include <string>
#include <ctime>

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QDragEnterEvent>
#include <QByteArray>
#include <QFile>
#include <QString>
#include <QPixmap>
#include <QHeaderView>
#include <QTableWidget>
#include <QCoreApplication>

#include <QDebug>

#include "viewerStyles.h"
#include "styles.h"
#include "md5.h"

#include "saveparser.h"
#include "PEbrowser.h"

class Viewer : public QTableWidget{

	Q_OBJECT

public:

	//EXCEPTIONS

	/**
	 * Exception class for Viewer class.
	 **/
	class ViewerException : public std::exception{
		public:
			/**
			 * implicit constructor
			 **/
			ViewerException( void );

			/**
			 * explicit constructor
			 * @param message
			 **/
			ViewerException( std::string msg );
	};

	/**
	 * Viewer::IndexItem_t structure defines indexes into viewer, file and
	 * bytes array. 
	 */
	typedef struct{
		int viewer;			/**< index into viewer */
		int file;			/**< index into file */
		int bytes;			/**< index into bytes */
	}Viewer::IndexItem_t;

	/**
	 * Explicit constructor
	 * @param file name for view
	 * @param number of rows
	 * @param number of columns
	 * @param Qt parent widget
	 * @param Qt flags
	 **/
	Viewer(QString filename = "",
			int rows = INIT_ROWS,
			int cols = INIT_COLS,
			QWidget *parent = 0,
			Qt::WFlags flags = 0);

	/**
	 * Destructor
	 **/
	~Viewer();

	//PROPERTIES
	
	/**
	 * Property like method, which returns file size in bytes.
	 * @return file size in bytes
	 **/
	int FileSize(void);

	/**
	 * File's md5
	 * @return sample's md5 hash
	 **/
	QString MD5( void );

	/**
	 * Property, which returns index array.
	 * @return index array.
	 **/
	QList<Viewer::IndexItem_t*>* Indexes( void );

	/**
	 * Property, which offers data content of viewer.
	 * @return data content
	 **/
	QList<int>* DataContent( void );

	/**
	 * Property like method, which returns size of shown buffer.
	 * @return size of shown buffer [number of bytes in viewer]
	 **/
	int CellsNumber(void);

	/**
	 * Property like method, which returns size of cells horizontally.
	 * @return size of shown buffer [number of bytes in viewer]
	 **/
	int CellsHorizontal(void);

	/**
	 * Property like method, which returns size of cells vertically.
	 * @return size of shown buffer [number of bytes in viewer]
	 **/
	int CellsVertical( void );	

	void setSaveIntervals( QList<SaveParser::saveInterval_t>* intrvs);

	void setPE( PEbrowser* ptrPE );

	/**
	 * Method which, should be called when object is created. It shows entire viewer
	 * content.
	 * @return nothing
	 **/
	void configure( void );

	/**
	 * Method determines if requested shift is valid or not.
	 * @param requested shift
	 * @return true if shift is valid, false otherwise.
	 **/
	bool isValidShift( int shift );

	/**
	 * Method search binary pattern [typed as hexa string] in file. If pattern is
	 * located address of this pattern is returned, otherwise -1.
	 * @param string typed hexa pattern
	 * @return offset to file, where finded pattern started, or -1 if pattern is not
	 * contained in file.
	 **/
	virtual int binarySearch( QString pattern );

	void forceRefresh( void );

	virtual void artificialResize(QResizeEvent* ptrEv);

	/**
	 * Method returns absolute top value [address] of viewer. 
	 * @return address of top left cell
	 **/
	int absoluteTopValue(void);

public slots:

	/**
	 * Method [slot], which responds to hexaviewer or scroll bar changes.
	 * @param byte shift
	 **/
	void viewerChanged( int shift );
	
	/**
	 * Method [slot], which responds to hexaviewer or scroll bar changes.
	 * @param shift byte shift
	 * @param pointer to file
	 * @param offset to viewer
	 **/
	void viewerChanged( int shift, int file, int offset );

	/**
	 * Method [slot], which responds to hexaviewer or scroll bar changes.
	 * @param seek into file
	 **/
	void viewerChangedAbs( int seek );

signals:

	/**
	 * This method [signal] signals to other components, that step
	 * propagation is imminent - shift version.
	 * @param hexaviewer shift
	 **/
	void viewerIsChanging( int shift );
	
	/**
	 * This method [signal] signals to other components, that step
	 * propagation is imminent - shift version.
	 * @param hexaviewer shift
	 * @param hexaviewer file pointer
	 * @param hexaviewer offset
	 **/
	void viewerIsChanging( int shift, int file, int offset );

	/**
	 * This method [signal] signals to other components, that step
	 * propagation is imminent - data content.
	 * @param file name
	 * @param data content array
	 **/
	void viewerIsChanging( QString fName, QList<int> *dataCont );

	/**
	 * This method [signal] signals to other components, that step
	 * propagation is imminent - data content.
	 * @param file name
	 * @param data content array
	 **/
	void viewerIsChangingSilent( QString fName, QList<int> *dataCont );

	/**
	 * This method [signal] signals to other components, that step
	 * propagation is imminent - absolute version.
	 * @param absolute seek position into file
	 **/
	void viewerIsChangingAbs( int value );

	/**
	 * Signal is sent, when mouse press event on cell is executed.
	 * @param sequence index to viewer
	 * @return nothing
	 **/
	void sgDragInProgress( int seqIdx );

	/**
	 * Signal is sent, when cell drop is executed.
	 * @return nothing
	 **/
	void sgDragIsFinished( void );

protected:
	
	//graphical components
	QFrame						*_base;		/**< parent frame, where all labels are situated */
	
	//file & buffer components
	QFile								*_contentFile;	/**< content file handle */
	QString								_fName;			/**< file name */
	QByteArray							*_bytes;		/**< list of bytes */	
	QList<Viewer::IndexItem_t*>			*_indexes;		/**< list of index control structures */
	QList<int>							*_dataCont;		/**< numeric content array  */
	QString								_md5;			/**< sample's md5 hash */

	QList<SaveParser::saveInterval_t>	*_ptrSaves;		/**< save pattern intervals */
	PEbrowser							*_ptrPE;		/**< PE borwser object */

	//QTableWidget *_ptrViewer;				/**< main viewer object */

	//indexes
	int _drag;			/**< index to indexes array */
	int _file;			/**< index to file - file position in front of viewer */
	int _offset;		/**< index to viewer - first file byte in viewer */
	int _shift;			/**< curent shift - based on move */

	//window size
	int _appHeight;			/**< widget height */
	int _appWidth;			/**< widget width */
	int _byteHeight;		/**< byte cell height */
	int _byteWidth;			/**< byte cell width */
	int _cellHorizontal;	/**< number of cells horizontally */
	int _cellVertical;		/**< number of cells vertically */
	int _cellsNum;			/**< number of cells */
		
	//other
	bool _configExists;		/**< flag if configuration file exists */
	QString _cellMimetype;	/**< cell mime type */

	//PROPERTY functions

	/**
	 * Method configures internal variables, which affect application design.
	 * @return nothing
	 **/
	virtual void configureClassSpecifics( void );

	/**
	 * Method configures internal table widget, which affect application design.
	 * @return nothing
	 **/
	void configureTWidget( void );
	
	//UI functions

	/**
	 * Method set up base frame.
	 * @return nothing
	 **/
	void setupBaseFrame(void);

	//CONTENT functions

	/**
	 * Method used for translation from char into hexa number of inputed char.
	 * @param inputed char [byte]
	 * @return char hexa value in string type.
	 **/
	virtual QString charToCellLabel(unsigned char byte );
	
	/**
	 * Method creates index list based on offsets and file content.
	 * @param file content
	 * @param start position where file content is places in viewer
	 * @param start position in file (seek)
	 * @return nothing
	 **/
	void createIndexes(QByteArray* bytes, int bytesOffset, int fileOffset);

	/**
	 * Method deallocates all objects from indexes.
	 * @return nothing
	 **/
	void clearIndexes(void);

	/**
	 * Method loads data from file [displayed]. Load is processed base on
	 * seek.
	 * @param seek to data file.
	 * @return nothing
	 **/
	void createBytes( int seek );

	/**
	 * Method destroys byte array loaded from data file.
	 * @return nothing
	 **/
	void clearBytes( void );

	/**
	 * Method renders bytes into base frame. Information are loaded from
	 * data and indexes arrays.
	 * @return nothing
	 **/
	virtual void createViewer( void );

	/**
	 * Method deallocates all objects from viewer.
	 * @return nothing
	 **/
	void clearViewer( void );

	/**
	 * Method refreshes values in viewer. Generaly, it changes only
	 * labels. Nothing new is created.
	 * @param silent flag - no eminent recomputation is requested
	 * @return nothing
	 **/
	void refreshViewer( bool silent = false );

	void reallocViewer(void);

	/**
	 * Method refresh viewer content.
	 * @return nothing
	 */
	void propagateStep( void );
	
	/**
	 * Method translates sequence number into point, which describes 
	 * position in base frame
	 * @param sequence number
	 * @return target point
	 **/
	QPoint* translateSequenceToPoint( int seq );

	/**
	 * Method translates inputed point into accord sequence number in
	 * viewer array.
	 * @param inputed point 
	 * @return sequence number
	 **/
	int translatePointToSequence( QPoint point );

	/**
	 * Method validates position (point) in buffer viewer. If position
	 * is outside of buffer point is changed into the most outer position.
	 * @param position in buffer
	 * @return nothing - point is changed itself (pointer)
	 **/
	void validateBufferPosition( QPoint* point );

	/**
	 * Method determines if shift is processed or not.
	 * @return true if shift is neccessary, false if otherwise.
	 **/
	bool shiftRequested( QPoint pos );

	/**
	 * Method computes shift from current position and original drag position.
	 * This result is stored into internal variable _shift.
	 * @param current position - point
	 * @return nothing
	 **/
	void computeShift( QPoint pos );

	/**
	 * Method creates file handler based on inputed file name. Open file for reading.
	 * If filename is empty 
	 **/
	void openFile( void );

	/**
	 * Method computes sample's md5 hash and store it into internal variable
	 * @return nothing
	 **/
	virtual void computeMD5( void );

	bool addressVisible( int addr );

	//DRAG & DROP functions

	/**
	 * Method is activated when mouse press into base frame.
	 * It is used as the first function in drag and droj
	 * procedure.
	 * @return nothing
	 **/
	virtual void mousePressEvent( QMouseEvent *event);

	/**
	 * Method is activated when dragged object entering into
	 * new area [in app enter to new cell].
	 * @param drag enter event
	 * @return nothing
	 **/
	virtual void dragEnterEvent( QDragEnterEvent *event );

	/**
	 * Method is activated when drag move is processed.
	 **/	
	virtual void dragMoveEvent( QDragMoveEvent *event);

	/**
	 * Method is activated when drop action is proceeded.
	 * @param drop event
	 * @return nothing
	 **/
	virtual void dropEvent( QDropEvent *event );
};

#endif // VIEWER_H
